/*
 * TcpClientSession.hpp
 *
 *  Created on: 06-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_SESSION_TCP_TCPCLIENTSESSION_HPP_
#define COMMON_INCLUDE_SESSION_TCP_TCPCLIENTSESSION_HPP_


#include <Common.hpp>

namespace obLib
{

class TcpClientSession
{

	int _socketFileDesc;
	struct addrinfo * _serverInfo; // A Linked list
	char _remoteHost[INET6_ADDRSTRLEN];
	std::string _remotePort;
	std::shared_ptr<std::thread> _receiverThread;
	volatile bool _shutdown;


public:

	TcpClientSession(const std::string& host, const std::string& port);
	bool connect();
	void disconnect();
	void send(const char* buffer, size_t len);


	inline const char * getRemoteHost() const {
		return _remoteHost;
	}

	inline const std::string& getRemotePort() const {
		return _remotePort;
	}

	~TcpClientSession();

private:
	void receiverThread();
};


}


#endif /* COMMON_INCLUDE_SESSION_TCP_TCPCLIENTSESSION_HPP_ */
