/*
 * ExchangeConnection.hpp
 *
 *  Created on: 13-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGECONNECTION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGECONNECTION_HPP_

#include <Common.hpp>
#include <SocketUtils.hpp>

namespace oms{

template <typename T>
class ExchangeConnection{

	enum{
		DEFAULT_SOCKET_BUFSIZE = 1048576,
		DEFAULT_CONNECTION_TIMEOUT = 3,
		DEFAULT_SELECT_TIMEOUT = 5,
		DEFAULT_MAX_CONNECT_RETRIES = 3,
		DEFAULT_RECONNECT_INTERVAL = 2
	};

	std::string _host;
	std::string _port;
	bool _noDelay;
	bool _nonBlocking;
	size_t _socketBufSize;
	bool _spinReaderThread;
	int _connectTimeout;
	int _maxRetries;
	int _reconnectInterval;
	int _selectTimeout;
	int _socketFileDesc;


public:

	ExchangeConnection() : _noDelay(true),
	_nonBlocking(true),
	_socketBufSize(DEFAULT_SOCKET_BUFSIZE),
	_spinReaderThread(false),
	_connectTimeout(DEFAULT_CONNECTION_TIMEOUT),
	_maxRetries(DEFAULT_MAX_CONNECT_RETRIES),
	_reconnectInterval(DEFAULT_RECONNECT_INTERVAL),
	_selectTimeout(DEFAULT_SELECT_TIMEOUT),
	_socketFileDesc(-1){

	}

	virtual ~ExchangeConnection(){

	}

	void init(const std::string& config){
		// Read all params from config
	}

	bool connect(){
		int retry = 0;
		do{
			int sd = SocketUtils::createConnection(_host, _port, _noDelay, _nonBlocking, _socketBufSize, _connectTimeout);
			if(sd != -1){
				_socketFileDesc = sd;
				return true;
			}
			std::this_thread::sleep_for(std::chrono::seconds(_reconnectInterval));
		}while(++retry < _maxRetries);
		return false;
	}

	void disconnect(bool reconnect){
		::shutdown(_socketFileDesc, SHUT_RDWR);
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		::select(0, NULL, NULL, NULL, &tv);
		::close(_socketFileDesc);
		_socketFileDesc = -1;
	}

	inline bool isConnected() const {
		return _socketFileDesc != -1;
	}

	inline bool isReadyRead() const{
		if(_nonBlocking && !_spinReaderThread){
			fd_set readset;
			struct timeval tv = {3,0};
			FD_ZERO(&readset);
			FD_SET(_socketFileDesc, &readset);

			int rv = ::select(_socketFileDesc+1, &readset, NULL, NULL, &tv);
			if(rv < 0){
				perror("ExchangeConnection::isReadyRead() - ");
				return false;
			}
			if(rv == 0){
				return false;
			}
		}
		return true;
	}


	inline ssize_t writeMsg(const char* msg, ssize_t size){
		ssize_t bytes_sent = 0;
		do{
			ssize_t nbytes = ::send(_socketFileDesc, msg+bytes_sent, size - bytes_sent, 0);
			if(nbytes == 0){
				return -1;
			}
			if(nbytes < 0){
				if((errno == EWOULDBLOCK) || (errno == EAGAIN) || (errno == EINTR)){
					// Wait for the socket to be ready before retrying
					struct timeval tv = {3,0};
					fd_set writeset;
					FD_ZERO(&writeset);
					FD_SET(_socketFileDesc, &writeset);

					if((::select(_socketFileDesc+1, NULL, &writeset, NULL, &tv) > 0) && (FD_ISSET(_socketFileDesc, &writeset))){
						continue;
					}
				}
				if(!bytes_sent){
					return -1;
				}
				break;
			}
			bytes_sent += nbytes;
		}while(bytes_sent < size);

		return bytes_sent;
	}

	inline ssize_t readMessage(const char* buf){
		return ::recv(_socketFileDesc, (void*)buf, _socketBufSize, 0);
	}

	inline bool wouldSpin() const{
		return (_nonBlocking && _spinReaderThread);
	}

	inline int getSocketSendBufSize() const {
		SocketUtils::getSocketSendBufferSize(_socketFileDesc);
	}

	inline int getSocketRecvBufSize() const {
		SocketUtils::getSocketRecvBufferSize(_socketFileDesc);
	}

	inline const std::string& getHost() const {
		return _host;
	}

	inline const std::string& getPort() const {
		return _port;
	}
};

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGECONNECTION_HPP_ */
