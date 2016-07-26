/*
 * Common.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_COMMON_HPP_
#define INCLUDE_COMMON_HPP_

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <chrono>
#include <memory>
#include <utility>
#include <cstdlib>
#include <limits>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <pthread.h>
#include <fstream>
#include <atomic>
#include <stack>
#include <deque>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/variant.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
namespace obLib{

#define LOG(m) \
    std::cout << m << std::endl

using Price = int32_t;
using Quantity = int32_t;
using OrderId = int64_t;
using ChangeId = int32_t;
using TokenId = int32_t;
using SeqNo = int32_t;

const Price INVALID_LEVEL_PRICE(0);
const Price PRICE_UNCHANGED(0);
const int32_t SIZE_UNCHANGED(0);

#define WMB() __asm__ __volatile__ ("" : : : "memory")
#define PAUSE() __asm__ __volatile__ ("rep; nop" : : : "memory")
#define INTEL_CACHE_LINE 64

static inline int xchg(volatile int *ptr, int x){
	__asm__("xchgl %0,%1" : "=r" (x) : "m" (*(ptr)), "0" (x) : "memory");
	return x;
}

inline void fast_l2a(char* buf, int len, unsigned long i){
	for(--len; len >= 0; --len){
		if(!i){
			buf[len] = '0';
		}else{
			buf[len] = (char)('0' + (i%10));
			i /= 10;
		}
	}
}

inline void fast_i2a(char* buf, int len, unsigned i){
	for(--len; len >= 0; --len){
		if(!i){
			buf[len] = '0';
		}else{
			buf[len] = (char)('0' + (i%10));
			i /= 10;
		}
	}
}

template <int n>
inline void fast_i2a(char* buf, unsigned i){
	for(int len = (n-1); len >= 0; --len){
		if(!i){
			buf[len] = '0';
		}else{
			buf[len] = (char)('0' + (i%10));
			i /= 10;
		}
	}
}

template<>
inline void fast_i2a<3>(char *buf, unsigned i){
	buf[2] = (char)('0' + (i%10)); i /= 10;
	buf[1] = (char)('0' + (i%10)); i /= 10;
	buf[0] = (char)('0' + (i%10));
}

template<>
inline void fast_i2a<2>(char *buf, unsigned i){
	buf[1] = (char)('0' + (i%10)); i /= 10;
	buf[0] = (char)('0' + (i%10));
}

template<>
inline void fast_i2a<1>(char *buf, unsigned i){
	buf[0] = (char)('0' + (i%10));
}

inline unsigned computeFixCheckSum(char * buf, size_t len){
	unsigned checksum = 0;
	for(size_t i = 0; i < len ; ++i){
		checksum += buf[i];
	}
	return checksum;
}

inline double fast_atof(const char* p){
	bool frac(false);
	double sign(1.), value(0.), scale(1.);
	while(*p == ' '){
		++p;
	}
	if(*p == '-'){
		sign = -1.;
		++p;
	}else if(*p == '+'){
		++p;
	}

	// Digits before decimal point
	while((unsigned int)(*p - '0') < 10){
		value = value * 10. + (*p - '0');
		++p;
	}

	// After decimal point
	if(*p == '.'){
		++p;
		double p10(10.);
		while((unsigned int)(*p - '0') < 10){
			value += (*p - '0') / p10;
			p10 *= 10;
			++p;
		}
	}

	// Handle Exponent
	if(toupper(*p) == 'E'){
		unsigned int expon(0);
		++p;

		if(*p == '-'){
			frac = true;
			++p;
		}
		else if(*p == '+'){
			++p;
		}

		// Digits of Exp
		while((unsigned int)(*p - '0') < 10){
			expon = expon * 10 + (*p - '0');
			++p;
		}

		if(expon > 308){
			expon = 308;
		}

		// Calculate scaling factor
		while(expon >= 50){
			scale *= 1E50;
			expon -= 50;
		}
		while(expon >= 8){
			scale *= 1E8;
			expon -= 8;
		}
		while(expon > 0){
			scale *= 10.0;
			expon -= 1;
		}
	}
	return sign * (frac ? (value/scale) : (value * scale));
}



}

#endif /* INCLUDE_COMMON_HPP_ */
