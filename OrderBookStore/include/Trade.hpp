/*
 * Trade.hpp
 *
 *  Created on: 19-Jun-2016
 *      Author: ankithbti
 */

#ifndef ORDERBOOKSTORE_INCLUDE_TRADE_HPP_
#define ORDERBOOKSTORE_INCLUDE_TRADE_HPP_


#include <Common.hpp>
#include <MktDataTradeMsg.hpp>

namespace obLib
{

struct Trade
{
	//    typedef std::shared_ptr<Order> SharedPtr;
	typedef Trade* SharedPtr;

	virtual SeqNo getSeqNo() const = 0;
	virtual char getType() const = 0;

	virtual int64_t getTimeStmap() const = 0;

	virtual OrderId buyOrderId() const = 0;
	virtual OrderId sellOrderId() const = 0;
	virtual std::pair<OrderId, OrderId> orderIds() const = 0;

	virtual TokenId tokenId() const = 0;

	virtual Price price() const = 0;

	virtual Quantity order_qty() const = 0;

	virtual ~Trade(){

	}

	virtual void print() const = 0;
	virtual void toString(std::string&) const = 0;

};

struct SimpleTrade : public Trade{

	MktDataGlobalHeaderMsg _globalMktDataHeader; // 8 Bytes - 0
	char _msgType; // 'T' - 8
	int64_t _timestamp; // milliseconds from 01-Jan-1980 00:00:00 - 9
	int64_t _buyOrderId; // Day Unique Order Ref # - 17
	int64_t _sellOrderId; // Day Unique Order Ref # - 25
	int32_t _toeknNo; // Unique Contract Identifier - 33
	int32_t _price; // in paise , // The price is in multiples of the tick size. - 37
	// For FO and CM segments this should be divided by 100 for converting into Rupees
	// For CD segment this should be divided by 10^7 for converting into Rupees. -
	int32_t _qty; // - 41

	SimpleTrade(const MktDataGlobalHeaderMsg& header, const char& msgType, const MktDataTradeMsg& msg) {
		_globalMktDataHeader._len = header._len;
		_globalMktDataHeader._seqNo = header._seqNo;
		_globalMktDataHeader._streamId = header._streamId;
		_msgType = msgType;
		_timestamp = msg._timestamp;
		_buyOrderId = msg._buySideOrderId;
		_sellOrderId = msg._sellSideOrderId;
		_toeknNo = msg._toeknNo;
		_price = msg._price;
		_qty = msg._qty;
	}

	void print() const{
		std::cout << " SimpleMessage - Header: [ " << _globalMktDataHeader._len << " " << _globalMktDataHeader._streamId << " " << _globalMktDataHeader._seqNo << " ] "
				<< " Body: [ " << _msgType << " " << _timestamp << " " << _buyOrderId << " " << _sellOrderId << " " << _toeknNo << " " << _price << " " << _qty << " ] " << std::endl;
	}

	void toString(std::string& str) const{
		std::stringstream ss;
		ss << " - H: [ " << _globalMktDataHeader._len << " " << _globalMktDataHeader._streamId << " " << _globalMktDataHeader._seqNo << " ] "
				<< " B: [ " << _msgType << " " << _timestamp << " " << _buyOrderId << " " << _sellOrderId << " " << _toeknNo << " " << _price << " " << _qty << " ] " ;
		str += ss.str();
	}

	virtual SeqNo getSeqNo() const
	{
		return _globalMktDataHeader._seqNo;
	}
	virtual int64_t getTimeStmap() const{
		return _timestamp;
	}

	virtual OrderId buyOrderId() const {
		return _buyOrderId;
	}

	virtual OrderId sellOrderId() const{
		return _sellOrderId;
	}

	virtual std::pair<OrderId, OrderId> orderIds() const{
		return std::make_pair(_buyOrderId, _sellOrderId);
	}

	virtual TokenId tokenId() const {
		return _toeknNo;
	}

	virtual char getType() const {
		return _msgType;
	}

	virtual Price price() const {
		return _price;
	}

	virtual Quantity order_qty() const{
		return _qty;
	}

};
}


#endif /* ORDERBOOKSTORE_INCLUDE_TRADE_HPP_ */
