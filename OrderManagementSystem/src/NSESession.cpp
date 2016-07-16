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


}


