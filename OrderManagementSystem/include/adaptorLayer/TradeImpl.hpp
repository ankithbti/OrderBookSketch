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

};
}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_TRADEIMPL_HPP_ */
