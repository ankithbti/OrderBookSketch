/*
 * IOrderList.hpp
 *
 *  Created on: 12-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_IORDERPTRTYPE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_IORDERPTRTYPE_HPP_

#include <Common.hpp>
#include <IOrder.hpp>

namespace oms{

class BaseOrderManager;

template <typename T>
struct IOrderPtrRef{
private:
	using DeleterFunc = std::function<void (T*)>;
public:

	T* _order;
	DeleterFunc _deleter;

	explicit IOrderPtrRef(T* o, const DeleterFunc& d) : _order(o), _deleter(d){

	}

};
template<typename T>
class IOrderPtrType{

	using DeleterFunc = std::function<void (T*)>;
	T* _order;
	DeleterFunc _deleter;

	void operator == (const IOrderPtrType& ) const ;
	void operator != (const IOrderPtrType& ) const ;

	explicit IOrderPtrType(T* order, const DeleterFunc& deleter) : _order(order), _deleter(deleter){

	}

	friend class BaseOrderManager;

public:

	IOrderPtrType(const IOrderPtrType& u ) : _order(u.release()), _deleter(u._deleter){

	}

	~IOrderPtrType(){
		reset();
	}

	IOrderPtrType& operator=(const IOrderPtrType& u){
		reset(u.release());
		_deleter = u._deleter;
		return *this;
	}

	T* operator->() const {
		return _order;
	}

	T& operator*() const {
		return *_order;
	}

	T* get() const {
		return _order;
	}

	T* release(){
		T* p = get();
		_order = NULL;
		return p;
	}

	void reset(T* p = NULL){
		if(p != get()){
			if(_order){
				_deleter(_order);
			}
			_order = p;
		}
	}

	IOrderPtrType(IOrderPtrRef<T> ref ) : _order(ref._order), _deleter(ref._deleter){

	}

	IOrderPtrType& operator=(IOrderPtrRef<T> ref){
		reset(ref._order);
		_deleter = ref._deleter;
		return *this;
	}

};

}



#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_IORDERPTRTYPE_HPP_ */
