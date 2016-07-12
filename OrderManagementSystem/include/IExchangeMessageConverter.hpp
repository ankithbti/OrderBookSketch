/*
 * IExchangeMessageConverter.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGEMESSAGECONVERTER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGEMESSAGECONVERTER_HPP_

#include <Common.hpp>
#include <IExchangeMessage.hpp>

namespace oms{

class IExchangeMessageConverter : public std::enable_shared_from_this<IExchangeMessageConverter>
{
protected:
	IExchangeMessageConverter(){

	}

	IExchangeMessageConverter(const IExchangeMessageConverter&){

	}
public:
	virtual ~IExchangeMessageConverter(){

	}

	virtual IExchangeMessagePtr convert(const char* msgBuff, size_t size) = 0;

};
using IExchangeMessageConverterPtr = std::shared_ptr<IExchangeMessageConverter>;

}





#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IEXCHANGEMESSAGECONVERTER_HPP_ */
