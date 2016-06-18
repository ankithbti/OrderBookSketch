/*
 * Order.hpp
 *
 *  Created on: 09-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDER_HPP_
#define INCLUDE_ORDER_HPP_

#include <Common.hpp>
#include <MktDataOrderMsg.hpp>

namespace obLib
{

  struct Order
  {
//    typedef std::shared_ptr<Order> SharedPtr;
    typedef Order* SharedPtr;

    bool is_limit() const{
      return price() > 0;
    }

    virtual OrderId orderId() const = 0;

    virtual TokenId tokenId() const = 0;

    virtual char getType() const = 0;
    virtual void setType(char type) = 0;

    virtual bool is_buy() const = 0;

    virtual Price price() const = 0;

    virtual Quantity order_qty() const = 0;
    virtual void order_qty(Quantity q) = 0;

    virtual ~Order(){

    }

  };

  struct SimpleOrder : public Order{

    MktDataGlobalHeaderMsg _globalMktDataHeader; // 8 Bytes - 0
    char _msgType; // 'N' - New / 'M' - Modify / 'X' - Cancel - 8
    int64_t _timestamp; // milliseconds from 01-Jan-1980 00:00:00 - 9
    int64_t _orderId; // Day Unique Order Ref # - 17
    int32_t _toeknNo; // Unique Contract Identifier - 25
    char _orderType; // 'B' / 'S' - 29
    int32_t _price; // in paise , // The price is in multiples of the tick size. - 30
    // For FO and CM segments this should be divided by 100 for converting into Rupees
    // For CD segment this should be divided by 10^7 for converting into Rupees. -
    int32_t _qty; // - 34

    SimpleOrder(const MktDataOrderMsg& msg){
      _globalMktDataHeader._len = msg._globalMktDataHeader._len;
      _globalMktDataHeader._seqNo = msg._globalMktDataHeader._seqNo;
      _globalMktDataHeader._streamId = msg._globalMktDataHeader._streamId;
      _msgType = msg._msgType;
      _timestamp = msg._timestamp;
      _orderId = msg._orderId;
      _toeknNo = msg._toeknNo;
      _orderType = msg._orderType;
      _price = msg._price;
      _qty = msg._qty;
    }

    void print() const{
      std::cout << " SimpleMessage - Header: [ " << _globalMktDataHeader._len << " " << _globalMktDataHeader._streamId << " " << _globalMktDataHeader._seqNo << " ] "
	  << " Body: [ " << _msgType << " " << _timestamp << " " << _orderId << " " << _toeknNo << " " << _orderType << " " << _price << " " << _qty << " ] " << std::endl;
    }

    virtual OrderId orderId() const{
      return _orderId;
    }

    virtual TokenId tokenId() const {
      return _toeknNo;
    }

    virtual char getType() const {
      return _msgType;
    }
    virtual void setType(char type){
      _msgType = type;
    }

    virtual bool is_buy() const {
      return _orderType == 'B';
    }

    virtual Price price() const {
      return _price;
    }

    virtual Quantity order_qty() const{
      return _qty;
    }

    virtual void order_qty(Quantity q){
      _qty = q;
    }

  };
}





#endif /* INCLUDE_ORDER_HPP_ */
