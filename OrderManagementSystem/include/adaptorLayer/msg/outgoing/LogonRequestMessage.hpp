/*
 * LogonMessage.hpp
 *
 *  Created on: 22-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_OUTGOING_LOGONREQUESTMESSAGE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_OUTGOING_LOGONREQUESTMESSAGE_HPP_

#include <adaptorLayer/msg/BaseMessage.hpp>
#include <stdio.h>
namespace oms{

struct StBrokerEligibilityPerMkt{
private:
	char _mktByte; // first 4 bits - Reserved, next 4 bits - [  Auction, Spot, Oddlot, Normal ]
	char _lastResByte;
public:

	StBrokerEligibilityPerMkt(){
		memset((void*)&_mktByte, 0, sizeof(char) );
		memset((void*)&_lastResByte, 0, sizeof(char) );
	}

	void dump(){
		for (int i = 0; i < 8; i++) {
			printf("%d", !!((_mktByte << i) & 0x80));
		}
		printf("\n");
	}

	bool isEligibleForAuctionMkt(){
		return (((_mktByte >> 3) & 1) ? true : false);
	}
	void enableAuctionMktEligibility(){
		_mktByte |= (1 << 3);
	}

	bool isEligibleForSpotMkt(){
		return (((_mktByte >> 2) & 1) ? true : false);
	}
	void enableSpotMktEligibility(){
		_mktByte |= 1 << 2;
	}

	bool isEligibleForOddlotMkt(){
		return (((_mktByte >> 1) & 1) ? true : false);
	}
	void enableOddlotMktEligibility(){
		_mktByte |= 1 << 1;
	}

	bool isEligibleForNormalMkt(){
		return (((_mktByte >> 0) & 1) ? true : false);
	}
	void enableNormalMktEligibility(){
		_mktByte |= 1 << 0;
	}

};

class LogonRequestMessage : public RequestBaseMessage{

	UserId _userId;
	Password _password; // 1 upper case letter,
	//1 lower case letter,
	// 1 numeral and
	// 1 special character from the list @#$%&*/\
	// Neat@FO1 - Default
	Password _newPassword; // should not be from the last 5 passwords. Could be blank.
	TraderName _traderName; // blank
	LastPwdChangeDate _lastPwdChangeDate; // 0 while logon
	BrokerId _brokerId; //
	Reserved _reserved1;
	BranchId _branchId; // 3 digits
	VersionNumber _versionNumber; // VERSION.RELEASE.SUB_RELEASE - 7.02.00 ????
	Batch2StartTime _batch2StartTime; // 0
	HostSwitchContext _hostSwitchContext;
	Colour _colour;
	Reserved _reserved2;
	UserType _userType; // 0
	SequenceNumber _seqNo; // 0
	WsClassName _wsClassName; // seven digit number
	BrokerStatus _brokerStatus;
	ShowIndex _showIndex;
	StBrokerEligibilityPerMkt _stBroketEligibilityPerMkt; // 0
	MemberType _memberType; // 0
	ClearingStatus _clearingStatus; // 0
	BrokerName _brokerName;

public:

	LogonRequestMessage() : RequestBaseMessage(){
		_msgHeader.setTransactionCode(TC_LOGON_REQ);
		_userId = 0;
		memset((void*)_password, ' ', sizeof(Password));
		memset((void*)_newPassword, ' ', sizeof(Password));
		memset((void*)_traderName, ' ', sizeof(TraderName));
		_lastPwdChangeDate = 0;
		memset((void*)_brokerId, ' ', sizeof(BrokerId));
		memset((void*)&_reserved1, ' ', sizeof(Reserved));
		_branchId = 123;
		_versionNumber = 1;
		_batch2StartTime = 0;
		memset((void*)&_hostSwitchContext, ' ', sizeof(HostSwitchContext));
		memset((void*)&_colour, ' ', sizeof(Colour));
		memset((void*)&_reserved2, ' ', sizeof(Reserved));
		_userType = 0;
		_seqNo = 0;
		memset((void*)&_wsClassName, ' ', sizeof(WsClassName));
		memset((void*)&_brokerStatus, ' ', sizeof(BrokerStatus));
		memset((void*)&_showIndex, ' ', sizeof(ShowIndex));
		_memberType = 0;
		memset((void*)&_clearingStatus, ' ', sizeof(ClearingStatus));
		memset((void*)&_brokerName, ' ', sizeof(BrokerName));
	}


	virtual std::string getMsgName() const{
		return "SIGN_ON_REQUEST_IN";
	}

	const UserId& getUserId() const{
		return _userId;
	}
	void setUserId(const UserId& uid){
		_userId = uid;
	}

	const Password& getPassword() const{
		return _password;
	}
	void setPassword(const Password& pass){
		memcpy((void*)_password, (void*)pass, sizeof(Password));
	}

	const Password& getNewPassword() const{
		return _newPassword;
	}
	void setNewPassword(const Password& pass){
		memcpy((void*)_newPassword, (void*)pass, sizeof(Password));
	}

	const TraderName& getTraderName() const {
		return _traderName;
	}
	void setTraderName(const TraderName& name){
		memcpy((void*)_traderName, (void*)name, sizeof(TraderName));
	}

	const LastPwdChangeDate& getLastPwdChangeDate() const {
		return _lastPwdChangeDate;
	}
	void setLastPwdChangeDate(const LastPwdChangeDate& lpcd){
		_lastPwdChangeDate = lpcd;
	}

	const BrokerId& getBrokerId() const{
		return _brokerId;
	}
	void setBrokerId(const BrokerId& bid){
		memcpy((void*)_brokerId, (void*)bid, sizeof(BrokerId));
	}

	const BranchId getBranchId() const{
		return _branchId;
	}
	void setBranchId(const BranchId& bid){
		_branchId = bid;
	}

	const VersionNumber& getVersionNumber() const{
		return _versionNumber;
	}
	void setVersionNumber(const VersionNumber& vn){
		_versionNumber = vn;
	}

	const Batch2StartTime& getBatch2StartTime() const{
		return _batch2StartTime;
	}
	void setBatch2StartTime(const Batch2StartTime& b2st){
		_batch2StartTime = b2st;
	}

	const HostSwitchContext& getHostSwitchContext() const{
		return _hostSwitchContext;
	}
	void setHostSwitchContext(const HostSwitchContext& hsc){
		_hostSwitchContext = hsc;
	}

	const Colour& getColour() const{
		return _colour;
	}
	void setColour(const Colour& colour){
		memcpy((void*)_colour, (void*)colour, sizeof(Colour));
	}

	const UserType& getUserType() const {
		return _userType;
	}
	void setUserType(const UserType& ut){
		_userType = ut;
	}

	const SequenceNumber& getBodySeqNum() const{
		return _seqNo;
	}
	void setSequenceNumber(const SequenceNumber& sn){
		_seqNo = sn;
	}

	const WsClassName& getWsClassName() const {
		return _wsClassName;
	}
	void setWsClassName(const WsClassName& ws){
		memcpy((void*)_wsClassName, (void*)ws, sizeof(WsClassName));
	}

	const BrokerStatus& getBrokerStatus() const{
		return _brokerStatus;
	}
	void setBrokerStatus(const BrokerStatus& bs){
		_brokerStatus = bs;
	}

	const ShowIndex& getShowIndex() const {
		return _showIndex;
	}
	void setShowIndex(const ShowIndex& si){
		_showIndex = si;
	}

	const StBrokerEligibilityPerMkt& getStBrokerEligibilityPerMkt() const {
		return _stBroketEligibilityPerMkt;
	}
	void setStBrokerEligibilityPerMkt(const StBrokerEligibilityPerMkt& bepm){
		_stBroketEligibilityPerMkt = bepm;
	}

	const MemberType& getMemberType() const{
		return _memberType;
	}
	void setMemberType(const MemberType& mt){
		_memberType = mt;
	}

	const ClearingStatus& getClearingStatus() const{
		return _clearingStatus;
	}
	void setClearingStatus(const ClearingStatus& cs){
		_clearingStatus = cs;
	}

	const BrokerName& getBrokerName() const{
		return _brokerName;
	}
	void setBrokerName(const BrokerName& bn){
		memcpy((void*)_brokerName, (void*)bn, sizeof(BrokerName));
	}

	virtual void marshall(LocalBuffer* buffer){

	}

	virtual void unmarshall(LocalBuffer* buffer) {

	}

	virtual int finalizeMessage(LocalBuffer* buffer, size_t msgLastPos){
		return 0;
	}

	virtual void dump(std::ostream& ){

	}

};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_MSG_OUTGOING_LOGONREQUESTMESSAGE_HPP_ */
