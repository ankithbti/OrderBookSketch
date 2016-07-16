/*
 * MemoryMapped.hpp
 *
 *  Created on: 14-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_UTIL_MEMORYMAPPED_HPP_
#define COMMON_INCLUDE_UTIL_MEMORYMAPPED_HPP_

#include <Common.hpp>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace obLib{

class MemoryMapped : private boost::noncopyable
{
	std::string _fileName;
	size_t _memoryMapsize;
	int _fileDesc;
	void * _memoryMap;

public:

	typedef boost::shared_ptr<MemoryMapped> SharedPtr;

	MemoryMapped(const std::string name, size_t size) :
		_fileName(name),
		_memoryMapsize(size),
		_fileDesc(0),
		_memoryMap(NULL){

		_fileDesc = open(_fileName.c_str(), O_RDWR|O_CREAT, (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH));
		if(_fileDesc == -1){
			throw std::runtime_error("MemoryMapped() - Could not open FileDescriptor.");
		}
		_memoryMap = mmap(NULL, _memoryMapsize, PROT_READ|PROT_WRITE, MAP_SHARED, _fileDesc, 0);

		if(_memoryMap == MAP_FAILED){
			_memoryMap = NULL;
			throw std::runtime_error("MemoryMapped() - Could not create MemoryMap.");
		}

		off_t offtmp;
		offtmp = lseek(_fileDesc, 0, SEEK_END);
		if(offtmp < _memoryMapsize){
			offtmp = lseek(_fileDesc, (_memoryMapsize-1), SEEK_SET);
			unsigned char ch='\0';
			write(_fileDesc, &ch, sizeof(ch));

			offtmp = lseek(_fileDesc, 0, SEEK_END);
			if(offtmp < _memoryMapsize){
				_memoryMap = NULL;
				throw std::runtime_error("MemoryMapped() - Memory map failed.");
			}
		}
	}

	~MemoryMapped(){
		msync(_memoryMap, _memoryMapsize, MS_SYNC);
		close(_fileDesc);
		munmap(_memoryMap, _memoryMapsize);
	}

	unsigned char* getAddress(){
		return (unsigned char*)(_memoryMap);
	}

	int flushAndWait(){
		return msync(_memoryMap, _memoryMapsize, MS_SYNC);
	}

	int flushNoWait(){
		return msync(_memoryMap, _memoryMapsize, MS_ASYNC);
	}

};
}


#endif /* COMMON_INCLUDE_UTIL_MEMORYMAPPED_HPP_ */
