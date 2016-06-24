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

		friend std::ostream &operator<<(std::ostream &out, const Level &level) {
			out << " { " << level.price << " | " << level.qty << " }";
			return out;
		}
	};


private:

	//typedef std::pair<int16_t, Order::SharedPtr> OrderEntry;

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
		return false;
	}



private:

	bool addNewOrder(Order::SharedPtr orderPtr){
		auto &side = orderPtr->is_buy() ? buy_ : sell_;
		int64_t prio = orderPtr->is_buy() ? -orderPtr->price() : orderPtr->price();
		auto it = side.insert(std::make_pair(prio, Level())).first;
		it->second.price = orderPtr->price();
		it->second.qty += orderPtr->order_qty();
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
			if(removeIt->second.qty <= 0){
				side.erase(removeIt);
			}
			bool viaErase = (removeIt == side.begin());

			// Add the new delta Qty in new Price level
			int64_t newPrio = newOrderPtr->is_buy() ? -newOrderPtr->price() : newOrderPtr->price();
			auto addIt = side.insert(std::make_pair(newPrio, Level())).first;
			addIt->second.price = newOrderPtr->price();
			addIt->second.qty += newOrderPtr->order_qty();

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

/*
template <typename Callback>
struct OrderBookManagerType2{
public:
	static constexpr int16_t NOBOOK = std::numeric_limits<int16_t>::max();
	static constexpr int16_t MAXBOOK = std::numeric_limits<int16_t>::max();
	static constexpr int16_t MAXSECURITIES = std::numeric_limits<int16_t>::max();
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

	Callback& _callback;
	size_t _sizeHint = 0;
	bool _all_orders = false;
	bool _all_books = false;

	std::vector<OrderBookType2> _books;
	HashMap<TokenId, uint16_t, Hash> _symbols; // Value of map gives the index of OrderBook in books_
	typedef std::pair<int16_t, Order::SharedPtr> OrderEntry;
	HashMap<OrderId, OrderEntry, Hash> _orders;

public:

	OrderBookManagerType2(Callback& cb, size_t sizeHint, bool all_orders = false, bool all_books = false) :
		_callback(cb),
		_all_orders(all_orders),
		_all_books(all_books),
		_symbols(MAXSECURITIES, 0),
		_orders(sizeHint, std::numeric_limits<uint64_t>::max()){
		_sizeHint = _orders.bucket_count();
		_books.reserve(MAXSECURITIES);
	}

	OrderBookType2 &Subscribe(TokenId instrument, void *data = NULL) {
		auto it = _symbols.find(instrument);
		if (it != _symbols.end()) {
			return _books[it->second];
		}

		if (_books.size() == MAXSECURITIES) {
			throw std::runtime_error("too many subscriptions");
		}

		_books.push_back(OrderBookType2());
		_symbols.emplace(instrument, _books.size() - 1);

		OrderBookType2 &book = _books.back();
		book.SetUserData(data);
		return book;
	}

	void Add(Order::SharedPtr orderPtr) {
		auto it = _symbols.find(orderPtr->tokenId());
		if (it == _symbols.end()) {
			//			if (!_all_books) {
			//				if (_all_orders) {
			//					orders_.emplace(ref, Order(price, qty, buy_sell, NOBOOK));
			//				}
			//				return;
			//			}
			if (_books.size() == MAXBOOK) {
				// too many books
				return;
			}
			_books.push_back(OrderBookType2());
			it = _symbols.emplace(orderPtr->tokenId(), _books.size() - 1).first;
		}
		int16_t bookid = it->second;
		OrderBookType2 &book = _books[bookid];
		if (_orders.emplace(orderPtr->orderId(), std::make_pair(bookid,orderPtr)).second) {
			bool top = book.Add(orderPtr);
			_callback(&book, top);
		}
	}

	void Executed(Order::SharedPtr orderPtr) {
		auto oit = _orders.find(orderPtr->orderId());
		if (oit == _orders.end()) {
			return;
		}

		OrderEntry &orderEntry = oit->second;
		if (orderEntry.first != NOBOOK) {
			OrderBookType2 &book = _books[orderEntry.first];
			bool top = book.Reduce(orderPtr);
			_callback.OnTrade(&book, top);
		}

		orderEntry.second->order_qty(orderEntry.second->order_qty() - orderPtr->order_qty());
		if (orderEntry.second->order_qty() <= 0) {
			_orders.erase(oit);
		}
	}

	void ExecutedAtPrice(Order::SharedPtr orderPtr) {
		auto oit = _orders.find(orderPtr->orderId());
		if (oit == _orders.end()) {
			return;
		}

		OrderEntry &orderEntry = oit->second;
		if (orderEntry.first != NOBOOK) {
			OrderBookType2 &book = _books[orderEntry.first];
			bool top = book.Reduce(orderPtr);
			_callback(&book, top);
		}

		orderEntry.second->order_qty(orderEntry.second->order_qty() - orderPtr->order_qty());
		if (orderEntry.second->order_qty() <= 0) {
			_orders.erase(oit);
		}
	}

	void Reduce(Order::SharedPtr orderPtr) {
		auto oit = _orders.find(orderPtr->orderId());
		if (oit == _orders.end()) {
			return;
		}

		OrderEntry &orderEntry = oit->second;
		if (orderEntry.first != NOBOOK) {
			OrderBookType2 &book = _books[orderEntry.first];
			bool top = book.Reduce(orderPtr);
			_callback(&book, top);
		}

		orderEntry.second->order_qty(orderEntry.second->order_qty() - orderPtr->order_qty());
		if (orderEntry.second->order_qty() <= 0) {
			_orders.erase(oit);
		}
	}

	void Delete(Order::SharedPtr orderPtr) {
		auto oit = _orders.find(orderPtr->orderId());
		if (oit == _orders.end()) {
			return;
		}

		OrderEntry &orderEntry = oit->second;
		if (orderEntry.first != NOBOOK) {
			OrderBookType2 &book = _books[orderEntry.first];
			bool top = book.Reduce(orderEntry.second);
			_callback(&book, top);
		}
		_orders.erase(oit);
	}

	void Replace(Order::SharedPtr orderPtr) {
		auto oit = _orders.find(orderPtr->orderId());
		if (oit == _orders.end()) {
			return;
		}

		OrderEntry &orderEntry = oit->second;
		if (orderEntry.first != NOBOOK) {
			OrderBookType2 &book = _books[orderEntry.first];
			bool top = book.Reduce(orderEntry.second);
			bool top2 = book.Add(orderPtr);
			_callback(&book, top || top2);
		}
		_orders.erase(oit);
		_orders.emplace(orderPtr->orderId(), std::make_pair(orderEntry.first, orderPtr));
	}

	void Modify(Order::SharedPtr orderPtr) {
		auto oit = _orders.find(orderPtr->orderId());
		if (oit == _orders.end()) {
			return;
		}

		OrderEntry &orderEntry = oit->second;
		if (orderEntry.first != NOBOOK) {
			OrderBookType2 &book = _books[orderEntry.first];
			bool top = book.Reduce(orderEntry.second); // Reduce the older Order
			bool top2 = book.Add(orderPtr); // Add the new Order
			_callback(&book, top || top2);
		}

		orderEntry.second->order_qty(orderPtr->order_qty());
		orderEntry.second->price(orderPtr->price());
		if (orderEntry.second->order_qty() <= 0) {
			_orders.erase(oit);
		}
	}






};

 */


}



#endif /* ORDERBOOKSTORE_INCLUDE_ORDERBOOKTYPE2_HPP_ */
