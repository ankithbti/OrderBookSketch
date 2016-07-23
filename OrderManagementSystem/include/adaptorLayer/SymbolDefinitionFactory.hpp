/*
 * SymbolDefinitionFactory.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLDEFINITIONFACTORY_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLDEFINITIONFACTORY_HPP_


#include <adaptorLayer/Dictionary.hpp>
#include <SymbolDefinition.hpp>

namespace oms{


class SymbolDefFactory{

	// This class may also hold the price factors for various symbols as well
public:
	SymbolDefFactory(){

	}
	virtual ~SymbolDefFactory(){

	}

	void init( const std::string& configStr){

	}

	ISymbolDefinitionPtr createSymDef(const DictionaryProductPtr& data){
		ISymbolDefinitionPtr symDef(new SymbolDefinition(
				data->getStreamId(),
				data->getExchangeProductId(),
				data->getExchangeInstId(),
				data->getOptionDictionary()->getRootSymbol(),
				data->getOptionDictionary()->getExpirationDateYYYYMMDD(),
				data->getOptionDictionary()->getStrike(),
				data->getOptionDictionary()->getOptionType(),
				"NSE", "IND",
				data->getInstrumentDict()->getPriceScalingFactor(),
				data->getInstrumentDict()->getPricePrecisionFactor()
				));
		if(symDef){
			SymbolMapper::getInstance().add(data->getOptionDictionary()->getRootSymbol(),
					symDef);
		}
		return symDef;
	}


};
using SymbolDefFactoryPtr = std::shared_ptr<SymbolDefFactory>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLDEFINITIONFACTORY_HPP_ */
