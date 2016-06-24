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
	//OrderBookManagerBase<OrderBookManagerTemp> _orderBookManager;
	//OrderBookManagerImproved _orderBookManager;
	OrderBookType2CB _cb;
	OrderBookManagerType2<OrderBookType2CB> _orderBookManager;
	//OrderBookManagerBase<OrderBookManagerOld> _orderBookManager;

	Subscriber(boost::asio::io_service& io, const std::string& host, const std::string& port) : _socket(io),
			_resolver(io), _orderBookManager(_cb, 100000){
		_orderBookManager.init();
		pcount = 0;
		LatencyChecker<>::_minlatency = std::numeric_limits<int>::max();
		LatencyChecker<>::_maxLatency = 0;
		connect(host, port);
	}

	~Subscriber(){

		if(LatencyChecker<>::isLatencyAvailable())
		{
			std::cout << "Min: " << LatencyChecker<>::getMin() << std::endl;
			std::cout << "Max: " << LatencyChecker<>::getMax() << std::endl;
			std::cout << "Average: " << LatencyChecker<>::getAverageLatency() << std::endl;
			std::cout << "90th Percentile: " << LatencyChecker<>::getPercentile(90.0) << std::endl;
			std::cout << "95th Percentile: " << LatencyChecker<>::getPercentile(95.0) << std::endl;
			std::cout << "99th Percentile: " << LatencyChecker<>::getPercentile(99.0) << std::endl;
			std::cout << "99.9th Percentile: " << LatencyChecker<>::getPercentile(99.9) << std::endl;
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
				char msgType;
				memcpy((void*)&msgType, (void*)(_data+sizeof(MktDataGlobalHeaderMsg)), sizeof(char));

				switch(msgType){
				case 'N':
				case 'M':
				case 'X':
				{
					MktDataOrderMsg* msg = new MktDataOrderMsg();
					memcpy((void*)msg, (void*)_data, sizeof(MktDataOrderMsg));
					std::string str;
					str = " Packet#: " + boost::lexical_cast<std::string>(++pcount) ;
					Order::SharedPtr order(new SimpleOrder(*msg));
					try{
						LatencyChecker<> lc(str);
						_orderBookManager.addOrder(order);
					}catch(const std::runtime_error& err){
						std::cout << " Exception " << err.what() << std::endl;
					}
					msg->toString(str);
					std::cout << str << std::endl;
					_orderBookManager.print();
					delete msg;
				}
				break;
				case 'T':
				{
					// To Do
				}
				break;
				default:
					break;
				}

				doReadHeader();
			}
			else
			{
				std::cout << "Exception comes - Socket Closed. " << std::endl;
				_socket.close();
			}
				});
	}


};
}



#endif /* INCLUDE_SUBSCRIBER_HPP_ */
