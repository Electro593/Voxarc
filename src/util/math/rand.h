/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_MATH_RAND_H_
#define UTIL_MATH_RAND_H_


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


#define UTIL_MATH__RAND__EXPORTS \
    FUNC(void, Rand_Init, u32 Seed) \

#define UTIL_MATH__RAND__FUNCS \
    FUNC(u32,  Rand_Int, void) \
    FUNC(s32,  Rand_IntRange, s32 Min, s32 Max) \


#endif