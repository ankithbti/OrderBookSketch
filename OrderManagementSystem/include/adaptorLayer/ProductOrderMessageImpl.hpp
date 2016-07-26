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
#include <log.hpp>

namespace oms{

class ProductOrderMessageImpl : public std::enable_shared_from_this<ProductOrderMessageImpl>,
public ProductOrderMessageI
{
	ProductOrderMessageImpl& operator=(const ProductOrderMessageImpl&){
	}

	IOrderManagerPtr _orderManager;
	std::string _symbol;
	OrderSource _orderSource;
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

	ProductOrderMessageImpl(IOrderManagerPtr om, const std::string& symbol, OrderSource isManual) :
		_orderManager(om),
		_symbol(symbol),
		_orderSource(isManual),
		_clientOrderId(0),
		_side(BUY),
		_manualOrderIndicator(false),
		_price(0.0),
		_qty(0)
	{
		CONSOLELOG(__FUNCTION__ << " Entered");
	}

	virtual ProductOrderMessageI::SharedPtr clone() {
		CONSOLELOG(__FUNCTION__ << " Cloning");
		return new ProductOrderMessageImpl(*this);
	}

	ProductOrderMessageImpl(const ProductOrderMessageImpl& rhs) :
		_orderManager(rhs._orderManager),
		_symbol(rhs._symbol),
		_orderSource(rhs._orderSource),
		_clientOrderId(rhs._clientOrderId),
		_side(rhs._side),
		_manualOrderIndicator(rhs._manualOrderIndicator),
		_price(rhs._price),
		_qty(rhs._qty){
		CONSOLELOG(__FUNCTION__ << " Entered");

	}

	unsigned long long getElapsedTimeInUSec(){

	}

	unsigned long long getElapsedSendTimeInUSec(){

	}

	IOrderManagerPtr getOrderManager() const{
		return _orderManager;
	}

	virtual void setOrderId(int orderId){
		_clientOrderId = orderId;
	}
	virtual void setSide(OrderSide side) {
		_side = side;
	}
	virtual void setType(OrderType type) {

	}
	virtual void setRestriction(TimeInForce tif){

	}
	virtual void setPrice(double price){
		_price = price;
	}
	virtual void setQty(int qty){
		_qty = qty;
	}
	virtual void setResevedQty(int qty){

	}
	virtual void setSellShort(bool sellShort){

	}

	virtual void setLocalReference(const std::string& ref){

	}
	// might return ClientOrderId as Local ref
	virtual std::string getLocalReference()  {
		return "";
	}


	virtual void setClientOrderId(const obLib::OrderId& id) {
		_clientOrderId = id;
	}
	virtual obLib::OrderId getClientOrderId(){
		return _clientOrderId;
	}

	virtual void setAcount(const std::string& account) {

	}
	virtual void setAcountId(int accountId) {

	}
	virtual void setClearingAccount(const std::string& account) {

	}

	virtual void setExchangeOrderId(const std::string& oid) {

	}

	virtual void setExtra(void*){

	}

	virtual void setUserName(const std::string& uname) {

	}
	virtual void setUserId(int uid) {

	}

	virtual void setSystemSource(SystemSource source) {

	}

	// To Some last step processing if needed
	virtual void setComplete(){

	}

	void setStartTime(){

	}
	void setEndTime(){

	}

	OrderSide getSide(){
		return _side;
	}
	obLib::Quantity getQty(){
		return _qty;
	}
	obLib::Price getPrice(){
		return _price;
	}
	bool getIsManual(){
		return _manualOrderIndicator;
	}
	TimeInForce getTif(){
		return IOC;
	}
	bool isCustomerOrFirm(){
		return false;
	}

};
using ProductOrderMessageImplPtr = ProductOrderMessageImpl*;

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
		CONSOLELOG(__FUNCTION__ << " Entered");
	}

	ProductOrderMessageImpl::SharedPtr getProductOrder(){
		CONSOLELOG(__FUNCTION__ << " Entered");
		return _productOrder; // pool should be for each options
		//return ProductOrderMessageImpl::SharedPtr(new ProductOrderMessageImpl(*_productOrder));
	}

};

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_PRODUCTORDERMESSAGEIMPL_HPP_ */
