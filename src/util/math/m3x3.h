/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_MATH_M3X3_H_
#define UTIL_MATH_M3X3_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: m3x3r32

typedef union m3x3r32
{
    v3r32 V[3];
    r32 M[3][3];
    r32 E[9];
} m3x3r32;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define UTIL_MATH__M3X3__EXPORTS \

#define UTIL_MATH__M3X3__FUNCS \
    FUNC(m3x3r32, M3x3r32_1, r32 N) \
    FUNC(m3x3r32, M3x3r32_3_3_3, v3r32 Col0, v3r32 Col1, v3r32 Col2) \
    FUNC(v3r32,   M3x3r32_Mul, m3x3r32 M, v3r32 V) \


#endif