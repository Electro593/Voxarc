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

#if !defined(GAME_UTIL_MATH_VOX_S32_H_)
#define      GAME_UTIL_MATH_VOX_S32_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define S32_BITS 32

#define S32_BIT_SIGN 31

#define S32_SHIFT_SIGN 31

#define S32_MIN 0x80000000 // -2147483648
#define S32_MAX 0x7FFFFFFF // 2147483647

#define S32_MAX_DIGITS 12


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME_UTIL_MATH__S32__EXPORTS \

#define GAME_UTIL_MATH__S32__FUNCS \
    PROC(s32, S32_Sign, s32 N) \
    PROC(s32, S32_Lerp, s32 A, s32 B, r32 Percent) \
    PROC(s32, S32_Abs, s32 N) \
    PROC(str, S32_ToStr, str *Dest, s32 N) \


#endif