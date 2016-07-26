/*
 * SessionListenerI.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONLISTENERI_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONLISTENERI_HPP_

#include <adaptorLayer/EnumTypes.hpp>
#include <log.hpp>

namespace oms{

class SessionListenerI {
public:

	typedef std::shared_ptr<SessionListenerI> SharedPtr;

	virtual ~SessionListenerI(){

	}

	virtual void release(){

	}

	virtual void onConnect(ConnectionType type, int connectionHandle) = 0;
	virtual void onDisconnect(ConnectionType type, int connectionHandle, DisconnectReason reason) = 0;
	virtual void onSessionUp(int totalConnections) = 0;
	virtual void onSessionDown() = 0;
	virtual void onOrderDelete(const std::string& exchangeOrderId) = 0;
	virtual void onOrderTrade(const TradeI::SharedPtr& trade ) = 0;

};

class DummySessionListener : public SessionListenerI{
public:
	virtual void onConnect(ConnectionType type, int connectionHandle) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onDisconnect(ConnectionType type, int connectionHandle, DisconnectReason reason) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onSessionUp(int totalConnections) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onSessionDown() {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onOrderDelete(const std::string& exchangeOrderId) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onOrderTrade(const TradeI::SharedPtr& trade ) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	};
};
using DummySessionListenerPtr = std::shared_ptr<DummySessionListener>;

class ReqListenerI{
public:
	typedef std::shared_ptr<ReqListenerI> SharedPtr;
	virtual ~ReqListenerI(){
	}

	// Placed Order on wire successfully
	virtual void onSendOk() = 0;
	virtual void onSendFailed(int errCode, const std::string& errMsg) = 0;
	virtual void onResponse(bool isFinal, const IncomingMessageIPtr& msg) = 0;
	virtual void onReject(bool isFinal, SessionStatus, RejectReason, const IncomingMessageIPtr& msg) = 0;
};

class DummyReqListener : public ReqListenerI{
public:
	virtual void onSendOk() {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onSendFailed(int errCode, const std::string& errMsg) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onResponse(bool isFinal, const IncomingMessageIPtr& msg) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual void onReject(bool isFinal, SessionStatus, RejectReason, const IncomingMessageIPtr& msg) {
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
};




}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONLISTENERI_HPP_ */
