/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
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

#if !defined(GAME_VOX_MODULE_H_)
#define      GAME_VOX_MODULE_H_


#define GAME__MODULE__EXPORTS \
    PROC(void, Game_Unload, game_exports *Functions, game_state *State) \
    PROC(void, Game_Load, platform_callbacks *PlatformCallbacks, game_exports *Functions, game_state *State) \


//TODO: Condense the math files down
#define GAME_UTIL_MATH__EXPORTS \
    GAME_UTIL_MATH__BIT__EXPORTS \
    GAME_UTIL_MATH__M3X3__EXPORTS \
    GAME_UTIL_MATH__M4X4__EXPORTS \
    GAME_UTIL_MATH__R32__EXPORTS \
    GAME_UTIL_MATH__RAND__EXPORTS \
    GAME_UTIL_MATH__S32__EXPORTS \
    GAME_UTIL_MATH__U32__EXPORTS \
    GAME_UTIL_MATH__U64__EXPORTS \
    GAME_UTIL_MATH__V2__EXPORTS \
    GAME_UTIL_MATH__V3__EXPORTS \
    GAME_UTIL_MATH__V4__EXPORTS \

#define GAME_UTIL_MATH__FUNCS \
    GAME_UTIL_MATH__BIT__FUNCS \
    GAME_UTIL_MATH__M3X3__FUNCS \
    GAME_UTIL_MATH__M4X4__FUNCS \
    GAME_UTIL_MATH__R32__FUNCS \
    GAME_UTIL_MATH__RAND__FUNCS \
    GAME_UTIL_MATH__S32__FUNCS \
    GAME_UTIL_MATH__U32__FUNCS \
    GAME_UTIL_MATH__U64__FUNCS \
    GAME_UTIL_MATH__V2__FUNCS \
    GAME_UTIL_MATH__V3__FUNCS \
    GAME_UTIL_MATH__V4__FUNCS \


#define GAME_UTIL__EXPORTS \
    GAME_UTIL_MATH__EXPORTS \
    \
    GAME_UTIL__JSON__EXPORTS \
    GAME_UTIL__MEM__EXPORTS \
    GAME_UTIL__STR__EXPORTS \

#define GAME_UTIL__FUNCS \
    GAME_UTIL_MATH__FUNCS \
    \
    GAME_UTIL__JSON__FUNCS \
    GAME_UTIL__MEM__FUNCS \
    GAME_UTIL__STR__FUNCS \


#define GAME__EXPORTS \
    GAME_UTIL__EXPORTS \
    \
    GAME__FILE__EXPORTS \
    GAME__GAME__EXPORTS \
    GAME__MESH__EXPORTS \
    GAME__UI__EXPORTS \

#define GAME__FUNCS \
    GAME__EXPORTS \
    \
    GAME_UTIL__FUNCS \
    \
    GAME__FILE__FUNCS \
    GAME__GAME__FUNCS \
    GAME__MESH__FUNCS \
    GAME__UI__FUNCS \


#define PROC(ReturnType, Name, ...) \
    typedef _VOX_GAME_API ReturnType _type__##Name(__VA_ARGS__);
GAME__EXPORTS
GAME__MODULE__EXPORTS
#undef PROC

struct game_exports
{
    #define PROC(ReturnType, Name, ...) \
        _type__##Name *Name;
    GAME__EXPORTS
    GAME__MODULE__EXPORTS
    #undef PROC
};



#if _VOX_MODULE == _VOX_MODULE_GAME
    
    #define PROC(ReturnType, Name, ...) \
        internal ReturnType Name(__VA_ARGS__);
    GAME__FUNCS
    #undef PROC
    
    #define PROC(ReturnType, Name, ...) \
        external _API_EXPORT ReturnType Name(__VA_ARGS__);
    GAME__MODULE__EXPORTS
    #undef PROC
    
#else
    
    #define PROC(ReturnType, Name, ...) \
        global_var _type__##Name *Name;
    GAME__EXPORTS
    GAME__MODULE__EXPORTS
    #undef PROC
    
#endif

#endif