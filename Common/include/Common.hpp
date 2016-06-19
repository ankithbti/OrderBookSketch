/*
 * Common.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_COMMON_HPP_
#define INCLUDE_COMMON_HPP_

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <chrono>
#include <memory>
#include <utility>
#include <cstdlib>
#include <limits>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <pthread.h>

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/variant.hpp>

namespace obLib{

#define LOG(m) \
    std::cout << m << std::endl

using Price = int32_t;
using Quantity = int32_t;
using OrderId = int64_t;
using ChangeId = int32_t;
using TokenId = int32_t;

const Price INVALID_LEVEL_PRICE(0);
const Price PRICE_UNCHANGED(0);
const int32_t SIZE_UNCHANGED(0);

}

#endif /* INCLUDE_COMMON_HPP_ */
