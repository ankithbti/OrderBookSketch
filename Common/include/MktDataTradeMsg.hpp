/*
 * MktDataTrade.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_MKTDATATRADEMSG_HPP_
#define INCLUDE_MKTDATATRADEMSG_HPP_

#include <MktDataGlobalHeaderMsg.hpp>

namespace obLib{

#pragma pack(push, 1) // For memory alignment
  struct MktDataTradeMsg
  {
    MktDataGlobalHeaderMsg _globalMktDataHeader;
    char _msgType; // 'T'
    int64_t _timestamp; // milliseconds from 01-Jan-1980 00:00:00

    // Any one of the below can be 0
    int64_t _buySideOrderId; // Day Unique Order Ref # for Buy Side Order
    int64_t _sellSideOrderId; // Day Unique Order Ref # for Sell Side Order

    int32_t _toeknNo; // Unique Contract Identifier
    int32_t _price; // in paise , // The price is in multiples of the tick size.
		    // For FO and CM segments this should be divided by 100 for converting into Rupees
		    // For CD segment this should be divided by 107 for converting into Rupees.
    int32_t _qty;
  };
#pragma pack(pop)

}

#endif /* INCLUDE_MKTDATATRADEMSG_HPP_ */
