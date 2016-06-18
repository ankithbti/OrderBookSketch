/*
 * Subscriber.hpp
 *
 *  Created on: 14-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_SUBSCRIBER_HPP_
#define INCLUDE_SUBSCRIBER_HPP_

#include <Common.hpp>
#include <OrderBook.hpp>
#include <OrderGenerator.hpp>
#include <OrderBookManager.hpp>
#include <LatencyChecker.hpp>

namespace obLib{
  struct Subscriber
  {
    enum { max_length = 1024 };
    char _data[max_length];
    int pcount ;

    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver _resolver;
    OrderBookManagerImproved _orderBookManager;
    std::shared_ptr<spdlog::logger> _logger;

    Subscriber(boost::asio::io_service& io, const std::string& host, const std::string& port) : _socket(io),
    		_resolver(io), _orderBookManager(),
    		_logger(spdlog::daily_logger_st("Subscriberlogger", "logs/SubscriberLog")){
      connect(host, port);
      pcount = 0;
    }

    ~Subscriber(){
      _orderBookManager.print();
	  if(LatencyChecker<>::isLatencyAvailable())
	  {
      	std::cout << "Min: " << LatencyChecker<>::getMin() << std::endl;
      	std::cout << "Max: " << LatencyChecker<>::getMax() << std::endl;
      	std::cout << "Average: " << LatencyChecker<>::getAverageLatency() << std::endl;
      	std::cout << "90th Percentile: " << LatencyChecker<>::getPercentile(90.0) << std::endl;
      	std::cout << "95th Percentile: " << LatencyChecker<>::getPercentile(95.0) << std::endl;
      	std::cout << "99th Percentile: " << LatencyChecker<>::getPercentile(99.0) << std::endl;
      	std::cout << "99.9th Percentile: " << LatencyChecker<>::getPercentile(99.9) << std::endl;
      	_logger->info() << "Min: " << LatencyChecker<>::getMin();
      	_logger->info() << "Max: " << LatencyChecker<>::getMax();
      	_logger->info() << "Average: " << LatencyChecker<>::getAverageLatency();
      	_logger->info() << "90th Percentile: " << LatencyChecker<>::getPercentile(90.0);
      	_logger->info() << "95th Percentile: " << LatencyChecker<>::getPercentile(95.0);
      	_logger->info() << "99th Percentile: " << LatencyChecker<>::getPercentile(99.0);
      	_logger->info() << "99.9th Percentile: " << LatencyChecker<>::getPercentile(99.9);
      	LatencyChecker<>::printLatencyArray();
		}
    }

  private:
    void connect(const std::string& host, const std::string& port){
      auto endpoint_it = _resolver.resolve({host, port});
      boost::asio::async_connect(_socket, endpoint_it,
				 [this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator)
				 {
	if (!ec)
	  {
	    doReadHeader();
	  }
				 });
    }

    void doReadHeader(){
      boost::asio::async_read(_socket,
			      boost::asio::buffer(_data, sizeof(MktDataOrderMsg)),
			      [this](boost::system::error_code ec, std::size_t length)
			      {
	if (!ec)
	  {
	    MktDataOrderMsg* msg = new MktDataOrderMsg();
	    memcpy((void*)msg, (void*)_data, sizeof(MktDataOrderMsg));
	    std::cout << " Read packet: " << ++pcount << std::endl;
	    //msg->print();
	    Order::SharedPtr order(new SimpleOrder(*msg));

	    try{
		LatencyChecker<> lc;
		_orderBookManager.addOrder(order);
	    }catch(const std::runtime_error& err){
		std::cout << " Exception " << err.what() << std::endl;
	    }
	    delete msg;
	    doReadHeader();
	    //_orderBookManager.print();
	  }
	else
	  {
	    _socket.close();
	  }
			      });
    }


  };
}



#endif /* INCLUDE_SUBSCRIBER_HPP_ */
