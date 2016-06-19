/*
 * TimeUtil.hpp
 *
 *  Created on: 19-Jun-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_TIMEUTIL_HPP_
#define COMMON_INCLUDE_TIMEUTIL_HPP_


#include <Common.hpp>

namespace obLib{

const int64_t MillisecondsBetweenUnixEpochAndNSEEpoch = 315513000000;

struct TimeUtil{

	enum TimeInMilliType {
		UnixEpoch = 0,
		NseEpoch = 2
	};

	static int64_t getMilliSecBetweenUnixEpochAndNSEEpoch(){
		std::tm nseTS = {};
		strptime("Jan 1 1980 00:00:00", "%b %d %Y %H:%M:%S", &nseTS);
		std::chrono::system_clock::time_point nseTP = std::chrono::system_clock::from_time_t(std::mktime(&nseTS));
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(nseTP.time_since_epoch()); // duration
		return diff.count();
	}

	static std::string getDateTimeStr(int64_t millisec, TimeInMilliType type ){
		std::string retVal;
		switch(type){
		case UnixEpoch:
		{
			int64_t finalMilli = millisec;
			boost::posix_time::ptime ti = boost::posix_time::from_time_t((finalMilli / 1000) + (finalMilli % 1000));
			retVal = boost::posix_time::to_simple_string(ti);
		}
		break;
		case NseEpoch:
		{
			int64_t finalMilli = millisec + MillisecondsBetweenUnixEpochAndNSEEpoch;
			boost::posix_time::ptime ti = boost::posix_time::from_time_t((finalMilli / 1000) + (finalMilli % 1000));
			retVal = boost::posix_time::to_simple_string(ti);
		}
		break;
		default:
			break;
		}
		return retVal;
	}

};

}


#endif /* COMMON_INCLUDE_TIMEUTIL_HPP_ */
