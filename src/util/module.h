/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
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