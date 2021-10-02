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

#ifndef UTIL_MATH_M4X4_H_
#define UTIL_MATH_M4X4_H_


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


#define UTIL_MATH__M4X4__EXPORTS \
    FUNC(m4x4r32, M4x4r32_1x1, r32 N) \
    FUNC(m4x4r32, M4x4r32_Mul_1x4m4, m4x4r32 A, m4x4r32 B) \
    FUNC(m4x4r32, M4x4r32_ModelMatrix, v3r32 Translation, v3r32 Scaling) \

#define UTIL_MATH__M4X4__FUNCS \
    FUNC(m4x4r32, M4x4r32_4x4, v4r32 Col0, v4r32 Col1, v4r32 Col2, v4r32 Col3) \
    FUNC(v4r32,   M4x4r32_Mul_1x4, m4x4r32 M, v4r32 V) \
    FUNC(m4x4r32, M4x4r32_Translation, r32 X, r32 Y, r32 Z) \
    FUNC(m4x4r32, M4x4r32_Scaling, r32 X, r32 Y, r32 Z) \
    FUNC(m4x4r32, M4x4r32_Perspective, r32 FOV, r32 AspectRatio, r32 Near, r32 Far) \
    FUNC(m4x4r32, M4x4r32_Orthographic, r32 Left, r32 Right, r32 Top, r32 Bottom, r32 Near, r32 Far) \
    FUNC(m4x4r32, M4x4r32_LookAt, v3r32 Eye, v3r32 Center, v3r32 Up) \

#define M4x4r32_Mul(m4x4r32__A, m4x4r32__B) M4x4r32_Mul_1x4m4(m4x4r32__A, m4x4r32__B)

#endif