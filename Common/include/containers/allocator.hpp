/*
 * allocator.hpp
 *
 *  Created on: 09-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_ALLOCATOR_HPP_
#define COMMON_INCLUDE_CONTAINERS_ALLOCATOR_HPP_


#include <util/templatepolicy.hpp>
#include <Common.hpp>

namespace obLib
{

#define MEMORY_ALIGNMENT sizeof(void*)

// Forward declaration for _owner pointers
template <typename T, bool shouldAllocateStorage>
class FreeList;

template <typename T, bool shouldAllocateStorage = false>
class MsgElement{
protected:
	size_t _size;
public:
	typedef T* ContainedType;
	ContainedType _value;
	FreeList<T, shouldAllocateStorage> * _owner;
	volatile MsgElement* _next;
	unsigned long _key;

	MsgElement(T* val, unsigned long key) : _size(0), _value(val), _next(0), _key(key){

	}

	MsgElement(T* val) : _size(0), _value(val), _next(0), _key(0){

	}

	MsgElement() : _size(0), _value(0), _next(0), _key(0){
		if(shouldAllocateStorage){
			_value = (T*)(new char[sizeof(T) + MEMORY_ALIGNMENT]);
			*(MsgElement<T, shouldAllocateStorage>**)_value = this;
			_value = (T*)((char*)_value + MEMORY_ALIGNMENT);
		}
	}

	MsgElement(size_t size) : _size(size), _value(0), _next(0), _key(0){
		if(_size > 0){
			_value = (T*)(new char[_size + MEMORY_ALIGNMENT]);
			*(MsgElement<T, shouldAllocateStorage>**)_value = this;
			_value = (T*)((char*)_value + MEMORY_ALIGNMENT);
		}
	}

	~MsgElement(){
		if(shouldAllocateStorage || _size > 0){
			char * realVal = (char*)_value;
			realVal -= MEMORY_ALIGNMENT;
			delete [] realVal;
		}
	}

};

template<>
inline MsgElement<void, true>::MsgElement(){
	throw std::runtime_error("Void implementation of MsgElement<T> used.");
}

template<>
inline MsgElement<void, false>::MsgElement(){
	throw std::runtime_error("Void implementation of MsgElement<T> used.");
}

template <typename T, bool shouldAllocateStorage>
class MsgElement<std::shared_ptr<T>, shouldAllocateStorage>{
private:
	struct SharedPtrDeleter{
		void operator()(T* ptr){
			char *realptr = (char*)ptr;
			realptr -= MEMORY_ALIGNMENT;
			delete [] realptr;
		}
	};

protected:
	size_t _size;
public:
	typedef std::shared_ptr<T> ContainedType;

	ContainedType _value;
	FreeList<ContainedType, shouldAllocateStorage>* _owner;
	volatile MsgElement* _next;
	unsigned long _key;

	MsgElement(ContainedType val, unsigned long key) : _size(0), _value(val), _next(0), _key(key){

	}

	MsgElement(ContainedType val) : _size(0), _value(val), _next(0), _key(0){

	}

	MsgElement() : _size(0), _value((T*)0), _next(0), _key(0){
		if(shouldAllocateStorage){
			T* tmpVal = (T*)(new char[sizeof(T) + MEMORY_ALIGNMENT]);
			*(MsgElement<ContainedType, shouldAllocateStorage>**)tmpVal = this;
			tmpVal = (T*)((char*)tmpVal + MEMORY_ALIGNMENT);
			_value = ContainedType(tmpVal, SharedPtrDeleter());
		}
	}

	MsgElement(size_t size) : _size(size), _value((void*)0), _next(0), _key(0){
		if(shouldAllocateStorage){
			void* tmpVal = (void*)(new char[_size + MEMORY_ALIGNMENT]);
			*(MsgElement<std::shared_ptr<void>, shouldAllocateStorage>**)tmpVal = this;
			tmpVal = (void*)((char*)tmpVal + MEMORY_ALIGNMENT);
			_value = std::shared_ptr<void>(tmpVal, SharedPtrDeleter());
		}
	}

};

template<> inline MsgElement<std::shared_ptr<void>, true >::MsgElement() {
	throw std::runtime_error("Void implementation of MsgElement<shared_ptr<T>> used.");
}

template<> inline MsgElement<std::shared_ptr<void>, false >::MsgElement() {
	throw std::runtime_error("Void implementation of MsgElement<shared_ptr<T>> used.");
}



template<typename T, bool shouldAllocateStorage = false>
class FreeListI {
public:
	virtual MsgElement<T, shouldAllocateStorage>* getElement() = 0;
	virtual void releaseElement(MsgElement<T, shouldAllocateStorage>* element) = 0;

	virtual typename MsgElement<T, shouldAllocateStorage>::ContainedType alloc() = 0;
	virtual void release(typename MsgElement<T, shouldAllocateStorage>::ContainedType element) = 0;

	virtual ~FreeListI(){

	}
};

template<typename T>
class DummyAllocator
{
public:
	static T* alloc(){
		return (T*)new char[sizeof(T)];
	}

	static void release(T* elem){
		delete [] elem;
	}
};

template<typename T>
class DummyAllocator<std::shared_ptr<T>>
{
private:
	class SharedPtrDeleter{
	public:
		void operator()(T* ptr){
			delete [] ptr;
		}
	};
public:
	static std::shared_ptr<T> alloc(){
		return std::shared_ptr<T>((T*)new char[sizeof(T)], SharedPtrDeleter());
	}

	static void release(T* elem){
		// Nothing to Do
	}
};

template<typename T, bool shouldAllocateStorage = false>
class DummyFreeListBase : public FreeListI<T, shouldAllocateStorage>{
protected:
	virtual MsgElement<T, shouldAllocateStorage>* allocNewElement(){
		return new MsgElement<T, shouldAllocateStorage>();
	}

public:
	DummyFreeListBase(int hwm, int lwm){

	}

	virtual ~DummyFreeListBase(){

	}

	virtual MsgElement<T, shouldAllocateStorage>* getElement(){
		return allocNewElement();
	}

	virtual void releaseElement(MsgElement<T, shouldAllocateStorage>* elem){
		delete elem;
	}

	virtual typename MsgElement<T, shouldAllocateStorage>::ContainedType alloc(){
		if(shouldAllocateStorage){
			return DummyAllocator<T>::alloc();
		}else{
			throw std::runtime_error("INFO: alloc - Can not call alloc when shouldAllocateStorage is false. ");
			//return typename MsgElement<T, shouldAllocateStorage>::ContainedType();
		}
	}

	virtual void release(typename MsgElement<T, shouldAllocateStorage>::ContainedType elem){
		if(shouldAllocateStorage){
			DummyAllocator<T>::release(elem);
		}else{
			throw std::runtime_error("INFO: alloc - Can not call release when shouldAllocateStorage is false. ");
		}
	}
};

template<typename T, bool shouldAllocateStorage = false>
class DummyFreeListVoidTypes : public DummyFreeListBase<T, shouldAllocateStorage>{
protected:
	size_t _allocSize;

	virtual MsgElement<T, shouldAllocateStorage>* allocNewElement(){
		return new MsgElement<T, shouldAllocateStorage>();
	}

public:
	DummyFreeListVoidTypes(size_t size, int hwm, int lwm) : DummyFreeListBase<T, shouldAllocateStorage>(hwm, lwm), _allocSize(size){

	}

	virtual ~DummyFreeListVoidTypes(){

	}
};

template <typename T, bool shouldAllocateStorage = false>
class DummyFreeList : public DummyFreeListBase<T, shouldAllocateStorage>{
public:

	DummyFreeList(int hwm, int lwm) : DummyFreeListBase<T, shouldAllocateStorage>(hwm, lwm){

	}

	~DummyFreeList(){

	}
};


template <bool shouldAllocateStorage>
class DummyFreeList<void, shouldAllocateStorage> : public DummyFreeListVoidTypes<void, shouldAllocateStorage>{
public:

	DummyFreeList(size_t size, int hwm, int lwm) : DummyFreeListVoidTypes<void, shouldAllocateStorage>(size, hwm, lwm){

	}

	~DummyFreeList(){

	}
};

template <bool shouldAllocateStorage>
class DummyFreeList<std::shared_ptr<void>, shouldAllocateStorage> : public DummyFreeListVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>{
public:

	DummyFreeList(size_t size, int hwm, int lwm) : DummyFreeListVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>(size, hwm, lwm){

	}

	~DummyFreeList(){

	}
};




template <typename T, bool shouldAllocateStorage>
class FreeListReleaseDiscriminator{
public:
	static MsgElement<T, shouldAllocateStorage>* getElement(T* e){
		MsgElement<T, shouldAllocateStorage>* elem =
				*(MsgElement<T, shouldAllocateStorage>**)((char*)e - MEMORY_ALIGNMENT);
		return elem;
	}
};

template<typename T, bool shouldAllocateStorage>
class FreeListReleaseDiscriminator<std::shared_ptr<T>, shouldAllocateStorage>{
public:
	static MsgElement<std::shared_ptr<T>, shouldAllocateStorage>* getElement(std::shared_ptr<T> e){
		MsgElement<std::shared_ptr<T>, shouldAllocateStorage>* elem =
				*(MsgElement<std::shared_ptr<T>, shouldAllocateStorage>**)(((char*)e.get()) - MEMORY_ALIGNMENT);
		return elem;
	}
};

template <typename T, bool shouldAllocateStorage = false>
class FreeListBase : public FreeListI<T, shouldAllocateStorage>{
protected:

	volatile MsgElement<T, shouldAllocateStorage>* volatile _freeHead;
	volatile MsgElement<T, shouldAllocateStorage>* volatile _freeTail;

	std::atomic<int> _freeListSize;

	int _highWaterMark;
	int _lowWaterMark;

	virtual MsgElement<T, shouldAllocateStorage>* allocNewElement(){
		return new MsgElement<T, shouldAllocateStorage>();
	}

public:
	FreeListBase(int hwm, int lwm) : _highWaterMark(hwm), _lowWaterMark(lwm){
		_freeListSize = _highWaterMark;
		_freeHead = _freeTail = NULL;
	}

	virtual ~FreeListBase(){
		volatile MsgElement<T, shouldAllocateStorage>* curr = _freeHead;
		volatile MsgElement<T, shouldAllocateStorage>* next;
		while(curr){
			next = curr->_next;
			delete curr;
			curr = next;
		}
	}

	void init(){
		volatile MsgElement<T, shouldAllocateStorage>* msg =  allocNewElement();
		_freeHead = msg;
		_freeTail = msg;

		for(int i = 0 ; i < _highWaterMark ; ++i){
			msg = allocNewElement();
			_freeTail->_next = msg;
			_freeTail = msg;
		}
	}

	virtual MsgElement<T, shouldAllocateStorage>* getElement() {
		volatile MsgElement<T, shouldAllocateStorage>* msg = 0;
		while(_freeListSize > _lowWaterMark){
			while(_freeHead == 0){
				//sched_yield();
				__asm__ volatile("pause");
			}
			msg = (MsgElement<T, shouldAllocateStorage>*)xchg(&_freeHead, NULL);
			if(msg){
				volatile MsgElement<T, shouldAllocateStorage>* tmp = msg->_next;
				if(tmp == 0){
					if(_freeListSize > _highWaterMark){
						// Spin
						_freeHead = msg;
						continue;
					}else{
						// Allocate
						_freeHead = msg;
						break;
					}
				}
				--_freeListSize;
				msg->_next = NULL;
				_freeHead = tmp;

				return const_cast<MsgElement<T, shouldAllocateStorage>*>(msg);
			}
		}// end of outer while

		return allocNewElement();
	}


	virtual void releaseElement(MsgElement<T, shouldAllocateStorage>* element) {

		volatile MsgElement<T, shouldAllocateStorage>* velement = element;
		if(velement){
			if(_freeListSize < _highWaterMark){
				velement->_next = 0;
				volatile MsgElement<T, shouldAllocateStorage>* oldTail =
						(MsgElement<T, shouldAllocateStorage>*)xchg(&_freeTail, velement);
				oldTail->_next = velement;
				++_freeListSize;
			}else{
				delete velement;
			}
		}else{

		}
		throw std::runtime_error(" Empty element to release.");

	}

	virtual typename MsgElement<T, shouldAllocateStorage>::ContainedType alloc(){
		if(shouldAllocateStorage){
			MsgElement<T, shouldAllocateStorage>* elem = getElement();
			return elem->_value;
		}
		throw std::runtime_error("Should not use Freelist alloc with shouldAllocateStorage to false.");
	}

	virtual void release(typename MsgElement<T, shouldAllocateStorage>::ContainedType element){
		if(shouldAllocateStorage){
			MsgElement<T, shouldAllocateStorage>* elem =
					FreeListReleaseDiscriminator<T, shouldAllocateStorage>::getElement(element);
			releaseElement(elem);
		}else{
			throw std::runtime_error("Should not use Freelist alloc with shouldAllocateStorage to false.");
		}
	}

};

template<typename T, bool shouldAllocateStorage = false>
class FreeListBaseVoidTypes : public FreeListBase<T, shouldAllocateStorage>{
protected:
	size_t _allocatedSize;

	virtual MsgElement<T, shouldAllocateStorage>* allocNewElement(){
		return new MsgElement<T, shouldAllocateStorage>(_allocatedSize);
	}

public:
	FreeListBaseVoidTypes(size_t size, int hwm, int lwm) : FreeListBase<T, shouldAllocateStorage>(hwm, lwm), _allocatedSize(size){
		this->init();
	}

	virtual typename MsgElement<T, shouldAllocateStorage>::ContainedType alloc(){
		if(_allocatedSize > 0){
			MsgElement<T, shouldAllocateStorage> * msg = this->getElement();
			return msg->_value;
		}else{
			throw std::runtime_error(" Can not use Freelist<void> with size <= 0");
		}
	}

	virtual void release(typename MsgElement<T, shouldAllocateStorage>::ContainedType element){
		if(_allocatedSize > 0){
			MsgElement<T, shouldAllocateStorage> * msg = FreeListReleaseDiscriminator<T, shouldAllocateStorage>::getElement(element);
			this->releaseElement(msg);
		}else{
			throw std::runtime_error(" Can not use Freelist<void> with size <= 0");
		}
	}

};


template <typename T, bool shouldAllocateStorage>
class FreeList : public FreeListBase<T, shouldAllocateStorage>{
public:
	FreeList(int hwm, int lwm) : FreeListBase<T, shouldAllocateStorage>(hwm, lwm){
		this->init();
	}

	virtual ~FreeList(){

	}
};


template <bool shouldAllocateStorage>
class FreeList<void, shouldAllocateStorage> : public FreeListBaseVoidTypes<void, shouldAllocateStorage>{
public:
	FreeList(size_t size, int hwm, int lwm) : FreeListBaseVoidTypes<void, shouldAllocateStorage>(size, hwm, lwm){
		this->init();
	}

	virtual ~FreeList(){

	}
};

template <bool shouldAllocateStorage>
class FreeList<std::shared_ptr<void>, shouldAllocateStorage> : public FreeListBaseVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>{
public:
	FreeList(size_t size, int hwm, int lwm) : FreeListBaseVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>(size, hwm, lwm){
		this->init();
	}

	virtual ~FreeList(){

	}
};

template <typename T, bool shouldAllocateStorage = false>
class OwnerFreeListBase : public FreeList<T, shouldAllocateStorage>{
	typedef FreeList<T, shouldAllocateStorage> BaseClass;
protected:

	OwnerFreeListBase(size_t size, int hwm, int lwm) : FreeList<T, shouldAllocateStorage>(size, hwm, lwm){

	}

public:
	OwnerFreeListBase(int hwm, int lwm) : FreeList<T, shouldAllocateStorage>(hwm, lwm){

	}

	virtual ~OwnerFreeListBase(){

	}

	virtual MsgElement<T, shouldAllocateStorage>* getElement(){
		MsgElement<T, shouldAllocateStorage>* msg = FreeList<T, shouldAllocateStorage>::getElement();
		msg->_owner = static_cast<FreeList<T, shouldAllocateStorage>*>(this);
		return msg;
	}

	virtual void releaseElement(MsgElement<T, shouldAllocateStorage>* elem){
		elem->_owner->FreeList<T, shouldAllocateStorage>::releaseElement(elem);
	}

};


template <typename T, bool shouldAllocateStorage = false>
class OwnerFreeList : public OwnerFreeListBase<T, shouldAllocateStorage>{
public:
	OwnerFreeList(int hwm, int lwm) : OwnerFreeListBase<T, shouldAllocateStorage>(hwm, lwm){

	}

	virtual ~OwnerFreeList(){

	}
};

template <typename T, bool shouldAllocateStorage = false>
class OwnerFreeListVoidTypes : public OwnerFreeListBase<T, shouldAllocateStorage>{
public:
	OwnerFreeListVoidTypes(size_t size, int hwm, int lwm) : OwnerFreeListBase<T, shouldAllocateStorage>(size, hwm, lwm){

	}

	virtual ~OwnerFreeListVoidTypes(){

	}
};

template <bool shouldAllocateStorage>
class OwnerFreeList<void, shouldAllocateStorage> : public OwnerFreeListVoidTypes<void, shouldAllocateStorage>{
public:
	OwnerFreeList(size_t size, int hwm, int lwm) : OwnerFreeListVoidTypes<void, shouldAllocateStorage>(size, hwm, lwm){

	}

	virtual ~OwnerFreeList(){

	}
};

template <bool shouldAllocateStorage>
class OwnerFreeList<std::shared_ptr<void>, shouldAllocateStorage> : public OwnerFreeListVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>{
public:
	OwnerFreeList(size_t size, int hwm, int lwm) : OwnerFreeListVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>(size, hwm, lwm){

	}

	virtual ~OwnerFreeList(){

	}
};

template <typename T, bool shouldAllocateStorage = false>
class NonSyncFreeListBase : public FreeListI<T, shouldAllocateStorage>{
protected:
	MsgElement<T, shouldAllocateStorage> * _freeHead;
	MsgElement<T, shouldAllocateStorage> * _freeTail;
	int _freeListSize;
	int _highWaterMark;
	int _lowWaterMark;

	virtual MsgElement<T, shouldAllocateStorage>* allocNewElement(){
		return new MsgElement<T, shouldAllocateStorage>();
	}

public:

	NonSyncFreeListBase(int hwm, int lwm) : _highWaterMark(hwm), _lowWaterMark(lwm){
		_freeListSize = _highWaterMark;
		_freeHead = _freeTail = NULL;
	}

	virtual ~NonSyncFreeListBase(){
		MsgElement<T, shouldAllocateStorage>* curr = _freeHead;
		MsgElement<T, shouldAllocateStorage>* tmp;
		while(curr){
			tmp = const_cast<MsgElement<T, shouldAllocateStorage>*>(curr->_next);
			delete curr;
			curr = tmp;
		}
	}

	void init(){
		MsgElement<T, shouldAllocateStorage>* msg = allocNewElement();
		_freeHead = msg;
		_freeTail = msg;

		for(int i = 0 ; i < _highWaterMark ; ++i){
			msg = allocNewElement();
			_freeTail->_next = msg;
			_freeTail = msg;

		}
	}

	virtual MsgElement<T, shouldAllocateStorage>* getElement(){
		if(_freeListSize > _lowWaterMark){
			MsgElement<T, shouldAllocateStorage>* msg;
			msg = _freeHead;
			_freeHead = const_cast<MsgElement<T, shouldAllocateStorage>*>(msg->_next);
			msg->_next = 0;
			--_freeListSize;
			return msg;
		}else{
			return allocNewElement();
		}
	}

	virtual void releaseElement(MsgElement<T, shouldAllocateStorage>* elem){
		if(_freeListSize < _highWaterMark){
			_freeTail->_next = elem;
			_freeTail = elem;
			elem->_next = 0;
			++_freeListSize;
		}else{
			delete elem;
		}
	}

	virtual typename MsgElement<T, shouldAllocateStorage>::ContainedType alloc(){
		if(shouldAllocateStorage){
			MsgElement<T, shouldAllocateStorage>* elem = getElement();
			return elem->_value;
		}else{
			throw std::runtime_error(" Can not use this to alloc if allocation set to false. ");
		}
	}

	virtual void release(typename MsgElement<T, shouldAllocateStorage>::ContainedType e){
		if(shouldAllocateStorage){
			MsgElement<T, shouldAllocateStorage>* elem =
					FreeListReleaseDiscriminator<T, shouldAllocateStorage>::getElement(e);
			releaseElement(elem);
		}else{
			throw std::runtime_error(" Can not use this to release if allocation set to false. ");
		}
	}
};


template <typename T,  bool shouldAllocateStorage = false>
class NonSyncFreeList : public NonSyncFreeListBase<T, shouldAllocateStorage>{
public:
	NonSyncFreeList(int hwm, int lwm) : NonSyncFreeListBase<T, shouldAllocateStorage>(hwm, lwm){
		this->init();
	}

	virtual ~NonSyncFreeList(){

	}
};

template <typename T,  bool shouldAllocateStorage = false>
class NonSyncFreeListVoidTypes : public NonSyncFreeListBase<T, shouldAllocateStorage>{
protected:
	size_t _allocSize;

	virtual MsgElement<T, shouldAllocateStorage>* allocNewElement(){
		return new MsgElement<T, shouldAllocateStorage>(_allocSize);
	}

public:

	NonSyncFreeListVoidTypes(size_t size, int hwm, int lwm) : NonSyncFreeListBase<T, shouldAllocateStorage>(hwm, lwm),
	_allocSize(size){
		this->init();
	}

	virtual ~NonSyncFreeListVoidTypes(){

	}

	virtual typename MsgElement<T, shouldAllocateStorage>::ContainedType alloc(){
		if(_allocSize > 0){
			MsgElement<T, shouldAllocateStorage>* elem = this->getElement();
			return elem->_value;
		}else{
			throw std::runtime_error(" Can not use this to alloc if allocation size <= 0");
		}
	}

	virtual void release(typename MsgElement<T, shouldAllocateStorage>::ContainedType e){
		if(_allocSize > 0){
			MsgElement<T, shouldAllocateStorage>* elem =
					FreeListReleaseDiscriminator<T, shouldAllocateStorage>::getElement(e);
			this->releaseElement(elem);
		}else{
			throw std::runtime_error(" Can not use this to release if allocation size <= 0");
		}
	}
};

template<bool shouldAllocateStorage>
class NonSyncFreeList<void, shouldAllocateStorage> : public NonSyncFreeListVoidTypes<void, shouldAllocateStorage>{
public:
	NonSyncFreeList(size_t size, int hwm, int lwm) : NonSyncFreeListVoidTypes<void, shouldAllocateStorage>(size, hwm, lwm){

	}

	virtual ~NonSyncFreeList(){

	}
};

template<bool shouldAllocateStorage>
class NonSyncFreeList<std::shared_ptr<void>, shouldAllocateStorage> : public NonSyncFreeListVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>{
public:
	NonSyncFreeList(size_t size, int hwm, int lwm) : NonSyncFreeListVoidTypes<std::shared_ptr<void>, shouldAllocateStorage>(size, hwm, lwm){

	}

	virtual ~NonSyncFreeList(){

	}
};

//BEGIN_POLICY_DEFAULTS(Allocator)
//DECLARE_PRIMITIVE_DEFAULT(Allocator, SHOULDALLOCATESTORAGE, false)
//DECLARE_PRIMITIVE_DEFAULT(Allocator, SYNCHRONIZED, true)
//DECLARE_PRIMITIVE_DEFAULT(Allocator, OWNERFREED, false)
//DECLARE_PRIMITIVE_DEFAULT(Allocator, HIGHWATERMARK, 1000)
//DECLARE_PRIMITIVE_DEFAULT(Allocator, LOWWATERMARK, 100)
//END_POLICY_DEFAULTS(Allocator)

class AllocatorDefaultPolicies{
public:
	enum { SHOULDALLOCATESTORAGE = false };
	enum { SYNCHRONIZED = true };
	enum { OWNERFREED = false };
	enum { HIGHWATERMARK = 1000 };
	enum { LOWWATERMARK = 100 };
};
class AllocatorDefaultPolicyArgs : virtual public AllocatorDefaultPolicies{
};


DECLARE_PRIMITIVE_POLICY(Allocator, SHOULDALLOCATESTORAGE, bool)
DECLARE_PRIMITIVE_POLICY(Allocator, SYNCHRONIZED, bool)
DECLARE_PRIMITIVE_POLICY(Allocator, OWNERFREED, bool)
DECLARE_PRIMITIVE_POLICY(Allocator, HIGHWATERMARK, int)
DECLARE_PRIMITIVE_POLICY(Allocator, LOWWATERMARK, int)



template <typename T, typename PolicySetter1 = AllocatorDefaultPolicyArgs,
typename PolicySetter2 = AllocatorDefaultPolicyArgs,
typename PolicySetter3 = AllocatorDefaultPolicyArgs>
class DummyFreeListAllocator
{
public:
	typedef PolicySelector3<PolicySetter1, PolicySetter3, PolicySetter3> Policies;

	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };

	static FreeListI<T,SHOULDALLOCATESTORAGE>* getFreeList(int hwm = 0, int lwm = 0){
		return new DummyFreeList<T, SHOULDALLOCATESTORAGE>(HIGHWATERMARK, LOWWATERMARK);
	}

	static void returnFreeList(FreeListI<T, SHOULDALLOCATESTORAGE>* list){
		delete list;
	}
};

template <typename PolicySetter1,
typename PolicySetter2,
typename PolicySetter3>
class DummyFreeListAllocator<void, PolicySetter1, PolicySetter2, PolicySetter3>
{
public:
	typedef PolicySelector3<PolicySetter1, PolicySetter3, PolicySetter3> Policies;

	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };

	static FreeListI<void>* getFreeList(size_t size = 0, int hwm = 0, int lwm = 0){
		return new DummyFreeList<void>(size, HIGHWATERMARK, LOWWATERMARK);
	}

	static void returnFreeList(FreeListI<void>* list){
		delete list;
	}
};


template <typename PolicySetter1,
typename PolicySetter2,
typename PolicySetter3>
class DummyFreeListAllocator<std::shared_ptr<void>, PolicySetter1, PolicySetter2, PolicySetter3>
{
public:
	typedef PolicySelector3<PolicySetter1, PolicySetter3, PolicySetter3> Policies;

	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };

	static FreeListI<std::shared_ptr<void>>* getFreeList(size_t size = 0, int hwm = 0, int lwm = 0){
		return new DummyFreeList<std::shared_ptr<void>>(size, HIGHWATERMARK, LOWWATERMARK);
	}

	static void returnFreeList(FreeListI<std::shared_ptr<void>>* list){
		delete list;
	}
};





}


#endif /* COMMON_INCLUDE_CONTAINERS_ALLOCATOR_HPP_ */
