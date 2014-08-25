#ifndef TYPE_DEF_H
#define TYPE_DEF_H

typedef char char8;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#if (defined _WIN32 || defined __int64)
typedef signed __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef signed long long int64;
typedef unsigned long long uint64;
#endif

#if defined(DEBUG) || defined(_DEBUG)
	#define static_assert(condition, name) typedef int static_assert_##name [condition]
#else
	#define static_assert(condition, name)
#endif


#endif
