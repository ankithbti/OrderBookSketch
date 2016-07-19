/*
 * IExchangeSessionFactory.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSIONFACTORY_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSIONFACTORY_HPP_

#include <Common.hpp>
#include <IExchangeSession.hpp>

namespace oms{
class IExchangeSessionFactory{
protected:
	IExchangeSessionFactory(const IExchangeSessionFactory& ){

	}

	IExchangeSessionFactory(){
	}
public:
	virtual ~IExchangeSessionFactory(){

	}

	virtual void init(const std::string& config) = 0;
	virtual IExchangeSessionPtr createSession(const std::string& sessionName, const std::string& sessionType) = 0;
};
using IExchangeSessionFactoryPtr = std::shared_ptr<IExchangeSessionFactory>;
}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSIONFACTORY_HPP_ */
