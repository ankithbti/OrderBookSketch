/*
 * ExchangeSession.hpp
 *
 *  Created on: 13-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGESESSION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGESESSION_HPP_

#include <Common.hpp>
#include <IExchangeSession.hpp>
#include <ExchangeConnection.hpp>
#include <SeqNumStore.hpp>

namespace oms{

class ExchangeSession : public IExchangeSession,
public SeqNumStore<ExchangeSession>,
public ExchangeConnection<ExchangeSession>
{

	enum{
		MAX_ADMIN_MESSAGE_SIZE = 1024,
		DEFAULT_HB_INTERVAL = 30,
		DEFAULT_MSG_SIZE = 2048,
		DEFAULT_CONNECTION_TIMEOUT_ON_HB_MISS = 3,
		DEFAULT_TEST_REQ_INTERVAL = 5,
		DEFAULT_LOGON_TIMEOUT = 10
	};

	std::string _name;
	std::shared_ptr<std::thread> _readerThread;
	volatile bool _shutdown;

	bool _loggedOn;
	bool _sentLogon;
	int _logonTimeout;
	bool _sentLogout;
	bool _sendPending;
	bool _needTestReq;
	int _disconnectTimeoutOnHBMiss;
	int _heartBeatInterval;
	time_t _lastSentTime;
	time_t _lastRecTime;
	int _testReqInterval;
	int _testReqCnt;

	char * _readBuf;
	size_t _sockRcvBufSz;
	size_t _readBufSz;
	size_t _msgSize;

protected:
	IExchangeSessionCallback * _sessionCallback;


public:
	ExchangeSession(const std::string& name);
	virtual ~ExchangeSession();


	/////////////////////
	// Function override from IExchangeSession
	/////////////////////
	virtual const std::string& name() const {
		return _name;
	}
	virtual void init(const std::string& configStr) ;
	virtual void setSessionCallback(IExchangeSessionCallback* cb){
		_sessionCallback = cb;
	}
	virtual void start();
	virtual void shutdown();

	virtual bool send(IOrderPtr) = 0;
	virtual bool send(ICancelOrderPtr) = 0;
	virtual bool send(IReplaceOrderPtr) = 0;
	virtual size_t registerSymbol(const ISymbolDefinitionPtr &) = 0;
	virtual IOrderManagerPtr getOrderManager(size_t symbolNum) = 0;
	/////////////////////

	virtual bool sendRaw(char * buf, size_t bufSize) = 0;

protected:

	virtual size_t fillLogonMsg(char* buf, size_t len) = 0;
	virtual size_t fillLogoutMsg(char* buf, size_t len) = 0;
	virtual size_t fillHeartbeatMsg(char* buf, size_t len) = 0;
	virtual size_t fillTestReqMsg(char* buf, size_t len) = 0;

	virtual void processInMessage(const char* buf, size_t len) = 0;
	virtual bool isFullMsg(const char* buf, size_t total, size_t& len) = 0;


	virtual void disconnect();
	virtual void sendLogon();
	virtual void sendLogout();
	virtual void sendHeartbeat();
	virtual void sendTestReq();

	bool sendMsg(char* buf, size_t size){
		if(size && (this->writeMsg(buf, size) == size)){
			// Successfully Sent the
			return true;
		}
		return false;
	}
	void setLoggedOn(bool val){
		_loggedOn = val;
	}
	void sentLogon(bool val){
		_sentLogon = val;
	}
	bool isLoggedOn() const {
		return _loggedOn;
	}
	bool isSentLogon() const {
		return _sentLogon;
	}
	void setDisconnectTimeoutOnHBMiss(int val){
		_disconnectTimeoutOnHBMiss = val;
	}
	void setNeedTestReq(bool val){
		_needTestReq = val;
	}
	bool isTestReqNeeded() const {
		return _needTestReq;
	}
	bool sentLogout() const{
		return _sentLogout;
	}


private:

	virtual void readThread();
	bool recvMsg(const char*& buf, ssize_t& len);
	void checkStatus();
};

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGESESSION_HPP_ */
