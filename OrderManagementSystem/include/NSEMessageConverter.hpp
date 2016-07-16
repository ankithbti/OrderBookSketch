/*
 * NSEMessageConverter.hpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_NSEMESSAGECONVERTER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_NSEMESSAGECONVERTER_HPP_

#include <IExchangeMessageConverter.hpp>

namespace oms{

class NSEMessageConverter : public IExchangeMessageConverter{
public:
	NSEMessageConverter();
	virtual ~NSEMessageConverter();
	virtual IExchangeMessagePtr convert(const char* msgBuff, size_t size);

};
using NSEMessageConverterPtr = std::shared_ptr<NSEMessageConverter>;

}




#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_NSEMESSAGECONVERTER_HPP_ */
