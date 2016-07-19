/*
 * SymbolDefinitionFactory.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLDEFINITIONFACTORY_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLDEFINITIONFACTORY_HPP_


#include <ISymbolDefinition.hpp>
#include <adaptorLayer/MessageInterface.hpp>
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

	ISymbolDefinitionPtr createSymDef(const DictionaryProduct::SharedPtr& data){

	}


};
using SymbolDefFactoryPtr = std::shared_ptr<SymbolDefFactory>;

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLDEFINITIONFACTORY_HPP_ */
