/*
 * ISymbolDefinition.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ISYMBOLDEFINITION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ISYMBOLDEFINITION_HPP_


#include <Common.hpp>
#include <adaptorLayer/EnumTypes.hpp>

namespace oms{

class ISymbolDefinition : public std::enable_shared_from_this<ISymbolDefinition>{
protected:
	ISymbolDefinition(){

	}

	ISymbolDefinition(const ISymbolDefinition&){

	}

public:
	virtual ~ISymbolDefinition(){

	}

	virtual const StreamId& getStreamId() const = 0;
	virtual const TokenNumber& getTokenNumber() const = 0;
	virtual const std::string& getSymbol() const = 0;
	virtual const std::string& getInstrument() const = 0;
	virtual const ExpiryDate& getExpiryDate() const = 0;
	virtual const StrikePrice& getStrikePrice() const = 0;
	virtual const std::string& getOptionType() const = 0;
	virtual const std::string& getExchange() const = 0;
	virtual const std::string& getCurr() const = 0;
	virtual const double& getPriceMultiplier() const = 0;
	virtual const size_t& getPricePrecision() const = 0;
};

using ISymbolDefinitionPtr = std::shared_ptr<ISymbolDefinition>;

class ISymbolDefinitionBuilder : public std::enable_shared_from_this<ISymbolDefinitionBuilder>{
protected:
	ISymbolDefinitionBuilder(){

	}

	ISymbolDefinitionBuilder(const ISymbolDefinitionBuilder&){

	}

public:
	virtual ~ISymbolDefinitionBuilder(){

	}

	virtual ISymbolDefinitionPtr build() = 0;


};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ISYMBOLDEFINITION_HPP_ */
