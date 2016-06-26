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
#include <logger/Logger.hpp>
#include <logger/LogPolicy.hpp>

namespace obLib{
struct Subscriber
{
	enum { max_length = 1024 };
	char _data[max_length];
	int pcount ;
	std::string _file;
	boost::asio::ip::tcp::socket _socket;
	boost::asio::ip::tcp::resolver _resolver;

	//OrderBookManagerImproved _orderBookManager;

	OrderBookType2CB _cb;
	OrderBookManagerType2<OrderBookType2CB> _orderBookManager;

	//OrderBookManagerBase<OrderBookManagerTemp> _orderBookManager;
	//OrderBookManagerBase<OrderBookManagerOld> _orderBookManager;

	Logger<FileLogPolicy> _logger;


	Subscriber(boost::asio::io_service& io, const std::string& host, const std::string& port, const std::string& file) : _file(file),
			_socket(io),
			_resolver(io),
			_orderBookManager(_cb, 100000),
			_logger("Subscriber", "logs/Subscribe"){
		_orderBookManager.init(_file);
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
				boost::asio::buffer(_data, sizeof(MktDataGlobalHeaderMsg) + sizeof(char)),
				[this](boost::system::error_code ec, std::size_t length)
				{
			if (!ec)
			{

				MktDataGlobalHeaderMsg* header = new MktDataGlobalHeaderMsg;
				memcpy((void*)header, (void*)(_data), sizeof(MktDataGlobalHeaderMsg));
				char* msgType = new char;
				memcpy((void*)msgType, (void*)(_data + sizeof(MktDataGlobalHeaderMsg)), sizeof(char));
				std::cout << " Packet#: " << ++pcount << std::endl;
				doReadBody(header, msgType);
			}
			else
			{
				std::cout << "Exception comes - Socket Closed. " << std::endl;
				_socket.close();
			}
				});
	}

	void doReadBody(MktDataGlobalHeaderMsg* header, char* msgType){

		switch(*msgType){
		case 'N':
		case 'M':
		case 'X':
		case 'G':
		case 'H':
		case 'J':
		{
			boost::asio::async_read(_socket,
					boost::asio::buffer(_data, sizeof(MktDataOrderMsg)),
					[this, &header, &msgType](boost::system::error_code ec, std::size_t length)
					{
				if(!ec){
					MktDataOrderMsg* body = new MktDataOrderMsg;
					memcpy((void*)body, (void*)_data, sizeof(MktDataOrderMsg));
					Order::SharedPtr order(new SimpleOrder(*header, *msgType, *body));
					{
						std::string str;
						try{
							LatencyChecker<> lc(str);
							_orderBookManager.addOrder(order);
						}catch(const std::runtime_error& err){
							std::cout << " Exception " << err.what() << std::endl;
						}
						order->toString(str);
						_orderBookManager.printOrderBookForSymbol(str, body->_toeknNo);
						_logger.log(std::move(str));
					}

					delete body;

					doReadHeader();
				}else{
					std::cout << "Exception comes - Socket Closed. " << std::endl;
					_socket.close();
				}
					});

		}
		break;
		case 'T':
		case 'K':
		{
			boost::asio::async_read(_socket,
					boost::asio::buffer(_data, sizeof(MktDataTradeMsg)),
					[this, &header, &msgType](boost::system::error_code ec, std::size_t length)
					{
				if(!ec){
					MktDataTradeMsg* body = new MktDataTradeMsg;
					memcpy((void*)body, (void*)_data, sizeof(MktDataTradeMsg));
					Trade::SharedPtr trade(new SimpleTrade(*header, *msgType, *body));

					{
						std::string str;
						try{
							LatencyChecker<> lc(str);
							_orderBookManager.addTrade(trade);
						}catch(const std::runtime_error& err){
							std::cout << " Exception " << err.what() << std::endl;
						}
						trade->toString(str);
						_orderBookManager.printOrderBookForSymbol(str, body->_toeknNo);
						_logger.log(std::move(str));
					}
					delete body;

					doReadHeader();
				}else{
					std::cout << "Exception comes - Socket Closed. " << std::endl;
					_socket.close();
				}
					});
		}
		break;
		default:
			std::cout << " Error: Unknown Packet comes.... " << std::endl;
			break;
		}
	}
};
}



#endif /* INCLUDE_SUBSCRIBER_HPP_ */
