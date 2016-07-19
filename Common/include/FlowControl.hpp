/*
 * FlowControl.hpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_FLOWCONTROL_HPP_
#define COMMON_INCLUDE_FLOWCONTROL_HPP_


#include <Common.hpp>
#include <HighResTimeStamp.hpp>

namespace obLib{

static const unsigned DEFAULT_TIMER_SERIAL_SIZE = 64;

struct Measurement{
	long long _begin;
	long long _duration;
	unsigned _count;

	Measurement() : _begin(0), _duration(0), _count(0){
	}

	Measurement(long long now, long long duration, unsigned count) : _begin(now),
			_duration(duration),
			_count(count){

	}

};

class MeasurementWindow{
	Measurement _serial[DEFAULT_TIMER_SERIAL_SIZE];
	unsigned _count;
	unsigned _sum;
	long long _duration;
	unsigned _index;
	long long _max_duration;

public:
	MeasurementWindow(unsigned timeSlice) : _count(0), _sum(0), _duration(0), _index(0), _max_duration(0){
		memset(&_serial, 0, sizeof(Measurement) * DEFAULT_TIMER_SERIAL_SIZE);
		_max_duration = DEFAULT_TIMER_SERIAL_SIZE * timeSlice * HighResTimeStamp::getMeasuredCPUFreq() * 1000;
	}

	unsigned calculateRate(long long now, Measurement &latest){
		_duration -= _serial[_index]._duration;
		_sum -= _serial[_index]._count;
		_duration += latest._duration;
		_sum += latest._count;
		_serial[_index] = latest;

		while(true){
			// move to next slot
			++_index;
			if(_index == DEFAULT_TIMER_SERIAL_SIZE){
				_index = 0;
			}
			if((_serial[_index]._duration && (now > _serial[_index]._begin + _max_duration))){
				_duration -= _serial[_index]_duration;
				_sum -= _serial[_index]._count;
			}else{
				break;
			}
		}

		long long millifrq = _duration / 1000000;
		if(millifrq){
			_count = (unsigned)((_sum * HighResTimeStamp::getMeasuredCPUFreq() ) /  millifrq);
		}
		return _count;
	}

	unsigned getRate(){
		return _count;
	}
};

class AdaptiveFlowControl{
	/// Max no of Msgs / Total Size an application can send in a sec
	unsigned _avgRate;

	/// Max no of Msgs / Total Size an application can send before triggering re-calculating the rate
	unsigned _burstCount;

	/// Actual Msgs / Bandwidth an Application can send within a TimeSlice
	unsigned _cumulative;

	/// CPU Ticks since computer reboot
	long long _startCount;

	/// CPU Ticks for re-calculation slice
	long long _milliCount;

	SpinLock _spinMutex;
	MeasurementWindow _window;
	HighResTimeStamp _timer;

public:

	/// This class paces the no of messages / Total Size an application can send over time.
	/// But also allows burst rate in short period.
	/// By Setting the different parameters, application can control the dynamic of message flow
	/// TimeSlice -- Max time passed in millisec before triggering the re-calculating the rate
	AdaptiveFlowControl(unsigned avgRate, unsigned burstCount, unsigned timeSlice) : _avgRate(avgRate),
	_burstCount(burstCount),
	_cumulative(0),
	_window(timeSlice){

		_startCount = _timer.elapsedTimeCounter();
		long long frq = _timer.getMeasuredCPUFreq() * timeSlice; // In USec
		_milliCount = frq * 1000;
	}

	/// Adding the number of messages / Total Size an Application sent
	void addTraffic(unsigned additional){
		obLib::SpinGuard lock(_spinMutex);
		_cumulative += additional;
	}

	/// Checking whether an application can send
	/// Return the number of messages / Total Size an Application can send within the same time slice,
	/// 0 if can not send
	unsigned isOkToSend(){
		SpinGuard lock(_spinMutex);
		long long now = _timer.elapsedTimeCounter();
		long long elapsed = now - _startCount;
		unsigned count ;

		if(elapsed > _milliCount || _cumulative > _burstCount){
			Measurement latest(now, elapsed, _cumulative);
			count = _window.calculateRate(now, latest);
			_startCount = now;
			_cumulative = 0;
		}else{
			count = _window.getRate();
		}

		if(count < _avgRate){
			return _burstCount - _cumulative;
		}else{
			return 0;
		}


	}

};


}


#endif /* COMMON_INCLUDE_FLOWCONTROL_HPP_ */
