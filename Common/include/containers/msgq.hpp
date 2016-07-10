/*
 * msgq.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_MSGQ_HPP_
#define COMMON_INCLUDE_CONTAINERS_MSGQ_HPP_

#include <Common.hpp>
#include <SpinLock.hpp>
#include <containers/allocator.hpp>

namespace obLib{

#ifndef INFINITE_TIME
#define INFINITE_TIME (-1)
#endif

double GetTime(){
	// To Do
}
long long GetTimeuSec(){

}
long GetGMTOffset(){

}


template<typename T,
typename LockType = SpinLock,
typename ConditionType = Condition,
typename BarrierType = SpinGuard>
class MsgQ
{
private:
	typedef MsgElement<T> TMessage;
public:
	volatile TMessage* _head;
	volatile TMessage* _tail;
	volatile int _waitingForTake;
	ConditionType _condT;
	LockType _mutexH;
	FreeListI<T>* _freeList;
	void(*_returnFreeListPtr)(FreeListI<T>*);

protected:
	T* get(unsigned long& key){
		volatile TMessage *msg, *newHead;
		T* val;
		{
			BarrierType sp(_mutexH);
			msg = _head;
			newHead = msg->_next;
			if(newHead == 0){
				return 0;
			}

			key = newHead->_key;
			val = newHead->_value;
			_head = newHead;
			_freeList->releaseElement(const_cast<TMessage*>(msg));
		}
		return val;
	}

public:

	template<typename Allocator>
	MsgQ(const Allocator&){
		volatile TMessage* msg = new TMessage();
		_tail = msg;
		_head = msg;
		_waitingForTake = 0;
		_freeList = Allocator::getFreeList();
		_returnFreeListPtr = &Allocator::returnFreeList;
	}

	MsgQ(){
		volatile TMessage* msg = new TMessage();
		_tail = msg;
		_head = msg;
		_waitingForTake = 0;
		_freeList = DummyFreeListAllocator<T>::getFreeList();
		_returnFreeListPtr = &DummyFreeListAllocator<T>::returnFreeList;
	}

	~MsgQ(){
		(*_returnFreeListPtr)(_freeList);
	}

	int post(T * val, unsigned long key){
		if(val == 0){
			return -1;
		}
		volatile TMessage* msg = _freeList->getElement();

		msg->_value = val;
		msg->_key = key;

		{
			BarrierType sp(_mutexH);
			_tail->_next = msg;
			_tail = msg;

			if(_waitingForTake > 0){
				_condT.signalOne();
			}
		}
		return 0;
	}

	int wait(int timeout, T * &data, unsigned long key){
		data = get(key);
		if(data != 0){
			return 0;
		}
		if(timeout == 0){
			data = 0;
			return -1;
		}

		{
			BarrierType sp(_mutexH);
			++_waitingForTake;
			if(timeout == INFINITE_TIME){
				while(1){
					data = get(key);
					if(data != 0){
						--_waitingForTake;
						return 0;
					}
					_condT.wait();
				}
			}

			double waitTime = timeout;
			double start = GetTime();

			while(1){
				data = get(key);
				if(data != 0){
					--_waitingForTake;
					return 0;
				}else{
					_condT.wait((int)waitTime);
					waitTime = (double)timeout - (GetTime() - start);
					if(waitTime <= 0){
						data = 0;
						--_waitingForTake;
						return -1;
					}

				}
			}
		}
	}
};

template<typename T,
typename LockType,
typename ConditionType,
typename BarrierType>
class MsgQ<std::shared_ptr<T>, LockType, ConditionType, BarrierType>
{
private:
	typedef MsgElement<std::shared_ptr<T>> TMessage;
public:
	volatile TMessage* _head;
	volatile TMessage* _tail;
	volatile int _waitingForTake;
	ConditionType _condT;
	LockType _mutexH;
	FreeListI<std::shared_ptr<T>>* _freeList;
	void(*_returnFreeListPtr)(FreeListI<std::shared_ptr<T>>*);

protected:
	std::shared_ptr<T> get(unsigned long& key){
		volatile TMessage *msg, *newHead;
		std::shared_ptr<T> val;
		{
			BarrierType sp(_mutexH);
			msg = _head;
			newHead = msg->_next;
			if(newHead == 0){
				return std::shared_ptr<T>();
			}

			key = newHead->_key;
			val = const_cast<TMessage*>(newHead)->_value;
			_head = newHead;
			const_cast<TMessage*>(newHead)->_value.reset();
			_freeList->releaseElement(const_cast<TMessage*>(msg));
		}
		return val;
	}

public:

	template<typename Allocator>
	MsgQ(const Allocator&){
		volatile TMessage* msg = new TMessage();
		_tail = msg;
		_head = msg;
		_waitingForTake = 0;
		_freeList = Allocator::getFreeList();
		_returnFreeListPtr = &Allocator::returnFreeList;
	}

	MsgQ(){
		volatile TMessage* msg = new TMessage();
		_tail = msg;
		_head = msg;
		_waitingForTake = 0;
		_freeList = DummyFreeListAllocator<std::shared_ptr<T>>::getFreeList();
		_returnFreeListPtr = &DummyFreeListAllocator<std::shared_ptr<T>>::returnFreeList;
	}

	~MsgQ(){
		(*_returnFreeListPtr)(_freeList);
	}

	int post(std::shared_ptr<T> val, unsigned long key){
		if(val == 0){
			return -1;
		}
		volatile TMessage* msg = _freeList->getElement();

		const_cast<TMessage*>(msg)->_value = val;
		msg->_key = key;

		{
			BarrierType sp(_mutexH);
			_tail->_next = msg;
			_tail = msg;

			if(_waitingForTake > 0){
				_condT.signalOne();
			}
		}
		return 0;
	}

	int wait(int timeout, std::shared_ptr<T> &data, unsigned long key){
		data = get(key);
		if(data != 0){
			return 0;
		}
		if(timeout == 0){
			data = 0;
			return -1;
		}

		{
			BarrierType sp(_mutexH);
			++_waitingForTake;
			if(timeout == INFINITE_TIME){
				while(1){
					data = get(key);
					if(data != 0){
						--_waitingForTake;
						return 0;
					}
					_condT.wait();
				}
			}

			double waitTime = timeout;
			double start = GetTime();

			while(1){
				data = get(key);
				if(data != 0){
					--_waitingForTake;
					return 0;
				}else{
					_condT.wait((int)waitTime);
					waitTime = (double)timeout - (GetTime() - start);
					if(waitTime <= 0){
						data = 0;
						--_waitingForTake;
						return -1;
					}

				}
			}
		}
	}
};

}


#endif /* COMMON_INCLUDE_CONTAINERS_MSGQ_HPP_ */
