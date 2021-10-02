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

#ifndef UTIL_MATH_V1_H_
#define UTIL_MATH_V1_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: R32

#define R32_DIGITS_PRECISION          6
#define R32_DIGITS_PRECISION_ROUNDING 9
#define R32_MASK_MANTISSA 0x007FFFFF
#define R32_MASK_EXPONENT 0x7F800000
#define R32_MASK_SIGN     0x80000000
#define R32_BIT_NAN_SIGNAL 22
#define R32_BIT_SIGN       31
#define R32_BITS_SIGN      1
#define R32_BITS_EXPONENT  8
#define R32_BITS_MANTISSA 23
#define R32_SHIFT_MANTISSA   0
#define R32_SHIFT_NAN_SIGNAL R32_BIT_NAN_SIGNAL
#define R32_SHIFT_EXPONENT   R32_BITS_MANTISSA
#define R32_SHIFT_SIGN       R32_BIT_SIGN
#define R32_HEX_ZERO     0x00000000 // 0 00000000 00000000000000000000000
#define R32_HEX_DEN      0x00000001 // 0 00000000 00000000000000000000001
#define R32_HEX_MIN      0x00800000 // 0 00000001 00000000000000000000000
#define R32_HEX_EPSILON  0x34000000 // 0 01101000 00000000000000000000000
#define R32_HEX_ONE      0x3F800000 // 0 01111111 00000000000000000000000
#define R32_HEX_PI       0x40490FDB // 0 10000000 10010010000111111011011
#define R32_HEX_DEGMAX   0x43b40000 // 0 10000111 01101000000000000000000
#define R32_HEX_MAXINT   0x4B800000 // 0 10010111 00000000000000000000000
#define R32_HEX_MAX      0x7F7FFFFF // 0 11111110 11111111111111111111111
#define R32_HEX_INF      0x7F800000 // 0 11111111 00000000000000000000000
#define R32_HEX_QNAN     0x7F800001 // 0 11111111 00000000000000000000001
#define R32_HEX_SNAN     0x7FC00000 // 0 11111111 10000000000000000000000
#define R32_HEX_NEG_ZERO 0x80000000 // 1 00000000 00000000000000000000000
#define R32_HEX_IND      0xFFC00000 // 1 11111111 10000000000000000000000
#define R32_DEN     LITERAL_CAST(u32, R32_HEX_DEN,     r32) // 0.00000000000000000000000000000000000000000000140129846432481707092372958328991613128026194187651577175706828388979108268586060148663818836212158203125
#define R32_MIN     LITERAL_CAST(u32, R32_HEX_MIN,     r32) // 0.000000000000000000000000000000000000011754943508222875079687365372222456778186655567720875215087517062784172594547271728515625
#define R32_EPSILON LITERAL_CAST(u32, R32_HEX_EPSILON, r32) // 0.00000011920928955078125
#define R32_PI      LITERAL_CAST(u32, R32_HEX_PI,      r32) // 3.1415927410125732421875
#define R32_DEGMAX  LITERAL_CAST(u32, R32_HEX_DEGMAX,  r32) // 360.0
#define R32_MAX     LITERAL_CAST(u32, R32_HEX_MAX,     r32) // 340282346638528859811704183484516925440.0
#define R32_INF     LITERAL_CAST(u32, R32_HEX_INF,     r32) // inf
#define R32_QNAN    LITERAL_CAST(u32, R32_HEX_QNAN,    r32) // nan
#define R32_SNAN    LITERAL_CAST(u32, R32_HEX_SNAN,    r32) // nan
#define R32_IND     LITERAL_CAST(u32, R32_HEX_IND,     r32) // -nan(ind)
#define R32_GET_SIGN(Binary)     ((Binary) >> R32_SHIFT_SIGN)
#define R32_GET_EXPONENT(Binary) (((Binary) & R32_MASK_EXPONENT) >> R32_SHIFT_EXPONENT)
#define R32_GET_MANTISSA(Binary) ((Binary) & R32_MASK_MANTISSA)

//SUBSECTION: S16

#define S16_BITS 16
#define S16_BIT_SIGN 15
#define S16_SHIFT_SIGN 15
#define S16_MIN 0x8000 // -32768
#define S16_MAX 0x7FFF //  32767

//SUBSECTION: S32

#define S32_BITS 32
#define S32_BIT_SIGN 31
#define S32_SHIFT_SIGN 31
#define S32_MIN 0x80000000 // -2147483648
#define S32_MAX 0x7FFFFFFF //  2147483647

//SUBSECTION: U32

#define U32_BITS 32
#define U32_MIN 0x00000000 // 0
#define U32_MAX 0xFFFFFFFF // 4294967295
#define U32_MAX_CHARS_DEC (10+1)
#define U32_MAX_CHARS_HEX (2 + U32_BITS/4 + 1)

//SUBSECTION: U64

#define U64_BITS 64
#define U64_MIN 0x0000000000000000 // 0
#define U64_MAX 0xFFFFFFFFFFFFFFFF // 18446744073709551615
#define U64_MAX_CHARS_DEC (20+1)
#define U64_MAX_CHARS_HEX (2 + U64_BITS/4 + 1)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: R32

typedef r32 pct32; // Percent
typedef r32 rad32; // Radians
typedef r32 deg32; // Degrees

typedef struct
{
    pct32 R;
    pct32 G;
    pct32 B;
} rgb32;

typedef struct
{
    rad32 H;
    pct32 S;
    pct32 V;
} hsv32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define UTIL_MATH__V1__EXPORTS \
    FUNC(s32,   R32_Cmp, r32 A, r32 B) \
    FUNC(r32,   R32_Abs, r32 N) \
    FUNC(r32,   R32_Floor, r32 N) \
    FUNC(r32,   R32_Ceil, r32 N) \
    FUNC(r32,   R32_Sqrt, r32 N) \
    FUNC(str,   R32_To_Str, str *Dest, r32 N) \
    FUNC(s32,   S32_Abs, s32 N) \
    FUNC(str,   U32_To_Str, str *Dest, u32 N) \

#define UTIL_MATH__V1__FUNCS \
    FUNC(b08,   R32_IsNAN, r32 N) \
    FUNC(r32,   R32_Min, r32 A, r32 B) \
    FUNC(r32,   R32_Max, r32 A, r32 B) \
    FUNC(b08,   R32_EqualApprox, r32 A, r32 B, r32 Epsilon) \
    FUNC(b08,   R32_EqualStrict, r32 A, r32 B, r32 Epsilon) \
    FUNC(r32,   R32_Round, r32 N) \
    FUNC(r32,   R32_Mod, r32 Dividend, r32 Divisor) \
    FUNC(r32,   R32_Lerp, r32 Start, r32 End, pct32 Percent) \
    FUNC(r32,   R32_Cbrt, r32 N) \
    FUNC(u32,   R32_RootsDeg1, r32 *Roots, r32 A, r32 B) \
    FUNC(u32,   R32_RootsDeg2, r32 *Roots, r32 A, r32 B, r32 C) \
    FUNC(u32,   R32_RootsDeg3, r32 *Roots, r32 A, r32 B, r32 C, r32 D) \
    FUNC(r32,   Sin, rad32 R) \
    FUNC(r32,   Cos, rad32 R) \
    FUNC(r32,   Tan, rad32 R) \
    FUNC(r32,   Acos, rad32 R) \
    FUNC(rad32, R32_Radians, deg32 Degrees) \
    FUNC(rgb32, R32_HSVToRGB, rad32 Hue, pct32 Saturation, pct32 Value) \
    FUNC(v3u08, R32_RGBToV3u08, rgb32 RGB) \
    FUNC(s16,   S16_Abs, s16 N) \
    FUNC(s32,   S32_Sign, s32 N) \
    FUNC(s32,   S32_Lerp, s32 A, s32 B, r32 Percent) \
    FUNC(str,   S32_To_Str, str *Dest, s32 N) \
    FUNC(r32,   U32_InvLerp, u32 Start, u32 End, u32 X) \
    FUNC(u32,   U32_DivUp, u32 Dividend, u32 Divisor) \
    FUNC(str,   U64_To_Str, str *Dest, u64 N) \
    FUNC(str,   U64_To_Str_Hex, str *Dest, u64 N) \

//CREDIT: R32_EqualAppprox, R32_EqualStrict, R32_GreaterThan, and R32_LessThan
//        are adapted from "The Art of Computer Programming" by Donald Knuth
#define R32_Equal(r32_A, r32_B) R32_EqualApprox(r32_A, r32_B, R32_EPSILON)

//TODO: Implement something decent
#define R32_Sin(rad32__R) Sin(rad32__R)
#define R32_Cos(rad32__R) Cos(rad32__R)
#define R32_Tan(rad32__R) Tan(rad32__R)
#define R32_Acos(rad32__R) Acos(rad32__R)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Data
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


global u32 U32_Factorial[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

#endif