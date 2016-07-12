/*
 * ITradeDefinition.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ITRADEDEFINITION_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ITRADEDEFINITION_HPP_


#include <Common.hpp>

namespace oms{

class ITradeDefinition : public std::enable_shared_from_this<ITradeDefinition>{
protected:
	ITradeDefinition(){

	}

	ITradeDefinition(const ITradeDefinition&){

	}

public:
	virtual ~ITradeDefinition(){

	}

	virtual const std::string& getTradeId() const  = 0;
	virtual const std::string& getAccount() const = 0;
	virtual const std::string& getSelfMatchPreventionId() const  = 0;
};
using ITradeDefinitionPtr = std::shared_ptr<ITradeDefinition>;

class ITradeDefinitionBuilder : public std::enable_shared_from_this<ITradeDefinitionBuilder>
{
protected:
	ITradeDefinitionBuilder(){

	}

	ITradeDefinitionBuilder(const ITradeDefinitionBuilder&){

	}

public:
	virtual ~ITradeDefinitionBuilder(){

	}

	virtual void setTradeId()  = 0;
	virtual void setAccount() = 0;
	virtual void seSelfMatchPreventionId()  = 0;
	virtual ITradeDefinitionPtr build() = 0;
};
using ITradeDefinitionBuilderPtr = std::shared_ptr<ITradeDefinitionBuilder>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ITRADEDEFINITION_HPP_ */
