/*
 * BaseOrderManager.cpp
 *
 *  Created on: 13-Jul-2016
 *      Author: ankithbti
 */

#include <BaseOrderManager.hpp>

namespace oms{

BaseOrderManager::BaseOrderManager() : _orderList(NULL),
		_cancelOrderList(NULL),
		_replaceOrderList(NULL){

}

BaseOrderManager::~BaseOrderManager(){
	while(_orderList != NULL){
		BaseOrder* order = _orderList;
		_orderList = order->_next;
		delete order;
	}
	while(_cancelOrderList != NULL){
		BaseCancelOrder* order = _cancelOrderList;
		_cancelOrderList = order->_next;
		delete order;
	}
	while(_replaceOrderList != NULL){
		BaseReplaceOrder* order = _replaceOrderList;
		_replaceOrderList = order->_next;
		delete order;
	}
}

void BaseOrderManager::initOrderPool(int orderPoolSize){
	IOrder* order = createOrder();
	putOrder(order);
	for(int i = 1 ; i < orderPoolSize ; ++i){
		putOrder(order->clone());
	}

	ICancelOrder* cancelOrder = createCancelOrder();
	putCancelOrder(cancelOrder);
	for(int i = 1 ; i < orderPoolSize ; ++i){
		putCancelOrder(cancelOrder->clone());
	}

	IReplaceOrder* replaceOrder = createReplaceOrder();
	putReplaceOrder(replaceOrder);
	for(int i = 1 ; i < orderPoolSize ; ++i){
		putReplaceOrder(replaceOrder->clone());
	}

}

IOrderPtr BaseOrderManager::getOrder(){
	obLib::SpinGuard lock(_orderLock);
	BaseOrder* order = _orderList;
	if(order->_next != NULL){
		_orderList = _orderList->_next;
		return IOrderPtr(order, std::bind(&BaseOrderManager::putOrder, this, _1));
	}
	return IOrderPtr(order->clone(), std::bind(&BaseOrderManager::putOrder, this, _1));
}

void BaseOrderManager::putOrder(IOrder* order){
	obLib::SpinGuard lock(_orderLock);
	register BaseOrder* baseOrder = static_cast<BaseOrder *>(order);
	baseOrder->_next = _orderList;
	_orderList = baseOrder;
}

ICancelOrderPtr BaseOrderManager::getCancelOrder(){

}

void BaseOrderManager::putCancelOrder(ICancelOrder*){

}

IReplaceOrderPtr BaseOrderManager::getReplaceOrder(){

}

void BaseOrderManager::putReplaceOrder(IReplaceOrder*){

}

}

