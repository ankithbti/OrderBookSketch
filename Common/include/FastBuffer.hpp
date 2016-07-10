/*
 * FastBuffer.hpp
 *
 *  Created on: 24-Jun-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_FASTBUFFER_HPP_
#define COMMON_INCLUDE_FASTBUFFER_HPP_

#include <containers/SpinLock.hpp>

namespace obLib{

#pragma pack(push, 1) // For memory alignment
class FastBuffer
{

	union{
		volatile unsigned long _pread;
		char _padding1[INTEL_CACHE_LINE];
	};

	union{
		volatile unsigned long _pwrite;
		char _padding2[INTEL_CACHE_LINE];
	};

	const size_t _size;
	void ** _buffer;

	SpinLock _spinLock;
	char _padding3[INTEL_CACHE_LINE - sizeof(size_t) - sizeof(char**)];

public:

	FastBuffer(unsigned long size) : _pread(0), _pwrite(0), _size(0), _buffer(0){
		if(!init()){
			std::cout << " Some issue while initializing the buffer. " << std::endl;
			throw std::runtime_error(" Some issue while initializing the buffer. ");
		}
		(void)_padding1;
		(void)_padding2;
		(void)_padding3;
	}

	~FastBuffer(){
		::free(_buffer);
	}

	inline void* begin() const {
		return _buffer[_pread];
	}

	inline size_t length() const {
		unsigned long tpread = _pread, twrite = _pwrite;
		long len = twrite-tpread;
		if(len>0)
			return static_cast<size_t>(len);
		if(len<0)
			return static_cast<size_t>(_size + len);
		if(_buffer[twrite] == NULL)
			return 0;
		return _size;
	}

	inline size_t bufferSize() const {
		return _size;
	}

	inline bool isEmpty() const{
		return (_buffer[_pread] == NULL);
	}

	inline bool isAvailable() const{
		return (_buffer[_pwrite] == NULL);
	}

	inline bool push_sp(void* const data){
		if(!data)
			return false;
		if(isAvailable()){
			WMB();
			_buffer[_pwrite] = data;
			// Circular Buffer
			_pwrite += (((_pwrite + 1) >= _size) ? (1-_size) : 1);
			return true;
		}
		return false;
	}

	inline bool pop_sc(void **data){
		if(isEmpty()){
			return false;
		}
		*data = _buffer[_pread];
		_buffer[_pread] = NULL;
		// Circular Buffer
		_pread += (((_pread+1) >= _size) ? (1-_size) : 1);
		return true;
	}

	inline bool push_mp(void* const data){
		_spinLock.lock();
		bool x = push_sp(data);
		_spinLock.unlock();
		return x;
	}

	inline bool pop_mp(void **data){
		_spinLock.lock();
		bool x = pop_sc(data);
		_spinLock.unlock();
		return x;
	}

private:

	bool init(){
		if(_buffer || (_size == 0)){
			return false;
		}
		if(posix_memalign((void**)_buffer, INTEL_CACHE_LINE*sizeof(long), _size * sizeof(void*))){
			return false;
		}
		reset();
		return true;
	}

	void reset(){
		_pread = _pwrite = 0;
		memset(_buffer, 0, _size * sizeof(void*));
	}


};
#pragma pack(pop)

}


#endif /* COMMON_INCLUDE_FASTBUFFER_HPP_ */
