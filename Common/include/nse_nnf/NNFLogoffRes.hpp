/*
 * NNFLogoffRes.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_NNFLOGOFFRES_HPP_
#define COMMON_INCLUDE_NSE_NNF_NNFLOGOFFRES_HPP_

#include <Common.hpp>

namespace obLib{

#pragma pack(push, 2) // For memory alignment

// SIGN_OFF_REQUEST_OUT (2321) - 190 Bytes - including Header - 40 bytes
struct NNFLogoffRes{
	int32_t _userId;
	char _reserved[145];
};
#pragma pack(pop)

}




#endif /* COMMON_INCLUDE_NSE_NNF_NNFLOGOFFRES_HPP_ */
