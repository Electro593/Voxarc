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

#if !defined(GAME_UTIL_MATH_VOX_V4_H_)
#define      GAME_UTIL_MATH_VOX_V4_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: v4r32

typedef union v4r32
{
    struct
    {
        r32 X;
        r32 Y;
        r32 Z;
        r32 W;
    };
    struct
    {
        v3r32 XYZ;
        r32 _UnusedXYZ_W_;
    };
    r32 E[4];
} v4r32;


//SUBSECTION: v4s32

typedef union v4s32
{
    struct
    {
        s32 X;
        s32 Y;
        s32 Z;
        s32 W;
    };
    struct
    {
        v2s32 XY;
        v2s32 ZW;
    };
    struct
    {
        v3s32 XYZ;
        s32 _UnusedXYZ_W_;
    };
    s32 E[4];
} v4s32;


//SUBSECTION: v4u08

typedef union v4u08
{
    struct
    {
        u08 X;
        u08 Y;
        u08 Z;
        u08 W;
    };
    u08 E[4];
} v4u08;


//SUBSECTION: v4u16

typedef union v4u16
{
    struct
    {
        u16 X;
        u16 Y;
        u16 Z;
        u16 W;
    };
    struct
    {
        v2u16 XY;
        v2u16 ZW;
    };
    u16 E[4];
} v4u16;


//SUBSECTION: v4u32

typedef union v4u32
{
    struct
    {
        u32 X;
        u32 Y;
        u32 Z;
        u32 W;
    };
    u32 E[4];
} v4u32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME_UTIL_MATH__V4__EXPORTS \
    PROC(v4u08, V4u08_4x1, u08 X, u08 Y, u08 Z, u08 W) \

#define GAME_UTIL_MATH__V4__FUNCS \
    PROC(v4r32, V4r32_1x1, r32 N) \
    PROC(v4r32, V4r32_4x1, r32 X, r32 Y, r32 Z, r32 W) \
    PROC(v4r32, V4r32_1x3_1x1, v3r32 XYZ, r32 W) \
    PROC(v4r32, V4r32_Add, v4r32 A, v4r32 B) \
    PROC(v4r32, V4r32_Sub, v4r32 A, v4r32 B) \
    PROC(v4r32, V4r32_Mul, v4r32 A, v4r32 B) \
    PROC(v4r32, V4r32_Div, v4r32 A, v4r32 B) \
    PROC(r32,   V4r32_Dot, v4r32 A, v4r32 B) \
    PROC(r32,   V4r32_Len, v4r32 V) \
    PROC(v4r32, V4r32_Normalize, v4r32 V) \
    PROC(b08,   V4r32_Equal, v4r32 A, v4r32 B) \
    PROC(s32,   V4r32_Cmp, v4r32 A, v4r32 B) \
    PROC(bfs32, V4r32_ToBfs32_Round, v4r32 V, bf_format Format) \
    PROC(v4u16, V4u16_1x1, u16 N) \
    PROC(v4u16, V4u16_4x1, u16 X, u16 Y, u16 Z, u16 W) \
    PROC(v4u16, V4u16_2x2, v2u16 XY, v2u16 ZW) \
    PROC(v4u16, V4u16_RectOverlap, v4u16 A, v4u16 B) \
    PROC(b08,   V4u16_Equal, v4u16 A, v4u16 B) \
    PROC(v4u32, V4u32_1x1, u32 N) \
    PROC(v4u32, V4u32_4x1, u32 X, u32 Y, u32 Z, u32 W) \
    PROC(v4u32, V4u32_Add, v4u32 A, v4u32 B) \
    PROC(v4u32, V4u32_Sub, v4u32 A, v4u32 B) \
    PROC(v4u32, V4u32_Mul, v4u32 A, v4u32 B) \
    PROC(v4u32, V4u32_Div, v4u32 A, v4u32 B) \
    PROC(u32,   V4u32_Dot, v4u32 A, v4u32 B) \
    PROC(r32,   V4u32_Len, v4u32 V) \
    PROC(v4u32, V4u32_Normalize, v4u32 V) \
    PROC(b08,   V4u32_Equal, v4u32 A, v4u32 B) \
    PROC(s32,   V4u32_Cmp, v4u32 A, v4u32 B) \


#endif