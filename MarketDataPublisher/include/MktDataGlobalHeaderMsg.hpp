/*
 * MktDataHeader.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_MKTDATAGLOBALHEADERMSG_HPP_
#define INCLUDE_MKTDATAGLOBALHEADERMSG_HPP_

#include <Common.hpp>

namespace obLib
{

#pragma pack(push, 1) // For memory alignment
  struct MktDataGlobalHeaderMsg
  {
    int16_t _len;
    int16_t _streamId;
    int32_t _seqNo; // will be 0 in case of HB message
  };
}
#pragma pack(pop)



#endif /* INCLUDE_MKTDATAGLOBALHEADERMSG_HPP_ */
