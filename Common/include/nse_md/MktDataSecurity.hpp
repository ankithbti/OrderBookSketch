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

class MktDataDictionaryOptMsg{
public:
	int16_t _streamId;

	std::string _exchange; // exchange Exchange
	int32_t _tokenNumber; // exchange OptionId
	char _instrument[6]; // exchange inst Symbol;
	char _symbol[10]; // exchange Option Symbol

	int _localExchangeId;
	int _localInstId;
	int _localOptionId;
	std::string _localSymbol;

	int32_t _expiryDate; // Expiry date of contract in seconds from 01-Jan-1980 00:00:00
	int32_t _strikePrice; // in paise
	char _optionType[2];


	// might implement getter / setter
};
using MktDataDictionaryOptMsgPtr = std::shared_ptr<MktDataDictionaryOptMsg>;

}


#endif /* COMMON_INCLUDE_NSE_MD_MKTDATASECURITY_HPP_ */
