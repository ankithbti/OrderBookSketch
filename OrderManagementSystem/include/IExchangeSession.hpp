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

	IExchangeSession(){

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

	virtual IOrderManagerPtr getOrderManager(size_t symbolNum) = 0;
};
using IExchangeSessionPtr = std::shared_ptr<IExchangeSession>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSION_HPP_ */
