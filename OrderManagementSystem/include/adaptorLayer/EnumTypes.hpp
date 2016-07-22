/*
 * EnumTypes.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ENUMTYPES_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ENUMTYPES_HPP_

namespace oms{

using PktLen = uint16_t; // 2 bytes
using PktSeqNo = uint32_t; // 4 bytes
using PktChkSum = char[16]; // 16 bytes

enum PktOffset{
	PKT_LEN_OFFSET = 0,
	PKT_SEQNO_OFFSET = 2,
	PKT_CHECKSUM_OFFSET = 6
};


using ApiTCode = int16_t; // 2
using ApiFuncId = int16_t; // 2
using LogTime = int32_t; // 4
using AlphaChar = int16_t; // 2
using TransactionCode = int16_t; // 2
using ErrorCode = int16_t; // 2
using TimeStamp = char[8]; // 8
using MsgLen = uint16_t; // 2 bytes

const TransactionCode TC_LOGON_REQ = 2300;
const TransactionCode TC_LOGON_RES = 2301;
const TransactionCode TC_HEARTBEAT = 23506;
const TransactionCode TC_LOGOFF_REQ = 2320;
const TransactionCode TC_LOGOFF_RES = 2321;
const TransactionCode TC_SYSTEM_INFO_REQ = 1600;
const TransactionCode TC_SYSTEM_INFO_RES = 1601;
const TransactionCode TC_ORDER_ENTRY_REQ = 2000;

using UserId = int32_t;
using Password = char[8];
using TraderName = char[26];
using LastPwdChangeDate = int32_t;
using BrokerId = char[5];
using Reserved = char;
using BranchId = int16_t;
using VersionNumber = int32_t;
using Batch2StartTime = int32_t;
using HostSwitchContext = char;
using Colour = char[50];
using UserType = int16_t;
using SequenceNumber = double;
using WsClassName = char[14];
using BrokerStatus = char;
using ShowIndex = char;
using MemberType = int16_t;
using ClearingStatus = char;
using BrokerName = char[25];



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
	SYSTEM_MANUAL,
	SYSTEM_ATM
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
	TRADE_QUOTE,
	TRADE_ATM,
	TRADE_GUI,
	TRADE_EXCHANGE
};

enum TradeSide{
	TRADE_BUY,
	TRADE_SELL,
	TRADE_SHORT_SELL,
	TRADE_UNKNOWN
};

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_ENUMTYPES_HPP_ */
