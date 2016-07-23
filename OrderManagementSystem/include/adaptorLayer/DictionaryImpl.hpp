/*
 * DictionaryImpl.hpp
 *
 *  Created on: 23-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_DICTIONARYIMPL_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_DICTIONARYIMPL_HPP_

#include <adaptorLayer/Dictionary.hpp>
#include <nse_md/MktDataSecurity.hpp>

namespace oms{

class DictionaryUserDataImpl{
protected:
	DictionaryUserDataPtr _orderUserData;
	DictionaryUserDataPtr _adaptorOrderUserData;
	DictionaryUserDataPtr _pipeOrderUserData;

	// Might support QuoteuserData in future here as well
public:
	virtual ~DictionaryUserDataImpl(){}

	virtual void setOrderuserData(const DictionaryUserDataPtr& userData){
		_orderUserData = userData;
	}
	virtual const DictionaryUserDataPtr& getOrderUserData(){
		return _orderUserData;
	}

	virtual void setAdaptorOrderuserData(const DictionaryUserDataPtr& userData){
		_adaptorOrderUserData = userData;
	}
	virtual const DictionaryUserDataPtr& getAdaptorOrderUserData(){
		return _adaptorOrderUserData;
	}

	virtual void setPipeOrderuserData(const DictionaryUserDataPtr& userData){
		_pipeOrderUserData = userData;
	}
	virtual const DictionaryUserDataPtr& getPipeOrderUserData(){
		return _pipeOrderUserData;
	}
};
using DictionaryUserDataImplPtr = std::shared_ptr<DictionaryUserDataImpl>;


class DictionaryInstrumentMsg{

};
using DictionaryInstrumentMsgPtr = std::shared_ptr<DictionaryInstrumentMsg>;

class DictionaryInstrumentImpl : public DictionaryInstrument{
	DictionaryInstrumentMsgPtr _dim;
public:
	virtual int getLocalExchangeId() {
		return 0;
	}
	virtual int getLocalInstId() {
		return 0;
	}
	virtual std::string getLocalInstSymbol() {
		return "EMPTY";
	}

	// If It has underlying as well - In case of future
	virtual std::string getUnderlyingSymbol() {
		return "EMPTY";
	}
	virtual int getUnderlyingInstId() {
		return 0;
	}

	// Exchange stuff
	virtual std::string getExchangeInstId() {
		return 0;
	}

	virtual int getLotSize(){
		return 10;
	}
	virtual int getPriceScalingFactor() {
		return 1;
	}
	virtual int getPricePrecisionFactor() {
		return 3;
	}

	virtual void setOrderUserData(const DictionaryUserDataPtr& )  {

	}
	virtual DictionaryUserDataPtr getOrderUserData() {
		return DictionaryUserDataImplPtr();
	}

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) {

	}
	virtual std::string getExtra() {
		return std::string();
	}
};

class DictionaryOptionMsg{
	obLib::MktDataDictionaryOptMsgPtr _mktDataDicOptMsg;
	std::string _extra;
public:
	DictionaryOptionMsg(const obLib::MktDataDictionaryOptMsgPtr& optMsg) : _mktDataDicOptMsg(optMsg){

	}

	virtual ~DictionaryOptionMsg(){
	}

	const obLib::MktDataDictionaryOptMsgPtr& getRaw() const{
		return _mktDataDicOptMsg;
	}

	virtual int getLocalExchangeId() {
		return _mktDataDicOptMsg->_localExchangeId;
	}
	virtual int getLocalInstId() {
		return _mktDataDicOptMsg->_localInstId;
	}
	virtual int getLocalOptId() {
		return _mktDataDicOptMsg->_localOptionId;
	}

	virtual int getExchangeInstId() {
		return _mktDataDicOptMsg->_instrument;
	}
	virtual int getExchangeOptionId() {
		return _mktDataDicOptMsg->_tokenNumber;
	}

	virtual std::string getOptionName() {
		return "EMPTY";
	}

	virtual std::string getRootSymbol() {
		return _mktDataDicOptMsg->_symbol;
	}

	virtual int getExpirationDateYYYYMMDD(){
		return _mktDataDicOptMsg->_expiryDate;
	}
	virtual double getStrike() {
		return _mktDataDicOptMsg->_strikePrice;
	}

	virtual bool isCall() {
		return false;
	}

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) {
		_extra = extra;
	}
	virtual std::string getExtra() {
		return _extra;
	}
};
using DictionaryOptionMsgPtr = std::shared_ptr<DictionaryOptionMsg>;

class DictionaryOptionImpl : public DictionaryOption{
	int _exchangeId;
	DictionaryOptionMsgPtr _dom;
	std::string _name;
	std::string _exchInstId;
	std::string _extraObj;
public:

	DictionaryOptionImpl(int exchId, const std::string& exchInstId,
			const DictionaryOptionMsgPtr& optionMsg) :
				_exchangeId(exchId),
				_dom(optionMsg),
				_exchInstId(exchInstId){
		//_name = "Name of Option from Local DB"; // To Do
	}

	DictionaryOptionMsgPtr getRaw(){
		return _dom;
	}

	virtual int getLocalExchangeId() {
		return _dom->getLocalExchangeId();
	}
	virtual int getLocalInstId() {
		return _dom->getLocalInstId();
	}
	virtual int getLocalOptId(){
		return _dom->getLocalOptId();
	}

	virtual std::string getExchangeInstId() {
		return _dom->getExchangeInstId();
	}
	virtual int getExchangeOptionId() {
		return _dom->getExchangeOptionId();
	}

	virtual std::string getOptionName() {
		return _name;
	}
	virtual std::string getRootSymbol() {
		return _dom->getRootSymbol();
	}
	virtual int getExpirationDateYYYYMMDD() {
		return _dom->getExpirationDateYYYYMMDD();
	}
	virtual double getStrike() {
		return _dom->getStrike();
	}
	virtual bool isCall() {
		return _dom->isCall();
	}

	virtual std::string getOptionType() {
		return "CO";
	}

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) {
		// Nothing to do
	}
	virtual std::string getExtra() {
		return _dom->getExtra();
	}
};
using DictionaryOptionImplPtr = std::shared_ptr<DictionaryOptionImpl>;


class DictionaryProductImpl : public DictionaryProduct{
	int _exchangeId;
	DictionaryInstrumentPtr _instDict;
	DictionaryOptionMsgPtr _dom;
	DictionaryOptionImplPtr _doi;

protected:
	DictionaryUserDataImplPtr _userData;
public:
	DictionaryProductImpl(int exchId, const DictionaryInstrumentPtr& instDic,
			const DictionaryOptionMsgPtr& optDicMsg) :
				_exchangeId(exchId),
				_instDict(instDic),
				_dom(optDicMsg),
				_doi(new DictionaryOptionImpl(exchId, instDic->getExchangeInstId(), _dom)),
				_userData(new DictionaryUserDataImpl()){
	}

	virtual int getStreamId() {
		return 0;
	}

	virtual DictionaryInstrumentPtr getInstrumentDict() {
		return _instDict;
	}
	virtual bool isSpread() {
		return false;
	}
	virtual int getLocalExchangeId() {
		return _exchangeId;
	}
	virtual int getLocalInstId() {
		return _dom->getLocalInstId();
	}
	virtual int getLocalProductId() {
		return _dom->getLocalOptId();
	}

	virtual int getExchangeInstId() {
		return _dom->getExchangeInstId();
	}
	virtual int getExchangeProductId() {
		return _doi->getExchangeOptionId();
	}
	virtual std::string getPacketGrouping() {
		return "";
	}

	virtual DictionaryOptionPtr getOptionDictionary() {
		return _doi;
	}
	virtual DictionarySpreadPtr getSpreadDictionary() {
		return DictionarySpreadPtr();
	}

	virtual void setOrderUserData(const DictionaryUserDataPtr& ud ) {
		_userData = ud;
	}
	virtual DictionaryUserDataPtr getOrderUserData() {
		return _userData;
	}

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) {

	}
	virtual std::string getExtra() {
		return "";
	}

	virtual void setOrderuserData(const DictionaryUserDataPtr& userData){
		_userData->setOrderuserData(userData);
	}
	virtual const DictionaryUserDataPtr& getOrderUserData(){
		return _userData->getOrderUserData();
	}

	virtual void setAdaptorOrderuserData(const DictionaryUserDataPtr& userData){
		_userData->setAdaptorOrderuserData(userData);
	}
	virtual const DictionaryUserDataPtr& getAdaptorOrderUserData(){
		return _userData->getAdaptorOrderUserData();
	}

};
using DictionaryProductImplPtr = std::shared_ptr<DictionaryProductImpl>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_DICTIONARYIMPL_HPP_ */
