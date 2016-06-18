/*
 * OrderBookImproved.cpp
 *
 *  Created on: 16-Jun-2016
 *      Author: ankithbti
 */
#include <OrderBookImproved.hpp>

namespace obLib
{

  void OrderBookImproved::processOrder(Order::SharedPtr orderPtr)
  {
    switch(orderPtr->getType()){
      case 'N':
	{
	  // New Order
	  if(orderPtr->is_buy()){
	      // Buy Side
	      MapIt it = _bids.emplace(orderPtr->price(), orderPtr);
	      std::pair<std::unordered_map<OrderId, MapIt>::iterator, bool> retVal = _bidsHash.emplace(orderPtr->orderId(), it);
	      if(!retVal.second){
		  throw std::runtime_error("Duplicate OrderId came in New Order.");
	      }
	  }else{
	      // Sell Side
	      //	      std::pair<Price, Order::SharedPtr> priceOrderPair = std::make_pair(orderPtr->price(), orderPtr);
	      //	      MapIt it = _asks.insert(priceOrderPair);
	      //	      std::pair<OrderId, MapIt> orderIdPriceMapItPair = std::make_pair(orderPtr->orderId(), it);
	      //	      std::pair<std::unordered_map<OrderId, MapIt>::iterator, bool> retVal = _asksHash.insert(orderIdPriceMapItPair);
	      //	      if(!retVal.second){
	      //		  throw std::runtime_error("Duplicate OrderId came in New Order.");
	      //	      }

	      MapIt it = _asks.emplace(orderPtr->price(), orderPtr);
	      std::pair<std::unordered_map<OrderId, MapIt>::iterator, bool> retVal = _asksHash.emplace(orderPtr->orderId(), it);
	      if(!retVal.second){
		  throw std::runtime_error("Duplicate OrderId came in New Order.");
	      }
	  }
	}
	break;
      case 'M':
	{
	  // Modify
	  HashMapIt it;
	  if(orderPtr->is_buy()){
	      if( ( it = _bidsHash.find(orderPtr->orderId())) != _bidsHash.end() ){
		  Order::SharedPtr orderFromMap = it->second->second;
		  // 2 ways - price might change OR Qty can change
		  if(orderFromMap->price() != orderPtr->price()){
		      // Remove old oder
		      _bids.erase(it->second);
		      // Treat it as new Order
		      it->second = _bids.emplace(orderPtr->price(), orderPtr);
		  }else{
		      // Price is same, Qty can change
		      orderFromMap->order_qty(orderPtr->order_qty());
		  }
	      }else{
		  // No Such Order - Treat it as New
		  orderPtr->setType('N');
		  processOrder(orderPtr);
	      }
	  }else{
	      if( ( it = _asksHash.find(orderPtr->orderId())) != _asksHash.end() ){
		  Order::SharedPtr orderFromMap = it->second->second;
		  // 2 ways - price might change OR Qty can change
		  if(orderFromMap->price() != orderPtr->price()){
		      // Remove old oder
		      _asks.erase(it->second);
		      // Treat it as new Order
		      it->second = _asks.emplace(orderPtr->price(), orderPtr);
		  }else{
		      // Price is same, Qty can change
		      orderFromMap->order_qty(orderPtr->order_qty());
		  }
	      }else{
		  // No Such Order - Treat it as New
		  orderPtr->setType('N');
		  processOrder(orderPtr);
	      }
	  }
	}
	break;
      case 'X':
	{
	  // Cancel
	  HashMapIt it;
	  if(orderPtr->is_buy()){
	      if( ( it = _bidsHash.find(orderPtr->orderId())) != _bidsHash.end() ){
		  // Erase this order
		  _bids.erase(it->second);
		  _bidsHash.erase(it);
	      }else{
		  // No such order - just ignore it
	      }
	  }else{
	      if( ( it = _asksHash.find(orderPtr->orderId())) != _asksHash.end() ){
		  // Erase this order
		  _asks.erase(it->second);
		  _asksHash.erase(it);
	      }else{
		  // No such order - just ignore it
	      }
	  }
	}
	break;
      default:
	break;
    }
  }

}



