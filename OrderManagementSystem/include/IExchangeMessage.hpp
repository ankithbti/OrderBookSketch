/*
 * IExchangeMessage.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGEMESSAGE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGEMESSAGE_HPP_

#include <Common.hpp>

namespace oms{

class IExchangeMessage : public std::enable_shared_from_this<IExchangeMessage>
{
protected:
	IExchangeMessage(){

	}

	IExchangeMessage(const IExchangeMessage&){

	}
public:
	virtual ~IExchangeMessage(){

	}
};
using IExchangeMessagePtr = std::shared_ptr<IExchangeMessage>;

class IOrderAck : public IExchangeMessage
{
protected:
	IOrderAck(){

	}

	IOrderAck(const IOrderAck&){

	}
public:
	virtual ~IOrderAck(){

	}

	virtual std::string getClientOrderId() const = 0;
	virtual std::string getExchangeOrderId() const = 0;
};
using IOrderAckPtr = std::shared_ptr<IOrderAck>;

class IOrderReject : public IExchangeMessage
{
protected:
	IOrderReject(){

	}

	IOrderReject(const IOrderReject&){

	}
public:
	virtual ~IOrderReject(){

	}

	virtual std::string getClientOrderId() const = 0;
	virtual std::string getExchangeOrderId() const = 0;

};
using IOrderRejectPtr = std::shared_ptr<IOrderReject>;

class ITrade : public IExchangeMessage
{
protected:
	ITrade(){

	}

	ITrade(const ITrade& ){

	}
public:
	virtual ~ITrade(){

	}

	virtual std::string getClientOrderId() const  = 0;
	virtual std::string getExchangeOrderId() const  = 0;
	virtual std::string getExchangeTradeRef() const  = 0;
	virtual std::string getAccount() const  = 0;
	virtual std::string getExchangeTradeTime() const  = 0;
	virtual std::string getExchangeProductId() const = 0;
	virtual TradeSide getSide() const  = 0;
	virtual double getPrice() const  = 0;
	virtual int getQty() const  = 0;
	virtual char getExecType() const  = 0;
};
using ITradePtr = std::shared_ptr<ITrade>;

}




#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGEMESSAGE_HPP_ */
