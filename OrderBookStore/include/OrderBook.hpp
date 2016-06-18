/*
 * OrderBook.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDERBOOK_HPP_
#define INCLUDE_ORDERBOOK_HPP_

#include <Common.hpp>
#include <OrderStateKeeper.hpp>
#include <Depth.hpp>

namespace obLib{

  class OrderBook : private boost::noncopyable
  {

  public:

    typedef std::unordered_map<OrderId, OrderStateKeeper*> BidsOrderId2OrderStateContainer;
    typedef std::unordered_map<OrderId, OrderStateKeeper*> AsksOrderId2OrderStateContainer;

  private:

    BidsOrderId2OrderStateContainer _bids;
    AsksOrderId2OrderStateContainer _asks;
    int _transactionId;
    Depth<> _depth;

  public:
    OrderBook() : _transactionId(0){

    }

    int getTransactionId() const {
      return _transactionId;
    }

    void addNew(Order* order){
      ++_transactionId;
      //std::cout << " New Order for " << order->tokenId() << " " << order->orderId() << std::endl;
      if(order->order_qty() == 0){
	  throw std::runtime_error("New Order can not have 0 Qty.");
      }

      OrderStateKeeper *osk = new OrderStateKeeper(order);
      if(order->is_buy()){
	  _bids.insert(std::pair<OrderId, OrderStateKeeper*>(order->orderId(), osk));
      }else{
	  _asks.insert(std::pair<OrderId, OrderStateKeeper*>(order->orderId(), osk));
      }

      // Try adjusting our depth
      _depth.add_order(order->price(), order->order_qty(), order->is_buy());
    }

    void cancel(Order* order){
      ++_transactionId;
      //std::cout << " Cancel Order for " << order->tokenId() << " " << order->orderId() << std::endl;
      if(order->is_buy()){
	  BidsOrderId2OrderStateContainer::iterator it = _bids.find(order->orderId());
	  if(it != _bids.end()){
	      Order* origOrder = it->second->ptr();
	      _depth.close_order(origOrder->price(), origOrder->order_qty(), origOrder->is_buy());
	      // Also, remove this orderId from our internalMap
	      _bids.erase(it);
	  }else{
	      // Just Ignore this cancel message
	  }
      }else{
	  AsksOrderId2OrderStateContainer::iterator it = _asks.find(order->orderId());
	  if(it != _asks.end()){
	      Order* origOrder = it->second->ptr();
	      _depth.close_order(origOrder->price(), origOrder->order_qty(), origOrder->is_buy());
	      // Also, remove this orderId from our internalMap
	      _bids.erase(it);
	  }else{
	      // Just Ignore this cancel message
	  }
      }
    }

    void replace(Order* order){
      ++_transactionId;
      //std::cout << " Modify Order for " << order->tokenId() << " " << order->orderId() << std::endl;
      // get the original Order
      if(order->is_buy()){
	  BidsOrderId2OrderStateContainer::iterator it = _bids.find(order->orderId());
	  if(it != _bids.end()){
	      Order* origOrder = it->second->ptr();
	      _depth.replace_order(it->second->getPrice(), order->price(), it->second->open_qty(), order->order_qty(), order->is_buy());
	      Quantity delta = origOrder->order_qty() - order->order_qty();
	      bool noChangeInPrice = order->price() == origOrder->price();
	      // Set the new Open Quantity
	      it->second->change_qty(delta);
	      if(!noChangeInPrice){
		  it->second->setPrice(order->price());
	      }
	  }else{
	      // Treat it as new order
	      addNew(order);
	  }
      }else{
	  AsksOrderId2OrderStateContainer::iterator it = _asks.find(order->orderId());
	  if(it != _asks.end()){
	      Order* origOrder = it->second->ptr();
	      Quantity delta = origOrder->order_qty() - order->order_qty();
	      bool noChangeInPrice = order->price() == origOrder->price();
	      if(noChangeInPrice){
		  // Only Qunatity changes
		  _depth.changeQtyOrder(order->price(), delta, order->is_buy());
	      }else{
		  // Price changes
		  // To Do
		  _depth.replace_order(it->second->getPrice(), order->price(), it->second->open_qty(), order->order_qty(), order->is_buy());
	      }
	      // Set the new Open Quantity
	      it->second->change_qty(delta);
	      if(!noChangeInPrice){
		  it->second->setPrice(order->price());
	      }
	  }else{
	      // Treat it as new order
	      addNew(order);
	  }
      }
    }



  };
}



#endif /* INCLUDE_ORDERBOOK_HPP_ */
