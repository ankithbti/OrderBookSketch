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
#include <OrderGenerator.hpp>
#include <spdlog/spdlog.h>
#include <Trade.hpp>

namespace obLib{

struct OrderBookManagerImproved : private boost::noncopyable
{
	typedef std::unordered_map<TokenId, OrderBookImproved::SharedPtr> OrderBookContainer;
	typedef std::unordered_map<TokenId, int> SecurityFrequencyCont;

	typedef OrderBookContainer::iterator OrderBookContainerIt;

	OrderBookContainer _orderBooks;
	std::shared_ptr<spdlog::logger> _logger;
	//SecurityFrequencyCont _freq;

	OrderBookManagerImproved() : _logger(spdlog::daily_logger_mt("OrderBookManagerImproved", "logs/OrderBook")){
		OrderGenerator og;
		for(auto token : og.getTokens()){
			OrderBookImproved::SharedPtr obi(new OrderBookImproved(token));
			std::pair<TokenId, OrderBookImproved::SharedPtr> p = std::make_pair(token, obi);
			//_logger->info() << "Creating OrderBook for Token: " << token ;
			_orderBooks.insert(p);
		}
	}

	std::shared_ptr<spdlog::logger> getLogger(){
		return _logger;
	}


	void addOrder(Order::SharedPtr order){
		// Check if we have already the OrderBook for this security
		OrderBookContainerIt it = _orderBooks.find(order->tokenId());
		if(it == _orderBooks.end()){
			throw std::runtime_error(" Unknown Security comes in Market Data.");
		}

//		std::pair<SecurityFrequencyCont::iterator, bool> retVal = _freq.emplace(order->tokenId(), 0);
//		retVal.first->second += 1;
		//_logger->info() << "Token: " << order->tokenId() << " --> " << retVal.first->second;
		// Now we have the orderBook
		it->second->processOrder(order); // might throw as well

	}

	void addTrade(Trade::SharedPtr trade){
		OrderBookContainerIt it = _orderBooks.find(trade->tokenId());
		if(it == _orderBooks.end()){
			throw std::runtime_error(" Unknown Security comes in Market Data.");
		}
		it->second->processTrade(trade);
	}

	void print(){

//		_logger->info() << " Total Securities for which orders are coming: " << _freq.size() ;
//		int maxFreq = 0;
//		TokenId maxToken;
//		for(auto& v: _freq){
//			_logger->info() << " Freq: " << v.first << " <--> " << v.second ;
//			if(v.second > maxFreq){
//				maxFreq = v.second;
//				maxToken = v.first;
//			}
//		}
//		_logger->info() << " Max Freq: " << maxToken << " <--> " << maxFreq ;


		for(auto& v : _orderBooks){
			std::string bookDepthStr ;
			v.second->printTop5(bookDepthStr);
			if(!bookDepthStr.empty()){
				_logger->info() << bookDepthStr ;
			}
		}




	}

};


// Old OBM


struct OrderBookManager : private boost::noncopyable
{
	typedef std::unordered_map<TokenId, OrderBook*> OrderBookContainer;
	OrderBook* _tmpOb;
	OrderBookContainer _orderBooks;

	void addOrder(Order* order){
		// Check if we have already the OrderBook for this security
		std::pair<OrderBookContainer::iterator, bool> retVal = _orderBooks.insert(std::make_pair(order->tokenId(), _tmpOb));
		if(retVal.second){
			retVal.first->second = new OrderBook();
		}
		// Now we have the orderBook
		OrderBook* book = retVal.first->second;
		switch(order->getType()){
		case 'N':
			book->addNew(order);
			break;
		case 'M':
			//book->replace(order);
			break;
		case 'X':
			//book->cancel(order);
			break;
		default:
			throw std::runtime_error("Unsupported Order Type.");
			break;
		}

	}

	void addTrade(){

	}

	void print(){



		for(auto i : _orderBooks){
			std::cout << " TokenNo: " << i.first << " Transactions in orderBook: " << i.second->getTransactionId() << std::endl;
		}
	}

};
}



#endif /* INCLUDE_ORDERBOOKMANAGER_HPP_ */
