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

namespace obLib{

  struct Session : public std::enable_shared_from_this<Session>
  {
    boost::asio::ip::tcp::socket _socket;
	std::string _filePath;
    enum{
      maxLength = 1024
    };
    char _data[maxLength];


    Session(boost::asio::ip::tcp::socket socket, const std::string& file) : _socket(std::move(socket)), _filePath(file){

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
      for(auto& o : og->getOrders()){
	  if(!_socket.is_open()){
	      std::cout << " Socket has been closed. " << std::endl;
	      break;
	  }
	  if(o->_msgType == 'N' || o->_msgType == 'M' || o->_msgType == 'X'){
	      // 43722 11589   48103 31317   47958 115526
	      if(o->_toeknNo != 47958){
		  continue;
	      }
	      std::cout << " Sending Packet # " << ++count << std::endl;
	      o->print();
	      char * data = new char[maxLength];
	      memcpy((void*)data,(void*)o, sizeof(MktDataOrderMsg));
	      try{
		  doWrite(data, sizeof(MktDataOrderMsg));
	      }catch(const std::runtime_error& err){
		  _socket.close();
	      }
	      delete [] data;
	      boost::this_thread::sleep(boost::posix_time::microseconds(100));
	  }
      }
    }


  };

  struct Publisher
  {

    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _socket;
	std::string _udpFilePath;

    Publisher(boost::asio::io_service& ioService, int port, const std::string& filePath) : _acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _socket(ioService), _udpFilePath(filePath){
      doAccept();
    }

  private:
    void doAccept(){
      std::cout << " Waitig for Connection Acceptance.  " << std::endl;
      _acceptor.async_accept(_socket, [this](boost::system::error_code ec){
	if(!ec){
	    std::make_shared<Session>(std::move(_socket), _udpFilePath)->start();
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
