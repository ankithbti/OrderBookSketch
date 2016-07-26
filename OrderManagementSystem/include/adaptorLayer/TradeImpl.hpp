/*
 * TradeImpl.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_TRADEIMPL_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_TRADEIMPL_HPP_

#include <adaptorLayer/MessageInterface.hpp>
#include <IExchangeMessage.hpp>

// Implement Itrade
namespace oms{

class TradeImpl : TradeI{

	ITradePtr _trade;

public:

	TradeImpl(ITradePtr trade) : _trade(trade){

	}

	virtual TradeMsgType getTradeMsgType(){
		return OPTION_TRADE;
	}

	virtual double getPrice() {
		return _trade->getPrice();
	}
	virtual int getQty(){
		return _trade->getQty();
	}
	virtual TradeSide geSide() {
		return _trade->getSide();
	}

	virtual TradeStatus getTradeStatus() {
		return CONFIRMED;
	}
	virtual TradeSource getTradeSource() {
		return TRADE_GUI;
	}

	virtual std::string getExchangeInstId() {
		return "DEFAULT";
	}
	virtual std::string getExchangeProductId() {
		return "PRODID";
	}

	virtual std::string getSpreadId() {
		return "SPREADID";
	}


	virtual int getCumulativeQty() {
		return 0;
	}

	virtual std::string getClientOrderId() {
		return "COID";
	}
	virtual std::string getLocalReferenceId() {
		return "LRID";
	}

	virtual std::string getExchnageOrderId() {
		return "EXCHOID";
	}

	// in case of Trade Burts/Rectify
	virtual std::string getParentExchnageTradeRef() {
		return "PETREFID";
	}


	virtual std::string getexchangeTradeReference() {
		return "DEFAULT";
	}

	virtual std::string getContraFirm() {
		return "DEFAULT";
	}
	virtual std::string getExecBroker() {
		return "DEFAULT";
	}

	virtual std::string getAccount(){
		return "DEFAULT";
	}

	virtual long long getExchangeTradeTime() {
		return 0;
	}

	virtual std::string getExchangeMessage() {
		return "DEFAULT";
	}

	virtual std::string getLiquidityIndicator() {
		return "";
	}

	virtual std::string getContraPartyType(){
		return "";
	}

	virtual std::string getExecutionId(){
		return "";
	}

};
}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_TRADEIMPL_HPP_ */
