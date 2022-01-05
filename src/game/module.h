/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_MODULE_H_
#define GAME_MODULE_H_

#include <platform/module.h>
#include <util/module.h>

#define GAME__MODULE__EXPORTS \
    FUNC(void, Game_Load, platform_exports *PlatformExports, util_exports *UtilExports, game_module *GameModule) \

#define _GAME__EXPORTS \
    GAME__FILE__EXPORTS \
    GAME__GAME__EXPORTS \
    GAME__MESH__EXPORTS \
    GAME__UI__EXPORTS \

#define GAME__EXPORTS \
    _GAME__EXPORTS \
    GAME__MODULE__EXPORTS \

#define GAME__FUNCS \
    _GAME__EXPORTS \
    GAME__FILE__FUNCS \
    GAME__GAME__FUNCS \
    GAME__MESH__FUNCS \
    GAME__UI__FUNCS \

#define FUNC(ReturnType, Name, ...) \
    typedef _API_GAME ReturnType _type__##Name(__VA_ARGS__);
GAME__EXPORTS
#undef FUNC

struct game_exports
{
    #define FUNC(ReturnType, Name, ...) \
        _type__##Name *Name;
    GAME__EXPORTS
    #undef FUNC
};
struct game_module
{
    game_exports Exports;
    game_state State;
    game_input Input;
    b08 Loaded;
};

#if _MODULE_GAME
    #define FUNC(ReturnType, Name, ...) \
        internal ReturnType Name(__VA_ARGS__);
    GAME__FUNCS
    #undef FUNC
    
    #define FUNC(ReturnType, Name, ...) \
        external _API_EXPORT ReturnType Name(__VA_ARGS__);
    GAME__MODULE__EXPORTS
    #undef FUNC
#else
    #define FUNC(ReturnType, Name, ...) \
        global _type__##Name *Name;
    GAME__EXPORTS
    #undef FUNC
#endif

#ifndef MEMSET_DEFINED
    #define MEMSET_DEFINED
    #pragma function(memset)
    external vptr memset(vptr Dest, s32 Data, u64 Size) { return Mem_Set(Dest, Data, Size); }
#endif

#endif