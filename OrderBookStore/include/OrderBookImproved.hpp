/*
 * OrderBookImproved.hpp
 *
 *  Created on: 15-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDERBOOKIMPROVED_HPP_
#define INCLUDE_ORDERBOOKIMPROVED_HPP_

#include <Common.hpp>
#include <OrderBookBase.hpp>

namespace obLib{

  struct OrderBookImproved
  {

    typedef std::shared_ptr<OrderBookImproved> SharedPtr;

    typedef std::multimap<Price, Order::SharedPtr, std::greater<Price> > BidsMap;
    typedef std::multimap<Price, Order::SharedPtr, std::less<Price> > AsksMap;
    typedef std::multimap<Price, Order::SharedPtr>::iterator MapIt;
    typedef std::multimap<Price, Order::SharedPtr>::const_iterator MapConstIt;

    typedef std::unordered_map<OrderId, MapIt> BidsHashMap;
    typedef std::unordered_map<OrderId, MapIt> AsksHashMap;
    typedef std::unordered_map<OrderId, MapIt>::iterator HashMapIt;


  private:

    BidsMap _bids;
    AsksMap _asks;

    BidsHashMap _bidsHash;
    AsksHashMap _asksHash;

    TokenId _token;

  public:

    OrderBookImproved(const TokenId& token) : _token(token){
    	(void)_token;
    }

    void processOrder(Order::SharedPtr orderPtr);
    void processTrade(Trade::SharedPtr tradePtr);
    void toString(std::string& str) const;

  };

}




#endif /* INCLUDE_ORDERBOOKIMPROVED_HPP_ */
