/*
 * LogPolicy.hpp
 *
 *  Created on: 25-Jun-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_LOGGER_LOGPOLICY_HPP_
#define COMMON_INCLUDE_LOGGER_LOGPOLICY_HPP_

#include <Common.hpp>

namespace obLib{

class LogPolicy : private boost::noncopyable{
protected:

	std::string _logInstanceName;

public:

	explicit LogPolicy(const std::string& instance = std::string("Default"), const std::string& filePath = std::string("Default")) : _logInstanceName(instance){
	}

	virtual void openStream() = 0;
	virtual void closeStream() = 0;
	virtual const std::ostream& getStream() const = 0;

	template<typename Obj>
	void write(const Obj& obj){}

	virtual ~LogPolicy(){
	}
};

struct FileLogPolicy : public LogPolicy
{
private:

	std::string _filePath;
	std::ofstream* _fstream;

public:

	explicit FileLogPolicy(const std::string& name = std::string("Default"), const std::string& filePath = std::string("Default")) : LogPolicy(name),
	_filePath(filePath + "_" + boost::lexical_cast<std::string>(::getpid()) + "_" + boost::lexical_cast<std::string>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()) + ".log"){
		openStream();
	}

	~FileLogPolicy(){
		closeStream();
	}

	virtual const std::ostream& getStream() const{
		return *_fstream;
	}

	virtual void openStream(){
		_fstream = new std::ofstream(_filePath.c_str());
		if(!_fstream->good()){
			throw std::runtime_error(" Failed to open filePath: " + _filePath);
		}
	}
	virtual void closeStream(){
		// Do Nothing
		if(_fstream->is_open()){
			_fstream->close();
		}
	}

	template<typename Obj>
	void write(const Obj& obj){
		*_fstream << _logInstanceName << " :: " << obj << std::endl;
	}
};

struct ScreenLogPolicy : public LogPolicy
{
private:

	explicit ScreenLogPolicy(const std::string& name = std::string("Default"), const std::string& filePath = std::string("Default")) : LogPolicy(name){
	}

	~ScreenLogPolicy(){

	}

	virtual const std::ostream& getStream() const{
		throw std::runtime_error("Can not return stream.");
	}

	virtual void openStream(){
		// Do Nothing
	}
	virtual void closeStream(){
		// Do Nothing
	}

	template<typename Obj>
	void write(const Obj& obj){
		std::cout << _logInstanceName << " :: " << obj << std::endl;
	}
};




}



#endif /* COMMON_INCLUDE_LOGGER_LOGPOLICY_HPP_ */
