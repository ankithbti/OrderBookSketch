/*
 * ReducedContentionAllocator.hpp
 *
 *  Created on: 10-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_REDUCEDCONTENTIONALLOCATOR_HPP_
#define COMMON_INCLUDE_CONTAINERS_REDUCEDCONTENTIONALLOCATOR_HPP_

#include <Common.hpp>
#include <containers/allocator.hpp>

namespace obLib{

template<typename T,
typename PolicySetter1 = AllocatorDefaultPolicyArgs,
typename PolicySetter2 = AllocatorDefaultPolicyArgs,
typename PolicySetter3 = AllocatorDefaultPolicyArgs,
typename PolicySetter4 = AllocatorDefaultPolicyArgs,
typename PolicySetter5 = AllocatorDefaultPolicyArgs>
class ReducedContentionAllocator{

public:
	typedef PolicySelector5<PolicySetter1,PolicySetter2, PolicySetter3, PolicySetter4, PolicySetter5> Policies;

	enum { SYNCHRONIZED = Policies::SYNCHRONIZED };
	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };
	enum { OWNERFREED = Policies::OWNERFREED };

	static FreeListI<T, SHOULDALLOCATESTORAGE>* getFreeList(int hwm = 0, int lwm = 0){
		if(SYNCHRONIZED){
			if(OWNERFREED){
				if((hwm > 0) && (lwm > 0)){
					return new OwnerFreeList<T, SHOULDALLOCATESTORAGE>(hwm, lwm);
				}else{
					return OwnerFreeList<T, SHOULDALLOCATESTORAGE>(HIGHWATERMARK, LOWWATERMARK);
				}
			}else{
				if((hwm > 0) && (lwm > 0)){
					return new FreeList<T, SHOULDALLOCATESTORAGE>(hwm, lwm);
				}else{
					return new FreeList<T, SHOULDALLOCATESTORAGE>(HIGHWATERMARK, LOWWATERMARK);
				}
			}
		}else{
			if((hwm > 0) && (lwm > 0)){
				return new NonSyncFreeList<T, SHOULDALLOCATESTORAGE>(hwm, lwm);
			}else{
				return new NonSyncFreeList<T, SHOULDALLOCATESTORAGE>(HIGHWATERMARK, LOWWATERMARK);
			}
		}
	}

	static void returnFreeList(FreeListI<T, SHOULDALLOCATESTORAGE>* list){
		delete list;
	}
};


// void template specialization
template<
typename PolicySetter1,
typename PolicySetter2,
typename PolicySetter3,
typename PolicySetter4,
typename PolicySetter5>
class ReducedContentionAllocator<void, PolicySetter1, PolicySetter2, PolicySetter3, PolicySetter4, PolicySetter5>{
public:
	typedef PolicySelector5<PolicySetter1,PolicySetter2, PolicySetter3, PolicySetter4, PolicySetter5> Policies;

	enum { SYNCHRONIZED = Policies::SYNCHRONIZED };
	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };
	enum { OWNERFREED = Policies::OWNERFREED };

	static FreeListI<void>* getFreeList(size_t size  = 0, int hwm = 0, int lwm = 0){
		if(SYNCHRONIZED){
			if(OWNERFREED){
				if((hwm > 0) && (lwm > 0)){
					return new OwnerFreeList<void, false>(size, hwm, lwm);
				}else{
					return new OwnerFreeList<void, false>(size, HIGHWATERMARK, LOWWATERMARK);
				}
			}else{
				if((hwm > 0) && (lwm > 0)){
					return new FreeList<void, false>(size, hwm, lwm);
				}else{
					return new FreeList<void, false>(size, HIGHWATERMARK, LOWWATERMARK);
				}
			}
		}else{
			if((hwm > 0) && (lwm > 0)){
				return new NonSyncFreeList<void, false>(size, hwm, lwm);
			}else{
				return new NonSyncFreeList<void, false>(size, HIGHWATERMARK, LOWWATERMARK);
			}
		}
	}

	static void returnFreeList(FreeListI<void>* list){
		delete list;
	}
};

// std::shared_ptr<void> template specialization
template<
typename PolicySetter1,
typename PolicySetter2,
typename PolicySetter3,
typename PolicySetter4,
typename PolicySetter5>
class ReducedContentionAllocator<std::shared_ptr<void>, PolicySetter1, PolicySetter2, PolicySetter3, PolicySetter4, PolicySetter5>{
public:
	typedef PolicySelector5<PolicySetter1,PolicySetter2, PolicySetter3, PolicySetter4, PolicySetter5> Policies;

	enum { SYNCHRONIZED = Policies::SYNCHRONIZED };
	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };
	enum { OWNERFREED = Policies::OWNERFREED };

	static FreeListI<std::shared_ptr<void>>* getFreeList(size_t size  = 0, int hwm = 0, int lwm = 0){
		if(SYNCHRONIZED){
			if(OWNERFREED){
				if((hwm > 0) && (lwm > 0)){
					return new OwnerFreeList<std::shared_ptr<void>, false>(size, hwm, lwm);
				}else{
					return new OwnerFreeList<std::shared_ptr<void>, false >(size, HIGHWATERMARK, LOWWATERMARK);
				}
			}else{
				if((hwm > 0) && (lwm > 0)){
					return new FreeList<std::shared_ptr<void>, false>(size, hwm, lwm);
				}else{
					return new FreeList<std::shared_ptr<void>, false>(size, HIGHWATERMARK, LOWWATERMARK);
				}
			}
		}else{
			if((hwm > 0) && (lwm > 0)){
				return new NonSyncFreeList<std::shared_ptr<void>, false >(size, hwm, lwm);
			}else{
				return new NonSyncFreeList<std::shared_ptr<void>, false >(size, HIGHWATERMARK, LOWWATERMARK);
			}
		}
	}

	static void returnFreeList(FreeListI<std::shared_ptr<void> >* list){
		delete list;
	}
};

}



#endif /* COMMON_INCLUDE_CONTAINERS_REDUCEDCONTENTIONALLOCATOR_HPP_ */
