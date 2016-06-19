/*
 * Publisher.hpp
 *
 *  Created on: 14-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_PUBLISHER_HPP_
#define INCLUDE_PUBLISHER_HPP_

#include <Common.hpp>
#include <OrderGenerator.hpp>
#include <spdlog/spdlog.h>

namespace obLib{

struct Session : public std::enable_shared_from_this<Session>
{
	boost::asio::ip::tcp::socket _socket;
	std::string _filePath;
	bool _runForSingleSec;
	enum{
		maxLength = 1024
	};
	char _data[maxLength];


	Session(boost::asio::ip::tcp::socket socket, const std::string& file, bool forSingleSec = false) : _socket(std::move(socket)),
			_filePath(file), _runForSingleSec(forSingleSec){

	}

	void start(){
		doRead();
	}

	void doWrite(char * buff, std::size_t length){
		auto self(shared_from_this());
		boost::asio::async_write(_socket, boost::asio::buffer(buff, length),
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
				{
			if (!ec)
			{
				//std::cout << " Write Successful. " << std::endl;
			}else{
				throw std::runtime_error("Socket Closed.");
			}
				});
	}

private:
	void doRead(){
		OrderGenerator * og = new OrderGenerator(_filePath);
		int count = 0;
		// 43722 11589   48103 31317   47958 115526
		for(OrderGenerator::MktDataContainer::value_type& v : og->getMktData()){

			if(!_socket.is_open()){
				std::cout << " Socket has been closed. " << std::endl;
				break;
			}

			char * data = new char[maxLength];

			switch(v.second.first){
			case 'N':
			case 'M':
			case 'X':
			{
				memcpy((void*)data,(void*)(boost::get<MktDataOrderMsg*>(v.second.second)), sizeof(MktDataOrderMsg));
				try{
					if(_runForSingleSec){
						if((boost::get<MktDataOrderMsg*>(v.second.second))->_toeknNo == 47958){
							doWrite(data, sizeof(MktDataOrderMsg));
							std::cout << " Writing Order: " << ++count << std::endl;
						}
					}else{
						doWrite(data, sizeof(MktDataOrderMsg));
						std::cout << " Writing Order: " << ++count << std::endl;
					}
				}catch(const std::runtime_error& err){
					_socket.close();
				}
			}
			break;
			case 'T':
			{
//				memcpy((void*)data,(void*)(boost::get<MktDataTradeMsg*>(v.second.second)), sizeof(MktDataTradeMsg));
				try{
					// To Do - Will open this later
					//doWrite(data, sizeof(MktDataOrderMsg));
					//std::cout << " Writing Trade: " << ++count << std::endl;
				}catch(const std::runtime_error& err){
					_socket.close();
				}
			}
			break;
			default:
				break;
			}
			delete [] data;
			//boost::this_thread::sleep(boost::posix_time::microseconds(10));
		}
		delete og;
	}
};

struct Publisher
{

	boost::asio::ip::tcp::acceptor _acceptor;
	boost::asio::ip::tcp::socket _socket;
	std::string _udpFilePath;
	bool _runForSingleSec;

	Publisher(boost::asio::io_service& ioService, int port, const std::string& filePath, bool forSingleSec = false) :
		_acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		_socket(ioService),
		_udpFilePath(filePath),
		_runForSingleSec(forSingleSec){
		doAccept();
	}

private:
	void doAccept(){
		std::cout << " Waitig for Connection Acceptance.  " << std::endl;
		_acceptor.async_accept(_socket, [this](boost::system::error_code ec){
			if(!ec){
				std::make_shared<Session>(std::move(_socket), _udpFilePath, _runForSingleSec)->start();
			}else{
				std::cout << " Error: " << ec.message() << std::endl;
				return;
			}
			doAccept();
		});
	}

};
}



#endif /* INCLUDE_PUBLISHER_HPP_ */
