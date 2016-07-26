/*
 * ExchangeSessionFactory.cpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#include <ExchangeSessionFactory.hpp>
#include <NSESession.hpp>
#include <log.hpp>

extern "C" oms::ExchangeSessionFactory * createExchangeSessionFactory(){
	return new oms::ExchangeSessionFactory();
}

namespace oms{

ExchangeSessionFactory::ExchangeSessionFactory(){
	CONSOLELOG(__FUNCTION__ << " Entered");
}

ExchangeSessionFactory::~ExchangeSessionFactory(){
	CONSOLELOG(__FUNCTION__ << " Entered");
}

void ExchangeSessionFactory::init(const std::string& configStr){
	CONSOLELOG(__FUNCTION__ << " Entered");
}

IExchangeSession::SharedPtr ExchangeSessionFactory::createSession(const std::string& sessionName, const std::string& sessionType){
	CONSOLELOG(__FUNCTION__ << " Entered");
	return IExchangeSession::SharedPtr(new NSESession(sessionName));
}


}


