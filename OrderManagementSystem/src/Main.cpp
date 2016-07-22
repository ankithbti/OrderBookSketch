/*
 * Main.cpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */


#include <adaptorLayer/msg/outgoing/LogonRequestMessage.hpp>
#include <Common.hpp>
#include <IOrderPtrType.hpp>

#include <util/MemoryMapped.hpp>
#include <LocalBuffer.hpp>

int main(){



	char p[10];
	std::cout << sizeof(p) << std::endl;


	oms::StBrokerEligibilityPerMkt brokerEligibility;
	brokerEligibility.dump();
	std::cout << std::endl;
	std::cout << brokerEligibility.isEligibleForAuctionMkt() << std::endl;
	brokerEligibility.enableAuctionMktEligibility();
	brokerEligibility.enableNormalMktEligibility();
	brokerEligibility.dump();
	std::cout << std::endl;

	std::cout << brokerEligibility.isEligibleForAuctionMkt() << std::endl;

	return 0;


	//std::shared_ptr<oms::ExchangeSession> es;
	oms::LocalBuffer lb(10, obLib::ByteOrderConverter::LITTLEENDIAN);
	char a = 'A';
	lb.put8(a);
	a = 'B';
	lb.put8(a);

	std::ostringstream ss;
	lb.toStream(ss);

	std::cout << ss.str();
	return 0;
}

