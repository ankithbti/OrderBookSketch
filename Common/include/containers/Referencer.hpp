/*
 * Referencer.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_REFERENCER_HPP_
#define COMMON_INCLUDE_CONTAINERS_REFERENCER_HPP_

#include <Common.hpp>
#include <containers/msglist.hpp>

namespace obLib{

typedef int ATOMIC_INTEGER;
#define HIGH_RES_TS long
typedef void* ATOMIC_PTR;

#define FUNCTOR_PRIORITY_MAX -1
#define FUNCTOR_PRIORITY_Q_SIZE -2

enum FlagStatus
{
	FLAG_STATUS_FREE = 0,
	FLAG_STATUS_INUSE,
	FLAG_STATUS_CANCELED,
	FLAG_STATUS_COMPLETED,
	FLAG_STATUS_MAX_DEFINED
};

enum ReturnBitmask
{
	UNKNOWN_BIT = 0,
	KEEP_BIT=1,
	LAST_BIT=2,
	DEL_BIT=4,
	END_BIT=8
};


class TaskMessage;
class TFunctor{
public:
	virtual int operator()(TaskMessage* msg = 0) = 0;
	virtual int call(TaskMessage* msg = 0) = 0;
	virtual ~TFunctor() {
	}
};

class Referencer;
class Dispatcher;

class TaskMessage{
protected:
	int _err;
	FlagStatus _status;
	Referencer * _ref;
	bool _deleteFlag;
	TFunctor* _functor;

public:
	inline void setError(int err) {
		_err = err;
	}

	inline int getError() const {
		return _err;
	}

	inline void setStatus(FlagStatus status){
		_status = status;
	}

	inline FlagStatus getStatus() const{
		return _status;
	}

	inline void setDeleteFlag(bool delFlag){
		_deleteFlag = delFlag;
	}

	inline bool getDeleteFlag() const{
		return _deleteFlag;
	}

	inline void setRef(Referencer * ref){
		_ref = ref;
	}

	inline Referencer* getRef() const{
		return _ref;
	}

	inline void setFunctor(TFunctor * func){
		_functor = func;
	}

	inline TFunctor* getFunctor() const{
		return _functor;
	}

	virtual ~TaskMessage(){

	}

	TaskMessage() : _err(0), _status(FLAG_STATUS_FREE), _ref(0), _deleteFlag(false){

	}

	virtual TaskMessage* execute() = 0;

};

class IDispatchQ{
public:
	virtual int post(TaskMessage* msg, unsigned long key) = 0;
	virtual int wait(int timeout, TaskMessage* &msg, unsigned long &key) = 0;
	virtual int associate(void*);
	virtual ~IDispatchQ(){
	}
};

class AspectRefI{
	virtual void init(Referencer* ref) = 0;
	virtual void before(Referencer* ref) = 0;
	virtual void after(Referencer* ref) = 0;
	virtual TaskMessage* around(Referencer* ref) = 0;
	virtual ~AspectRefI(){

	}
};

class Referencer{
private:
	IDispatchQ * _dispatchQ;
	ATOMIC_INTEGER _listSize;
	ATOMIC_INTEGER _priority;
	MsgList<TaskMessage> _completionList;
	AspectRefI * _aspectI;
	HIGH_RES_TS _hrts;

public:
	Referencer(AspectRefI* aspect = 0) : _listSize(0),
	_priority(FUNCTOR_PRIORITY_MAX),
	_completionList(),
	_aspectI(aspect){

	}

	template<typename Allocator>
	Referencer(const Allocator& a, AspectRefI* aspect) : _listSize(0),
	_priority(FUNCTOR_PRIORITY_MAX),
	_completionList(a),
	_aspectI(aspect){

	}

	AspectRefI* getAspect() {
		return _aspectI;
	}

	void setDispatchQ(IDispatchQ* dispatchQ){
		_dispatchQ = dispatchQ;
	}

	ATOMIC_INTEGER setPriority(ATOMIC_INTEGER newPrio){
		return xchg(&_priority, newPrio);
	}

	ATOMIC_INTEGER getPriority(){
		return _priority;
	}

	HIGH_RES_TS& getQueuingTimeStamp(){
		return _hrts;
	}

	ATOMIC_INTEGER getListSize(){
		return _completionList.size();
	}

	bool put(TaskMessage* msg){
		msg->setStatus(FLAG_STATUS_INUSE);
		if(_completionList.put(msg) == 1){
			_hrts = 0; // reset HRTS
			if(_dispatchQ->post(msg, 0) < 0){
				return false;
			}
		}
		return true;
	}

	TaskMessage* add(TaskMessage* msg){
		msg->setStatus(FLAG_STATUS_INUSE);
		if(_completionList.put(msg) == 1){
			return msg;
		}
		return 0;
	}

	TaskMessage* getNext(){
		TaskMessage* tmp;
		if(_completionList.get(tmp) > 0){
			_completionList.peek(tmp);
		}
		return tmp;
	}

	TaskMessage* execute(TaskMessage* msg){
		ATOMIC_INTEGER listSize;
		TaskMessage* tmp = msg;
		ATOMIC_INTEGER priority = _priority;
		TaskMessage* ret = 0;

		if(priority == FUNCTOR_PRIORITY_Q_SIZE){
			priority = _completionList.size();
			if(tmp->getStatus() != FLAG_STATUS_INUSE){
				++priority;
			}
		}

		if(_aspectI != 0){
			_aspectI->init(this);
		}

		while(1){

			listSize = 1;

			if(tmp == 0){
				return 0;
			}

			if(tmp->getStatus() != FLAG_STATUS_INUSE){
				tmp->setStatus(FLAG_STATUS_INUSE);
				listSize = _completionList.size();
			}

			if(listSize == 1){
				if(_aspectI != 0){
					_aspectI->before(this);
				}

				// execute Functor
				int res = tmp->getFunctor()->call(tmp);
				if(res & END_BIT){
					return 0;
				}

				if(_aspectI != 0){
					_aspectI->after(this);
				}

				if(priority > 0){
					--_priority;
				}

				if(_priority == 0 || res & LAST_BIT){
					if(_aspectI != 0){
						ret = _aspectI->around(this);
					}

					if(res & DEL_BIT){
						delete tmp;
						tmp = 0;
					}

					if(!(res & KEEP_BIT)){
						if(_completionList.get(tmp) > 0){
							_completionList.peek(tmp);
						}else{
							tmp = 0;
						}
					}

					if(tmp != 0){
						_dispatchQ->post(tmp, 0);
					}

					if(_priority == FUNCTOR_PRIORITY_Q_SIZE){
						// Reset Time stamp
						_hrts = 0;
					}
					break; // out of while loop
				}

				if(_completionList.size() < 2 || (res & KEEP_BIT)){
					if(_aspectI != 0){
						ret = _aspectI->around(this);
					}
				}
				if(res & DEL_BIT){
					delete tmp;
					tmp = 0;
				}

				if(!(res & KEEP_BIT)){
					if(_completionList.get(tmp) > 0){
						_completionList.peek(tmp);
					}else{
						tmp = 0;
					}
				}
			}

			if(tmp == 0){
				break;
			}


		}// end of outer while
		return ret;
	}

	virtual ~Referencer(){

	}

};


}



#endif /* COMMON_INCLUDE_CONTAINERS_REFERENCER_HPP_ */
