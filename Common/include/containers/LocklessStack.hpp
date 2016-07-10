/*
 * LocklessStack.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_LOCKLESSSTACK_HPP_
#define COMMON_INCLUDE_CONTAINERS_LOCKLESSSTACK_HPP_

#include <Common.hpp>

namespace obLib
{


template<typename T>
struct LocklessStack
{
private:

	template<typename T>
	struct Node{
		T _data;
		struct Node<T>* _next;

		Node() : _next(NULL){

		}

		Node(T&& data) : _data(std::move(data)), _next(NULL){
			std::cout << " Contrusctor with Move data. " << std::endl;
		}
	};

	std::atomic<Node<T>*> _head;
	std::atomic<Node<T>*> _tail;
	std::atomic<size_t> _size;

public:

	LocklessStack(){
		// One element always there as Sentinel
		Node<T>* sentinel = new Node<T>();
		_head = sentinel;
		_tail = sentinel;
	}

	bool push(T&& data){
		Node<T>* newNode = new Node<T>(std::move(data));
		newNode->_next = _head.load(std::memory_order_relaxed);
		while(!std::atomic_compare_exchange_weak_explicit(
				&head,
				&newNode->_next,
				newNode,
				std::memory_order_release,std::memory_order_relaxed
		));
	}

	bool front(T& data);
	bool pop();
	size_t size() const ;
};




#endif /* COMMON_INCLUDE_CONTAINERS_LOCKLESSSTACK_HPP_ */
