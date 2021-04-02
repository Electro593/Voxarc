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

#if !defined(GAME_UTIL_MATH_VOX_RAND_H_)
#define      GAME_UTIL_MATH_VOX_RAND_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define POLYNOMIAL_MASK32 0xB4BCD35C
#define POLYNOMIAL_MASK31 0x7A5BC2E3


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


typedef struct random
{
    u32 LFSR32;
    u32 LFSR31;
} random;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME_UTIL_MATH__RAND__EXPORTS \
    PROC(void, Rand_Init, u32 Seed) \

#define GAME_UTIL_MATH__RAND__FUNCS \
    PROC(u32,  Rand_Int, void) \
    PROC(s32,  Rand_IntRange, s32 Min, s32 Max) \


#endif