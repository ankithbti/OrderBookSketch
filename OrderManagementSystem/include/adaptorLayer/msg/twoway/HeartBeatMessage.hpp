/*
 * HeartBeatMessage.hpp
 *
 *  Created on: 23-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_TWOWAY_HEARTBEATMESSAGE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_TWOWAY_HEARTBEATMESSAGE_HPP_

#include <adaptorLayer/msg/BaseMessage.hpp>

namespace oms{

// HB only has Header only with TC - 23506
class HeartBeatMessage : RequestBaseMessage{

	HeartBeatMessage(){
		_msgHeader.setTransactionCode(TC_HEARTBEAT);
	}

	virtual std::string getMsgName() const{
		return "HEARTBEAT";
	}

	virtual void marshall(LocalBuffer* buffer){

	}

	virtual void unmarshall(LocalBuffer* buffer) {

	}

	virtual int finalizeMessage(LocalBuffer* buffer, size_t msgLastPos){
		return 0;
	}

	virtual void dump(std::ostream& ){

	}
};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_TWOWAY_HEARTBEATMESSAGE_HPP_ */
