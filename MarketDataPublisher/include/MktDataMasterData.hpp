/*
 * MktDataMasterData.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_MKTDATAMASTERDATA_HPP_
#define INCLUDE_MKTDATAMASTERDATA_HPP_

#include <MktDataGlobalHeaderMsg.hpp>

namespace obLib
{

#pragma pack(push, 1) // For memory alignment
  struct MktDataMasterDataHeaderMsg
  {
    MktDataGlobalHeaderMsg _globalMktDataHeader;
    char _msgType; // 'B'
    int32_t _toekncount;
  };
#pragma pack(pop)

#pragma pack(push, 1) // For memory alignment
  struct MktDataMasterDataContractInformationMsg
  {
    MktDataGlobalHeaderMsg _globalMktDataHeader;
    char _msgType; // 'C'
    int16_t _streamId;
    int32_t _tokenNo;
    char _instrument[6]; // The instrument received is right padded with blanks and is not NULL terminated.
			 // CM: Set to “EQUITY”.
    char _symbol[10]; // The symbol received is right padded with blanks and is not NULL terminated.
    int32_t _expiryDate; // seconds from 01-Jan-1980 00:00:00
    int32_t _strikePrice; // In paise - For FO segment this should be divided by 100 for converting into Rupees.
			  // For CD segment this should be divided by 107 for converting into Rupees.
			  // This will be zero in case of futures contract
			  // CM: Not used, set to 0.
    char _optionType[2];

  };
#pragma pack(pop)

#pragma pack(push, 1) // For memory alignment
  struct MktDataMasterDataTrailerMsg
  {
    MktDataGlobalHeaderMsg _globalMktDataHeader;
    char _msgType; // 'E'
    int32_t _toekncount;
  };
#pragma pack(pop)


}

#endif /* INCLUDE_MKTDATAMASTERDATA_HPP_ */
