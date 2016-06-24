/*
 * OrderBookBase.hpp
 *
 *  Created on: 24-Jun-2016
 *      Author: ankithbti
 */

#ifndef ORDERBOOKSTORE_INCLUDE_ORDERBOOKBASE_HPP_
#define ORDERBOOKSTORE_INCLUDE_ORDERBOOKBASE_HPP_


#include <Common.hpp>
#include <Order.hpp>
#include <Trade.hpp>

namespace obLib
{

template<typename T>
struct OrderBookBase
{
	void processOrder(Order::SharedPtr order) {
		static_cast<T*>(this)->processOrder(order);
	}

	void processTrade(Trade::SharedPtr trade) {
		static_cast<T*>(this)->processTrade(trade);
	}
};


struct OrderBookTemp : public OrderBookBase<OrderBookTemp>
{
	void processOrder(Order::SharedPtr order) {
		std::cout << " Processing Order - Temp " << std::endl;
	}

	void processTrade(Trade::SharedPtr trade) {

	}
};

}

#endif /* ORDERBOOKSTORE_INCLUDE_ORDERBOOKBASE_HPP_ */
