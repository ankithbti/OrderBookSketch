/*
 * SeqNumStore.hpp
 *
 *  Created on: 15-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_SEQNUMSTORE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_SEQNUMSTORE_HPP_


#include <Common.hpp>
#include <util/MemoryMapped.hpp>

namespace oms{

template <typename T>
class SeqNumStore{
	enum{
		DEFAULT_MSG_SEQNO_LEN = 9
	};

	obLib::MemoryMapped::SharedPtr _inSeqNumMM;
	obLib::MemoryMapped::SharedPtr _outSeqNumMM;
	unsigned long _nextInSeqNum;
	unsigned long _nextOutSeqNum;
	int _seqNoLen;
	char * _inSeqNumBuff;
	char * _outSeqNumBuff;

protected:
	inline void storeInSeqNum(){
		obLib::fast_i2a(_inSeqNumBuff, _seqNoLen, _nextInSeqNum);
	}
	inline void storeOutSeqNum(){
		obLib::fast_i2a(_outSeqNumBuff, _seqNoLen, _nextOutSeqNum);
	}
public:
	SeqNumStore(unsigned long inSeqNo = 1, unsigned long outSeqNo = 1) :
		_nextInSeqNum(inSeqNo),
		_nextOutSeqNum(outSeqNo),
		_seqNoLen(DEFAULT_MSG_SEQNO_LEN),
		_inSeqNumBuff(NULL),
		_outSeqNumBuff(NULL){

	}

	void initStore(const std::string& configStr){

	}

	inline unsigned long getAndIncrNextOutSeqNum(){
		unsigned long currOutSeqNo = _nextOutSeqNum++;
		storeOutSeqNum();
		return currOutSeqNo;
	}

	inline unsigned long getNextOutSeqNum() const{
		return _nextOutSeqNum;
	}

	inline void setNextOutSeqNum(unsigned long seqNo){
		_nextOutSeqNum = seqNo;
		storeOutSeqNum();
	}

	inline unsigned long incrInSeqNum(){
		++_nextInSeqNum;
		storeInSeqNum();
	}

	inline unsigned long getNextInSeqNum() const{
		return _nextInSeqNum;
	}

	inline void setNextInSeqNum(unsigned long seqNo){
		_nextInSeqNum = seqNo;
		storeInSeqNum();
	}

	inline int getSeqNoLen() const {
		return _seqNoLen;
	}
};

}


#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_SEQNUMSTORE_HPP_ */


