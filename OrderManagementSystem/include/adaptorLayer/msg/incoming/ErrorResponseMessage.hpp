/*
 * ErrorResponseMessage.hpp
 *
 *  Created on: 23-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_INCOMING_ERRORRESPONSEMESSAGE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_INCOMING_ERRORRESPONSEMESSAGE_HPP_


#include <adaptorLayer/msg/BaseMessage.hpp>

// 10th pos - Transaction Code in MsgHeader
// 12th pos - Error Code in MsgHeader

// When the Error Code in the MESSAGE_HEADER is not zero, the structure sent is ERROR RESPONSE

/*Key // This field contains the token number of the Contract
CHAR
14
40

ErrorMessage // This field contains the error message.
CHAR
128
54
*/
namespace oms{
class ErrorResponseMessage : NormalResponseBaseMessage{
public:

};
}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_INCOMING_ERRORRESPONSEMESSAGE_HPP_ */
