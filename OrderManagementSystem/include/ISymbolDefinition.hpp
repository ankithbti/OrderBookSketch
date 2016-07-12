/*
 * ISymbolDefinition.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ISYMBOLDEFINITION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ISYMBOLDEFINITION_HPP_


#include <Common.hpp>

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

	virtual const std::string& getSymbol() const = 0;
	virtual const std::string& getSecurityId() const = 0;
	virtual const std::string& getSourceId() const = 0;
	virtual const std::string& getSecType() const = 0;
	virtual const std::string& getSecDesc() const = 0;
	virtual const std::string& getExchange() const = 0;
	virtual const std::string& getCurr() const = 0;
	virtual const std::string& getPriceMultiplier() const = 0;
	virtual const std::string& getPricePrecision() const = 0;
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

	virtual void setSymbol() = 0;
	virtual void setSecurityId() = 0;
	virtual void setSourceId()= 0;
	virtual void setSecType() = 0;
	virtual void setSecDesc() = 0;
	virtual void setExchange() = 0;
	virtual void setCurr() = 0;
	virtual void setPriceMultiplier() = 0;
	virtual void setPricePrecision() = 0;

	virtual ISymbolDefinitionPtr build() = 0;


};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ISYMBOLDEFINITION_HPP_ */
