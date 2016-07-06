/*
 * OrderBookManager.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDERBOOKMANAGER_HPP_
#define INCLUDE_ORDERBOOKMANAGER_HPP_

#include <Common.hpp>
#include <OrderBook.hpp>
#include <OrderBookImproved.hpp>
#include <OrderBookType2.hpp>
#include <OrderGenerator.hpp>
#include <spdlog/spdlog.h>
#include <HashMap.h>
#include <FastBuffer.hpp>

#include <sparsehash/dense_hash_map>


namespace obLib{

// CRTP
template<class T>
struct OrderBookManagerBase : private boost::noncopyable{

	//typedef std::unordered_map<TokenId, OrderBookImproved > OrderBookContainer;
	//typedef google::dense_hash_map<TokenId, OrderBookImproved > OrderBookContainer;

	typedef std::unordered_map<TokenId, OrderBookImproved, std::hash<TokenId>,  std::equal_to<TokenId> > OrderBookContainer;

	typedef std::unordered_map<TokenId, int> SecurityFrequencyCont;
	typedef OrderBookContainer::iterator OrderBookContainerIt;
	OrderBookContainer _orderBooks;

	typedef std::unordered_map<TokenId, OrderBookImproved > OrderBookContainerOld;
	OrderBookContainerOld _orderBooksOld;

	void init(){
		static_cast<T*>(this)->init();
	}

	void addOrder(Order::SharedPtr order){
		static_cast<T*>(this)->addOrder(order);
	}
	void addTrade(Trade::SharedPtr trade){
		static_cast<T*>(this)->addTrade(trade);
	}

	void print(){
		static_cast<T*>(this)->print();
	}
};

struct OrderBookManagerTemp : public OrderBookManagerBase<OrderBookManagerTemp>{

	int _state;
	void init(){
		_state = 100;
	}
	void addOrder(Order::SharedPtr order) {
		std::cout << _state << std::endl;
	}
	void addTrade(Trade::SharedPtr trade) {

	}
	void orderBookStr(std::string& str){

	}
};

struct OrderBookType2CB{
	void operator()(OrderBookType2* b, bool isBBOUpdated){

	}
};

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
	size_t _sizeHint;
	bool _all_orders = false;
	bool _all_books = false;


	std::vector<OrderBookType2> _books;
	HashMap<TokenId, uint16_t, Hash> _symbols; // Value of map gives the index of OrderBook in books_

public:

	OrderBookManagerType2(Callback& cb, size_t sizeHint, bool all_orders = false, bool all_books = false) :
		_callback(cb),
		_sizeHint(sizeHint),
		_all_orders(all_orders),
		_all_books(all_books),
		_symbols(MAXSECURITIES, 0){
		_books.reserve(MAXSECURITIES);
	}

	void init(const std::string& file){
		OrderGenerator og(file);
		for(auto token : og.getTokens()){
			subscribe(token);
			//OrderBookImproved ob(token);
			//_orderBooks.emplace(token, ob);
		}
	}

	OrderBookType2& subscribe(TokenId instrument, void *data = NULL) {
		auto it = _symbols.find(instrument);
		if (it != _symbols.end()) {
			return _books[it->second];
		}

		if (_books.size() == MAXSECURITIES) {
			throw std::runtime_error("too many subscriptions");
		}

		_books.push_back(OrderBookType2(_sizeHint));
		_symbols.emplace(instrument, _books.size() - 1);

		OrderBookType2 &book = _books.back();
		book.SetUserData(data);
		return book;
	}

	void addOrder(Order::SharedPtr orderPtr){
		auto it = _symbols.find(orderPtr->tokenId());
		if (it == _symbols.end()) {
			std::cout << " Error - Unknown Security comes, Don't have any OB for this. " << std::endl;
			throw std::runtime_error("Error - Unknown Security comes, Don't have any OB for this.");

			if (_books.size() == MAXBOOK) {
				// too many books
				std::cout << " Error - Too many books." << std::endl;
				throw std::runtime_error(" Too many books.");
				return;
			}
			_books.push_back(OrderBookType2(_sizeHint));
			it = _symbols.emplace(orderPtr->tokenId(), _books.size() - 1).first;
		}
		int16_t bookid = it->second;
		OrderBookType2 &book = _books[bookid];
		book.processOrder(orderPtr);
		//		bool isBBoUpdated = book.processOrder(orderPtr);
		//_callback(&book, isBBoUpdated);
	}

	void addTrade(Trade::SharedPtr trade){
		auto it = _symbols.find(trade->tokenId());
		if (it == _symbols.end()) {
			std::cout << " Error - Unknown Security comes, Don't have any OB for this. " << std::endl;
			throw std::runtime_error("Error - Unknown Security comes, Don't have any OB for this.");

			if (_books.size() == MAXBOOK) {
				// too many books
				std::cout << " Error - Too many books." << std::endl;
				throw std::runtime_error(" Too many books.");
				return;
			}
			_books.push_back(OrderBookType2(_sizeHint));
			it = _symbols.emplace(trade->tokenId(), _books.size() - 1).first;
		}
		int16_t bookid = it->second;
		OrderBookType2 &book = _books[bookid];
		book.processTrade(trade);
	}

	void printOrderBookForSymbol(std::string& str, TokenId token){
		auto it = _symbols.find(token);
		if (it != _symbols.end()) {
			_books[it->second].toString(str);
		}
	}
};


struct OrderBookManagerImproved : public OrderBookManagerBase<OrderBookManagerImproved>
{
public:

	void init(const std::string& file){
//		_orderBooks.set_empty_key(-1);
//		_orderBooks.set_deleted_key(-2);
		OrderGenerator og(file);
		for(auto token : og.getTokens()){
			OrderBookImproved ob(token);
			_orderBooks.emplace(token, ob);
//			_orderBooks.insert(std::pair<TokenId, OrderBookImproved>(token, ob));
		}
	}

	void addOrder(Order::SharedPtr order){
		// Check if we have already the OrderBook for this security
		OrderBookContainerIt it = _orderBooks.find(order->tokenId());
		if(it == _orderBooks.end()){
			throw std::runtime_error(" Unknown Security comes in Market Data.");
		}
		it->second.processOrder(order); // might throw as well
	}

	void addTrade(Trade::SharedPtr trade){
		OrderBookContainerIt it = _orderBooks.find(trade->tokenId());
		if(it == _orderBooks.end()){
			throw std::runtime_error(" Unknown Security comes in Market Data.");
		}
		it->second.processTrade(trade);
	}

	void printOrderBookForSymbol(std::string& str, TokenId token){
		OrderBookContainerIt it = _orderBooks.find(token);
		if (it != _orderBooks.end()) {
			it->second.toString(str);
		}
	}
};

/*
// Old OBM
struct OrderBookManagerOld : public OrderBookManagerBase<OrderBookManagerOld>
{
	void init(){

	}
	void addOrder(Order::SharedPtr order){
		// Check if we have already the OrderBook for this security
		OrderBook tmpOb;
		std::pair<OrderBookContainerOld::iterator, bool> retVal = _orderBooksOld.emplace(order->tokenId(), std::move(tmpOb));
//		if(retVal.second){
//			retVal.first->second = new OrderBook();
//		}
		// Now we have the orderBook
		switch(order->getType()){
		case 'N':
			retVal.first->second.processOrder(order);
			break;
		case 'M':
			//book->replace(order);
			break;
		case 'X':
			//book->cancel(order);
			break;
		default:
			//throw std::runtime_error("Unsupported Order Type.");
			break;
		}

	}

	void addTrade(Trade::SharedPtr trade){

	}
};
 */

}



#endif /* INCLUDE_ORDERBOOKMANAGER_HPP_ */
