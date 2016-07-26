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
#include <log.hpp>
#include <adaptorLayer/AdaptorFactory.hpp>

using namespace oms;
int main(){

	AdaptorFactoryPtr af = std::make_shared<AdaptorFactory>();
	SessionListenerI* sl = new DummySessionListener();
	SessionType st = ORDER_OUTRIGHT;
	SessionI* session = af->createSession(st, sl, "CONFIG_PROPS", "IND");

	CONSOLELOG(__FUNCTION__ << " Session Created. ");
	session->start();


	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return 0;
}

