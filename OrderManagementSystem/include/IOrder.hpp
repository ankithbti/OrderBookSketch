/*
 * IOrder.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IORDER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IORDER_HPP_

#include <Common.hpp>

namespace oms{

enum OrderSide{
	BUY=0,
	SELL=1,
	SELL_SHORT=2
};

enum TimeInForce{
	DAY,
	GTC,
	OPG, // At the Opening
	IOC,
	FOK,
	GTX, // good till crossing
	GTD
};

class IExchangeSession;

class IOrder{
protected:
	IOrder(){

	}

	IOrder(const IOrder&){

	}
public:
	virtual ~IOrder(){

	}

	virtual bool send(IExchangeSession* session) = 0;
	virtual IOrder* clone() const  = 0;

	virtual void setClientOrderId(obLib::OrderId) = 0;
	virtual void setSide(OrderSide) = 0;
	virtual void setQty(obLib::Quantity) = 0;
	virtual void setPrice(obLib::Price) = 0;
	virtual void setTif(TimeInForce) = 0;
	virtual void setCustomerOrFirm(bool) = 0;
	virtual void setManualOrderIndicator(bool) = 0;
};

class ICancelOrder{
protected:
	ICancelOrder(){

	}

	ICancelOrder(const ICancelOrder&){

	}
public:
	virtual ~ICancelOrder(){

	}

	virtual bool send(IExchangeSession* session) = 0;
	virtual ICancelOrder* clone() const  = 0;

	virtual void setOrigClientOrderId(obLib::OrderId) = 0;
	virtual void setClientOrderId(obLib::OrderId) = 0;
	virtual void setExchangeOrderId(obLib::OrderId) = 0;
	virtual void setSide(OrderSide) = 0;
};

class IReplaceOrder{
protected:
	IReplaceOrder(){

	}

	IReplaceOrder(const IReplaceOrder&){

	}
public:
	virtual ~IReplaceOrder(){

	}

	virtual bool send(IExchangeSession* session) = 0;
	virtual IReplaceOrder* clone() const  = 0;

	virtual void setOrigClientOrderId(obLib::OrderId) = 0;
	virtual void setClientOrderId(obLib::OrderId) = 0;
	virtual void setExchangeOrderId(obLib::OrderId) = 0;
	virtual void setSide(OrderSide) = 0;
	virtual void setQty(obLib::Quantity) = 0;
	virtual void setPrice(obLib::Price) = 0;
	virtual void setTif(TimeInForce) = 0;

};


}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IORDER_HPP_ */
