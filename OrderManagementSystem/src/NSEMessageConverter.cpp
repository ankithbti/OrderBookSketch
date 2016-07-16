/*
 * NSEMessageConverter.cpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#include <NSEMessageConverter.hpp>

namespace oms{


NSEMessageConverter::NSEMessageConverter(){

}

NSEMessageConverter::~NSEMessageConverter(){

}

IExchangeMessagePtr NSEMessageConverter::convert(const char* msgBuff, size_t size){
	// This is the place where we will parse the message to NSEMessage
	// This will be used in processInMessage in NSESession
	// Validation about Len / SeqNo / CheckSum should be done here.
}

}


