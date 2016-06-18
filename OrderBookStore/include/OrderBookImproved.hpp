/*
 * OrderBookImproved.hpp
 *
 *  Created on: 15-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDERBOOKIMPROVED_HPP_
#define INCLUDE_ORDERBOOKIMPROVED_HPP_

#include <Common.hpp>
#include <Order.hpp>

namespace obLib{

  struct OrderBookImproved : private boost::noncopyable
  {

    typedef std::shared_ptr<OrderBookImproved> SharedPtr;

    typedef std::multimap<Price, Order::SharedPtr, std::greater<Price> > BidsMap;
    typedef std::multimap<Price, Order::SharedPtr, std::less<Price> > AsksMap;
    typedef std::multimap<Price, Order::SharedPtr>::iterator MapIt;

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

    }

    void processOrder(Order::SharedPtr orderPtr);

    void printTop5(std::string& str){
      if(_bids.size() == 0 && _asks.size() == 0){
	  return;
      }
      std::stringstream obStr;
      obStr << _token << " : [ " ;
      int count = 0;
      obStr << " Bids { " ;
      for(MapIt it = _bids.begin() ; it != _bids.end(); it = _bids.upper_bound(it->first)){
	  std::pair<MapIt, MapIt> eqRangeIt = _bids.equal_range(it->first);
	  Quantity aggQuantity = 0;
	  for(MapIt si = eqRangeIt.first ; si != eqRangeIt.second ; ++si){
	      aggQuantity += si->second->order_qty();
	  }
	  obStr << it->first << "|" << aggQuantity ;
	  if(++count == 5){
	      break;
	  }else{
	      obStr << ", ";
	  }
      }
      obStr << " } ";
      obStr << " <--> Asks { ";
      count = 0;
      for(MapIt it = _asks.begin() ; it != _asks.end(); it = _asks.upper_bound(it->first)){
	  std::pair<MapIt, MapIt> eqRangeIt = _asks.equal_range(it->first);
	  Quantity aggQuantity = 0;
	  for(MapIt si = eqRangeIt.first ; si != eqRangeIt.second ; ++si){
	      aggQuantity += si->second->order_qty();
	  }
	  obStr << it->first << "|" << aggQuantity ;
	  if(++count == 5){
	      break;
	  }else{
	      obStr << ", ";
	  }
      }
      obStr << " } ";
      obStr << " ] " ;
      str = obStr.str();
    }

  };

}




#endif /* INCLUDE_ORDERBOOKIMPROVED_HPP_ */
