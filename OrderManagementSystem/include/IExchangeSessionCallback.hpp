/*
 * IExchangeSessionCallback.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSIONCALLBACK_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSIONCALLBACK_HPP_

#include <Common.hpp>

#include <IExchangeMessage.hpp>

namespace oms{

class IExchangeSessionCallback
{
protected:
	IExchangeSessionCallback(){

	}
	IExchangeSessionCallback(const IExchangeSessionCallback&){

	}

public:
	virtual ~IExchangeSessionCallback(){

	}

	virtual void onSessionOpen() = 0;
	virtual void onSessionClose() = 0;

	virtual void onOrderAck(const IOrderAckPtr&) = 0;
	virtual void onOrderReject(const IOrderRejectPtr&) = 0;
	virtual void onTrade(const ITrade::SharedPtr&) = 0;
};

}




#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGESESSIONCALLBACK_HPP_ */
