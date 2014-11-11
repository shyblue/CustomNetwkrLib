// [STANDARD LIBRARY]

#ifndef PUBLIC_LIBRARY_H_
#define PUBLIC_LIBRARY_H_

#pragma warning( disable : 4996)

#include <WinSock2.h>
#include <Windows.h>
#include <cstdint>
#include <cstdlib>

#include <vector>
#include <string>
#include <sstream>


#ifdef WIN32
#define __PRETTY_FUNCTION__		__FUNCTION__

#if( _MSC_VER < 1600 ) // vc 10.0 이상일때만 정의 되어있음.
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef int64_t	INT64;
typedef uint64_t UINT64;
#endif // (_MSC_VER < 1600)
#endif // WIN32

// [BOOST LIBRARY]
#define _BOOST_SHREAD_PTR			1
#endif // PUBLIC_LIBRARY_H_


