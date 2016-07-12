/*
 * BaseOrderManager.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_BASEORDERMANAGER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_BASEORDERMANAGER_HPP_

#include <IOrderManager.hpp>
#include <BaseOrder.hpp>
#include <containers/SpinLock.hpp>

namespace oms{

class BaseOrderManager : public IOrderManager{

	BaseOrder* _orderList;
	BaseCancelOrder* _cancelOrderList;
	BaseReplaceOrder* _replaceOrderList;

	obLib::SpinLock _orderLock;
	obLib::SpinLock _cancelOrderLock;
	obLib::SpinLock _replaceOrderLock;

public:
	BaseOrderManager();
	virtual ~BaseOrderManager();


	virtual IOrderPtr getOrder() ;
	virtual ICancelOrderPtr getCancelOrder() ;
	virtual IReplaceOrderPtr getReplaceOrder();

protected:

	virtual void putOrder(IOrder*) ;
	virtual void putCancelOrder(ICancelOrder*) ;
	virtual void putReplaceOrder(IReplaceOrder*) ;

	virtual IOrder* createOrder() = 0;
	virtual ICancelOrder* createCancelOrder() = 0;
	virtual IReplaceOrder* createReplaceOrder() = 0;

	void initOrderPool(int orderPoolSize);
};

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_BASEORDERMANAGER_HPP_ */
