/*
 * locklessQ_New.hpp
 *
 *  Created on: 17-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_LOCKLESSQ_NEW_HPP_
#define COMMON_INCLUDE_CONTAINERS_LOCKLESSQ_NEW_HPP_

#include <Common.hpp>

namespace obLib{

template <typename T, bool Blocking>
class CheckCondition;

template<typename T, bool Blocking = true>
class LLQueue{

	friend class CheckCondition<T, Blocking>;

	class NodeBase{
		volatile NodeBase* _next;
		friend class LocklessQ<T>;

	public:
		NodeBase() : _next(NULL){

		}
		virtual ~NodeBase(){

		}
	};

	class Node : public NodeBase{
		T _value;
		friend class LocklessQ<T>;
	public:
		Node(const T& val) : NodeBase(), _value(val){

		}

	};

	volatile NodeBase* _head;
	volatile NodeBase* _tail;
	std::condition_variable _nonEmpty;
	bool _destroying;
	std::atomic<int> _waitingCount;
	CheckCondition<T, Blocking> _checkCondition;


	public:


	LLQueue() : _nonEmpty(), _checkCondition(*this){
		volatile NodeBase* node = new NodeBase;
		_head = node;
		_tail = node;
		_destroying = false;
	}

	~LLQueue(){
		_destroying = true;
		while(_waitingCount > 0){
			// Wake up all, no matter if we signal too many times
			for(int i = 0 ; i < _waitingCount ; ++i){
				_nonEmpty.notify_one();
			}
		}

		volatile NodeBase* curr = _head;
		volatile NodeBase* next = curr->_next;
		while(next != NULL){
			delete curr;
			curr = next;
			next = curr->_next;
		}
		delete curr;
	}

	// Adding new node at the End of the Q [ at Tail ]
	void enqueue(const T& val){
		if(_destroying){
			return;
		}

		Node* node = new Node(val);
		volatile NodeBase* tail;
		while(true){
			tail = _tail;
			volatile NodeBase* tailNext = tail->_next;
			if(_tail == tail){
				if(tailNext == NULL){
					volatile NodeBase* oldval ; // = (volatile NodeBase*)(Atomic CAS on Pointer); // Atomic CAS Ptrs - &_tail->_next, node, tailNext
					if(oldval == tailNext){
						break;
					}
				}else{
					// tail is out of sync
					// Atomic CAS on pointers &_tail, tailNext, tail [ Comapre with 1 & 3 , swap 1 & 2 ]
				}
			}
		}

		// Try to move tail to new Node
		// Atomic CAS on pointers _tail, node, tail

		_nonEmpty.notify_one();
	}

	// Take from head
	bool dequeue(T& val){
		if(_destroying){
			return false;
		}

		while(true){
			volatile NodeBase* head = _head;
			volatile NodeBase* tail = _tail;
			volatile NodeBase* headNext = head->_next;
			if(head == _head){
				if(head == tail){
					if(headNext == NULL){
						// Empty Q
						if(_checkCondition()){
							continue;
						}else{
							return false;
						}
					}

					// Tail is falling behind - advance it
					volatile NodeBase* tailNext = tail->_next;
					// Atomic CAS on pointers &_tail, tailNext, tail [ Comapre with 1 & 3 , swap 1 & 2 ]
				}else{
					val = (((Node*)headNext)->_value);
					volatile NodeBase* oldval ; // = (volatile NodeBase*)(Atomic CAS on Pointer); // Atomic CAS Ptrs - &_head, headNext, head
					if(oldval == head){
						delete oldval;
						return true;
					}
				}
			}
		} // end of while
	}
};

template <typename T, bool Blocking>
class CheckCondition{
	friend class LLQueue<T, Blocking>;

	LLQueue<T, Blocking>& _q;

	CheckCondition(LLQueue<T, Blocking>& q) : _q(q){

	}

	bool operator()(){
		++_q._waitingCount; // Atomic Increment
		_q._nonEmpty.wait();
		--_q._waitingCount; // Atomic Decrement // ATOMIC_DEC(&_q._waitingCount);
		if(_q._destroying){
			return false;
		}
		return true;
	}
};

template <typename T>
class CheckCondition<T, false>{
	friend class LLQueue<T, false>;

	CheckCondition(LLQueue<T, false>& q) {

	}

	bool operator()(){
		return false;
	}

};


}

#endif /* COMMON_INCLUDE_CONTAINERS_LOCKLESSQ_NEW_HPP_ */
