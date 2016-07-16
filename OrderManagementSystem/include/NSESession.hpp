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

	virtual size_t fillLogonMsg(char* buf, size_t len) ;
	virtual size_t fillLogoutMsg(char* buf, size_t len);
	virtual size_t fillHeartbeatMsg(char* buf, size_t len);
	virtual size_t fillTestReqMsg(char* buf, size_t len) ;

	virtual bool isFullMsg(const char* buf, size_t total, size_t& len) ;
	virtual void processInMessage(const char* buf, size_t len) ;


	void fillMsgField(const char * buff, int offset, int bytes, void* data);

	inline obLib::SeqNo getNextOutgoingSeqNo() {
		return _outSeqNo++;
	}

	virtual void disconnect();
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

	// To Do

};
using NSESessionPtr = std::shared_ptr<NSESession>;


}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_NSESESSION_HPP_ */
