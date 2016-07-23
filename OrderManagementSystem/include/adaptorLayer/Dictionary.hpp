/*
 * Security.hpp
 *
 *  Created on: 20-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_DICTIONARY_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_DICTIONARY_HPP_

#include <Common.hpp>

namespace oms{


// Fill the Dictionary Messages using the MarketData messages
// Write their concrete classes to take the Market Data message and fill themselves

class DictionaryUserData{
public:
	virtual ~DictionaryUserData(){

	}
};
using DictionaryUserDataPtr = std::shared_ptr<DictionaryUserData>;

class DictionaryInstrument{
public:
	virtual ~DictionaryInstrument(){
	}

	virtual int getLocalExchangeId() = 0;
	virtual int getLocalInstId() = 0;
	virtual std::string getLocalInstSymbol() = 0;

	// If It has underlying as well - In case of future
	virtual std::string getUnderlyingSymbol() = 0;
	virtual int getUnderlyingInstId() = 0;

	// Exchange stuff
	virtual int getExchangeInstId() = 0;

	virtual int getLotSize() = 0;
	virtual int getPriceScalingFactor() = 0;
	virtual int getPricePrecisionFactor() = 0;

	virtual void setOrderUserData(const DictionaryUserDataPtr& )  = 0;
	virtual DictionaryUserDataPtr getOrderUserData() = 0;

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) = 0;
	virtual std::string getExtra() = 0;

};
using DictionaryInstrumentPtr = std::shared_ptr<DictionaryInstrument>;

class DictionaryOption{
public:
	virtual ~DictionaryOption(){
	}

	virtual int getLocalExchangeId() = 0;
	virtual int getLocalInstId() = 0;
	virtual int getLocalOptId() = 0;

	virtual std::string getExchangeInstId() = 0;
	virtual int getExchangeOptionId() = 0;

	virtual std::string getOptionName() = 0;
	virtual std::string getRootSymbol() = 0;
	virtual int getExpirationDateYYYYMMDD() = 0;
	virtual double getStrike() = 0;
	virtual bool isCall() = 0;
	virtual std::string getOptionType() = 0;

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) = 0;
	virtual std::string getExtra() = 0;

};
using DictionaryOptionPtr = std::shared_ptr<DictionaryOption>;

class DictionaryLeg{
public:
	virtual ~DictionaryLeg(){
	}

	virtual int getLocalExchangeId() = 0;
	virtual int getLocalInstId() = 0;

	virtual int getQty() = 0;
	virtual double getPrice() = 0;
	virtual bool isBuy() = 0;

	virtual int getLocalOptionId() = 0;

	virtual std::string getExchangeInstId() = 0;
	virtual std::string getExchangeSpreadId() = 0;

	virtual std::string getOptionName() = 0;
	virtual std::string getRootSymbol() = 0;
	virtual int getExpirationDateYYYYMMDD() = 0;
	virtual double getStrike() = 0;
	virtual bool isCall() = 0;

	virtual void setExtra(std::string& extra) = 0;
	virtual std::string getExtra() = 0;

};
using DictionaryLegPtr = std::shared_ptr<DictionaryLeg>;

class DictionarySpread{
public:
	virtual ~DictionarySpread(){
	}

	virtual int getLocalExchangeId() = 0;
	virtual int getLocalInstId() = 0;
	virtual int getLocalSpreadId() = 0;

	virtual std::string getExchangeInstId() = 0;
	virtual std::string getExchangeSpreadId() = 0;

	virtual bool isSpreadWithStock() = 0;
	virtual const std::vector<DictionaryLegPtr>& getLegs() = 0;
	virtual int getLotSize() = 0;

	virtual void setExtra(std::string& extra) = 0;
	virtual std::string getExtra() = 0;
};
using DictionarySpreadPtr = std::shared_ptr<DictionarySpread>;

class DictionaryProduct{
public:
	virtual ~DictionaryProduct(){
	}

	virtual int getStreamId() = 0;
	virtual DictionaryInstrumentPtr getInstrumentDict() = 0;
	virtual bool isSpread() = 0;
	virtual int getLocalExchangeId() = 0;
	virtual int getLocalInstId() = 0;
	virtual int getLocalProductId() = 0;

	virtual int getExchangeInstId() = 0;
	virtual int getExchangeProductId() = 0;
	virtual std::string getPacketGrouping() = 0;

	virtual DictionaryOptionPtr getOptionDictionary() = 0;
	virtual DictionarySpreadPtr getSpreadDictionary() = 0;

	virtual void setOrderUserData(const DictionaryUserDataPtr& )  = 0;
	virtual DictionaryUserDataPtr getOrderUserData() = 0;

	// May be we can have serialized Market Data packet in it if we need
	virtual void setExtra(std::string& extra) = 0;
	virtual std::string getExtra() = 0;

};
using DictionaryProductPtr = std::shared_ptr<DictionaryProduct>;
}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_DICTIONARY_HPP_ */
