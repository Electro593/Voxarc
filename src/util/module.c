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