/*
 * IOrderManager.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IORDERMANAGER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IORDERMANAGER_HPP_

#include <Common.hpp>
#include <IOrder.hpp>
#include <IOrderPtrType.hpp>

namespace oms{

using IOrderPtr = IOrderPtrType<IOrder>;
using ICancelOrderPtr = IOrderPtrType<ICancelOrder>;
using IReplaceOrderPtr = IOrderPtrType<IReplaceOrder>;

class IOrderManager : public std::enable_shared_from_this<IOrderManager>
{
protected:
	IOrderManager(){

	}

	IOrderManager(const IOrderManager&){

	}

public:
	virtual ~IOrderManager(){

	}

	virtual IOrderPtr getOrder() = 0;
	virtual ICancelOrderPtr getCancelOrder() = 0;
	virtual IReplaceOrderPtr getReplaceOrder() = 0;

	virtual void putOrder(IOrder*) = 0;
	virtual void putCancelOrder(ICancelOrder*) = 0;
	virtual void putReplaceOrder(IReplaceOrder*) = 0;

};

using IOrderManagerPtr = IOrderManager*;


}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IORDERMANAGER_HPP_ */
