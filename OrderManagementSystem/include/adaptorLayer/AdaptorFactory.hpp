/*
 * ProductMessageFactory.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ADAPTORFACTORY_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ADAPTORFACTORY_HPP_

#include <adaptorLayer/MessageInterface.hpp>
#include <adaptorLayer/SessionImpl.hpp>
#include <adaptorLayer/ProductOrderMessageImpl.hpp>
#include <ExchangeSessionFactory.hpp>
#include <log.hpp>

/// Have an option of ProductUserData in MarketData to store the ProductOrderPoolI::SharedPtr,
/// which will be set during willSendOrder call [ only once ]
/// and later ( while sending Order ) it will just reach out to that ProductOrderPool and get the PoolOrder and
/// fill that and call sendProductOrder on SessionImpl

namespace oms{

class FactoryI{
public:

	virtual ~FactoryI(){

	}

	virtual SessionI* createSession(const SessionType& type, SessionListenerI* listener,
			const std::string& configProp, const std::string& exchangeTimeZone) = 0;

	virtual ProductOrderMessageI::SharedPtr createProductOrderMessage(SessionI* session,
			const DictionaryProductPtr& prod, OrderOperation op,
			TimeInForce tif, OrderSource source) = 0;

	virtual ProductOrderPoolI::SharedPtr createProductOrderMessagePool(SessionI* session,
			const DictionaryProductPtr& prod,
			const ProductOrderMessageI::SharedPtr& orderTemplate){
		return ProductOrderPoolI::SharedPtr();
	}

	virtual InstrumentOrderMessageI::SharedPtr createInstrumentOrderMessage(SessionI* session,
			const DictionaryInstrumentPtr& prod, OrderOperation op,
			TimeInForce tif, OrderSource source) = 0;

	// May be for Quotes in Future

};


class AdaptorFactory : public FactoryI{
public:
	AdaptorFactory(){
		CONSOLELOG(__FUNCTION__ << " Entered");
	}
	virtual ~AdaptorFactory(){

	}

	virtual SessionI* createSession(const SessionType& type, SessionListenerI* listener,
			const std::string& configProp, const std::string& exchangeTimeZone) {
		CONSOLELOG(__FUNCTION__ << " Entered");
		// Read config parameters
		// clientOrderIdFile [ mmap ],

		SymbolDefFactoryPtr symFactoy(new SymbolDefFactory());
		symFactoy->init(configProp);

		IExchangeSessionFactoryPtr sessionFactory(new ExchangeSessionFactory());
		sessionFactory->init(configProp);

		std::string sessionName, sessionType, clientOrderIdFile;

		IExchangeSession::SharedPtr session = sessionFactory->createSession(sessionName, sessionType);
		if(!session){
			throw std::runtime_error(" Can not create the session. ");
		}
		session->init(configProp);
		return new SessionImpl(session, type, listener, clientOrderIdFile, symFactoy);
	}

	virtual ProductOrderMessageI::SharedPtr createProductOrderMessage(SessionI* session,
			const DictionaryProductPtr& prod, OrderOperation op,
			TimeInForce tif, OrderSource source) {
		CONSOLELOG(__FUNCTION__ << " Entered");
		return static_cast<SessionImpl*>(session)->createProductOrderMessage(prod, op, tif, source);
	}

	virtual ProductOrderPoolI::SharedPtr createProductOrderMessagePool(SessionI* session,
			const DictionaryProductPtr& prod,
			const ProductOrderMessageI::SharedPtr& orderTemplate){
		CONSOLELOG(__FUNCTION__ << " Entered");
		return ProductOrderMessagePoolImpl::SharedPtr(new ProductOrderMessagePoolImpl(
				static_cast<ProductOrderMessageImpl*>(orderTemplate)));
	}

	virtual InstrumentOrderMessageI::SharedPtr createInstrumentOrderMessage(SessionI* session,
			const DictionaryInstrumentPtr& prod, OrderOperation op,
			TimeInForce tif, OrderSource source) {
		CONSOLELOG(__FUNCTION__ << " Entered");
		// Not implemented right now
		return InstrumentOrderMessageI::SharedPtr();
	}
};

using AdaptorFactoryPtr = std::shared_ptr<AdaptorFactory>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ADAPTORFACTORY_HPP_ */
