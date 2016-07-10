/*
 * LogonMessage.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_LOGONMESSAGE_HPP_
#define COMMON_INCLUDE_NSE_NNF_LOGONMESSAGE_HPP_



#include <Common.hpp>
#include <nse_nnf/NNFLogonReq.hpp>
#include <nse_nnf/HeaderMessage.hpp>

namespace obLib{

class LogonMessage {

private:

	char * _buffer;
	PacketHeader _finalPktHeader;
	HeaderMessage _dataHeader;
	NNFLogonReq* _logonReqPOD;

public:




};

}

#endif /* COMMON_INCLUDE_NSE_NNF_LOGONMESSAGE_HPP_ */
