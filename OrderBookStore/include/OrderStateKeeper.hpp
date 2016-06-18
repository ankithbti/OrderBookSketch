/*
 * OrderStateKeeper.hpp
 *
 *  Created on: 09-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDERSTATEKEEPER_HPP_
#define INCLUDE_ORDERSTATEKEEPER_HPP_

#include <Common.hpp>
#include <Order.hpp>

namespace obLib
{

  struct OrderStateKeeper
  {
  private:
    Order* _order;
    Quantity _openQty;
    Price _price;

  public:

    OrderStateKeeper(Order* order) : _order(order), _openQty(_order->order_qty()), _price(order->price()){

    }

    void setPrice(Price p) {
      _price = p;
    }

    Price getPrice() const {
      return _price;
    }

    void change_qty(Quantity delta){
      if(delta < 0 && (_openQty < std::abs(delta))){
	  throw std::runtime_error("Replaced deducted Qty is greater than the Order's open Qty.");
      }
      _openQty += delta;
    }

    void fill(Quantity qty){
      if(qty > _openQty){
	  std::runtime_error("Fill size if greater than open Qty");
      }
      _openQty -= qty;
    }

    bool filled() const{
      return _openQty == 0;
    }

    Quantity filled_qty() const{
      return _order->order_qty() - _openQty;
    }

    Quantity open_qty() const{
      return _openQty;
    }

    const Order* ptr() const{
      return _order;
    }

    /// @brief get the order pointer
    Order* ptr(){
      return _order;
    }


  };
}





#endif /* INCLUDE_ORDERSTATEKEEPER_HPP_ */
