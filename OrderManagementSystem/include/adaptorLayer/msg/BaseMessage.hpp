/*
 * Base.hpp
 *
 *  Created on: 22-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_BASEMESSAGE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_BASEMESSAGE_HPP_

#include <LocalBuffer.hpp>

namespace oms{

class PktMsgHeader{
	PktLen _len;
	PktSeqNo _seqNo;
	PktChkSum _chkSum;
public:

	PktMsgHeader() : _len(0), _seqNo(0){
		memset((void*)_chkSum, ' ', sizeof(PktChkSum));
	}

	// 1st field in msgPakcet // 2 bytes
	const PktLen& getPktLen() const{
		return _len;
	}
	void setPktLen(const PktLen& len){
		_len = len;
	}

	// 2nd field in msgPakcet // 4 bytes
	const PktSeqNo& getSeqNo() const{
		return _seqNo;
	}
	void setSeqNo(const PktSeqNo& seqNo){
		_seqNo = seqNo;
	}

	// 3rd field in msgPacket // 16 bytes
	const PktChkSum& getCheckSum() const{
		return _chkSum;
	}
	void setChkSum(const PktChkSum& chkSum){
		memcpy((void*)_chkSum, (void*)chkSum, sizeof(PktChkSum));
	}
};

class BaseMsgHeader {
	ApiTCode _apiTCode;
	ApiFuncId _apiFuncId;
	LogTime _logTime;
	AlphaChar _alphaChar;
	TransactionCode _transactionCode;
	ErrorCode _errCode;
	TimeStamp _timeStamp;
	TimeStamp _timeStamp1;
	TimeStamp _timeStamp2;
	MsgLen _msgLen; // includes the header length itself [ body len + 40 ]
public:
	const ApiTCode& getApiTCode() const{
		return _apiTCode;
	}
	void setApiTCode(const ApiTCode& apiTCode){
		_apiTCode = apiTCode;
	}

	const ApiFuncId& getApiFuncId() const{
		return _apiFuncId;
	}
	void setApiFuncId(const ApiFuncId& apiFuncId){
		_apiFuncId = apiFuncId;
	}

	const LogTime& getLogTime() const {
		return _logTime;
	}
	void setLogTime(const LogTime& logTime){
		_logTime = logTime;
	}

	const AlphaChar& getAlphaChar() const {
		return _alphaChar;
	}
	void setAlphaChar(const AlphaChar& ac){
		_alphaChar = ac;
	}

	const TransactionCode& getTransactionCode() const {
		return _transactionCode;
	}
	void setTransactionCode(const TransactionCode& tc){
		_transactionCode = tc;
	}

	const ErrorCode& getErrorCode() const {
		return _errCode;
	}
	void setErrorCode(const ErrorCode& ec){
		_errCode = ec;
	}

	const TimeStamp& getTimeStamp() const {
		return _timeStamp;
	}

	const TimeStamp& getTimeStamp1() const {
		return _timeStamp1;
	}

	const TimeStamp& getTimeStamp2() const {
		return _timeStamp2;
	}

	const MsgLen& getMsgLen() const {
		return _msgLen;
	}
	void setMsgLen(const MsgLen& len){
		_msgLen = len;
	}
};
class BaseMessage{
protected:
	size_t _initSize;
	BaseMsgHeader _header;
public:
	BaseMessage(size_t size = 2048) : _initSize(size){
	}

	virtual ~BaseMessage(){
	}

	virtual std::string getMsgName() const = 0; // just for debugging


	virtual const size_t& getPktLenOffset(){
		return PKT_LEN_OFFSET;
	}

	virtual const size_t& getPktSeqNoOffset(){
		return PKT_SEQNO_OFFSET;
	}

	virtual const size_t& getPktCheckSumOffset(){
		return PKT_CHECKSUM_OFFSET;
	}


	virtual bool isBroadcast() const = 0 ;

	virtual void marshall(LocalBuffer* buffer) = 0;
	virtual void unmarshall(LocalBuffer* buffer) = 0;

	// This is to fill msgLen as the final step
	// Also PktMsgHeader fill can be done here
	virtual inline int finalizeMessage(LocalBuffer* buffer, size_t msgLastPos) = 0;

	virtual void dump(std::ostream& ) = 0;

	inline size_t getInitSize() const { return _initSize; }
};



class RequestBaseMessage : public BaseMessage{

protected:
	BaseMsgHeader _msgHeader;

public:
	const ApiTCode& getApiTCode() const{
		return _msgHeader._apiTCode;
	}
	void setApiTCode(const ApiTCode& apiTCode){
		_msgHeader._apiTCode = apiTCode;
	}

	const ApiFuncId& getApiFuncId() const{
		return _msgHeader._apiFuncId;
	}
	void setApiFuncId(const ApiFuncId& apiFuncId){
		_msgHeader._apiFuncId = apiFuncId;
	}

	const LogTime& getLogTime() const {
		return _msgHeader._logTime;
	}
	void setLogTime(const LogTime& logTime){
		_msgHeader._logTime = logTime;
	}

	const AlphaChar& getAlphaChar() const {
		return _msgHeader._alphaChar;
	}
	void setAlphaChar(const AlphaChar& ac){
		_msgHeader._alphaChar = ac;
	}

	const TransactionCode& getTransactionCode() const {
		return _msgHeader._transactionCode;
	}
	void setTransactionCode(const TransactionCode& tc){
		_msgHeader._transactionCode = tc;
	}

	const ErrorCode& getErrorCode() const {
		return _msgHeader._errCode;
	}
	void setErrorCode(const ErrorCode& ec){
		_msgHeader._errCode = ec;
	}

	const TimeStamp& getTimeStamp() const {
		return _msgHeader._timeStamp;
	}

	const TimeStamp& getTimeStamp1() const {
		return _msgHeader._timeStamp1;
	}

	const TimeStamp& getTimeStamp2() const {
		return _msgHeader._timeStamp2;
	}

	const MsgLen& getMsgLen() const {
		return _msgHeader._msgLen;
	}
	void setMsgLen(const MsgLen& len){
		_msgHeader._msgLen = len;
	}

	RequestBaseMessage(size_t size = 2048) : BaseMessage(size){
	}

	virtual ~RequestBaseMessage(){
	}

	virtual bool isBroadcast() const {
		return false;
	}

};

class NormalResponseBaseMessage : public BaseMessage{
protected:
	BaseMsgHeader _msgHeader;

public:
	const ApiTCode& getApiTCode() const{
		return _msgHeader._apiTCode;
	}
	void setApiTCode(const ApiTCode& apiTCode){
		_msgHeader._apiTCode = apiTCode;
	}

	const ApiFuncId& getApiFuncId() const{
		return _msgHeader._apiFuncId;
	}
	void setApiFuncId(const ApiFuncId& apiFuncId){
		_msgHeader._apiFuncId = apiFuncId;
	}

	const LogTime& getLogTime() const {
		return _msgHeader._logTime;
	}
	void setLogTime(const LogTime& logTime){
		_msgHeader._logTime = logTime;
	}

	const AlphaChar& getAlphaChar() const {
		return _msgHeader._alphaChar;
	}
	void setAlphaChar(const AlphaChar& ac){
		_msgHeader._alphaChar = ac;
	}

	const TransactionCode& getTransactionCode() const {
		return _msgHeader._transactionCode;
	}
	void setTransactionCode(const TransactionCode& tc){
		_msgHeader._transactionCode = tc;
	}

	const ErrorCode& getErrorCode() const {
		return _msgHeader._errCode;
	}
	void setErrorCode(const ErrorCode& ec){
		_msgHeader._errCode = ec;
	}

	const TimeStamp& getTimeStamp() const {
		return _msgHeader._timeStamp;
	}

	const TimeStamp& getTimeStamp1() const {
		return _msgHeader._timeStamp1;
	}

	const TimeStamp& getTimeStamp2() const {
		return _msgHeader._timeStamp2;
	}

	const MsgLen& getMsgLen() const {
		return _msgHeader._msgLen;
	}
	void setMsgLen(const MsgLen& len){
		_msgHeader._msgLen = len;
	}

	NormalResponseBaseMessage(size_t size = 2048) : BaseMessage(size){
	}

	virtual ~NormalResponseBaseMessage(){
	}

	virtual bool isBroadcast() const {
		return false;
	}
};

class BroadcasrResponseBaseMessage : public BaseMessage{
public:
	virtual ~BroadcasrResponseBaseMessage(){
	}

	virtual bool isBroadcast() const {
		return true;
	}
};


class Group{
public:
	Group(){
	}
	virtual ~Group(){
	}

	virtual void marshall(LocalBuffer* buffer) = 0;
	virtual void unmarshall(LocalBuffer* buffer) = 0;

	virtual void dump(std::ostream&) = 0;
};

template<typename GroupType>
class RepeatingGroup : public Group{

public:
	std::vector<GroupType> _items;

	RepeatingGroup(){
	}

	~RepeatingGroup(){
		_items.clear();
	}

	GroupType& operator[](size_t index){
		while(_items.size() < (index+1)){
			_items.push_back(GroupType());
		}
		return _items[index];
	}

	size_t size() const{
		return _items.size();
	}

};



}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_BASEMESSAGE_HPP_ */
