/*
 * NSEMessage.hpp
 *
 *  Created on: 16-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_NSEMESSAGE_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_NSEMESSAGE_HPP_


#include <Common.hpp>
#include <IExchangeMessage.hpp>

namespace oms{

class NSEMessage : public IExchangeMessage{

	int _msgtType;
	int _errCode;
	size_t _msgSize;

public:
	NSEMessage(const char* msg, size_t msgSize);
	virtual ~NSEMessage();

	std::ostream& operator<<(std::ostream& oss) const;

	inline  int getMsgType() const {
		return _msgtType;
	}

	inline int getErrCode() const{
		return _errCode;
	}
};

using NSEMessagePtr = std::shared_ptr<NSEMessage>;

class NSEOrderAck : public IOrderAck{
	NSEMessagePtr _msg;
public:
	NSEOrderAck(const NSEMessagePtr& msg)  : _msg(msg) {

	}

	virtual ~NSEOrderAck(){

	}

	virtual std::string getClientOrderId() const{
		return std::string();
	}
	virtual std::string getExchangeOrderId() const{
		return std::string();
	}
};
using NSEOrderAckPtr = std::shared_ptr<NSEOrderAck>;

class NSEOrderReject : public IOrderReject{
	NSEMessagePtr _msg;
public:
	NSEOrderReject(const NSEMessagePtr& msg)  : _msg(msg) {

	}

	virtual ~NSEOrderReject(){

	}

	virtual std::string getClientOrderId(){
		return std::string();
	}

	virtual std::string getExchangeOrderId() {
		return std::string();
	}

};
using NSEOrderRejectPtr = std::shared_ptr<NSEOrderReject>;


class NSETrade : public ITrade{
	NSEMessagePtr _msg;
public:
	NSETrade(const NSEMessagePtr& msg)  : _msg(msg) {

	}

	virtual ~NSETrade(){

	}

	virtual std::string getClientOrderId() const{
		return std::string();
	}

	virtual std::string getExchangeOrderId() const {
		return std::string();
	}

	virtual std::string getExchangeTradeRef() const {
		return std::string();
	}
	virtual std::string getAccount() const {
		return std::string();
	}
	virtual std::string getExchangeTradeTime() const {
		return std::string();
	}
	virtual std::string getExchangeProductId() const {
		return std::string();
	}
	virtual char getSide() const {
		return '1';
	}
	virtual double getPrice() const {
		return 0.0;
	}
	virtual int getQty() const {
		return 0;
	}
	virtual char getExecType() const {
		return 'F';
	}

};
using NSETradePtr = std::shared_ptr<NSETrade>;

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_NSEMESSAGE_HPP_ */
