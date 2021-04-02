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

#if !defined(GAME_UTIL_MATH_VOX_M4X4_H_)
#define      GAME_UTIL_MATH_VOX_M4X4_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: m4x4r32

typedef union m4x4r32
{
    v4r32 V[4];
    r32 M[4][4];
    r32 E[16];
} m4x4r32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME_UTIL_MATH__M4X4__EXPORTS \

#define GAME_UTIL_MATH__M4X4__FUNCS \
    PROC(m4x4r32, M4x4r32_1, r32 N) \
    PROC(m4x4r32, M4x4r32_4_4_4_4, v4r32 Col0, v4r32 Col1, v4r32 Col2, v4r32 Col3) \
    PROC(v4r32,   M4x4r32_Mul_4, m4x4r32 M, v4r32 V) \
    PROC(m4x4r32, M4x4r32_Mul_4x4, m4x4r32 A, m4x4r32 B) \
    PROC(m4x4r32, M4x4r32_Translation, r32 X, r32 Y, r32 Z) \
    PROC(m4x4r32, M4x4r32_Scaling, r32 X, r32 Y, r32 Z) \
    PROC(m4x4r32, M4x4r32_Perspective, r32 FOV, r32 AspectRatio, r32 Near, r32 Far) \
    PROC(m4x4r32, M4x4r32_Orthographic, r32 Left, r32 Right, r32 Top, r32 Bottom, r32 Near, r32 Far) \
    PROC(m4x4r32, M4x4r32_LookAt, v3r32 Eye, v3r32 Center, v3r32 Up) \
    PROC(m4x4r32, M4x4r32_ModelMatrix, v3r32 Translation, v3r32 Scaling) \


#endif