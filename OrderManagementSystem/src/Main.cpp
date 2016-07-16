/*
 * Main.cpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */


#include <Common.hpp>
#include <IOrderPtrType.hpp>

#include <util/MemoryMapped.hpp>
#include <LocalBuffer.hpp>

int main(){

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

