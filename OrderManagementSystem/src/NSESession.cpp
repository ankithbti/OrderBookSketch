/*
 * NSESession.cpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#include <NSESession.hpp>
#include <NSEMessageConverter.hpp>

namespace oms{

NSESession::NSESession(const std::string& name) : ExchangeSession(name),
		_ioWork(_ioService),
		_outSeqNo(1)
{
	inithandlers();
	_worker.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &_ioService)));
}


void NSESession::inithandlers(){
	_msgHandler[1001] = &NSESession::processLogonRes;
	_messageConverter.reset(new NSEMessageConverter());
}

NSESession::~NSESession(){
	_ioService.poll();
	_ioService.stop();
	_worker->join();
}

void NSESession::init(const std::string& configStr) : _configPx(configStr){
	// UserName
	// Pass
	// TraderName
	// HB Interval
	// Account
	// Self Matching Prev ID
	// TraderID
	// MemberName
}

void NSESession::shutdown(){
	sendLogout();
}

IOrderManagerPtr NSESession::getOrderManager(size_t symbolNum){
	return _orderManagers[symbolNum];
}

size_t NSESession::registerSymbol(const ISymbolDefinitionPtr & symDef ){
	NSEOrderManagerPtr om(new NSEOrderManager(this, symDef, _tradeDef));
	if(!om){
		throw std::runtime_error("NSESession::registerSymbol() - faled to register symbol.");
	}
	om->init(_configPx);
	return addOrderManager(om);
}

bool NSESession::sendRaw(char * buf, size_t bufSize){
	// Locking the Socket for sending packet
	obLib::SpinGuard lock(_spinMutex);
	unsigned long seqNum = getAndIncrNextOutSeqNum();
	fillMsgField(buf, _seqNoOffset, getSeqNoLen(), (void*)&seqNum);

	return sendMsg(buf, bufSize);
}

size_t NSESession::fillLogonMsg(char* buf, size_t len){
	// Create the LogonReq of NSE [ Application Level Message ]
	// Serialise / Marshall it
	// return the size of created buffer
}

size_t NSESession::fillLogoutMsg(char* buf, size_t len){

}

size_t NSESession::fillHeartbeatMsg(char* buf, size_t len){

}

size_t NSESession::fillTestReqMsg(char* buf, size_t len) {

}

bool NSESession::isFullMsg(const char* buf, size_t total, size_t& len){
	// Get the Len and check if we have full msg
	return false; // true
}

void NSESession::processInMessage(const char* buf, size_t len){

	try{
		// After we got full msg, this function will create the NSEMessage
		NSEMessagePtr msg = std::static_pointer_cast<NSEMessage>(_messageConverter->convert(buf, len));

		if(msg){
			int msgType = msg->getMsgType();
			int errCode = msg->getErrCode();
			// If no err Code , then depending upon the MsgType, call the respective handler
			MsgHandlerCont::iterator it = _msgHandler.find(msgType);
			if(it != _msgHandler.end()){
				(this->*(it->second))(msg);
			}
		}
	}catch(...){

	}
}


void NSESession::fillMsgField(char * buf, int offset, int bytes, void* data){
	char *p = (char*)data;
	int pos = offset;
	for(int i = 0; i < bytes ; ++i){
		buf[pos] = *p;
		++pos;
	}
}


void NSESession::disconnect(){
	ExchangeSession::disconnect();
	_ioService.post(boost::bind(&NSESession::notifySessionState, this, false));
}

bool NSESession::sendRawFillOrigSendingTime(char * buf, size_t bufSize, size_t origSendingTimeOffset){
	// To Do - might not needed
}

void NSESession::notifySessionState(bool isOpen){
	if(isOpen){
		if(_sessionCallback){
			_sessionCallback->onSessionOpen();
		}
	}else{
		if(_sessionCallback){
			_sessionCallback->onSessionClose();
		}
	}
}

void NSESession::processOrderAck(NSEMessagePtr msg){

}

void NSESession::processLogonRes(NSEMessagePtr msg){

}
void NSESession::processLogoutRes(NSEMessagePtr msg){

}
void NSESession::processErrorRes(NSEMessagePtr msg){

}
void NSESession::processHeartBeat(NSEMessagePtr msg){

}
void NSESession::processTrade(NSEMessagePtr msg){

}
void NSESession::processReject(NSEMessagePtr msg){

}




}


