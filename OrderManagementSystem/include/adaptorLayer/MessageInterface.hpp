/*
 * MessageInterface.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MESSAGEINTERFACE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MESSAGEINTERFACE_HPP_

#include <Common.hpp>

namespace oms{

class MessageI{
public:
	virtual ~MessageI(){

	}
};

class IncomingMessageI : public MessageI{
public:

};
using IncomingMessageIPtr = std::shared_ptr<IncomingMessageI>;

class OutgoingMessageI : public MessageI{
public:

};
using OutgoingMessageIPtr = std::shared_ptr<OutgoingMessageI>;

class InstrumentOrderMessageI : OutgoingMessageI{
public:
	typedef std::shared_ptr<InstrumentOrderMessageI> SharedPtr;

	virtual void setOrderId(int orderId) = 0;
	virtual void setSide(OrderSide side) = 0;
	virtual void setType(OrderType type) = 0;
	virtual void setRestriction(TimeInForce tif) = 0;
	virtual void setPrice(double price) = 0;
	virtual void setQty(int qty) = 0;
	virtual void setResevedQty(int qty) = 0;
	virtual void setSellShort(bool sellShort) = 0;

	virtual void setLocalReference(const std::string& ref) = 0;
	virtual std::string getLocalReference() = 0; // might return ClientOrderId as Local ref

	virtual void setClientOrderId(const std::string& id) = 0;
	virtual std::string getClientOrderId() = 0; // might return ClientOrderId as Local ref

	virtual void setAcount(const std::string& account) = 0;
	virtual void setAcountId(int accountId) = 0;
	virtual void setClearingAccount(const std::string& account) = 0;

	virtual void setExchangeOrderId(const std::string& oid) = 0;

	virtual void setExtra(void*);

	virtual void setUserName(const std::string& uname) = 0;
	virtual void setUserId(int uid) = 0;

	virtual void setSystemSource(SystemSource source) = 0;

	// To Some last step processing if needed
	virtual void setComplete() = 0;

};

class InstrumentOrderAckMessageI : public IncomingMessageI{

public:
	typedef std::shared_ptr<InstrumentOrderAckMessageI> SharedPtr;

	virtual OrderStatus getOrderStatus() = 0; // Live/Deleted
	virtual std::string getClientOrderId() = 0;
	virtual std::string getExchOrderId() = 0;
};

class InstrumentOrderRejectMessageI : public IncomingMessageI{
public:
	typedef std::shared_ptr<InstrumentOrderRejectMessageI> SharedPtr;

	virtual OrderStatus getOrderStatus() = 0; // Live/Deleted
	virtual std::string getExchangeRejectCode() = 0;
	virtual std::string getRejectReason() = 0;
};


class ProductOrderMessageI : public OutgoingMessageI{
public:
	typedef std::shared_ptr<ProductOrderMessageI> SharedPtr;

	virtual void setOrderId(int orderId) = 0;
	virtual void setSide(OrderSide side) = 0;
	virtual void setType(OrderType type) = 0;
	virtual void setRestriction(TimeInForce tif) = 0;
	virtual void setPrice(double price) = 0;
	virtual void setQty(int qty) = 0;
	virtual void setResevedQty(int qty) = 0;
	virtual void setSellShort(bool sellShort) = 0;

	virtual void setLocalReference(const std::string& ref) = 0;
	virtual std::string getLocalReference() = 0; // might return ClientOrderId as Local ref

	virtual void setClientOrderId(const obLib::OrderId& id) = 0;
	virtual obLib::OrderId getClientOrderId() = 0; // might return ClientOrderId as Local ref

	virtual void setAcount(const std::string& account) = 0;
	virtual void setAcountId(int accountId) = 0;
	virtual void setClearingAccount(const std::string& account) = 0;

	virtual void setExchangeOrderId(const std::string& oid) = 0;

	virtual void setExtra(void*);

	virtual void setUserName(const std::string& uname) = 0;
	virtual void setUserId(int uid) = 0;

	virtual void setSystemSource(SystemSource source) = 0;

	// To Some last step processing if needed
	virtual void setComplete() = 0;

};

class ProductOrderAckMessageI : public IncomingMessageI{
public:
	typedef std::shared_ptr<ProductOrderAckMessageI> SharedPtr;

	virtual OrderStatus getOrderStatus() = 0; // Live/Deleted
	virtual std::string getClientOrderId() = 0;
	virtual std::string getExchOrderId() = 0;
};

class ProductOrderRejectMessageI : public IncomingMessageI{
public:
	typedef std::shared_ptr<ProductOrderRejectMessageI> SharedPtr;

	virtual OrderStatus getOrderStatus() = 0; // Live/Deleted
	virtual std::string getExchangeRejectCode() = 0;
	virtual std::string getRejectReason() = 0;
};

class TradeI : public IncomingMessageI{
public:
	typedef std::shared_ptr<TradeI> SharedPtr;

	virtual TradeMsgType getTradeMsgType() = 0;
	virtual TradeStatus getTradeStatus() = 0;
	virtual TradeSource getTradeSource() = 0;

	virtual std::string getExchangeInstId() = 0;
	virtual std::string getExchangeProductId() = 0;

	virtual std::string getSpreadId() = 0;

	virtual double getPrice() = 0;
	virtual int getQty() = 0;
	virtual TradeSide geSide() = 0;

	virtual int getCumulativeQty() = 0;

	virtual std::string getClientOrderId() = 0;
	virtual std::string getLocalReferenceId() = 0;

	virtual std::string getExchnageOrderId() = 0;

	// in case of Trade Burts/Rectify
	virtual std::string getParentExchnageTradeRef() = 0;


	virtual std::string getexchangeTradeReference() = 0;

	virtual std::string getContraFirm() = 0;
	virtual std::string getExecBroker() = 0;

	virtual std::string getAccount() = 0;

	virtual long long getExchangeTradeTime() = 0;

	virtual std::string getExchangeMessage() = 0;

	virtual std::string getLiquidityIndicator() {
		return "";
	}

	virtual std::string getContraPartyType(){
		return "";
	}

	virtual std::string getExecutionId(){
		return "";
	}
};

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MESSAGEINTERFACE_HPP_ */
