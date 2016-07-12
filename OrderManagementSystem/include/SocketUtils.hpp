/*
 * SocketUtils.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_SOCKETUTILS_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_SOCKETUTILS_HPP_


#include <Common.hpp>

namespace oms{

class SocketUtils{
public:
	static int createConnection(const std::string& host, const std::string& port, bool noDelay, bool nonBlock,
			int socketBufferSize, int connectionTimeout);
	static int getSocketRecvBufferSize(int sd);
	static int getSocketSendBufferSize(int sd);
};

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_SOCKETUTILS_HPP_ */
