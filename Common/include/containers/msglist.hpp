/*
 * msglist.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_MSGLIST_HPP_
#define COMMON_INCLUDE_CONTAINERS_MSGLIST_HPP_



//#include <containers/ReducedContentionAllocator.hpp>
//#include <containers/ReducedMemUsageAllocator.hpp>
#include <containers/allocator.hpp>
#include <containers/SpinLock.hpp>
#include <Common.hpp>

namespace obLib{

template<typename T>
class MsgList
{
private:
	typedef MsgElement<T> TMessage;

public:

	volatile TMessage* _head;
	volatile TMessage* _tail;
	SpinLock _mutexH;
	SpinLock _mutexT;
	//std::atomic_int _size;
	//ATOMIC_INT_LOCK_FREE _size
	int _size;
	FreeListI<T>* _freeList;
	void (*_returnFreePtrList)(FreeListI<T>*);

	template<typename Allocator>
	MsgList(const Allocator&){
		volatile TMessage* msg = new TMessage();
		_tail = msg;
		_head = msg;
		_size = 0;
		_freeList = Allocator::getFreeList();
		_returnFreePtrList = &Allocator::returnFreeList;
	}

	MsgList(){
		volatile TMessage* msg = new TMessage();
		_tail = msg;
		_head = msg;
		_size = 0;
		_freeList = DummyFreeListAllocator<T>::getFreeList();
		_returnFreePtrList = &DummyFreeListAllocator<T>::returnFreeList;
	}

	~MsgList(){
		(*_returnFreePtrList)(_freeList);
	}

	// Cleanup the last entry in Q.
	void cleanup(){
		_mutexH.lock();
		delete _head;
		_head = 0;
		_mutexH.unlock();
	}

	int put(T* val){
		int size = -1;
		if(val == 0){
			return -1;
		}
		volatile TMessage* msg = _freeList->getElement();
		msg->_value = val;

		{
			_mutexT.lock();
			_tail->_next = msg;
			_tail = msg;
			size = ++_size;
			_mutexT.unlock();
		}
		return size;
	}

	int get(T* &elem){
		volatile TMessage * msg, *newHead;
		int size = 0;
		elem = 0;
		_mutexH.lock();
		msg = _head;
		newHead = msg->_next;
		if(newHead != 0){
			size = --_size;
			elem = newHead->_value;
			_head = newHead;
		}

		if(elem != 0){
			msg->_value = 0;
			_freeList->releaseElement(const_cast<TMessage*>(msg));
		}
		_mutexH.unlock();

		return size;
	}

	int peek(T* &elem){
		volatile TMessage * msg, *newHead;
		elem = 0;
		{
			msg = _head;
			newHead = msg->_next;
			if(newHead != 0){
				elem = newHead->_value;
			}
		}
		return _size;
	}

	// Result might now correct all the time
	// So only use when u don't case about actual bang on size at that instant
	size_t size() const {
		//return _size.load(std::memory_order_acquire);
		return _size;
	}

};

}


#endif /* COMMON_INCLUDE_CONTAINERS_MSGLIST_HPP_ */
