/*
 * NSEOrder.hpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_NSEORDER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_NSEORDER_HPP_


#include <BaseOrder.hpp>
#include <NSEOrderManager.hpp>

namespace oms{

class NSEOrder : public BaseOrder{

protected:
	const NSEOrderManagerPtr _manager;
	LocalBufferPtr _localBuffer;

public:
	NSEOrder(NSEOrderManagerPtr manager) : _manager(manager),
	_localBuffer(new LocalBuffer(2048, obLib::ByteOrderConverter::LITTLEENDIAN)){
	}
	virtual ~NSEOrder(){

	}

	virtual bool send(IExchangeSession* session){
		_manager->setTransactTime(_localBuffer);
		return static_cast<ExchangeSession*>(session)->sendRaw(_localBuffer->getBuffer(),
				_localBuffer->getPosition());
	}

	virtual IOrder* clone() const{
		return new NSEOrder(*this);
	}

	virtual void setClientOrderId(obLib::OrderId id){
		_manager->setClientOrderId(_localBuffer, id);
	}
	virtual void setSide(OrderSide side ){
		_manager->setSide(_localBuffer, side);
	}
	virtual void setQty(obLib::Quantity qty){
		_manager->setQty(_localBuffer, qty);
	}
	virtual void setPrice(obLib::Price p){
		_manager->setPrice(_localBuffer, p);
	}
	virtual void setTif(TimeInForce tif){
		_manager->setTimeInForce(_localBuffer, tif);
	}
	virtual void setCustomerOrFirm(bool isCust){
		_manager->setCustOrFirmIndicator(_localBuffer, isCust);
	}
	virtual void setManualOrderIndicator(bool isManual) {
		_manager->setManualIndicator(_localBuffer, isManual);
	}
};
using NSEOrderPtr = std::shared_ptr<NSEOrder>;

class NSECancelOrder : public BaseCancelOrder{

};
using NSECancelOrderPtr = std::shared_ptr<NSECancelOrder>;

class NSEReplaceOrder : public BaseReplaceOrder{

};
using NSEReplaceOrderPtr = std::shared_ptr<NSEReplaceOrder>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_NSEORDER_HPP_ */
