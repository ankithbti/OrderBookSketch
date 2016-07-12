/*
 * BaseOrder.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_BASEORDER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_BASEORDER_HPP_

#include <IOrder.hpp>

namespace oms{

class BaseOrderManager;

class BaseOrder : public IOrder{

	friend class BaseOrderManager;
	BaseOrder* _next;
protected:
	BaseOrder() : _next(NULL){

	}
};

class BaseCancelOrder : public ICancelOrder{

	friend class BaseOrderManager;
	BaseCancelOrder* _next;

protected:
	BaseCancelOrder() : _next(NULL){

	}
};

class BaseReplaceOrder : public IReplaceOrder{
	friend class BaseOrderManager;
	BaseReplaceOrder* _next;

protected:
	BaseReplaceOrder() : _next(NULL){

	}
};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_BASEORDER_HPP_ */
