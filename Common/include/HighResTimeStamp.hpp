/*
 * HighResTimeStamp.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_HIGHRESTIMESTAMP_HPP_
#define COMMON_INCLUDE_HIGHRESTIMESTAMP_HPP_

#include <Common.hpp>

namespace obLib{


class HighResTimeStamp{

	static long long _reCalibrateCount;
	static double _CPUFreqMHz;
	static long long _secDiv;
	static long long _tenthSecDiv;
	static long long _hundredthSecDiv;
	static long long _msecDiv;

	long long _ticks;
	long long _lastCount;
	long long _initTime;

public:
	HighResTimeStamp() : _lastCount(0){
		init();
		_ticks = readCounter();
		initSystemTime();
	}

	HighResTimeStamp(unsigned long long tickCount) : _ticks(tickCount), _lastCount(0){
		init();
		initSystemTime();
	}

	HighResTimeStamp(const HighResTimeStamp& rhs) :_ticks(rhs._ticks),
			_lastCount(rhs._lastCount){
		init();
		initSystemTime();
	}

	void reset(){
		_ticks = readCounter();
		initSystemTime();
	}

	long long elapsedTimeSec(){
		long long currentCount = readCounter() - _ticks;
		checkRecalibrate(currentCount);
		return currentCount/_secDiv;
	}

	long long elapsedTimeTenthSec(){
		long long currentCount = readCounter() - _ticks;
		checkRecalibrate(currentCount);
		return currentCount/_tenthSecDiv;
	}

	long long elapsedTimeHundredSec(){
		long long currentCount = readCounter() - _ticks;
		checkRecalibrate(currentCount);
		return currentCount/_hundredthSecDiv;
	}

	long long elapsedTimeMSec(){
		long long currentCount = readCounter() - _ticks;
		checkRecalibrate(currentCount);
		return currentCount/_msecDiv;
	}

	long long elapsedTimeUSec(){
		return (long long)((readCounter() - _ticks) / (_CPUFreqMHz));
	}

	long long elapsedTimeCounter(){
		return (readCounter() - _ticks);
	}

	long long getTimeCounter(){
		return _ticks;
	}

	static double getMeasuredCPUFreq(){
		init();
		return _CPUFreqMHz;
	}

	static unsigned long long readCounter();

protected:

	void initSystemTime(){
		const long USEC = 1000000;
		struct timeval now;
		gettimeofday(&now, NULL);
		_initTime = now.tv_sec * USEC + now.tv_usec;
	}

	void checkRecalibrate(long long currCount){
		if(currCount - _lastCount > _reCalibrateCount){
			reCalibrate(currCount);
		}
	}

	// To Adjust the ticks to system time
	void reCalibrate(long long currentCount){
		const long USEC = 1000000;
		_lastCount = currentCount;
		struct timeval now;
		gettimeofday(&now, NULL);
		long long nowTime = now.tv_sec * USEC + now.tv_usec;
		long long currentUSec = nowTime - _initTime;

		// Adjusting ticks to System time
		_ticks = (long long)(readCounter() - currentUSec * _CPUFreqMHz);
	}


	static void init();
	static void calibrate();
};

}


#endif /* COMMON_INCLUDE_HIGHRESTIMESTAMP_HPP_ */
