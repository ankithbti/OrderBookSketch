/*
 * IExchangeSession.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSION_HPP_

#include <Common.hpp>
#include <IExchangeSessionCallback.hpp>
#include <IOrderManager.hpp>
#include <ISymbolDefinition.hpp>

namespace oms{

class IExchangeSession : public std::enable_shared_from_this<IExchangeSession>
{
protected:
	IExchangeSession(){

	}
	IExchangeSession(const IExchangeSession&){

	}
public:
	typedef std::shared_ptr<IExchangeSession> SharedPtr;

	virtual ~IExchangeSession(){

	}

	virtual const std::string& name() const = 0;
	virtual void init(const std::string& configStr) = 0;
	virtual void setSessionCallback(IExchangeSessionCallback* cb) = 0;

	virtual void start() = 0;
	virtual void shutdown() = 0;

	virtual bool send(IOrderPtr) = 0;
	virtual bool send(ICancelOrderPtr) = 0;
	virtual bool send(IReplaceOrderPtr) = 0;

	virtual size_t registerSymbol(const ISymbolDefinitionPtr &) = 0;

	virtual IOrderManager* getOrderManager(size_t symbolNum) = 0;
};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSION_HPP_ */
