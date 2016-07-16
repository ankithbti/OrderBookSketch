/*
 * ExchangeSession.cpp
 *
 *  Created on: 15-Jul-2016
 *      Author: ankithbti
 */

#include <ExchangeSession.hpp>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

namespace oms{

struct SocketInit{
	SocketInit(){
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGPIPE, &sa, 0);
	}
};

void * init_once(){
	static SocketInit initialized;
	return &initialized;
}

ExchangeSession::ExchangeSession(const std::string& name) : _name(name), _shutdown(false),
		_loggedOn(false),
		_sentLogon(false),
		_logonTimeout(DEFAULT_LOGON_TIMEOUT),
		_sentLogout(false),
		_sendPending(false),
		_needTestReq(false),
		_disconnectTimeoutOnHBMiss(DEFAULT_CONNECTION_TIMEOUT_ON_HB_MISS),
		_heartBeatInterval(DEFAULT_HB_INTERVAL),
		_testReqInterval(DEFAULT_TEST_REQ_INTERVAL),
		_testReqCnt(0),
		_readBuf(NULL),
		_sockRcvBufSz(0),
		_readBufSz(0),
		_msgSize(0),
		_sessionCallback(NULL){
			init_once();
		}

		ExchangeSession::~ExchangeSession(){
			delete [] _readBuf;
		}

		void ExchangeSession::init(const std::string& configStr){
			SeqNumStore<ExchangeSession>::initStore(configStr);
			// Fill parameters from config
		}

		void ExchangeSession::start(){
			_shutdown = false;
			if(!_readerThread)
				_readerThread = std::shared_ptr<std::thread>(new std::thread(std::bind(&ExchangeSession::readThread, this)));
		}

		void ExchangeSession::shutdown(){
			_shutdown = true;
			_readerThread->join();
			_readerThread.reset();
		}

		void ExchangeSession::disconnect(){
			ExchangeConnection<ExchangeSession>::disconnect(false);
			_loggedOn = false;
			_sentLogon = false;
			_sentLogout = false;
		}

		void ExchangeSession::readThread(){
			ssize_t totalSize = 0;
			char * start = NULL;
			const char* currReadBuf = NULL;
			const char* readBufEnd = NULL;

			while(!_shutdown){
				try{
					if(!_loggedOn){
						if(_sentLogon){
							checkStatus();
						}else{
							start = NULL;
							currReadBuf = NULL;
							readBufEnd = NULL;
							sendLogon();
						}
					}

					ssize_t sz = 0;
					if(!ExchangeSession::recvMsg(currReadBuf, sz))
					{
						currReadBuf = NULL;
						totalSize = 0;
						continue;
					}

					if(!start){
						start = (char*)currReadBuf;
						readBufEnd = start + sz;
					}

					totalSize += sz;
					currReadBuf += sz;

					char * currMsgBuf = start;

					for(;;){

						size_t msgLen = 0;
						if(totalSize && isFullMsg(currMsgBuf, totalSize, msgLen)){
							_lastRecTime = time(NULL);
							try{
								processInMessage(currMsgBuf, msgLen);
							}catch(...){

							}

							if(msgLen > totalSize){
								// Invalid situation
								this->disconnect();
							}

							currMsgBuf += msgLen;
							totalSize -= msgLen;

							if(currMsgBuf >= readBufEnd && totalSize){
								// Invalid situation
								this->disconnect();
							}

						}else{
							if(currMsgBuf != start){
								if(currMsgBuf >= readBufEnd && totalSize){
									this->disconnect();
									break;
								}
								memcpy(start, currMsgBuf, totalSize);
								currMsgBuf = start + totalSize;
							}
							break;
						}
					} // end of for;;

					checkStatus();

				}catch(...){

				}
			} // end of while
		}

		bool ExchangeSession::recvMsg(const char* &buf, ssize_t &len){
			if((buf < _readBuf) || (buf >= _readBuf + _msgSize)){
				buf = _readBuf;
			}

			for(;;){
				if(!this->isReadyRead()){
					if(!this->isConnected()){
						return false;
					}
					checkStatus();
					continue;
				}

				ssize_t bytesRead = this->readMessage(buf);
				if(bytesRead > 0){
					len = bytesRead;
					return true;
				}else if(bytesRead == 0){
					// Peer disconnected
					this->disconnect();
					return false;
				}else{
					// Error while reading
					if((errno == EWOULDBLOCK) || (errno == EAGAIN) || (errno == EINTR)){
						if(this->wouldSpin()){
							checkStatus();
						}
						continue;
					}
					this->disconnect();
					return false;
				}
			}// end of for;;
		}

		void ExchangeSession::checkStatus(){
			time_t now = time(NULL);
			time_t elapsed = now - _lastSentTime;

			if(!this->isConnected() || !_loggedOn){
				if(this->isConnected() && _sentLogon && elapsed > _logonTimeout){
					this->disconnect();
				}
				return;
			}

			time_t timeSinceLastRecv = now - _lastRecTime;
			if(timeSinceLastRecv >= _disconnectTimeoutOnHBMiss){
				_loggedOn = false;
				this->disconnect();
			}else if(_needTestReq){
				if(timeSinceLastRecv >= ((_testReqCnt+1)* _testReqInterval)){
					if(_sendPending){
						sendTestReq();
						++_testReqCnt;
					}
				}else if(_testReqCnt && (timeSinceLastRecv < _testReqInterval)){
					_testReqCnt = 0;

				}
			}

			if(elapsed >= _heartBeatInterval){
				sendHeartbeat();
			}
		}

		void ExchangeSession::sendLogon(){
			if(!this->isConnected() && !this->connect()){
				std::stringstream str;
				str << "Unable to connect to Host:Port " << this->getHost() << ":" << this->getPort() ;
				throw std::runtime_error(str.str());
			}

			if(_loggedOn){
				return;
			}

			int socketSndBufSz = this->getSocketSendBufSize();
			_sockRcvBufSz = this->getSocketRecvBufSize();
			_readBufSz = _sockRcvBufSz + _msgSize;

			std::stringstream msg;
			msg << " Connected to Host:Port [ " << this->getHost() << ":" << this->getPort()
									<< " ] "
									<< " Socket RcvBuffSize: " << _sockRcvBufSz
									<< " Socket SendBuffSize: " << socketSndBufSz
									<< " Read Buffer Size: " << _readBufSz
									<< " ReadBuffer: " << &_readBuf[0];
			std::cout << msg.str() << std::endl;

			_sentLogon = false;

			char buf[MAX_ADMIN_MESSAGE_SIZE];
			size_t sz = fillLogonMsg(buf, sizeof(buf));

			if(!sendRaw(buf, sz)){
				throw std::runtime_error("sendLogon() - Failed to send Logon.");
			}

			_sentLogon = true;

		}

		void ExchangeSession::sendLogout(){
			if(_sentLogout || !_loggedOn){
				return;
			}
			char buf[MAX_ADMIN_MESSAGE_SIZE];
			size_t sz = fillLogoutMsg(buf, sizeof(buf));

			if(!sendRaw(buf, sz)){
				throw std::runtime_error("sendLogout() - Failed to send Logout.");
			}

			_sentLogout = true;
		}

		void ExchangeSession::sendHeartbeat(){
			if(_sendPending){
				return;
			}
			if(!_loggedOn){
				return;
			}
			char buf[MAX_ADMIN_MESSAGE_SIZE];
			size_t sz = fillHeartbeatMsg(buf, sizeof(buf));

			if(!sendRaw(buf, sz)){
				throw std::runtime_error("sendHeartbeat() - Failed to send HB.");
			}

		}

		void ExchangeSession::sendTestReq(){
			if(!_loggedOn){
				return;
			}

			char buf[MAX_ADMIN_MESSAGE_SIZE];
			size_t sz = fillTestReqMsg(buf, sizeof(buf));

			if(!sendRaw(buf, sz)){
				throw std::runtime_error("sendTestReq() - Failed to send TestReq.");
			}
		}

}


