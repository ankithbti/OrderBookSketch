/*
 * MktDataReceiver.hpp
 *
 *  Created on: 30-May-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_MKTDATARECEIVER_HPP_
#define INCLUDE_MKTDATARECEIVER_HPP_

#include <Common.hpp>

namespace obLib{

  /**
   * UDP Connection
   * 1 thread for receiving UDP packets
   * Get the Payload and put it in a Q
   * Multiple Consumers pick the packets from Q and put on different Referencers [ only the Order/Trades ] based upon the type of Token#
   * Multiple Threads [ might be using ThreadPool ] work on DispatchQ of Refereners
   * After picking the packet, convert them into Order/Trade and update the OrderBook.
   *
   * To Do: Functor class, Referencer class, DispatchQ, ThreadPool
   */
  class MktDataReceiver : private boost::noncopyable
  {

  };
}



#endif /* INCLUDE_MKTDATARECEIVER_HPP_ */
