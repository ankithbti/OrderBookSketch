/*
 * SymbolDefinition.hpp
 *
 *  Created on: 23-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_SYMBOLDEFINITION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_SYMBOLDEFINITION_HPP_

#include <ISymbolDefinition.hpp>
#include <Common.hpp>

namespace oms{

class SymbolDefinition : public ISymbolDefinition{

	StreamId _streamId;
	TokenNumber _tokenNo;
	std::string _instrument;
	std::string _symbol;
	ExpiryDate _expiryDate;
	StrikePrice _strikePrice;
	std::string _optionType;
	std::string _exchange;
	std::string _currency;
	double _priceMultiplier;
	size_t _pricePrecision;

public:
	SymbolDefinition(const short& streamId, const int& tokenNumber, const std::string& instrument,
			const std::string& symbol, ExpiryDate expiryDate, StrikePrice strikePrice, std::string type,
			const std::string& exchange, const std::string& curr, double priceMultiplier,
			size_t pricePrecision) : _streamId(streamId),
			_tokenNo(tokenNumber),
			_expiryDate(expiryDate),
			_strikePrice(strikePrice),
			_exchange(exchange),
			_currency(curr),
			_priceMultiplier(priceMultiplier),
			_pricePrecision(pricePrecision)
{
//		std::memcpy((void*)_instrument, (void*)instrument, sizeof(Instrument));
//		std::memcpy((void*)_symbol, (void*)symbol, sizeof(Symbol));
//		std::memcpy((void*)_optionType, (void*)type, sizeof(OptionType));
}

	virtual const StreamId& getStreamId() const {
		return _streamId;
	}

	virtual const TokenNumber& getTokenNumber() const{
		return _tokenNo;
	}

	virtual const std::string& getSymbol() const {
		return _symbol;
	}

	virtual const std::string& getInstrument() const {
		return _instrument;
	}

	virtual const ExpiryDate& getExpiryDate() const {
		return _expiryDate;
	}

	virtual const StrikePrice& getStrikePrice() const {
		return _strikePrice;
	}

	virtual const std::string& getOptionType() const {
		return _optionType;
	}

	virtual const std::string& getExchange() const {
		return _exchange;
	}

	virtual const std::string& getCurr() const {
		return _currency;
	}

	virtual const double& getPriceMultiplier() const {
		return _priceMultiplier;
	}

	virtual const size_t& getPricePrecision() const {
		return _pricePrecision;
	}


};

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_SYMBOLDEFINITION_HPP_ */
