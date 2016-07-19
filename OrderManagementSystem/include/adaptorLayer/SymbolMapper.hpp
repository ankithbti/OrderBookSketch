/*
 * SymbolMapper.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLMAPPER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLMAPPER_HPP_

#include <ISymbolDefinition.hpp>

namespace oms{

class SymbolMapper{
	// Singleton - To Do

	SymbolMapper();
	SymbolMapper(const SymbolMapper&);
	SymbolMapper& operator=(const SymbolMapper&);

	typedef std::map<std::string, ISymbolDefinitionPtr> SymbolContainer;

	SymbolContainer _symbols;
	obLib::SpinLock _spinMutex;

public:
	static SymbolMapper& getInstance(){
		static SymbolMapper sm;
		return sm;
	}

	ISymbolDefinitionPtr find(const std::string& symbol) const{
		obLib::SpinGuard lock(_spinMutex);
		SymbolContainer::const_iterator it = _symbols.find(symbol);
		return (it != _symbols.end()) ? it->second : ISymbolDefinitionPtr();
	}

	void add(const std::string& symbol, const ISymbolDefinitionPtr& symbolDef){
		obLib::SpinGuard lock(_spinMutex);
		_symbols[symbol] = symbolDef;
	}
};
using SymbolMapperPtr = std::shared_ptr<SymbolMapper>;

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_ADAPTORLAYER_SYMBOLMAPPER_HPP_ */
