#ifndef MATH_VOX_V2S32_H_

#include "util/vox_defines.h"
#include "math/vox_math.h"

typedef union v2s32
{
    struct
    {
        s32 X;
        s32 Y;
    };
    s32 E[2];
} v2s32, v2s;

inline v2s32
V2s32_1(s32 N)
{
    v2s32 Result;
    Result.X = N;
    Result.Y = N;
    return Result;
}

inline v2s32
V2s32_2(s32 X, s32 Y)
{
    v2s32 Result;
    Result.X = X;
    Result.Y = Y;
    return Result;
}

inline s32
Dot(v2s32 A, v2s32 B)
{
    return A.X*B.X + A.Y*B.Y;
}

inline r32
Length(v2s32 V)
{
    return Sqrt((r32)Dot(V, V));
}

inline b08
V2s32_Equal(v2s32 A, v2s32 B)
{
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2s32_NotEqual(v2s32 A, v2s32 B)
{
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        return FALSE;
    }
    return TRUE;
}

inline b08
V2s32_LessThan(v2s32 A, v2s32 B)
{
    if(Length(A) < Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2s32_GreaterThan(v2s32 A, v2s32 B)
{
    if(Length(A) > Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline v2s32
V2s32_Add(v2s32 A, v2s32 B)
{
    v2s32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return Result;
}

inline v2s32
V2s32_Subtract(v2s32 A, v2s32 B)
{
    v2s32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return Result;
}

inline v2s32
V2s32_Multiply(v2s32 A, v2s32 B)
{
    v2s32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    return Result;
}

inline v2s32
V2s32_Divide(v2s32 A, v2s32 B)
{
    v2s32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    return Result;
}

inline v2s32
V2s32_DivideS(v2s32 V, s32 S)
{
    v2s32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    return Result;
}

inline void
V2s32_AddEqual(v2s32 *A, v2s32 B)
{
    *A = V2s32_Add(*A, B);
}

inline void
V2s32_SubtractEqual(v2s32 *A, v2s32 B)
{
    *A = V2s32_Subtract(*A, B);
}

inline void
V2s32_MultiplyEqual(v2s32 *A, v2s32 B)
{
    *A = V2s32_Multiply(*A, B);
}

inline void
V2s32_DivideEqual(v2s32 *A, v2s32 B)
{
    *A = V2s32_Divide(*A, B);
}

#define MATH_VOX_V2S32_H_
#endif