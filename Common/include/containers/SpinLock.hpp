/*
 * SpinLock.hpp
 *
 *  Created on: 24-Jun-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_SPINLOCK_HPP_
#define COMMON_INCLUDE_SPINLOCK_HPP_

#include <Common.hpp>

namespace obLib{

class SpinLock
{
public:
	enum{
		UNLOCKED=0,
		LOCKED=1
	};
	typedef volatile int Lock[1] ;

	Lock _lock;

	SpinLock(){
		_lock[0] = UNLOCKED;
	}

	inline void lock(){
		while(xchg((int*)_lock, LOCKED) != UNLOCKED){
			// Spin only using reads, reduces bus traffic
			while(_lock[0]);
		}
	}

	inline void unlock(){
		WMB();
		_lock[0]=UNLOCKED;
	}
};

class SpinGuard
{
private:
	SpinLock * _mutex;
public:
	SpinGuard(SpinLock& mutex) : _mutex(mutex){
		_mutex->lock();
	}

	~SpinGuard(){
		_mutex->unlock();
	}
};

class Lockable{
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual ~Lockable(){
	}
};
class Condition : public Lockable{
public:
	void signalOne() const;
	void wait() const;
	bool wait(int timeout) const;
	Condition();
	Condition(SpinLock& mutex);
	virtual ~Condition();
};


}



#endif /* COMMON_INCLUDE_SPINLOCK_HPP_ */
