/*
 * SocketUtils.cpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#include <SocketUtils.hpp>

namespace oms{

int createConnection(const std::string& host, const std::string& port, bool noDelay, bool nonBlock,
		int socketBufferSize, int connectionTimeout){

	int sd;
	int status = 0;
	struct addrinfo * _serverInfo; // A Linked lists
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; // TCP

	if((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &_serverInfo)) != 0){
		// Error
		throw std::runtime_error("Error : " + std::string(gai_strerror(status)));
	}

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
		if( (sd = ::socket(p->ai_addr->sa_family, p->ai_socktype, p->ai_protocol)) != -1){
			// Got socket File Descriptor Successfully

			if(noDelay){
				int optVal = 1;
				::setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, (char*)&optVal, sizeof(optVal));
			}
			if(nonBlock){
				int flags = fcntl(sd, F_GETFL, 0);
				fcntl(sd, F_SETFL, flags | O_NONBLOCK);
			}
			if(socketBufferSize){
				::setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (void*)&socketBufferSize, (int)sizeof(socketBufferSize));
				::setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (void*)&socketBufferSize, (int)sizeof(socketBufferSize));
			}

			int res = ::connect(sd, p->ai_addr, p->ai_addrlen);
			if(res == 0){
				return sd;
			}

			if(nonBlock){
				if((errno == EINPROGRESS ) || (errno == EWOULDBLOCK)){
					struct timeval tv;
					tv.tv_sec = connectionTimeout;
					tv.tv_usec = 0;

					fd_set writeset;
					FD_ZERO(&writeset);
					FD_SET(sd, &writeset);

					if(( ::select(sd+1, NULL, &writeset, NULL, &tv) > 0) && FD_ISSET(sd, &writeset)){
						int err = 0;
						socklen_t len = sizeof(err);

						if((::getsockopt(sd, SOL_SOCKET, SO_ERROR, (void*)&err, &len) != -1) && (err == 0) ){
							return sd;
						}

						errno = err;
						::perror("ready socket has error");
					}

				}else{
					::perror("non-blocking connect failed.");
				}
			}else{
				::perror("Connect Failed");
			}
			::close(sd);
		}
	}
	::perror("Not able to connect.");
	return -1;
}
int getSocketRecvBufferSize(int sd){
	int socketBufferSize = 0;
	socklen_t size = sizeof(socketBufferSize);
	int rv = getsockopt(sd, SOL_SOCKET, SO_RCVBUF, (void*)&socketBufferSize, &size);
	return (rv != -1) ? socketBufferSize : -1;

}
int getSocketSendBufferSize(int sd){
	int socketBufferSize = 0;
	socklen_t size = sizeof(socketBufferSize);
	int rv = getsockopt(sd, SOL_SOCKET, SO_SNDBUF, (void*)&socketBufferSize, &size);
	return (rv != -1) ? socketBufferSize : -1;
}
}


