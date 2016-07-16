/*
 * LocalBuffer.hpp
 *
 *  Created on: 16-Jul-2016
 *      Author: ankithbti
 */

#ifndef ORDERMANAGEMENTSYSTEM_INCLUDE_LOCALBUFFER_HPP_
#define ORDERMANAGEMENTSYSTEM_INCLUDE_LOCALBUFFER_HPP_


#include <Common.hpp>
#include <ByteConverter.hpp>

namespace oms{

struct CharData{
	virtual ~CharData() {

	}
	virtual char operator[](int index) const = 0;
};

template<typename T>
struct BufferAdaptor : public CharData {
	char * _ptr;
	BufferAdaptor(const T* t) {
		_ptr = (char*) t;
	}

	~BufferAdaptor(){

	}

	virtual char operator[](int index) const{
		return _ptr[index];
	}
};

template<typename T, typename CharData>
T& dumpHexBase(T& out, int bytes, const CharData& ptr, int maxSize = 4096, const int PERLINE=16){
	const std::string LINE_SEPARATOR =  "	";
	if(bytes > maxSize){
		bytes = maxSize;
	}
	int i = 0;
	int index = 1;
	std::ostringstream asciiStr;

	for(i = 0 , index = 1 ; i < bytes ; ++i, ++index){
		// Hex Part
		{
			std::ostringstream str;
			str << std::setw(2) << std::setfill('0') << std::right << std::hex << (int)(ptr[i]&0xFF) << " ";
			out << str.str();
		}
		// ascii part
		asciiStr << (char) (isprint(ptr[i]) ? ptr[i] : '.');

		// print ascii only at the end of the line
		if( (index % PERLINE == 0) && i!= 0){
			out << LINE_SEPARATOR << asciiStr.str() << "\n";
			asciiStr.str("");
		}
	}


	// in case of last line is incomplete insert space before ascii format
	for(index-- ; (index%PERLINE) != 0 ; index++){
		out << "   "; // width of hex entry with space
	}
	out << LINE_SEPARATOR << asciiStr.str() << "\n";
	return out;
}

template<typename T, typename CharData>
T& dumpHex(T& out, int bytes, const CharData& ptr, int maxSize = 4096, const int PERLINE=16){
	return dumpHexBase(out, bytes, ptr, maxSize, PERLINE);
}

template<typename T, char*>
T& dumpHex(T& out, int bytes, const char*& ptr, int maxSize = 4096, const int PERLINE=16){
	return dumpHexBase(out, bytes, BufferAdaptor<const char>(ptr), maxSize, PERLINE);
}

template<typename T, unsigned char*>
T& dumpHex(T& out, int bytes, const unsigned char*& ptr, int maxSize = 4096, const int PERLINE=16){
	return dumpHexBase(out, bytes, BufferAdaptor<const unsigned char>(ptr), maxSize, PERLINE);
}

template<typename T, int*>
T& dumpHex(T& out, int bytes, const int*& ptr, int maxSize = 4096, const int PERLINE=16){
	return dumpHexBase(out, bytes, BufferAdaptor<const int>(ptr), maxSize, PERLINE);
}

template<typename T, unsigned int*>
T& dumpHex(T& out, int bytes, const unsigned int*& ptr, int maxSize = 4096, const int PERLINE=16){
	return dumpHexBase(out, bytes, BufferAdaptor<const unsigned int>(ptr), maxSize, PERLINE);
}


class LocalBuffer{
	char * _buffer;
	size_t _bufferSize;
	size_t _positionInBuffer;
	obLib::ByteOrderConverter::ByteOrder _endianType;

public:
	LocalBuffer(const size_t& size, obLib::ByteOrderConverter::ByteOrder byteOrder) :
		_buffer(NULL), _bufferSize(size), _positionInBuffer(0), _endianType(byteOrder){
		_buffer = (char*)malloc(_bufferSize*sizeof(char));
		if(_buffer == NULL){
			throw std::runtime_error("LocalBuffer::LocalBuffer() - Allocation failed to buffer.");
		}
		memset((void*)_buffer, 0, _bufferSize);
	}
	~LocalBuffer(){
		if(_buffer){
			free(_buffer);
		}
	}
	bool put8(const char& byte){
		if(_positionInBuffer < _bufferSize){
			_buffer[_positionInBuffer] = byte;
			++_positionInBuffer;
			return true;
		}
		return false;
	}
	bool put8(const unsigned char& byte){
		if(_positionInBuffer < _bufferSize){
			_buffer[_positionInBuffer] = byte;
			++_positionInBuffer;
			return true;
		}
		return false;
	}

	size_t getPosition() const {
		return _positionInBuffer;
	}

	void setPosition(size_t pos){
		if(pos <= _bufferSize){
			_positionInBuffer = pos;
			return;
		}
		throw std::runtime_error("setPosition() - Can not set position beyon size of Buffer.");
	}

	bool get8(unsigned char& byte){
		if(_positionInBuffer < _bufferSize){
			byte = _buffer[_positionInBuffer];
			++_positionInBuffer;
			return true;
		}
		return false;
	}

	bool get8(char& byte){
		if(_positionInBuffer < _bufferSize){
			byte = _buffer[_positionInBuffer];
			++_positionInBuffer;
			return true;
		}
		return false;
	}

	void toStream(std::ostream& out, size_t maxSize = 4096) const{
		dumpHex(out, _bufferSize, _buffer);
	}

	char* getBuffer(){
		return _buffer;
	}

};
using LocalBufferPtr = std::shared_ptr<LocalBuffer>;

}

#endif /* ORDERMANAGEMENTSYSTEM_INCLUDE_LOCALBUFFER_HPP_ */
