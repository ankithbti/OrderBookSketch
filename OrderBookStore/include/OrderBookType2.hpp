/*
 * OrderBookType2.hpp
 *
 *  Created on: 20-Jun-2016
 *      Author: ankithbti
 */

#ifndef ORDERBOOKSTORE_INCLUDE_ORDERBOOKTYPE2_HPP_
#define ORDERBOOKSTORE_INCLUDE_ORDERBOOKTYPE2_HPP_

#include <Common.hpp>
#include "HashMap.h"
#include <Order.hpp>
#include <Trade.hpp>

namespace obLib
{
struct BestPrice {
	int64_t bidqty;
	int64_t bid;
	int64_t ask;
	int64_t askqty;

	BestPrice(int64_t bidqty, int64_t bid, int64_t ask, int64_t askqty)
	: bidqty(bidqty), bid(bid), ask(ask), askqty(askqty) {}

	BestPrice() : bidqty(0), bid(0), ask(0), askqty(0) {}

	friend std::ostream &operator<<(std::ostream &out, const BestPrice &best) {
		out << "BestPrice(" << best.bidqty << ", " << best.bid << ", " << best.ask
				<< ", " << best.askqty << ")";
		return out;
	}

	void toString(std::stringstream& out){
		out << "BestPrice(" << bidqty << ", " << bid << ", " << ask
				<< ", " << askqty << ")";
	}
};

class OrderBookType2
{
private:
	struct Hash {
		size_t operator()(uint64_t h) const noexcept {
			h ^= h >> 33;
			h *= 0xff51afd7ed558ccd;
			h ^= h >> 33;
			h *= 0xc4ceb9fe1a85ec53;
			h ^= h >> 33;
			return h;
		}
	};
public:
	struct Level {
		int64_t price = 0;
		int64_t qty = 0;
		int32_t orderCount = 0;

		friend std::ostream &operator<<(std::ostream &out, const Level &level) {
			out << " { " << level.price << " | " << level.qty << " | " << level.orderCount << " }";
			return out;
		}

		void toString(std::stringstream& out){
			out << price << "|" << qty << "|" << orderCount ;
		}
	};


private:

	//typedef std::pair<int16_t, Order::SharedPtr> OrderEntry;
	typedef HashMap<OrderId, Order::SharedPtr, Hash>::iterator OrdersIt;
	typedef HashMap<OrderId, Order::SharedPtr, Hash>::const_iterator OrdersConstIt;

	void *data_ = nullptr;
	size_t _sizeHint = 0;
	boost::container::flat_map<int64_t, Level, std::greater<int64_t>> buy_, sell_;
	HashMap<OrderId, Order::SharedPtr, Hash> _orders;


public:

	OrderBookType2(size_t sh, void *data = NULL) : data_(NULL),
	_sizeHint(sh),
	_orders(_sizeHint, std::numeric_limits<uint64_t>::max()) {
		_sizeHint = _orders.bucket_count();
	}

	void *GetUserData() const { return data_; }

	void SetUserData(void *data) { data_ = data; }


	bool processOrder(Order::SharedPtr orderPtr){
		switch(orderPtr->getType()){
		case 'N':
		{
			if (_orders.emplace(orderPtr->orderId(), orderPtr).second) {
				// New Order
				return addNewOrder(orderPtr);
			}else{
				// Already have such orderId, Throw exception
				throw std::runtime_error(" Duplicate OrderID comes in New Order.");
			}
		}
		break;
		case 'M':
		{
			auto retVal = _orders.emplace(orderPtr->orderId(), orderPtr);
			if(retVal.second){
				// Treat it as new Order as we dont have such OrderID
				return addNewOrder(retVal.first->second);
			}
			// Treat it as Modify Order
			return modifyOrder(orderPtr, retVal.first->second);
		}
		break;
		case 'X':
		{
			// Get Old order
			auto it = _orders.find(orderPtr->orderId());
			if(it == _orders.end()){
				// Ignore this as we don't have its reference
				return false;
			}
			Order::SharedPtr o = it->second;
			_orders.erase(it);
			return cancelOrder(o);

		}
		break;
		default:
			break;
		}
		return false;
	}

	bool processTrade(Trade::SharedPtr tradePtr){
		OrderId buyOrderId = tradePtr->buyOrderId();
		OrderId sellOrderId = tradePtr->sellOrderId();
		bool retVal = false;
		if(buyOrderId != 0){
			auto it = _orders.find(buyOrderId);
			if(it == _orders.end()){
				// Ignore this as we don't have its reference
			}else{
				if(it->second->order_qty() <= tradePtr->order_qty()){
					// Delet this Order as well
					_orders.erase(it);
				}
				retVal = doTrade(true, tradePtr->price(), tradePtr->order_qty());
			}
		}
		if(sellOrderId != 0){
			auto it = _orders.find(buyOrderId);
			if(it == _orders.end()){
				// Ignore this as we don't have its reference
				retVal = retVal || false;
			}else{
				if(it->second->order_qty() <= tradePtr->order_qty()){
					// Delet this Order as well
					_orders.erase(it);
				}
				retVal = (retVal || doTrade(false, tradePtr->price(), tradePtr->order_qty()));
			}
		}
		return retVal;
	}

private:

	bool doTrade(bool isBuy, Price price, Quantity qty)
	{
		auto &side = isBuy ? buy_ : sell_;
		int64_t prio = isBuy ? -price : price;
		auto it = side.find(prio);
		if (it == side.end()) {
			return false;
		}
		// Reduce Qty in Level
		it->second.qty -= qty;
		it->second.orderCount -= 1;
		if(it->second.qty <= 0){
			side.erase(it);
		}
		return it == side.begin(); // BBO changes
	}

	bool addNewOrder(Order::SharedPtr orderPtr){
		auto &side = orderPtr->is_buy() ? buy_ : sell_;
		int64_t prio = orderPtr->is_buy() ? -orderPtr->price() : orderPtr->price();
		auto it = side.insert(std::make_pair(prio, Level())).first;
		it->second.price = orderPtr->price();
		it->second.qty += orderPtr->order_qty();
		it->second.orderCount += 1;
		return it == side.begin(); // Has been added to BBO
	}

	bool modifyOrder(Order::SharedPtr newOrderPtr, Order::SharedPtr oldOrderPtr){
		// Check if Price has been changed ??
		if(newOrderPtr->price() != oldOrderPtr->price()){
			// Erase the Qty from Prev Price level
			auto &side = oldOrderPtr->is_buy() ? buy_ : sell_;
			int64_t prio = oldOrderPtr->is_buy() ? -oldOrderPtr->price() : oldOrderPtr->price();
			auto removeIt = side.insert(std::make_pair(prio, Level())).first;
			removeIt->second.qty -= oldOrderPtr->order_qty();
			removeIt->second.orderCount -= 1;
			if(removeIt->second.qty <= 0){
				side.erase(removeIt);
			}
			bool viaErase = (removeIt == side.begin());

			// Add the new delta Qty in new Price level
			int64_t newPrio = newOrderPtr->is_buy() ? -newOrderPtr->price() : newOrderPtr->price();
			auto addIt = side.insert(std::make_pair(newPrio, Level())).first;
			addIt->second.price = newOrderPtr->price();
			addIt->second.qty += newOrderPtr->order_qty();
			addIt->second.orderCount += 1;

			bool viaAdd = (addIt == side.begin());
			return (viaAdd || viaErase);

		}else if(newOrderPtr->order_qty() != oldOrderPtr->order_qty()){
			// Check if only Quantity has been changed
			auto &side = oldOrderPtr->is_buy() ? buy_ : sell_;
			int64_t prio = oldOrderPtr->is_buy() ? -oldOrderPtr->price() : oldOrderPtr->price();
			auto it = side.insert(std::make_pair(prio, Level())).first;
			it->second.qty += (newOrderPtr->order_qty() - oldOrderPtr->order_qty()); // Adding delta
			if(it->second.qty <= 0){
				side.erase(it);
			}
			return it == side.begin(); // Has been added to BBO

		}else{
			// Nothing cnhanges
			return false;
		}
	}

	bool cancelOrder(Order::SharedPtr oldOrderPtr){
		auto &side = oldOrderPtr->is_buy() ? buy_ : sell_;
		int64_t prio = oldOrderPtr->is_buy() ? -oldOrderPtr->price() : oldOrderPtr->price();
		auto it = side.find(prio);
		if (it == side.end()) {
			return false;
		}
		// Reduce Qty in Level
		it->second.qty -= oldOrderPtr->order_qty();
		it->second.orderCount -= 1;
		if(it->second.qty <= 0){
			side.erase(it);
		}
		return it == side.begin(); // BBO changes
	}


	BestPrice GetBestPrice() const {
		auto buy = buy_.rbegin();
		auto sell = sell_.rbegin();
		BestPrice bp;
		if (buy != buy_.rend()) {
			bp.bidqty = buy->second.qty;
			bp.bid = buy->second.price;
		}
		if (sell != sell_.rend()) {
			bp.askqty = sell->second.qty;
			bp.ask = sell->second.price;
		}
		return bp;
	}

public:
	void toString(std::string& str){
		std::stringstream out;
		if(buy_.size() > 0){
			out << std::endl << " Buy: ";
		}
		int count = 0;
		for (auto it = buy_.rbegin(); it != buy_.rend(); it++) {
			it->second.toString(out) ;
			if(++count == 10){
				break;
			}else{
				out << ", ";
			}
		}

		if(sell_.size() > 0){
			out << std::endl << " Sell: " ;
		}
		count  = 0;
		for (auto it = sell_.rbegin(); it != sell_.rend(); it++) {
			it->second.toString(out) ;
			if(++count == 10){
				break;
			}else{
				out << ", ";
			}
		}
		str += out.str();
	}

	friend std::ostream &operator<<(std::ostream &out, const OrderBookType2 &book) {
		if(book.buy_.size() > 0){
			out << " Buy: ";
		}
		int count = 0;
		for (auto it = book.buy_.rbegin(); it != book.buy_.rend(); it++) {
			if(++count < 6){
				out << it->second ;
			}else{
				break;
			}
		}

		if(book.sell_.size() > 0){
			out << " Sell: " ;
		}
		count  = 0;
		for (auto it = book.sell_.rbegin(); it != book.sell_.rend(); it++) {
			if(++count < 6){
				out << it->second ;
			}else{
				break;
			}
		}
		return out;
	}
};

}



#endif /* ORDERBOOKSTORE_INCLUDE_ORDERBOOKTYPE2_HPP_ */
