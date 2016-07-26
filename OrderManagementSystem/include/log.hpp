/*
 * log.hpp
 *
 *  Created on: 25-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_LOG_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_LOG_HPP_

#include <logger/Logger.hpp>
#include <logger/LogPolicy.hpp>
#include <sstream>

namespace oms{


static obLib::Logger<obLib::ScreenLogPolicy>& getConsoleLogger(){
	static obLib::Logger<obLib::ScreenLogPolicy> logger("OMS");
	return logger;
}

#define CONSOLELOG(msg) \
	std::stringstream consoleMsg; \
	consoleMsg << msg; \
	std::cout << consoleMsg.str() << std::endl;
	//oms::getConsoleLogger().log(consoleMsg.str());


}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_LOG_HPP_ */
