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

void OrderBookImproved::processTrade(Trade::SharedPtr tradePtr){
	std::pair<OrderId, OrderId> orderIds(tradePtr->orderIds());

	HashMapIt it;
	if(orderIds.first != 0){
		if( ( it = _bidsHash.find(orderIds.first)) != _bidsHash.end() ){
			Order::SharedPtr orderFromMap = it->second->second;
			Quantity delta = orderFromMap->order_qty() - tradePtr->order_qty();
			if(delta > 0){
				orderFromMap->order_qty(delta);
			}else if(delta < 0){
				throw std::runtime_error(" OrderBookImproved::processTrade() - Trade comes for Quantity lesser than the Order qty.");
			}else{
				// remove this order from orderBook
				_bids.erase(it->second);
				_bidsHash.erase(it);
			}
		}
	}
	if(orderIds.second != 0){
		if( ( it = _asksHash.find(orderIds.second)) != _bidsHash.end() ){
			Order::SharedPtr orderFromMap = it->second->second;
			Quantity delta = orderFromMap->order_qty() - tradePtr->order_qty();
			if(delta > 0){
				orderFromMap->order_qty(delta);
			}else if(delta < 0){
				throw std::runtime_error(" OrderBookImproved::processTrade() - Trade comes for Quantity lesser than the Order qty.");
			}else{
				// remove this order from orderBook
				_asks.erase(it->second);
				_asksHash.erase(it);
			}
		}
	}
}

void OrderBookImproved::printTop5(std::string& str) const{
	if(_bids.size() == 0 && _asks.size() == 0){
		return;
	}
	std::stringstream obStr;
	obStr << _token << " : [ " ;
	int count = 0;
	obStr << " Bids { " ;
	for(MapConstIt it = _bids.begin() ; it != _bids.end(); it = _bids.upper_bound(it->first)){
		std::pair<MapConstIt, MapConstIt> eqRangeIt = _bids.equal_range(it->first);
		Quantity aggQuantity = 0;
		int orderCount = 0;
		for(MapConstIt si = eqRangeIt.first ; si != eqRangeIt.second ; ++si){
			aggQuantity += si->second->order_qty();
			++orderCount;
		}
		obStr << it->first << "|" << aggQuantity << "|" << orderCount;
		if(++count == 10){
			break;
		}else{
			obStr << ", ";
		}
	}
	obStr << " } ";
	obStr << " <--> Asks { ";
	count = 0;
	for(MapConstIt it = _asks.begin() ; it != _asks.end(); it = _asks.upper_bound(it->first)){
		std::pair<MapConstIt, MapConstIt> eqRangeIt = _asks.equal_range(it->first);
		Quantity aggQuantity = 0;
		int orderCount = 0;
		for(MapConstIt si = eqRangeIt.first ; si != eqRangeIt.second ; ++si){
			aggQuantity += si->second->order_qty();
			++orderCount;
		}
		obStr << it->first << "|" << aggQuantity << "|" << orderCount;
		if(++count == 10){
			break;
		}else{
			obStr << ", ";
		}
	}
	obStr << " } ";
	obStr << " ] " ;
	str = obStr.str();
}

}



