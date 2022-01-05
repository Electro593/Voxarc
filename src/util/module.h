/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_MODULE_H_
#define UTIL_MODULE_H_

#include <platform/module.h>

struct util_state
{
    random Rand;
    stack *Stack;
    
    heap *StrHeap;
    #if _MODE_DEBUG
        heap_data_DEBUG StrHeapDebugData;
    #endif
};

#define UTIL__MODULE__EXPORTS \
    FUNC(void, Util_Load, platform_exports *PlatformExports, util_module *UtilModule) \

#define _UTIL__EXPORTS \
    UTIL__MEM__EXPORTS \
    UTIL__STR__EXPORTS \
    UTIL__JSON__EXPORTS \
    UTIL_MATH__RAND__EXPORTS \
    UTIL_MATH__V1__EXPORTS \
    UTIL_MATH__V2__EXPORTS \
    UTIL_MATH__V3__EXPORTS \
    UTIL_MATH__V4__EXPORTS \
    UTIL_MATH__M3X3__EXPORTS \
    UTIL_MATH__M4X4__EXPORTS \
    UTIL_MATH__BIT__EXPORTS \

#define UTIL__EXPORTS \
    _UTIL__EXPORTS \
    UTIL__MODULE__EXPORTS \

#define UTIL__FUNCS \
    _UTIL__EXPORTS \
    UTIL__STR__FUNCS \
    UTIL__MEM__FUNCS \
    UTIL__JSON__FUNCS \
    UTIL_MATH__RAND__FUNCS \
    UTIL_MATH__V1__FUNCS \
    UTIL_MATH__V2__FUNCS \
    UTIL_MATH__V3__FUNCS \
    UTIL_MATH__V4__FUNCS \
    UTIL_MATH__M3X3__FUNCS \
    UTIL_MATH__M4X4__FUNCS \
    UTIL_MATH__BIT__FUNCS \

#define FUNC(ReturnType, Name, ...) \
    typedef _API_UTIL ReturnType _type__##Name(__VA_ARGS__);
UTIL__EXPORTS
#undef FUNC

struct util_exports
{
    #define FUNC(ReturnType, Name, ...) \
        _type__##Name *Name;
    UTIL__EXPORTS
    #undef FUNC
};
struct util_module
{
    util_exports Exports;
    util_state State;
    b08 Loaded;
};

#if _MODULE_UTIL
    #define FUNC(ReturnType, Name, ...) \
        internal ReturnType Name(__VA_ARGS__);
    UTIL__FUNCS
    #undef FUNC
    #define FUNC(ReturnType, Name, ...) \
        external _API_EXPORT ReturnType Name(__VA_ARGS__);
    UTIL__MODULE__EXPORTS
    #undef FUNC
#else
    #define FUNC(ReturnType, Name, ...) \
        global _type__##Name *Name;
    UTIL__EXPORTS
    #undef FUNC
#endif

#endif