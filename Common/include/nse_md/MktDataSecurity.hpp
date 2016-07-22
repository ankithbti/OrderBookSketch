/*
 * MktDataSecurity.hpp
 *
 *  Created on: 23-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_MD_MKTDATASECURITY_HPP_
#define COMMON_INCLUDE_NSE_MD_MKTDATASECURITY_HPP_

// StreamID - short
// Token # - int
// Instrument - char[6] // right padded with blanks and is not NULL terminated.
// Symbol - char[10]
// expiry date - int
// strike price - int
// option type - char[2]

namespace obLib{

class MktDataSecurity{
	int16_t _streamId;
	int32_t _tokenNumber;
	char _instrument[6];
	char _symbol[10];
	int32_t _expiryDate; // Expiry date of contract in seconds from 01-Jan-1980 00:00:00
	int32_t _strikePrice; // in paise
	char _optionType[2];
};

}


#endif /* COMMON_INCLUDE_NSE_MD_MKTDATASECURITY_HPP_ */
