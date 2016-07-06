#include <Subscriber.hpp>
#include <TimeUtil.hpp>
#include <HashMap.h>
#include <logger/Logger.hpp>
#include <logger/LogPolicy.hpp>
#include <memory>
#include <sparsehash/dense_hash_map>
#include <session/tcp/TcpClientSession.hpp>

using namespace obLib;

struct Sub{

	std::string _host;
	std::string _port;
	std::string _file;

	Sub(const std::string& host, const std::string& port, const std::string& file) : _host(host), _port(port), _file(file){
	}

	void run(){
		try{

			boost::asio::io_service io;
			Subscriber sub(io, _host, _port, _file);
			io.run();
		}catch(const std::runtime_error& err){
			std::cout << " Sub::run() - Error: Caught exception: " << err.what() << std::endl;
		}
	}
};

int main(int argc, char ** argv)
{

	TcpClientSession tcs("www.google.com", "80");

	if(tcs.connect()){
		std::cout << "Successfully Connected to " << tcs.getRemoteHost() << ":" << tcs.getRemotePort() << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;


	{
		OrderBook::SharedPtr ob;
		std::unordered_map<int64_t, OrderBook::SharedPtr> myMap;
		std::string str;
		{

//			for(int i = 0 ; i < 100000; ++i){
//				myMap.emplace(i, ob);
//			}
			myMap.emplace(80000,ob);
			//myMap.find(20000);
			LatencyChecker<> lc(str);
			myMap.find(80000);
//			myMap.find(10);
//			myMap.find(40000);
		}
		std::cout << " Latency of Insertion in std::hashmap -> " << str << std::endl;
	}

	{
		OrderBook::SharedPtr ob;
		google::dense_hash_map<int64_t, OrderBook::SharedPtr> myMap;
		myMap.set_empty_key(-1);
		myMap.set_deleted_key(-2);
		std::string str;
		{

//			for(int i = 0 ; i < 100000; ++i){
//				myMap.insert(std::pair<int64_t, OrderBook::SharedPtr>(i, ob));
//			}
			myMap.insert(std::pair<int64_t, OrderBook::SharedPtr>(80000, ob));
			//myMap.find(20000);
			LatencyChecker<> lc(str);
			myMap.find(80000);
//			myMap.find(10);
//			myMap.find(40000);
		}

		std::cout << " Latency of Insertion in google::densehashmap -> " << str << std::endl;
	}

	if(argc < 4)
	{
		std::cout << " main() - Usage: " << argv[0] << " <Host> <Port> <UdpHexPacketFile>" << std::endl;
		return 1;
	}

	try{

		std::ifstream in(argv[3]);
		if(!in.good()){
			std::cout << " main() - Error: File: " << argv[3] << " , is not readable. " << std::endl;
			return 2;
		}

		Sub s(argv[1], argv[2], argv[3]);
		std::thread t2(std::bind(&Sub::run, s));

		// Set Thread Priority
		sched_param schedulingParams;
		schedulingParams.sched_priority = 1;
		if(pthread_setschedparam(t2.native_handle(), SCHED_RR, &schedulingParams)){
			std::cout << " main() - Error: Can not set Thread Priority. " << std::endl;
			return 2;
		}
		t2.join();
	}catch(const std::runtime_error& err){
		std::cout << " main() - Error: Caught Execption: " << err.what() << std::endl;
		return 3;
	}

	std::cout << " Exiting Application. " << std::endl;
	return 0;
};
