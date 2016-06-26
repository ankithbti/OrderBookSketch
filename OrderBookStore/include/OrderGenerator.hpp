/*
 * OrderGenerator.hpp
 *
 *  Created on: 10-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_ORDERGENERATOR_HPP_
#define INCLUDE_ORDERGENERATOR_HPP_


#include <Common.hpp>
#include <MktDataOrderMsg.hpp>
#include <MktDataTradeMsg.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <spdlog/spdlog.h>

namespace obLib{

std::string hex_to_string(const std::string& input)
{
	static const char* const lut = "0123456789abcdef";
	size_t len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (size_t i = 0; i < len; i += 2)
	{
		char a = input[i];
		//std::cout << " Considering " << a << std::endl;
		const char* p = std::lower_bound(lut, lut + 16, a);
		if (*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		//std::cout << " Considering " << b << std::endl;
		const char* q = std::lower_bound(lut, lut + 16, b);
		if (*q != b) throw std::invalid_argument("not a hex digit");

		output.push_back(((p - lut) << 4) | (q - lut));
	}
	return output;
}

struct OrderGenerator{

	struct OrderMsg{

	};

	struct TradeMsg{

	};

	std::set<int32_t> _tokens;

	const std::set<int32_t>& getTokens() const {
		return _tokens;
	}

	~OrderGenerator(){

	}

	OrderGenerator(const std::string& str){
		std::ifstream in(str.c_str(), std::ios::in|std::ios::binary);
		if(in.good()){
			std::cout << " File is readable." << std::endl;
			boost::regex expression("^[0-9]+[ \t]{2}([0-9 a-z]+)[ \t]{2}.+[ \t\n\r]*");

			std::string line;
			int count = 0;
			int orderCount = 0;
			int tradeCount = 0;
			std::stringstream dataPacketStr;

			int lineNo = 0;
			while(std::getline(in, line)){
				++lineNo;
				boost::cmatch what;

				if(boost::regex_match(line.c_str(), what, expression))
				{
					dataPacketStr << what[1] ;
				}
				if(++count == 3){
					std::string hexStr = dataPacketStr.str();
					hexStr.erase(remove_if(hexStr.begin(), hexStr.end(), isspace), hexStr.end());
					// Check
					std::string decimalStr;
					try{
						decimalStr = hex_to_string(hexStr);
					}catch(const std::invalid_argument& err){
						std::cout << " Exception: " << err.what() << " While conveting string: " <<  hexStr << " || " << lineNo << " " << what[1] << std::endl;
						count = 0;
						dataPacketStr.clear();
						dataPacketStr.str("");
						continue;
					}

					MktDataGlobalHeaderMsg header;
					memcpy((void*)&header, (void*)decimalStr.c_str() , sizeof(MktDataGlobalHeaderMsg));
					char msgType;
					memcpy((void*)&msgType, (void*)(decimalStr.c_str() + sizeof(MktDataGlobalHeaderMsg)) , sizeof(char));

					switch(msgType){
					case 'M':
					case 'N':
					case 'X':
					case 'G':
					case 'H':
					case 'J':
					{
						// Outright Order
						MktDataOrderMsg body;
						memcpy((void*)&body, (void*)(decimalStr.c_str() + sizeof(MktDataGlobalHeaderMsg) + sizeof(char)), sizeof(MktDataOrderMsg));
						++orderCount;
						_tokens.insert(body._toeknNo);

					}
					break;
					case 'T':
					case 'K':
					{
						// Trade
						MktDataTradeMsg body;
						memcpy((void*)&body, (void*)(decimalStr.c_str() + sizeof(MktDataGlobalHeaderMsg) + sizeof(char)), sizeof(MktDataTradeMsg));
						++tradeCount;
						_tokens.insert(body._toeknNo);
					}
					break;
					default:
						throw std::runtime_error(" Unsupported MktData message.");
						break;
					}
					count = 0;
					dataPacketStr.clear();
					dataPacketStr.str("");
				}
			}
			std::cout << " OrderCount: " << orderCount << " TradeCount: " << tradeCount << std::endl;
			in.close();
		}else{
			std::cout << " OrderGenerator() - Error : File: " << str << " , is not readable." << std::endl;
		}
	}

};

}


#endif /* INCLUDE_ORDERGENERATOR_HPP_ */
