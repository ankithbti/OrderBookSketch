/*
 * DepthLevel.hpp
 *
 *  Created on: 09-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_DEPTHLEVEL_HPP_
#define INCLUDE_DEPTHLEVEL_HPP_

#include <Common.hpp>
#include <OrderStateKeeper.hpp>

namespace obLib{

  struct DepthLevel
  {
    Price _price;
    Quantity _totalQty;
    int32_t _ordersCount;
    std::vector<OrderStateKeeper*> _orders;
    bool _isExcess;
    ChangeId _last_change;

    DepthLevel() : _price(INVALID_LEVEL_PRICE), _totalQty(0), _ordersCount(0), _isExcess(false), _last_change(0){

    }

    DepthLevel& operator=(const DepthLevel& rhs){
      _totalQty = rhs._totalQty;
      _ordersCount = rhs._ordersCount;
      _orders = rhs._orders;
      _price = rhs._price;
      if(rhs._price != INVALID_LEVEL_PRICE){
	  _last_change = rhs._last_change;
      }
      return *this;
    }

    void init(Price price, bool is_excess){
      _price = price;
      _totalQty = 0;
      _ordersCount = 0;
      _isExcess = is_excess;

    }

    void add_order(Quantity qty){
      ++_ordersCount;
      _totalQty += qty;
    }

    void increase_qty(Quantity qty){
      _totalQty += qty;
    }

    void decrease_qty(Quantity qty){
      _totalQty -= qty;
    }

    void set(Price price,
	     Quantity qty,
	     uint32_t order_count,
	     ChangeId last_change = 0){
      _price = price;
      _totalQty = qty;
      _ordersCount = order_count;
      _last_change = last_change;
    }

    bool close_order(Quantity qty){
      bool emptyLevel = false;
      if(_ordersCount == 0){
	  throw std::runtime_error("No orders are there in this depthLevel. Can not remove.");
      }
      if(_ordersCount == 1){
	  // Only 1 Order
	  _ordersCount = 0;
	  _totalQty = 0;
	  emptyLevel = true;
      }else{
	  // More than 1 orders at this depthLevel
	  --_ordersCount;
	  if(_totalQty >= qty){
	      _totalQty -= qty;
	  }else{
	      std::runtime_error("DepthLevel Total Qty is lower than required.");
	  }
      }
      return emptyLevel;
    }

    /// @brief set last changed stamp on this level
    void last_change(ChangeId last_change) {
      _last_change = last_change;
    }

    /// @brief get last change stamp for this level
    ChangeId last_change() const {
      return _last_change;
    }

    bool changed_since(ChangeId last_published_change) const{
      return _last_change > last_published_change;
    }

  };
}



#endif /* INCLUDE_DEPTHLEVEL_HPP_ */
