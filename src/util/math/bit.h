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

#ifndef UTIL_MATH_BIT_H_
#define UTIL_MATH_BIT_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


typedef enum bf_format
{
    BFF_1X2_3X10,
} bf_format;


//SUBSECTION: bfs32

typedef u32 bfs32;
typedef bfs32 bfs32_1x2_3x10; // Code annotation purposes

typedef union bfs32_1x2_3x10_d
{ // Debug
    bfs32 Bitfield;
    struct
    {
        s32 W : 10;
        s32 Z : 10;
        s32 Y : 10;
        s32 X :  2;
    };
} bfs32_1x2_3x10_d;


//SUBSECTION: bfu16

typedef u16 bfu16;


//SUBSECTION: bfu32

typedef u32 bfu32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define UTIL_MATH__BIT__EXPORTS \

#define UTIL_MATH__BIT__FUNCS \
    FUNC(s32,            Bfs32_GetComponent, bfs32 Bitfield, u32 Size, u32 Offset) \
    FUNC(void,           Bfs32_SetComponent, bfs32 *Bitfield, s32 Data, u32 Size, u32 Offset) \
    FUNC(bfs32_1x2_3x10, Bfs32_1x2_3x10, s32 N0, s32 N1, s32 N2, s32 N3) \
    FUNC(v4s32,          Bfs32_1x2_3x10_To_V4s32, bfs32_1x2_3x10 Bitfield) \
    FUNC(u32,            Bfu32_GetComponent, bfu32 Bitfield, u32 Size, u32 Offset) \
    FUNC(void,           Bfu32_SetComponent, bfu32 *Bitfield, u32 Data, u32 Size, u32 Offset) \


#endif