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

#if !defined(GAME_UTIL_MATH_VOX_M3X3_H_)
#define      GAME_UTIL_MATH_VOX_M3X3_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: m3x3r32

typedef union m3x3r32
{
    v3r32 V[3];
    r32 M[3][3];
    r32 E[9];
} m3x3r32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME_UTIL_MATH__M3X3__EXPORTS \

#define GAME_UTIL_MATH__M3X3__FUNCS \
    PROC(m3x3r32, M3x3r32_1, r32 N) \
    PROC(m3x3r32, M3x3r32_3_3_3, v3r32 Col0, v3r32 Col1, v3r32 Col2) \
    PROC(v3r32,   M3x3r32_Mul, m3x3r32 M, v3r32 V) \


#endif