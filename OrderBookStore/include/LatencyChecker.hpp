/*
 * LatencyChecker.hpp
 *
 *  Created on: 10-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_LATENCYCHECKER_HPP_
#define INCLUDE_LATENCYCHECKER_HPP_


#include <Common.hpp>

namespace obLib{
  template<typename Clock = std::chrono::steady_clock>
  struct LatencyChecker{

    typename Clock::time_point _startTime;

    static long _averageLatency ;
    static long _count;
    static long _maxLatency;
    static long _minlatency;
    static std::vector<size_t> _latencies;

//    static std::shared_ptr<spdlog::logger> _logger;

    LatencyChecker() : _startTime(Clock::now()){
      _latencies.reserve(200000);
    }

    ~LatencyChecker(){
      auto latency = (std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - _startTime)).count();
      _latencies.push_back(latency);
      if(_minlatency > latency){
	  _minlatency = latency;
      }
      if(_maxLatency < latency){
	  _maxLatency = latency;
      }
      //_averageLatency += latency;
      //_logger->info() << "Curr Stats : " <<  latency << " ns" << " Min: " << _minlatency << " Max: " << _maxLatency << " Average: " << getAverageLatency();
    }

    static size_t getMin()
    {
      // Min
      return _minlatency;

    }

    static size_t getMax()
    {
      return _maxLatency;
    }


	static bool isLatencyAvailable(){
		return (_latencies.size() > 0);
	}

    static size_t getAverageLatency(){	
		if(!isLatencyAvailable())
			return 0;
      size_t sum = 0;
      for_each(_latencies.begin(), _latencies.end(), [&sum](size_t lat){
	sum += lat;
      });
      return sum/_latencies.size();
    }

    static size_t getPercentile(float per){
		if(!isLatencyAvailable())
             return 0;
      float tmp = std::ceil((float)(per/100) * _latencies.size());
	  std::sort(_latencies.begin(), _latencies.end(), std::less<size_t>());
      return _latencies[tmp];
    }

    static void printLatencyArray() {

      int count = 0;
	  std::sort(_latencies.begin(), _latencies.end(), std::less<size_t>());
      for(auto& i : _latencies){
    	  //_logger->info() << "Index: " << ++count << " - Latency: " << i;
      }
      //_logger->info() << " Latency Size: " << _latencies.size();
    }

  };

  template<typename Clock>
  long LatencyChecker<Clock>::_count = 0;

  template<typename Clock>
  long LatencyChecker<Clock>::_maxLatency = 0;

  template<typename Clock>
  long LatencyChecker<Clock>::_minlatency = std::numeric_limits<long>::max();

  template<typename Clock>
  long LatencyChecker<Clock>::_averageLatency = 0;

  template<typename Clock>
  std::vector<size_t> LatencyChecker<Clock>::_latencies ;

//  template<typename Clock>
//  std::shared_ptr<spdlog::logger> LatencyChecker<Clock>::_logger = spdlog::daily_logger_mt("LatencyLogger", "logs/LatencyLogger");
}


#endif /* INCLUDE_LATENCYCHECKER_HPP_ */
