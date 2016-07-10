/*
 * ReducedMemUsageAllocator.hpp
 *
 *  Created on: 10-Jul-2016
 *      Author: ankithbti
 */

#ifndef COMMON_INCLUDE_CONTAINERS_REDUCEDMEMUSAGEALLOCATOR_HPP_
#define COMMON_INCLUDE_CONTAINERS_REDUCEDMEMUSAGEALLOCATOR_HPP_

#include <Common.hpp>
#include <containers/allocator.hpp>

namespace obLib{

template<typename T,
typename PolicySetter1 = AllocatorDefaultPolicyArgs,
typename PolicySetter2 = AllocatorDefaultPolicyArgs,
typename PolicySetter3 = AllocatorDefaultPolicyArgs,
typename PolicySetter4 = AllocatorDefaultPolicyArgs>
class ReducedMemUsageAllocator{

public:
	typedef PolicySelector4<PolicySetter1,PolicySetter2, PolicySetter3, PolicySetter4> Policies;

	enum { SYNCHRONIZED = Policies::SYNCHRONIZED };
	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };

	static FreeListI<T, SHOULDALLOCATESTORAGE>* getFreeList(int hwm = 0, int lwm = 0){
		static FreeListI<T, SHOULDALLOCATESTORAGE>* _list  = 0;
		if(!_list){
			if((hwm > 0) && (lwm > 0)){
				_list = new FreeList<T, SHOULDALLOCATESTORAGE>(hwm, lwm);
			}else{
				_list = new FreeList<T, SHOULDALLOCATESTORAGE>(HIGHWATERMARK, LOWWATERMARK);
			}
		}
		return _list;
	}

	static void returnFreeList(FreeListI<T, SHOULDALLOCATESTORAGE>* list){
		// Do Nothing
	}
};

template<
typename PolicySetter1,
typename PolicySetter2,
typename PolicySetter3,
typename PolicySetter4>
class ReducedMemUsageAllocator<void, PolicySetter1, PolicySetter2, PolicySetter3, PolicySetter4>{

public:
	typedef PolicySelector4<PolicySetter1,PolicySetter2, PolicySetter3, PolicySetter4> Policies;

	enum { SYNCHRONIZED = Policies::SYNCHRONIZED };
	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };

	static FreeListI<void>* getFreeList(size_t size = 0, int hwm = 0, int lwm = 0){
		static std::map<int, FreeListI<void>*> m;
		std::map<int, FreeListI<void>*>::iterator it = m.find(size);
		if(it != m.end()){
			return it->second;
		}else{
			FreeListI<void>* list;
			if((hwm > 0) && (lwm > 0)){
				list = new FreeList<void, false>(size, hwm, lwm);
			}else{
				list = new FreeList<void, false>(size, HIGHWATERMARK, LOWWATERMARK);
			}
			m.insert(std::pair<int, FreeListI<void>*>(size, list));
			return list;
		}
	}

	static void returnFreeList(FreeListI<void>* list){
		// Nothing
	}
};

template<
typename PolicySetter1,
typename PolicySetter2,
typename PolicySetter3,
typename PolicySetter4>
class ReducedMemUsageAllocator<std::shared_ptr<void>, PolicySetter1, PolicySetter2, PolicySetter3, PolicySetter4>{

public:
	typedef PolicySelector4<PolicySetter1,PolicySetter2, PolicySetter3, PolicySetter4> Policies;

	enum { SYNCHRONIZED = Policies::SYNCHRONIZED };
	enum { SHOULDALLOCATESTORAGE = Policies::SHOULDALLOCATESTORAGE };
	enum { HIGHWATERMARK = Policies::HIGHWATERMARK };
	enum { LOWWATERMARK = Policies::LOWWATERMARK };

	static FreeListI<std::shared_ptr<void>>* getFreeList(size_t size = 0, int hwm = 0, int lwm = 0){
		static std::map<int, FreeListI<std::shared_ptr<void>>*> m;
		std::map<int, FreeListI<std::shared_ptr<void>>*>::iterator it = m.find(size);
		if(it != m.end()){
			return it->second;
		}else{
			FreeListI<std::shared_ptr<void>>* list;
			if((hwm > 0) && (lwm > 0)){
				list = new FreeList<std::shared_ptr<void>, false>(size, hwm, lwm);
			}else{
				list = new FreeList<std::shared_ptr<void>, false>(size, HIGHWATERMARK, LOWWATERMARK);
			}
			m.insert(std::pair<int, FreeListI<std::shared_ptr<void>>*>(size, list));
			return list;
		}
	}

	static void returnFreeList(FreeListI<std::shared_ptr<void>>* list){
		// Nothing
	}
};


}



#endif /* COMMON_INCLUDE_CONTAINERS_REDUCEDMEMUSAGEALLOCATOR_HPP_ */
