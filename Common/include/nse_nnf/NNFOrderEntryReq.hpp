/*
 * NNFOrderEntryReq.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_NNFORDERENTRYREQ_HPP_
#define COMMON_INCLUDE_NSE_NNF_NNFORDERENTRYREQ_HPP_


#include <nse_nnf/NNFHeader.hpp>

namespace obLib{

// BOARD_LOT_IN (2000) - 240 Bytes [ Header - 40 bytes ]
struct NNFOrderEntryReq{

	char _participantType;
	char _reserved1;
	int16_t _competitorPeriod;
	int16_t _solicitorPeriod;
	char _modifiedOrCancelledBy;
	char _reserved2;
	int16_t _reasonCode;
	char _reserved3[4];
	int32_t _tokenNumber;
	ContractDesc _contractDesc;
	char _counterPartyBrokerId[5];
	char _reserved4;
	char _reserved5[2];
	char _closeoutFlag;
	char _reserved6;
	int16_t _orderType;
	int64_t _orderNumber;
	char _accountNumber[10];
	int16_t _bookType;
	int16_t _buySellIndicator;
	int32_t _disclosedVol;
	int32_t _disclosedVolRem;
	int32_t _totalVolRem;
	int32_t _volume;
	int32_t _volFilledToday;
	int32_t _price;
	int32_t _triggerPrice;
	int32_t _goodTillDate;
	int32_t _entryDateTime;
	int32_t _minimumFill;
	int32_t _lastModified;
	StOrderFlags _stOrderFlags;
	int16_t _branchId;
	int32_t _traderId;
	char _brokerId[5];
	char _clientOrderFiller[24];
	char _openClose;
	char _settlor[12];
	int16_t _clientIndicator;
	int16_t _settlementPrice;
	AdditionalOrderFlags _additionalOrderFlags;
	char _giveupFlag;
	OrderFillerStruct _fillerStruct;
	int64_t _nnfField;
	int64_t _mktReplay;
};

// Note: Order Response Message (2001) is stopped to reduce the packet sent from the host end.
// Responses
// 1 - PRICE_CONFIRMATION (2012)
// 2 - ORDER_CONFIRMATION_OUT (2073)
// 3 - FREEZE_TO_CONTROL (2170)
// 4 - ORDER_ERROR_OUT (2231)

// To Modify Order
// 1 - ORDER_MOD_IN (2040)

// Note: Order Modification Response (2041) is stopped to reduce the packet sent from the host end.

// Response of Modify
// 1 - ORDER_MOD_CONFIRM_OUT (2074).
// 2 - ORDER_MOD_REJ_OUT (2042).

}


#endif /* COMMON_INCLUDE_NSE_NNF_NNFORDERENTRYREQ_HPP_ */
