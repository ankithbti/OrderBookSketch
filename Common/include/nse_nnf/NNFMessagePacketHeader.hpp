/*
 * NNFMessagePacket.hpp
 *
 *  Created on: 11-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_NSE_NNF_NNFMESSAGEPACKETHEADER_HPP_
#define COMMON_INCLUDE_NSE_NNF_NNFMESSAGEPACKETHEADER_HPP_

namespace obLib{
// This structure is applicable to all messages that flow between NNF and Host through TAP box.

// Len [ 2 bytes ] + SeqNo [ 4 bytes ] + CheckSum [ MD5 - 16 bytes ] + Data [ 40 bytes Header + Body ]

// Max length will be the predefined value of 1024 bytes.
// Len = size of length field (2 bytes) + 4 + 16 + size of Data
// Sequence number will start from 1 and will be incremented for every packet
// The checksum algorithm used will be MD5.
// Checksum is applied only on the Message data field and not on the entire packet.
// MD5 - http://www.faqs.org/rfcs/rfc1321.html

// Validation will be done through the combination of Checksum, Sequence Number and Length field.
// If any one of these validations fails,
// the front-end needs to drop the connection and re-establish a fresh connection.
// Same way Host will disconnect client also if validation fails.


#pragma pack(push, 2)
struct NNFMessagePktHeader{
	int16_t _pktLength;
	int32_t _seqNo;
	char _checksum[16];
};
#pragma pack(pop)

struct PacketHeader{
private:
	NNFMessagePktHeader * _pktHeader;

public:
	void setPktLen(int16_t len);
	void setSeqNo(int32_t seqNo);
	void setChecksum(char* cs);
};


}

#endif /* COMMON_INCLUDE_NSE_NNF_NNFMESSAGEPACKETHEADER_HPP_ */
