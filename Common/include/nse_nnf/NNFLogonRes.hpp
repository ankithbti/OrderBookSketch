/*
 * NNFLogonRes.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_NNFLOGONRES_HPP_
#define COMMON_INCLUDE_NSE_NNF_NNFLOGONRES_HPP_

#include <nse_nnf/NNFHeader.hpp>

namespace obLib{

#pragma pack(push, 2) // For memory alignment

// SIGN_ON_REQUEST_OUT (2301) - 214 bytes [ with Header - 40 bytes ]
struct NNFLogonRes{
	int32_t _userId;	// This field should contain the user ID of the member/broker. It accepts
	// numbers only
	char _password[8];	// The password should be of exact eight characters in length
	char _newPassword[8];	// This field should be entered only when the trader wishes to
	// change the password or the password has expired
	// The new password should be of eight characters.
	// The new password should be alphanumeric i.e password should contain
	// 1 upper case letter,1 lower case letter,
	// 1 numeral and 1 special character from the list @#$%&*/\.
	// The new password entered should not be from the last 5 passwords.
	//Otherwise this field should be blank.
	// The New Password should be entered along with the old password in the
	// Password field. While logging on to the system for the first time,
	// the default password provided by NSE i.e Neat@FO1 must be changed.
	char _traderName[26];			// This field should be set to blank while sending to the host
	int32_t _lastPassChangeDate;	// This field should be set to numerical zero while log on.
	char _brokerId[5];			// This field should contain the trading member ID.
	char _reserved1;			//
	int16_t _branchId;			// This field should contain the Branch ID to which the broker belongs.
	// Branch ID can be of 3 digits.
	int32_t _versionNumber;		// This field should contain the version number of the trading system
	// The format is VERSION.RELEASE.SUB_RELEASE. (For example, 7.02.00)
	int32_t _batch2StartTime;	// This field should be set to numerical zero.
	char _hostSwitchContext;	// This field should be set to blank.
	char _colour[50];		// This field should be set to blank.
	char _reserved2;		// This field should be set to blank.
	int16_t _userType;		//	This field should be set to zero while sending to the host.
	int64_t _seqNo;			// This field should be set to numerical zero while sending the request to host.

	char _wsClassName[14];	// This field should contain the network ID of the workstation.
	// This is a seven digit number.
	// The first five digits are fixed by the Exchange and represent
	// the various port / switch locations.
	// The last two digits denote the user’s PC - ID.
	// It must be any number other than ‘00’.

	char _broketStatus;		// This field should be set to blank.
	char _showIndex;		// This field should be set to blank
	NNFStBroketEligibilityPerMkt _stBroketEligibilityPerMkt;	// This field should be set to numerical zero
	int16_t _memberType;	// This field should be set to numerical zero
	char _clearingStatus;	// This field should be set to blank.
	char _brokerName[25];	// This field should contain the broker’s name.
};
#pragma pack(pop)

}



#endif /* COMMON_INCLUDE_NSE_NNF_NNFLOGONRES_HPP_ */
