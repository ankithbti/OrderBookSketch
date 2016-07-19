/*
 * SessionListenerI.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONLISTENERI_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONLISTENERI_HPP_

#include <adaptorLayer/EnumTypes.hpp>

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


}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONLISTENERI_HPP_ */
