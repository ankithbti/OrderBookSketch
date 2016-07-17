/*
 * NSESession.hpp
 *
 *  Created on: 16-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_NSESESSION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_NSESESSION_HPP_

#include <IExchangeMessageConverter.hpp>
#include <ExchangeSession.hpp>
#include <NSEMessage.hpp>
#include <NSEOrderManager.hpp>

namespace oms{

class NSESession : public ExchangeSession
{

protected:

	ITradeDefinitionPtr _tradeDef;
	obLib::SpinLock _spinMutex;
	obLib::SpinLock _spinIncomingMsgQMutex;
	std::string _senderSubId;
	std::string _memberName;
	std::string _user;
	std::string _password;
	std::string _configPx;
	obLib::SeqNo _outSeqNo;
	int _hbInterval;
	int _seqNoOffset;

	typedef void(NSESession::*NSESessionMessageHandler)(NSEMessagePtr);
	typedef std::unordered_map<int, NSESessionMessageHandler> MsgHandlerCont;
	MsgHandlerCont _msgHandler;

	IExchangeMessageConverterPtr _messageConverter;

private:
	std::deque<NSEOrderManagerPtr> _orderManagers;
	boost::asio::io_service _ioService;
	boost::asio::io_service::work _ioWork;
	boost::shared_ptr<boost::thread> _worker;

	typedef std::map<obLib::SeqNo, NSEMessagePtr> InComingMsgQ;
	InComingMsgQ _incomingMsgQ;

public:
	NSESession(const std::string& name);
	virtual ~NSESession();

	virtual void init(const std::string& strConfig);
	virtual void shutdown();

	virtual IOrderManagerPtr getOrderManager(size_t symbolNum);
	virtual size_t registerSymbol(const ISymbolDefinitionPtr &);

	virtual bool sendRaw(char * buf, size_t bufSize);

	virtual bool send(IOrderPtr order){
		return order->send(this);
	}
	virtual bool send(ICancelOrderPtr cancelOrder){
		return cancelOrder->send(this);
	}
	virtual bool send(IReplaceOrderPtr replaceOrder){
		return replaceOrder->send(this);
	}

protected:

	void processIncomingMsgQ(){
		try{
			NSEMessagePtr msg;
			while(retreiveMsgFromIncomingQ(getNextInSeqNum(), msg)){
				char msgType = msg->getMsgType();
				// handle this msg
				(this->*(_msgHandler[msgType]))(msg);
			}
		}catch(...){

		}
	}

	void queueIncomingMsg(obLib::SeqNo seqNo, NSEMessagePtr msg){
		obLib::SpinGuard lock(_spinIncomingMsgQMutex);
		_incomingMsgQ[seqNo] = msg;
	}

	bool retreiveMsgFromIncomingQ(obLib::SeqNo seqNo, NSEMessagePtr& msg){
		obLib::SpinGuard lock(_spinIncomingMsgQMutex);
		InComingMsgQ::iterator it = _incomingMsgQ.find(seqNo);
		if(it != _incomingMsgQ.end()){
			msg = it->second;
			_incomingMsgQ.erase(it);
			return true;
		}
		return false;
	}

	void clearIncomingMsgQ(){
		_incomingMsgQ.clear();
	}

	virtual void disconnect();
	virtual size_t fillLogonMsg(char* buf, size_t len) ;
	virtual size_t fillLogoutMsg(char* buf, size_t len);
	virtual size_t fillHeartbeatMsg(char* buf, size_t len);
	virtual size_t fillTestReqMsg(char* buf, size_t len) ;

	virtual bool isFullMsg(const char* buf, size_t total, size_t& len) ;
	virtual void processInMessage(const char* buf, size_t len) ;


	void fillMsgField(char * buff, int offset, int bytes, void* data);

	inline obLib::SeqNo getNextOutgoingSeqNo() {
		return _outSeqNo++;
	}


	void inithandlers();
	bool sendRawFillOrigSendingTime(char * buf, size_t bufSize, size_t origSendingTimeOffset);

	size_t addOrderManager(NSEOrderManagerPtr orderManager){
		_orderManagers.push_back(orderManager);
		return _orderManagers.size() - 1;
	}

	void notifySessionState(bool isOpen);

	void processOrderAck(NSEMessagePtr msg);
	void processLogonRes(NSEMessagePtr msg);
	void processLogoutRes(NSEMessagePtr msg);
	void processErrorRes(NSEMessagePtr msg);
	void processHeartBeat(NSEMessagePtr msg);
	void processTrade(NSEMessagePtr msg);
	void processReject(NSEMessagePtr msg);

};
using NSESessionPtr = std::shared_ptr<NSESession>;


}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_NSESESSION_HPP_ */
