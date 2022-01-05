/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_MATH_V4_H_
#define UTIL_MATH_V4_H_


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


#define UTIL_MATH__V4__EXPORTS \
    FUNC(v4r32, V4r32_1x3_1x1, v3r32 XYZ, r32 W) \
    FUNC(bfs32, V4r32_RoundTo_Bfs32, v4r32 V, bf_format Format) \
    FUNC(v4u08, V4u08_4x1, u08 X, u08 Y, u08 Z, u08 W) \
    FUNC(v4u16, V4u16_2x2, v2u16 XY, v2u16 ZW) \
    FUNC(b08,   V4u16_Equal, v4u16 A, v4u16 B) \
    FUNC(v4u16, V4u16_RectOverlap, v4u16 A, v4u16 B) \

#define UTIL_MATH__V4__FUNCS \
    FUNC(v4r32, V4r32_1x1, r32 N) \
    FUNC(v4r32, V4r32_4x1, r32 X, r32 Y, r32 Z, r32 W) \
    FUNC(v4r32, V4r32_Add, v4r32 A, v4r32 B) \
    FUNC(v4r32, V4r32_Sub, v4r32 A, v4r32 B) \
    FUNC(v4r32, V4r32_Mul, v4r32 A, v4r32 B) \
    FUNC(v4r32, V4r32_Div, v4r32 A, v4r32 B) \
    FUNC(r32,   V4r32_Dot, v4r32 A, v4r32 B) \
    FUNC(r32,   V4r32_Len, v4r32 V) \
    FUNC(v4r32, V4r32_Normalize, v4r32 V) \
    FUNC(b08,   V4r32_Equal, v4r32 A, v4r32 B) \
    FUNC(s32,   V4r32_Cmp, v4r32 A, v4r32 B) \
    FUNC(v4u16, V4u16_1x1, u16 N) \
    FUNC(v4u16, V4u16_4x1, u16 X, u16 Y, u16 Z, u16 W) \
    FUNC(v4u32, V4u32_1x1, u32 N) \
    FUNC(v4u32, V4u32_4x1, u32 X, u32 Y, u32 Z, u32 W) \
    FUNC(v4u32, V4u32_Add, v4u32 A, v4u32 B) \
    FUNC(v4u32, V4u32_Sub, v4u32 A, v4u32 B) \
    FUNC(v4u32, V4u32_Mul, v4u32 A, v4u32 B) \
    FUNC(v4u32, V4u32_Div, v4u32 A, v4u32 B) \
    FUNC(u32,   V4u32_Dot, v4u32 A, v4u32 B) \
    FUNC(r32,   V4u32_Len, v4u32 V) \
    FUNC(b08,   V4u32_Equal, v4u32 A, v4u32 B) \
    FUNC(s32,   V4u32_Cmp, v4u32 A, v4u32 B) \


#endif