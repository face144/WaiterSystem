#pragma once

#ifdef WAITERENGINE_API_EXPORTS
#define WAITERENGINE_API __declspec(dllexport)
#else
#define WAITERENGINE_API __declspec(dllimport)
#endif

typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;