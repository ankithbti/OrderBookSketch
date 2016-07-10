/*
 * LocklessQ.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_LOCKLESSQ_HPP_
#define COMMON_INCLUDE_CONTAINERS_LOCKLESSQ_HPP_

#include <Common.hpp>

namespace obLib
{


template<typename T>
struct LocklessQ
{
private:
	template <typename U>
	struct QNode{
		U _data;
		QNode<U>* _next;

		QNode() : _next(NULL){

		}

		QNode(T&& data) : _data(std::move(data)), _next(NULL){
		}
	};

	std::atomic<QNode<T>*> _head;
	std::atomic<QNode<T>*> _tail;
	std::atomic<size_t> _size;
	std::atomic<QNode<T>*> _tmpAtomic;

	QNode<T>* _emptyNode;

public:

	LocklessQ() : _emptyNode(NULL){
		// One element always there as Sentinel
		QNode<T>* sentinel = new QNode<T>();
		_head = sentinel;
		_tail = sentinel;
	}

	bool enqueue(T&& data){
		QNode<T>* newNode = new QNode<T>(std::move(data));
		QNode<T>* tmp;
		bool succ = false;
		do
		{
			tmp = _tail;
			_tmpAtomic.store(tmp->_next, std::memory_order_release);
			succ = std::atomic_compare_exchange_weak_explicit(&_tmpAtomic, &_emptyNode, newNode, std::memory_order_release,std::memory_order_relaxed);
			tmp->_next = _tmpAtomic.load(std::memory_order_acquire);
			if(!succ){
				std::atomic_compare_exchange_weak_explicit(&_tail, &tmp, tmp->_next, std::memory_order_release,std::memory_order_relaxed);
			}
		}while(!succ);
		std::atomic_compare_exchange_weak_explicit(&_tail, &tmp, newNode, std::memory_order_release,std::memory_order_relaxed);
		++_size;
		return true;
	}

	bool dequeue(T& data){
		QNode<T>* tmp;
		do
		{
			tmp = _head;
			if(!tmp->_next){
				return false; // empty Q
			}
		}while(!std::atomic_compare_exchange_weak_explicit(&_head, &tmp, tmp->_next, std::memory_order_release,std::memory_order_relaxed));
		data = tmp->_next->_data;
		--_size;
		return true;
	}

	size_t size() const {
		return _size.load(std::memory_order_acquire);
	}
};

}



#endif /* COMMON_INCLUDE_CONTAINERS_LOCKLESSQ_HPP_ */
