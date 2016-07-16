/*
 * ExchangeSessionFactory.cpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#include <ExchangeSessionFactory.hpp>
#include <NSESession.hpp>

extern "C" oms::ExchangeSessionFactory * createExchangeSessionFactory(){
	return new oms::ExchangeSessionFactory();
}

namespace oms{

ExchangeSessionFactory::ExchangeSessionFactory(){

}

ExchangeSessionFactory::~ExchangeSessionFactory(){

}

void ExchangeSessionFactory::init(const std::string& configStr){

}

IExchangeSessionPtr ExchangeSessionFactory::createSession(const std::string& sessionName, const std::string& sessionType){
	return IExchangeSessionPtr(new NSESession(sessionName));
}


}


