#include <Publisher.hpp>

using namespace obLib;

// packets: 558273 - each packet is 80 bytes == 44661840 bytes

struct Pub{
	size_t _port;
	std::string _filePath;
	bool _runForSingleSec;

	Pub(size_t port, const std::string& file, bool runForSingleSec = false) : _port(port), _filePath(file),
			_runForSingleSec(runForSingleSec){
	}

  void run(){
	try{
    boost::asio::io_service io;
    Publisher pub(io, _port, _filePath, _runForSingleSec);
    io.run();
	}catch(const std::runtime_error& err)
	{
		std::cout << " Pub::run() - Error: Caught Exception: " << err.what() << std::endl;
	}
  }
};

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		std::cout << " main() - Usage: " << argv[0] << " <Port> <UdpHexPacketFile> <1/0 - runForSingleSec/No>" << std::endl;
		return 1;
	}
	try{
		std::ifstream in(argv[2]);
		if(!in.good()){
			std::cout << " main() - Error: File: " << argv[2] << " , is not readable. " << std::endl;
			return 2;
		}

  		Pub p(boost::lexical_cast<size_t>(argv[1]), argv[2], boost::lexical_cast<bool>(argv[3]));
  		std::thread t1(std::bind(&Pub::run, p));
  		t1.join();	
	}catch(const boost::bad_lexical_cast& err){
		std::cout << " main() - Error: Caught exception: " << err.what() << std::endl;
	}
  	return 0;
};
