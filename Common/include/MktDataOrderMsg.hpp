/*
 * MktDataOrder.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_MKTDATAORDERMSG_HPP_
#define INCLUDE_MKTDATAORDERMSG_HPP_

#include <MktDataGlobalHeaderMsg.hpp>

namespace obLib{

#pragma pack(push, 1) // For memory alignment
  struct MktDataOrderMsg
  {
    MktDataGlobalHeaderMsg _globalMktDataHeader; // 8 Bytes - 0
    char _msgType; // 'N' - New / 'M' - Modify / 'X' - Cancel - 8
    int64_t _timestamp; // milliseconds from 01-Jan-1980 00:00:00 - 9
    int64_t _orderId; // Day Unique Order Ref # - 17
    int32_t _toeknNo; // Unique Contract Identifier - 25
    char _orderType; // 'B' / 'S' - 29
    int32_t _price; // in paise , // The price is in multiples of the tick size. - 30
    // For FO and CM segments this should be divided by 100 for converting into Rupees
    // For CD segment this should be divided by 10^7 for converting into Rupees. -
    int32_t _qty; // - 34

    void print() const{
      std::cout << " Message - Header: [ " << _globalMktDataHeader._len << " " << _globalMktDataHeader._streamId << " " << _globalMktDataHeader._seqNo << " ] "
	  << " Body: [ " << _msgType << " " << _timestamp << " " << _orderId << " " << _toeknNo << " " << _orderType << " " << _price << " " << _qty << " ] " << std::endl;
    }
  };
#pragma pack(pop)

}



#endif /* INCLUDE_MKTDATAORDERMSG_HPP_ */
