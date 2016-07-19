/*
 * ProductOrderMessageImpl.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_PRODUCTORDERMESSAGEIMPL_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_PRODUCTORDERMESSAGEIMPL_HPP_


#include <IOrderManager.hpp>
#include <adaptorLayer/MessageInterface.hpp>

namespace oms{

class ProductOrderMessageImpl : public std::enable_shared_from_this<ProductOrderMessageImpl>,
public ProductOrderMessageI
{
	ProductOrderMessageImpl& operator=(const ProductOrderMessageImpl&);

	IOrderManagerPtr _orderManager;
	std::string _symbol;
	obLib::OrderId _clientOrderId;
	OrderSide _side;
	bool _manualOrderIndicator;
	double _price;
	int _qty;
	struct timeval _tv;
	struct timeval _startTV;
	struct timeval _endTV;

public:

	virtual ~ProductOrderMessageImpl(){

	}

	ProductOrderMessageImpl(IOrderManagerPtr om, const std::string& symbol, OrderSource isManual);

	ProductOrderMessageImpl(const ProductOrderMessageImpl& rhs);

	unsigned long long getElapsedTimeInUSec();

	unsigned long long getElapsedSendTimeInUSec();

	IOrderManagerPtr getOrderManager() const;

	virtual void setOrderId(int orderId) ;
	virtual void setSide(OrderSide side) ;
	virtual void setType(OrderType type) ;
	virtual void setRestriction(TimeInForce tif) ;
	virtual void setPrice(double price) ;
	virtual void setQty(int qty) ;
	virtual void setResevedQty(int qty) ;
	virtual void setSellShort(bool sellShort) ;

	virtual void setLocalReference(const std::string& ref) ;
	virtual std::string getLocalReference() ; // might return ClientOrderId as Local ref

	virtual void setClientOrderId(const obLib::OrderId& id) ;
	virtual obLib::OrderId getClientOrderId(); // might return ClientOrderId as Local ref

	virtual void setAcount(const std::string& account) ;
	virtual void setAcountId(int accountId) ;
	virtual void setClearingAccount(const std::string& account) ;

	virtual void setExchangeOrderId(const std::string& oid) ;

	virtual void setExtra(void*);

	virtual void setUserName(const std::string& uname) ;
	virtual void setUserId(int uid) ;

	virtual void setSystemSource(SystemSource source) ;

	// To Some last step processing if needed
	virtual void setComplete();

	void setStartTime();
	void setEndTime();

	OrderSide getSide();
	obLib::Quantity getQty();
	obLib::Price getPrice();
	bool getIsManual();
	TimeInForce getTif();
	bool isCustomerOrFirm();

};
using ProductOrderMessageImplPtr = std::shared_ptr<ProductOrderMessageImpl>;

class ProductOrderPoolI{
public:
	typedef std::shared_ptr<ProductOrderPoolI> SharedPtr;
	virtual ~ProductOrderPoolI(){

	}
	virtual ProductOrderMessageI::SharedPtr getProductOrder() = 0;
};


class ProductOrderMessagePoolImpl : public ProductOrderPoolI{

	ProductOrderMessageImpl::SharedPtr _productOrder;

public:

	ProductOrderMessagePoolImpl(ProductOrderMessageImpl::SharedPtr orderTemplate) : _productOrder(orderTemplate){

	}

	ProductOrderMessageImpl::SharedPtr getProductOrder(){
		return ProductOrderMessageImpl::SharedPtr(new ProductOrderMessageImpl(*_productOrder));
	}

};

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_PRODUCTORDERMESSAGEIMPL_HPP_ */
