/*
 * NSEOrderManager.hpp
 *
 *  Created on: 16-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_NSEORDERMANAGER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_NSEORDERMANAGER_HPP_


#include <BaseOrderManager.hpp>
#include <ISymbolDefinition.hpp>
#include <ITradeDefinition.hpp>
#include <LocalBuffer.hpp>

namespace oms{

class NSESession;

class NSEOrderManager : public BaseOrderManager{

protected:
	NSESession * const _session;
	const ISymbolDefinitionPtr _symbolDef;
	const ITradeDefinitionPtr _tradeDef;

	size_t _msgSeqNumOffset;
	size_t _clientOrderIdOffset;
	size_t _qtyOffset;
	size_t _priceOffset;
	size_t _sideOffset;
	size_t _transactTimeOffset;
	size_t _timeInForceOffset;
	size_t _custOrFirmOffset;
	size_t _manualOrAutoOffset;


	int _clientOrderIdLen;
	int _exchOrderIdLen;
	int _qtyLen;
	double _priceMultiplier;
	int _pricePrecision; // _priceFractDigits;

	enum{
		DEFAULT_ORDERPOOL_SIZE = 10,
		DEFAULT_QTY_LEN = 8,
		DEFAULT_PRICE_MULTIPLIER = 1,
		DEFAULT_PRICE_PRECISION = 3
	};


public:
	NSEOrderManager(NSESession * session, const ISymbolDefinitionPtr& symbolDef, const ITradeDefinitionPtr& tradeDef);

	virtual ~NSEOrderManager();

	virtual IOrder* createOrder() ;
	virtual ICancelOrder* createCancelOrder();
	virtual IReplaceOrder* createReplaceOrder() ;

	size_t fillHeader(LocalBufferPtr buf, char msgType) const;
	void setClientOrderId(LocalBufferPtr buf, obLib::OrderId clOrderId);
	void setPrice(LocalBufferPtr buf, obLib::Price price);
	void setQty(LocalBufferPtr buf, obLib::Quantity qty);
	void setSide(LocalBufferPtr buf, OrderSide side);
	void setTimeInForce(LocalBufferPtr buf, TimeInForce tif);
	void setTransactTime(LocalBufferPtr buf);
	void setManualIndicator(LocalBufferPtr buf, bool isManual);
	void setCustOrFirmIndicator(LocalBufferPtr buf, bool isCust);

	const ISymbolDefinitionPtr& getSymbolDef() const {
		return _symbolDef;
	}

	const ITradeDefinitionPtr& getTradeDef() const{
		return _tradeDef;
	}

	int getClientOrderIdLen() const {
		return _clientOrderIdLen;
	}

	int getExchOrderIdLen() const {
		return _exchOrderIdLen;
	}

	int getQtyLen() const {
		return _qtyLen;
	}

	double getPriceMultiplier() const {
		return _priceMultiplier;
	}

	int getPricePrecision() const{
		return _pricePrecision;
	}


	void setMsgSeqNumOffset(size_t offset){
		_msgSeqNumOffset = offset;
	}
	void setClientOrderIdOffset(size_t offset) {
		_clientOrderIdOffset = offset;
	}

	void setSideOffset(size_t offset) {
		_sideOffset = offset;
	}

	void setQtyOffset(size_t offset){
		_qtyOffset = offset;
	}

	void setPriceOffset(size_t offset){
		_priceOffset = offset;
	}

	void setTransactTimeOffset(size_t offset){
		_transactTimeOffset = offset;
	}

	void setTimeInForceOffset(size_t offset){
		_timeInForceOffset = offset;
	}

	void setManualOrAutoIndicatorOffset(size_t offset){
		_manualOrAutoOffset = offset;
	}

	void setCustOrFirmOffset(size_t offset){
		_custOrFirmOffset = offset;
	}

};
using NSEOrderManagerPtr = NSEOrderManager*;

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_NSEORDERMANAGER_HPP_ */
