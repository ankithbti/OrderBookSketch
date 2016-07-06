/*
 * TcpClientSession.cpp
 *
 *  Created on: 06-Jul-2016
 *      Author: ankithbti
 */

#include <session/tcp/TcpClientSession.hpp>

namespace obLib
{

#define MAX_REC_SIZE 1024

TcpClientSession::TcpClientSession(const std::string& host, const std::string& port) : _socketFileDesc(-1),
		_serverInfo(NULL), _remotePort(port), _shutdown(true){

	int status = 0;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; // TCP

	if((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &_serverInfo)) != 0){
		// Error
		throw std::runtime_error("TcpClientSession::TcpClientSession() - Error : " + std::string(gai_strerror(status)));
	}

	// Lets print the serverinfo
	struct addrinfo *p;
	char ipstr[INET6_ADDRSTRLEN];
	for(p = _serverInfo ; p!= NULL; p = p->ai_next){
		void *addr;
		const char *ipver;
		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		std::cout <<  ipver << ": " << ipstr << std::endl;
	}
}

bool TcpClientSession::connect(){
	struct addrinfo *p;
	for(p = _serverInfo ; p!= NULL; p = p->ai_next){
		void *addr;
		const char *ipver;
		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		if( (_socketFileDesc = socket(p->ai_addr->sa_family, p->ai_socktype, p->ai_protocol)) != -1){
			// Got socket File Descriptor Successfully

			// Set Nonblocking Mode ON
			::fcntl(_socketFileDesc, F_SETFL, O_NONBLOCK);

			// Try connecting to the IP
			if( (::connect(_socketFileDesc, p->ai_addr, p->ai_addrlen) != -1) ){
				inet_ntop(p->ai_family, addr, _remoteHost, sizeof(_remoteHost));
				// Connection successful
				_shutdown = false;
				_receiverThread = std::make_shared<std::thread>(std::bind(&TcpClientSession::receiverThread, this));
				return true;
			}
		}
	}
	return false;
}

void TcpClientSession::disconnect(){
	_shutdown = true;
	_receiverThread->join();
	_receiverThread.reset();
}

TcpClientSession::~TcpClientSession(){
	// Free the serverInfo Linked list
	freeaddrinfo(_serverInfo);
	disconnect();
}


void TcpClientSession::send(const char* buffer, size_t len)
{

}

void TcpClientSession::receiverThread(){
	std::cout << " Started Rec thread. " << std::endl;
	while(!_shutdown){

//		char buf[MAX_REC_SIZE];
//		int ret = recv(_socketFileDesc, buf, MAX_REC_SIZE-1, 0);
//		switch(ret){
//		case -1:
//			std::cout << " Failed Reading.  " << std::endl;
//			break;
//		default:
//			buf[ret] = '\0';
//			std::cout << " Read Buff: " << buf << std::endl;
//			break;
//		}

	}
	std::cout << " End Rec thread. " << std::endl;
}

}

