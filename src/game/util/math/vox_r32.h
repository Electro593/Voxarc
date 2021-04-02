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

#if !defined(GAME_UTIL_MATH_VOX_R32_H_)
#define      GAME_UTIL_MATH_VOX_R32_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


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


//CREDIT: R32_EqualAppprox, R32_EqualStrict, R32_GreaterThan, and R32_LessThan
//        are adapted from "The Art of Computer Programming" by Donald Knuth

#define GAME_UTIL_MATH__R32__EXPORTS \
    PROC(r32,   R32_Floor, r32 N) \
    PROC(r32,   R32_Ceil, r32 N) \
    PROC(r32,   R32_Sqrt, r32 N) \
    PROC(str,   R32_ToStr, str *Dest, r32 N) \

#define GAME_UTIL_MATH__R32__FUNCS \
    PROC(r32,   R32_Min, r32 A, r32 B) \
    PROC(r32,   R32_Max, r32 A, r32 B) \
    PROC(b08,   R32_EqualApprox, r32 A, r32 B, r32 Epsilon) \
    PROC(b08,   R32_EqualStrict, r32 A, r32 B, r32 Epsilon) \
    PROC(s32,   R32_Cmp, r32 A, r32 B) \
    PROC(r32,   R32_Round, r32 N) \
    PROC(r32,   R32_Abs, r32 N) \
    PROC(r32,   R32_Mod, r32 Dividend, r32 Divisor) \
    PROC(r32,   R32_Lerp, r32 Start, r32 End, pct32 Percent) \
    PROC(r32,   Sin, rad32 R) \
    PROC(r32,   Cos, rad32 R) \
    PROC(r32,   Tan, rad32 R) \
    PROC(rad32, R32_Radians, deg32 Degrees) \
    PROC(rgb32, R32_HSVToRGB, rad32 Hue, pct32 Saturation, pct32 Value) \
    PROC(v3u08, R32_RGBToV3u08, rgb32 RGB) \

#define R32_Equal(r32_A, r32_B) R32_EqualApprox(r32_A, r32_B, R32_EPSILON)

#endif