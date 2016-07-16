/*
 * NSEOrderManager.cpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */
#include <NSEOrderManager.hpp>
#include <NSESession.hpp>
#include <NSEOrder.hpp>

namespace oms{

NSEOrderManager::NSEOrderManager(NSESession * session, const ISymbolDefinitionPtr& symbolDef, const ITradeDefinitionPtr& tradeDef) :
	_session(session),
	_symbolDef(symbolDef),
	_tradeDef(tradeDef){

}

NSEOrderManager::~NSEOrderManager(){

}

IOrder* NSEOrderManager::createOrder() {
	return new NSEOrder(this);
}

ICancelOrder* NSEOrderManager::createCancelOrder(){

}

IReplaceOrder* NSEOrderManager::createReplaceOrder() {

}

size_t NSEOrderManager::fillHeader(LocalBufferPtr buf, char msgType) const{

}

void NSEOrderManager::setClientOrderId(LocalBufferPtr buf, obLib::OrderId clOrderId){

}

void NSEOrderManager::setPrice(LocalBufferPtr buf, obLib::Price price){

}

void NSEOrderManager::setQty(LocalBufferPtr buf, obLib::Quantity qty){

}

void NSEOrderManager::setSide(LocalBufferPtr buf, OrderSide side){

}

void NSEOrderManager::setTimeInForce(LocalBufferPtr buf, TimeInForce tif){

}

void NSEOrderManager::setTransactTime(LocalBufferPtr buf){

}

void NSEOrderManager::setManualIndicator(LocalBufferPtr buf, bool ind){

}

void NSEOrderManager::setCustOrFirmIndicator(LocalBufferPtr buf, bool ind){

}


}



