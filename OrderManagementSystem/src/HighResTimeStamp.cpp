/*
 * HighResTimeStamp.cpp
 *
 *  Created on: 19-Jul-2016
 *      Author: ankithbti
 */


#include <HighResTimeStamp.hpp>

namespace obLib{

double HighResTimeStamp::_CPUFreqMHz = 0;
long long HighResTimeStamp::_secDiv = 1;
long long HighResTimeStamp::_tenthSecDiv = 1;
long long HighResTimeStamp::_hundredthSecDiv = 1;
long long HighResTimeStamp::_msecDiv = 1;

// The calibrate timer in CPUTicks, for 3GHz CPU == 30 sec
long long HighResTimeStamp::_reCalibrateCount = 90000000000LL;

void HighResTimeStamp::init(){

	if(_CPUFreqMHz == 0){
		calibrate();
	}
}

unsigned long long HighResTimeStamp::readCounter(){
	union{
		int ticks[2];
		long long ticks64;
	} tick;

	__asm__ (
			"\trdtsc" // Read processor timestamp counter into %%edx::%%eax
			"\n\tmovl %%eax, %0" // Move lo word into ticks[0]
			"\n\tmovl %%edx, %1" // Move hi word into ticks[1]
			: "=m"(tick.ticks[0]), "=m"(tick.ticks[1])
			  : // No input registers
			  : "%eax", "%edx"
	);

	return tick.ticks64;
}

void HighResTimeStamp::calibrate(){
	struct timeval tvstart, tvstop;
	struct timezone tz;

	unsigned long long ticks, ticks2;

	memset(&tz, 0, sizeof(tz));
	ticks = readCounter();
	gettimeofday(&tvstart, NULL);
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	ticks2 = readCounter();
	gettimeofday(&tvstop, NULL);

	double CPUFreqMHz = (ticks2 - ticks) / (((( double ) tvstop.tv_sec - tvstart.tv_sec ) * 1000000 )
			+ (tvstop.tv_usec - tvstart.tv_usec));

	double CPUFreqMHzAggregate = CPUFreqMHz ;

	_msecDiv = (long long )(CPUFreqMHz * 1000LL);
	_hundredthSecDiv = (long long)(CPUFreqMHz * 10000LL);
	_tenthSecDiv = (long long)(CPUFreqMHz * 100000LL);
	_secDiv = (long long)(CPUFreqMHz * 1000000LL);

	_CPUFreqMHz = CPUFreqMHz;
}

}
