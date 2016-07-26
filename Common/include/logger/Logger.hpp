/*
 * Logger.hpp
 *
 *  Created on: 25-Jun-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_LOGGER_LOGGER_HPP_
#define COMMON_INCLUDE_LOGGER_LOGGER_HPP_


#include <Common.hpp>

namespace obLib{

struct BaseTransport : boost::intrusive::list_base_hook<>
{
	virtual void write() const  = 0;
	virtual ~BaseTransport(){

	}
};

template<typename T, typename Policy>
struct Transport : public BaseTransport{
private:
	const T _t;
	Policy* _policy;

public:
	Transport(const T& t, Policy* p) : _t(std::move(t)), _policy(p){

	}

	virtual void write() const {
		_policy->write(_t);
	}

	virtual ~Transport(){

	}
};

template<typename Policy>
struct Transport<const char*, Policy> : public BaseTransport{
private:
	char* _t;
	Policy* _policy;

public:
	Transport(const char* t, Policy* p) : _policy(p){
		_t = (char*)malloc(sizeof(char)* strlen(t));
		memcpy(_t,t,strlen(t));
	}

	virtual void write() const {
		_policy->write(_t);
	}

	virtual ~Transport(){

	}
};

//template<typename T>
//struct Deleter
//{
//	void operator()(T* t){
//		delete t;
//	}
//};

template <typename T>
struct IntrusiveList : public boost::intrusive::list<T>
{
	struct Deleter{
		void operator()(T* t){
			delete t;
		}
	};

	~IntrusiveList(){
		boost::intrusive::list<T>::clear_and_dispose(Deleter());
	}

};


typedef IntrusiveList<BaseTransport> StandardContainer;


template <typename Policy>
struct LogContainer{
	StandardContainer _cont;
	Policy* _policy;

	LogContainer(Policy* p) : _policy(p){

	}

	template<typename T>
	void put(const T& t){
		std::auto_ptr<Transport<T, Policy> > ap(new Transport<T, Policy>(t, _policy));
		_cont.push_back(*ap.release());
	}

	void put(const char* t){
		std::auto_ptr<Transport<const char*, Policy> > ap(new Transport<const char*, Policy>(t, _policy));
		_cont.push_back(*ap.release());
	}

	void pop(){
		StandardContainer::iterator it = _cont.begin();
		while(!_cont.empty()){
			StandardContainer::iterator curr = it;
			++it;
			BaseTransport& bt = *curr;
			bt.write();
			_cont.erase_and_dispose(curr, StandardContainer::Deleter());
			// For just one entry
			break;
		}
	}

	void popAll(){
		BOOST_FOREACH(const BaseTransport& bt , _cont){
			bt.write();
		}
		_cont.clear_and_dispose(StandardContainer::Deleter());
	}

	bool isEmpty() const{
		return _cont.empty();
	}

	size_t getSize() const {
		return _cont.size();
	}

};

template<typename LogPolicy>
struct Logger : private boost::noncopyable {
private:
	LogPolicy * _logPolicy;
	size_t _numOfLogThreads;
	volatile bool _isRunning;
	LogContainer<LogPolicy> _logContainer;
	std::mutex _logMutex;
	std::condition_variable _logCV;
	std::thread _loggerThread;

public:

	Logger(const std::string& name = std::string("Default"), const std::string& filePath = std::string("Default"), int numOfThreads = 1 ) : _logPolicy(new LogPolicy(name, filePath)),
	_numOfLogThreads(numOfThreads),
	_isRunning(true),
	_logContainer(_logPolicy),
	_loggerThread(&Logger::run, this){
		std::cout << " Log Created. " << std::endl;
	}

	~Logger(){
		stop();
	}

	void stop(){
		_isRunning = false;
		{
			std::unique_lock<std::mutex> lock(_logMutex);
			_logCV.notify_one();
		}
		_loggerThread.join();
	}

	template<typename T>
	void log(const T& t){
		std::unique_lock<std::mutex> lock(_logMutex);
		_logContainer.put(t);
		_logCV.notify_one();
	}

	void log(const char* t){
		std::unique_lock<std::mutex> lock(_logMutex);
		_logContainer.put(t);
		_logCV.notify_one();
	}

	const std::ostream& getStream() const {
		return _logPolicy->getStream();
	}

	void run(){
		while(_isRunning){
			std::unique_lock<std::mutex> lock(_logMutex);
			while(_logContainer.isEmpty()){
				_logCV.wait(lock);
			}
			_logContainer.popAll();
		}
	}
};
}

#endif /* COMMON_INCLUDE_LOGGER_LOGGER_HPP_ */
