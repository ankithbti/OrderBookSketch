/*
 * EnumTypes.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ENUMTYPES_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ENUMTYPES_HPP_

namespace oms{

enum RejectReason{
	WRONG_PRICE_TICK,
	EXCEED_QTY_ALLOWED,
	EXCEED_FLOW_CONTROL_LIMIT
};

enum OrderSide{
	BUY=0,
	SELL=1,
	SELL_SHORT=2
};

enum TimeInForce{
	DAY,
	GTC,
	OPG, // At the Opening
	IOC,
	FOK,
	GTX, // good till crossing
	GTD
};

enum OrderSource{
	MANUAL,
	AUTO
};

enum OrderType{
	MARKET,
	LIMIT
};

enum ProductType{
	UNKNOWN,
	OPTION,
	SPREAD,
	FUTURE,
	INSTRUMENT
};

enum SystemSource{
	MANUAL,
	ATM
};

enum SessionType{
	ORDER_OUTRIGHT,
	ORDER_SPREAD,
	QUOTE_OUTRIGHT, // may be in future
	QUOTE_SPREAD
};

enum SessionStatus{

	CONNECTED,
	DISCONNECTED
};

enum DisconnectReason{
	FAILED
};

enum ConnectionType{
	ANY,
	PULL,
	ORDER,
	QUOTE,
	AUCTION
};

enum TradeStatus{
	CONFIRMED,
	BUST,
	RECTIFY,
	REPLAY
};
enum OrderOperation{
	NEW,
	MODIFY,
	DELETE
};
enum OrderStatus{
	LIVE,
	DELETED
};


enum TradeMsgType{
	INSTRUMENT_TRADE,
	OPTION_TRADE,
	FUTURE_TRADE,
	SPREAD_TRADE,
	SPREAD_LEG_TRADE,
	SPREAD_LEG_OPTION_TRADE,
	SPREAD_LEG_FUTURE_TRADE,
	SPREAD_LEG_INSTRUMENT_TRADE,
};

enum TradeSource{
	QUOTE,
	ATM,
	GUI,
	EXCHANGE
};

enum TradeSide{
	BUY,
	SELL,
	SHORT_SELL,
	UNKNOWN
};

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ENUMTYPES_HPP_ */
