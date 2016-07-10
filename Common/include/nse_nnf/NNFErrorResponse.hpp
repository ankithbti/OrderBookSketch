/*
 * NNFErrorResponse.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_NNFERRORRESPONSE_HPP_
#define COMMON_INCLUDE_NSE_NNF_NNFERRORRESPONSE_HPP_

#include <Common.hpp>

namespace obLib{

#pragma pack(push, 2) // For memory alignment


struct NNFErrorResponse{
	char _key[14]; // This field contains the token number of the Contract
	char _errorMessage[128]; // This field contains the error message.
};
#pragma pack(pop)

}

#endif /* COMMON_INCLUDE_NSE_NNF_NNFERRORRESPONSE_HPP_ */
