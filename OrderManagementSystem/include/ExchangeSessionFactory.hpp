/*
 * ExchangeSessionFactory.hpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGESESSIONFACTORY_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGESESSIONFACTORY_HPP_


#include <IExchangeSessionFactory.hpp>

namespace oms{

class ExchangeSessionFactory : public IExchangeSessionFactory{
public:
	ExchangeSessionFactory();
	~ExchangeSessionFactory();

	virtual void init(const std::string& configStr);
	virtual IExchangeSession::SharedPtr createSession(const std::string& sessionName, const std::string& sessionType);
};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_EXCHANGESESSIONFACTORY_HPP_ */
