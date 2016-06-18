/*
 * MktDataHeartBeat.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_MKTDATAHEARTBEATMSG_HPP_
#define INCLUDE_MKTDATAHEARTBEATMSG_HPP_


#include <MktDataGlobalHeaderMsg.hpp>

namespace obLib{

#pragma pack(push, 1) // For memory alignment
  struct MktDataHBMsg
  {
    MktDataGlobalHeaderMsg _globalMktDataHeader; // Header will have 0 SeqNo in HB case
    char _msgType; // 'Z'
    int32_t _lastSeqNo; // Last sent data sequence no. of the Stream
  };
#pragma pack(pop)

}



#endif /* INCLUDE_MKTDATAHEARTBEATMSG_HPP_ */
