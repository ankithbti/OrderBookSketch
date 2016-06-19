#include <Subscriber.hpp>
#include <TimeUtil.hpp>

using namespace obLib;

struct Sub{

	std::string _host;
	std::string _port;

	Sub(const std::string& host, const std::string& port) : _host(host), _port(port){
	}

	void run(){
		try{
			boost::asio::io_service io;
			Subscriber sub(io, _host, _port);
			io.run();
		}catch(const std::runtime_error& err){
			std::cout << " Sub::run() - Error: Caught exception: " << err.what() << std::endl;
		}
	}
};


int main(int argc, char ** argv)
{

	if(argc < 3)
	{
		std::cout << " main() - Usage: " << argv[0] << " <Host> <Port>" << std::endl;
		return 1;
	}

	try{

		Sub s(argv[1], argv[2]);
		std::thread t2(std::bind(&Sub::run, s));

		// Set Thread Priority
		sched_param schedulingParams;
		schedulingParams.sched_priority = 2;
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
