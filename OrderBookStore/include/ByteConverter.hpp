/*
 * ByteConverter.hpp
 *
 *  Created on: 09-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_BYTECONVERTER_HPP_
#define INCLUDE_BYTECONVERTER_HPP_


#include <Common.hpp>

namespace obLib
{
  struct ByteOrderConverter{
    enum ByteOrder {
      BIGENDIAN = 0x80,
      LITTLEENDIAN = 0x00
    };

    static inline short convertByteOrder(short val, ByteOrder order){
      return convert16bit(val, order);
    }

    static inline unsigned short convertByteOrder(unsigned short val, ByteOrder order){
      short tmp = convert16bit(*(short*)&val, order);
      return *((unsigned short*)(&tmp));
    }

    static inline int convertByteOrder(int val, ByteOrder order){
      return convert32bit(val, order);
    }

    static inline unsigned int convertByteOrder(unsigned int val, ByteOrder order){
      int tmp = convert32bit(*(int*)&val, order);
      return *((unsigned int*)(&tmp));
    }

    static inline long long convertByteOrder(long long val, ByteOrder order){
      return convert64bit(val, order);
    }

    static inline unsigned long long convertByteOrder(unsigned long long val, ByteOrder order){
      return (unsigned long long)convert64bit(*(long long*)&val, order);
    }

    static inline float convertByteOrder(const float& val, ByteOrder order){
      int tmp = convert32bit(*(int*)&val, order);
      return *((float*)(&tmp));
    }

    static inline double convertByteOrder(const double& val, ByteOrder order){
      long long tmp = convert64bit(*(long long*)&val, order);
      return *((double*)(&tmp));
    }

    static short convert16bit(short val, ByteOrder order){
      if(order == LITTLEENDIAN){
	  return val;
      }else{
	  // reverse the byte order
	  __asm__(
	      "xchg %b0, %h0"
	      : "=a"(val)
		: "0"(val)
	  );
	  return val;
      }

    }

    static int convert32bit(short val, ByteOrder order){
      if(order == LITTLEENDIAN){
	  return val;
      }else{
	  // reverse the byte order
	  __asm__(
	      "bswap %0"
	      : "=r"(val)
		: "0"(val)
	  );
	  return val;
      }
    }

    static long long convert64bit(short val, ByteOrder order){
      if(order == LITTLEENDIAN){
	  return val;
      }else{
	  union{
	    long long ll;
	    int i[2];
	  }u;

	  u.ll = val;

	  __asm__ (
	      "bswap %0\n"
	      "bswap %1\n"
	      : "=a"(u.i[0]), "=d"(u.i[1])
		: "1"(u.i[0]), "0"(u.i[1])
	  );

	  return u.ll;
      }

    }


    static short convert16bitSlow(short val, ByteOrder order){
      if(order == LITTLEENDIAN){
	  return val;
      }else{
	  union{
	    short s;
	    unsigned char c[2];
	  }u, u2;

	  u.s = val;
	  u2.c[0] = u.c[1];
	  u2.c[1] = u.c[0];

	  return u2.s;
      }

    }

    static int convert32bitSlow(short val, ByteOrder order){
      if(order == LITTLEENDIAN){
	  return val;
      }else{
	  union{
	    int s;
	    unsigned char c[4];
	  }u, u2;

	  u.s = val;
	  u2.c[0] = u.c[3];
	  u2.c[1] = u.c[2];
	  u2.c[2] = u.c[1];
	  u2.c[3] = u.c[0];

	  return u2.s;
      }

    }
    static long long convert64bitSlow(short val, ByteOrder order);


  };
}

#endif /* INCLUDE_BYTECONVERTER_HPP_ */
