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

#if !defined(GAME_UTIL_MATH_VOX_V3_H_)
#define      GAME_UTIL_MATH_VOX_V3_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: v3r32

typedef union v3r32
{
    struct
    {
        r32 X;
        r32 Y;
        r32 Z;
    };
    r32 E[3];
} v3r32;


//SUBSECTION: v3s16

typedef union v3s16
{
    struct
    {
        s16 X;
        s16 Y;
        s16 Z;
    };
    s16 E[3];
} v3s16;


//SUBSECTION: v3s32

typedef union v3s32
{
    struct
    {
        s32 X;
        s32 Y;
        s32 Z;
    };
    s32 E[3];
} v3s32;


//SUBSECTION: v3u08

typedef union v3u08
{
    struct
    {
        u08 X;
        u08 Y;
        u08 Z;
    };
    u08 E[3];
} v3u08;


//SUBSECTION: v3u16

typedef union v3u16
{
    struct
    {
        u16 X;
        u16 Y;
        u16 Z;
    };
    u16 E[3];
} v3u16;


//SUBSECTION: v3u32

typedef union v3u32
{
    struct
    {
        u32 X;
        u32 Y;
        u32 Z;
    };
    u32 E[3];
} v3u32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME_UTIL_MATH__V3__EXPORTS \

#define GAME_UTIL_MATH__V3__FUNCS \
    PROC(v3r32, V3r32_1x1, r32 N) \
    PROC(v3r32, V3r32_3x1, r32 X, r32 Y, r32 Z) \
    PROC(v3r32, V3r32_Add, v3r32 A, v3r32 B) \
    PROC(v3r32, V3r32_Sub, v3r32 A, v3r32 B) \
    PROC(v3r32, V3r32_Mul_V_V, v3r32 A, v3r32 B) \
    PROC(v3r32, V3r32_Mul_V_S, v3r32 V, r32 S) \
    PROC(v3r32, V3r32_Div_V_V, v3r32 A, v3r32 B) \
    PROC(v3r32, V3r32_Div_V_S, v3r32 V, r32 S) \
    PROC(v3r32, V3r32_Div_S_V, r32 S, v3r32 V) \
    PROC(v3r32, V3r32_AddS, v3r32 V, r32 S) \
    PROC(v3r32, V3r32_SubS, v3r32 V, r32 S) \
    PROC(r32,   V3r32_Dot, v3r32 A, v3r32 B) \
    PROC(v3r32, V3r32_Cross, v3r32 A, v3r32 B) \
    PROC(r32,   V3r32_Len, v3r32 V) \
    PROC(v3r32, V3r32_Normalize, v3r32 V) \
    PROC(v3r32, V3r32_Clamp, v3r32 Min, v3r32 V, v3r32 Max) \
    PROC(b08,   V3r32_Equal, v3r32 A, v3r32 B) \
    PROC(s32,   V3r32_Cmp, v3r32 A, v3r32 B) \
    PROC(v3s32, V3r32_ToV3s32, v3r32 V) \
    PROC(v3u16, V3r32_ToV3u16_Round, v3r32 V) \
    PROC(v3s32, V3s32_1x1, s32 N) \
    PROC(v3s32, V3s32_3x1, s32 X, s32 Y, s32 Z) \
    PROC(v3s32, V3s32_Add, v3s32 A, v3s32 B) \
    PROC(v3s32, V3s32_Sub, v3s32 A, v3s32 B) \
    PROC(v3s32, V3s32_Mul, v3s32 A, v3s32 B) \
    PROC(v3s32, V3s32_Div, v3s32 A, v3s32 B) \
    PROC(s32,   V3s32_Dot, v3s32 A, v3s32 B) \
    PROC(v3s32, V3s32_Cross, v3s32 A, v3s32 B) \
    PROC(r32,   V3s32_Len, v3s32 V) \
    PROC(v3s32, V3s32_Normalize, v3s32 V) \
    PROC(b08,   V3s32_Equal, v3s32 A, v3s32 B) \
    PROC(s32,   V3s32_Cmp, v3s32 A, v3s32 B) \
    PROC(v3r32, V3s32_ToV3r32, v3s32 V) \
    PROC(v3u16, V3u16_3x1, u16 X, u16 Y, u16 Z) \
    PROC(v3r32, V3u16_ToV3r32, v3u16 V) \
    PROC(v3u32, V3u32_1x1, u32 N) \
    PROC(v3u32, V3u32_3x1, u32 X, u32 Y, u32 Z) \
    PROC(v3u32, V3u32_Add, v3u32 A, v3u32 B) \
    PROC(v3u32, V3u32_Sub, v3u32 A, v3u32 B) \
    PROC(v3u32, V3u32_Mul, v3u32 A, v3u32 B) \
    PROC(v3u32, V3u32_Div, v3u32 A, v3u32 B) \
    PROC(u32,   V3u32_Dot, v3u32 A, v3u32 B) \
    PROC(v3u32, V3u32_Cross, v3u32 A, v3u32 B) \
    PROC(r32,   V3u32_Len, v3u32 V) \
    PROC(v3u32, V3u32_Normalize, v3u32 V) \
    PROC(b08,   V3u32_Equal, v3u32 A, v3u32 B) \
    PROC(s32,   V3u32_Cmp, v3u32 A, v3u32 B) \

#define V3r32_Mul(v3r32__A, v3r32__B) V3r32_Mul_V_V(v3r32__A, v3r32__B)
#define V3r32_Mul_S_V(v3r32__S, v3r32__V) V3r32_Mul_V_S(v3r32__V, v3r32__S)
#define V3r32_Div(v3r32__A, v3r32__B) V3r32_Div_V_V(v3r32__A, v3r32__B)


#endif