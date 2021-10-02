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

#ifndef UTIL_MATH_V2_H_
#define UTIL_MATH_V2_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: v2r32

typedef union v2r32
{
    struct
    {
        r32 X;
        r32 Y;
    };
    r32 E[2];
} v2r32;


//SUBSECTION: v2s16

typedef union
{
    struct
    {
        s16 X;
        s16 Y;
    };
    s16 E[2];
} v2s16;


//SUBSECTION: v2s32

typedef union v2s32
{
    struct
    {
        s32 X;
        s32 Y;
    };
    s32 E[2];
} v2s32;


//SUBSECTION: v2u08

typedef union
{
    struct
    {
        u08 X;
        u08 Y;
    };
    u08 E[2];
} v2u08;


//SUBSECTION: v2u16

typedef union
{
    struct
    {
        u16 X;
        u16 Y;
    };
    u16 E[2];
} v2u16;


//SUBSECTION: v2u32

typedef union v2u32
{
    struct
    {
        u32 X;
        u32 Y;
    };
    u32 E[2];
} v2u32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define UTIL_MATH__V2__EXPORTS \
    FUNC(v2r32, V2r32_2x1,       r32 X, r32 Y) \
    FUNC(v2r32, V2r32_Sub,       v2r32 A, v2r32 B) \
    FUNC(r32,   V2r32_Dot,       v2r32 A, v2r32 B) \
    FUNC(v2r32, V2r32_Normalize, v2r32 V) \
    FUNC(v2s16, V2s16_2x1,       s16 X, s16 Y) \
    FUNC(v2s16, V2s16_Sub,       v2s16 A, v2s16 B) \
    FUNC(v2s16, V2s16_Lerp,      v2s16 A, v2s16 B, pct32 T) \
    FUNC(s32,   V2s16_Dot,       v2s16 A, v2s16 B) \
    FUNC(r32,   V2s16_Len,       v2s16 V) \
    FUNC(r32,   V2s16_Cross,     v2s16 A, v2s16 B) \
    FUNC(v2r32, V2s16_To_V2r32,   v2s16 V) \
    FUNC(v2s32, V2s32_2x1,       s32 X, s32 Y) \
    FUNC(v2s32, V2s32_Add,       v2s32 A, v2s32 B) \
    FUNC(v2s32, V2s32_Sub,       v2s32 A, v2s32 B) \
    FUNC(b08,   V2s32_Equal,     v2s32 A, v2s32 B) \
    FUNC(v2u16, V2u16_1x1,       u16 N) \
    FUNC(v2u16, V2u16_2x1,       u16 X, u16 Y) \
    FUNC(v2u16, V2u16_Swap,      v2u16 V) \
    FUNC(b08,   V2u16_IsWithin,  v2u16 A, v2u16 B) \
    FUNC(b08,   V2u16_Equal,     v2u16 A, v2u16 B) \
    FUNC(u32,   V2u16_RectArea,  v2u16 V) \
    FUNC(v2u32, V2u32_2x1,       u32 X, u32 Y) \
    FUNC(v2r32, V2u32_To_V2r32,   v2u32 V) \

#define UTIL_MATH__V2__FUNCS \
    FUNC(v2r32, V2r32_1x1,       r32 N) \
    FUNC(v2r32, V2r32_Add,       v2r32 A, v2r32 B) \
    FUNC(v2r32, V2r32_Mul,       v2r32 A, v2r32 B) \
    FUNC(v2r32, V2r32_Div,       v2r32 A, v2r32 B) \
    FUNC(v2r32, V2r32_DivS,      v2r32 V, r32 S) \
    FUNC(r32,   V2r32_Len,       v2r32 V) \
    FUNC(b08,   V2r32_Equal,     v2r32 A, v2r32 B) \
    FUNC(s32,   V2r32_Cmp,       v2r32 A, v2r32 B) \
    FUNC(v2s16, V2s16_1x1,       s16 N) \
    FUNC(v2s16, V2s16_Add,       v2s16 A, v2s16 B) \
    FUNC(v2s32, V2s32_1x1,       s32 N) \
    FUNC(v2s32, V2s32_Mul,       v2s32 A, v2s32 B) \
    FUNC(v2s32, V2s32_Div,       v2s32 A, v2s32 B) \
    FUNC(s32,   V2s32_Dot,       v2s32 A, v2s32 B) \
    FUNC(r32,   V2s32_Len,       v2s32 V) \
    FUNC(s32,   V2s32_Cmp,       v2s32 A, v2s32 B) \
    FUNC(v2u32, V2u32_1x1,       u32 N) \
    FUNC(v2u32, V2u32_Add,       v2u32 A, v2u32 B) \
    FUNC(v2u32, V2u32_Sub,       v2u32 A, v2u32 B) \
    FUNC(v2u32, V2u32_Mul,       v2u32 A, v2u32 B) \
    FUNC(v2u32, V2u32_Div,       v2u32 A, v2u32 B) \
    FUNC(u32,   V2u32_Dot,       v2u32 A, v2u32 B) \
    FUNC(r32,   V2u32_Len,       v2u32 V) \
    FUNC(b08,   V2u32_Equal,     v2u32 A, v2u32 B) \
    FUNC(s32,   V2u32_Cmp,       v2u32 A, v2u32 B) \


#endif