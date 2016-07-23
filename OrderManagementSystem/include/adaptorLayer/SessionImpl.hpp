/*
 * SessionImpl.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONIMPL_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONIMPL_HPP_


#include <adaptorLayer/Dictionary.hpp>
#include <IExchangeSessionCallback.hpp>
#include <adaptorLayer/EnumTypes.hpp>
#include <adaptorLayer/SessionListenerI.hpp>
#include <adaptorLayer/SymbolDefinitionFactory.hpp>
#include <adaptorLayer/DictionaryImpl.hpp>

namespace oms{

class OrderSessionI{
public:
	virtual ~OrderSessionI(){

	}

	// The upper layer [ Strategy ] will call willSend to notify the session layet that an order
	// can be sent in future for these Dictionaries...be Prepared
	virtual void willSendOrder(const DictionaryInstrumentPtr& ) = 0;
	virtual void willSendOrder(const DictionaryProductPtr& ) = 0;

	// Subscription for getting trades is on Instrument Level
	// Should be subscribe before connection is made
	virtual void subscribeOrderTrade(const DictionaryProductPtr&) = 0;
	virtual void subscribeOrderTrade(const DictionaryInstrumentPtr&) = 0;

	// might be useful in future
	virtual void startInstrumentOrder(const DictionaryInstrumentPtr& inst,
			const ReqListenerI::SharedPtr&) = 0;
	virtual void stopInstrumentOrder(const DictionaryInstrumentPtr& inst,
			const ReqListenerI::SharedPtr&) = 0;

	virtual void sendProductOrder( bool isAtmOrder, OrderOperation,
			const ProductOrderMessageI::SharedPtr&, const ReqListenerI::SharedPtr&) = 0;
	virtual void sendInstrumentOrder( bool isAtmOrder, OrderOperation,
			const InstrumentOrderMessageI::SharedPtr&, const ReqListenerI::SharedPtr&) = 0;

	virtual void sendCreateProductRequest(const bool isManual, const DictionaryProductPtr&,
			const ReqListenerI::SharedPtr&) = 0;
};

class QuoteSessionI{
public:
	virtual ~QuoteSessionI(){

	}
};

class SessionI{
public:
	virtual ~SessionI(){

	}

	virtual int addConnection(ConnectionType, const std::string& ip, const std::string& port,
			const std::string& traderId, const std::string& userId, const std::string& password,
			const std::string& configProperties) = 0;

	virtual void connect(int id = -1) = 0;
	virtual void disconnect(int id = -1) = 0;

	virtual OrderSessionI* getOrderSession() = 0;

	// For Future
	virtual QuoteSessionI* getQuoteSession() = 0;

};

class SessionImpl : public SessionI, public OrderSessionI, public IExchangeSessionCallback{

	IExchangeSessionPtr _session;
	SessionType _sessionType;
	SessionListenerI * _sessionListener;
	bool _isSessionOpen;

	std::string _clientOrderId;
	SymbolDefFactoryPtr _symDefFactory;

public:
	SessionImpl(IExchangeSessionPtr session, SessionType type, SessionListenerI* sl,
			const std::string& clOrderIdFile,
			SymbolDefFactoryPtr symbDefFactory) :
				_session(session),
				_sessionType(type),
				_sessionListener(sl),
				_isSessionOpen(false),
				_symDefFactory(symbDefFactory){
		_session->setSessionCallback(this);
	}
	virtual ~SessionImpl(){
		_session->shutdown();
	}

	////////////////////////////
	// Overridden functions from SessionI
	////////////////////////////
	virtual int addConnection(ConnectionType, const std::string& ip, const std::string& port,
			const std::string& traderId, const std::string& userId, const std::string& password,
			const std::string& configProperties){
		_session->start();
		return 0;
	}
	virtual void connect(int id = -1){
		// Nothing to do here as of now
	}
	virtual void disconnect(int id = -1){
		// Nothing to do here as of now
	}
	virtual OrderSessionI* getOrderSession(){
		return this;
	}
	virtual QuoteSessionI* getQuoteSession() {
		return NULL;
	}

	////////////////////////////
	// Overriden functions from OrderSessionI
	////////////////////////////
	// The upper layer [ Strategy ] will call willSend to notify the session layet that an order
	// can be sent in future for these Dictionaries...be Prepared
	virtual void willSendOrder(const DictionaryInstrumentPtr& ) {
		// Nothing to do here as of now
	}
	virtual void willSendOrder(const DictionaryProductPtr& ){
		// Nothing to do here as of now
	}

	virtual void subscribeOrderTrade(const DictionaryInstrumentPtr&){
		// Nothing to do here as of now
	}
	virtual void subscribeOrderTrade(const DictionaryProductPtr&){
		// Nothing to do here as of now
	}
	virtual void startInstrumentOrder(const DictionaryInstrumentPtr& inst,
			const ReqListenerI::SharedPtr&) {
		// Nothing to do here as of now
	}
	virtual void stopInstrumentOrder(const DictionaryInstrumentPtr& inst,
			const ReqListenerI::SharedPtr&) {
		// Nothing to do here as of now
	}
	virtual void sendProductOrder( bool isAtmOrder, OrderOperation op,
			const ProductOrderMessageI::SharedPtr& order, const ReqListenerI::SharedPtr& listener) {

		if(!_isSessionOpen){
			if(listener){
				listener->onSendFailed(0, "Session is not open yet.");
			}
			return;
		}

		switch(op){
		case NEW:
			sendNewProductOrder(order, listener);
			break;
		case MODIFY:
			sendModifyProductOrder(order, listener);
			break;
		case DELETE:
			sendCancelProductOrder(order, listener);
			break;
		default:
			break;
		}
	}


	virtual void sendInstrumentOrder( bool isAtmOrder, OrderOperation op,
			const InstrumentOrderMessageI::SharedPtr& order, const ReqListenerI::SharedPtr& listener) {
		// To Do - future
	}


	virtual void sendCreateProductRequest(const bool isManual,
			const DictionaryProductPtr&, const ReqListenerI::SharedPtr&) ;
	////////////////////////////

	////////////////////////////
	// Overrideb functions from IExchangeSessioncallback
	////////////////////////////
	virtual void onSessionOpen(){
		if(!_isSessionOpen){
			_sessionListener->onConnect(ANY, 1);
			_sessionListener->onSessionUp(1);
			_isSessionOpen = true;
		}
	}
	virtual void onSessionClose() {
		if(_isSessionOpen){
			_sessionListener->onSessionDown();
			_sessionListener->onDisconnect(ANY, 1, FAILED);
			_isSessionOpen  = false;
		}
	}
	virtual void onOrderAck(const IOrderAckPtr& ack) {
		// Nothing to do here as of now
	}
	virtual void onOrderReject(const IOrderRejectPtr& reject){
		// Nothing to do here as of now
	}
	virtual void onTrade(const ITradePtr& trade){
		TradeImpl::SharedPtr tradeImpl(new TradeImpl(trade));
		_sessionListener->onOrderTrade(tradeImpl);
	}

	/// Helper functions
	ProductOrderMessageI::SharedPtr createProductOrderMessage(const DictionaryProductPtr& prod,
			OrderOperation op, TimeInForce tif, OrderSource source){
		ISymbolDefinitionPtr symDef = _symDefFactory->createSymDef(prod);
		size_t symbolId = _session->registerSymbol(symDef);

		IOrderManagerPtr orderMgr = _session->getOrderManager(symbolId);
		return ProductOrderMessageI::SharedPtr(new ProductOrderMessageImpl(orderMgr, symDef->getSymbol(), source));
	}

protected:

	virtual void sendNewProductOrder(const ProductOrderMessageI::SharedPtr& order,
			const ReqListenerI::SharedPtr& listener){

		ProductOrderMessageImplPtr productOrder = std::static_pointer_cast<ProductOrderMessageImpl>(order);

		obLib::OrderId uniqueOId = 1;
		productOrder->setClientOrderId(uniqueOId);

		IOrderPtr templateOrder = productOrder->getOrderManager()->getOrder();

		templateOrder->setClientOrderId(productOrder->getClientOrderId());

		productOrder->setStartTime();

		templateOrder->setSide(productOrder->getSide());
		templateOrder->setQty(productOrder->getQty());
		templateOrder->setPrice(productOrder->getPrice());
		templateOrder->setManualOrderIndicator(productOrder->getIsManual());
		templateOrder->setTif(productOrder->getTif());
		templateOrder->setCustomerOrFirm(productOrder->isCustomerOrFirm());


		if(_session->send(templateOrder)){
			productOrder->setEndTime();

			if(listener){
				listener->onSendOk();
			}
		}else{
			if(listener){
				listener->onSendFailed(0, "Send call failed.");
			}
		}

	}
	virtual void sendModifyProductOrder(const ProductOrderMessageI::SharedPtr& order,
			const ReqListenerI::SharedPtr& listener){
		// To Do - future
	}
	virtual void sendCancelProductOrder(const ProductOrderMessageI::SharedPtr& order,
			const ReqListenerI::SharedPtr& listener){
		// To Do - future
	}

};
using SessionImplPtr = std::shared_ptr<SessionImpl>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SESSIONIMPL_HPP_ */
