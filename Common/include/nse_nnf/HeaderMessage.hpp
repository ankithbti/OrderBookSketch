/*
 * HeaderMessage.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_HEADERMESSAGE_HPP_
#define COMMON_INCLUDE_NSE_NNF_HEADERMESSAGE_HPP_

#include <Common.hpp>
#include <nse_nnf/NNFLogonReq.hpp>

namespace obLib{

class HeaderMessage {

private:
	NNFHeader* _headerPOD;

public:
	void setUserId(short userId);
	void traderId(short userId);
	void setAlphaChar(char* alphaChar);
	void setTransactionCode(short tCode);
	void setMessageLength(short len);
};

}




#endif /* COMMON_INCLUDE_NSE_NNF_HEADERMESSAGE_HPP_ */
