/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

#include <util/module.h>

global util_state *UtilState;

#include <util/mem.c>
#include <util/str.c>
#include <util/json.c>
#include <util/math/rand.c>
#include <util/math/bit.c>
#include <util/math/v1.c>
#include <util/math/v2.c>
#include <util/math/v3.c>
#include <util/math/v4.c>
#include <util/math/m3x3.c>
#include <util/math/m4x4.c>

external void
Util_Load(platform_exports *PlatformExports,
          util_module *UtilModule)
{
    UtilState = &UtilModule->State;
    
    if(!UtilModule->Loaded)
    {
        #define FUNC(ReturnType, Name, ...) UtilModule->Exports.Name = Name;
        UTIL__EXPORTS
        #undef FUNC
        
        UtilModule->Loaded = TRUE;
    }
    
    if(PlatformExports)
    {
        #define FUNC(_0,Name,...)   Name = PlatformExports->Name;
        #define PROC(_0,_1,_2,Name,...) Name = PlatformExports->Name;
        PLATFORM__EXPORTS
        PLATFORM__PROCS
        #undef FUNC
        #undef PROC
    }
}