#ifndef MATH_VOX_V2R32_H_

#include "util/vox_defines.h"
#include "vox_math.h"

typedef union v2r32
{
    struct
    {
        r32 X;
        r32 Y;
    };
    r32 E[2];
} v2r32, v2r;

inline v2r32
V2r32_1(r32 N)
{
    v2r32 Result;
    Result.X = N;
    Result.Y = N;
    return Result;
}

inline v2r32
V2r32_2(r32 X, r32 Y)
{
    v2r32 Result;
    Result.X = X;
    Result.Y = Y;
    return Result;
}

inline v2r32
V2r32_Add(v2r32 A, v2r32 B)
{
    v2r32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return Result;
}

inline v2r32
V2r32_Subtract(v2r32 A, v2r32 B)
{
    v2r32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return Result;
}

inline v2r32
V2r32_Multiply(v2r32 A, v2r32 B)
{
    v2r32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    return Result;
}

inline v2r32
V2r32_Divide(v2r32 A, v2r32 B)
{
    v2r32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    return Result;
}

inline v2r32
V2r32_DivideS(v2r32 V, r32 S)
{
    v2r32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    return Result;
}

inline void
V2r32_AddEqual(v2r32 *A, v2r32 B)
{
    *A = V2r32_Add(*A, B);
}

inline void
V2r32_SubtractEqual(v2r32 *A, v2r32 B)
{
    *A = V2r32_Subtract(*A, B);
}

inline void
V2r32_MultiplyEqual(v2r32 *A, v2r32 B)
{
    *A = V2r32_Multiply(*A, B);
}

inline void
V2r32_DivideEqual(v2r32 *A, v2r32 B)
{
    *A = V2r32_Divide(*A, B);
}

inline r32
V2r32_Dot(v2r32 A, v2r32 B)
{
    return A.X*B.X + A.Y*B.Y;
}

inline r32
V2r32_Length(v2r32 V)
{
    return Sqrt(V2r32_Dot(V, V));
}

inline v2r32
V2r32_Normalize(v2r32 V)
{
    r32 Len = V2r32_Length(V);
    if(Len == 0)
        return V;
    return V2r32_DivideS(V, Len);
}

inline b08
V2r32_Equal(v2r32 A, v2r32 B)
{
    if(AbsR(A.X - B.X) < EPSILON32 &&
       AbsR(A.Y - B.Y) < EPSILON32)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2r32_LessThan(v2r32 A, v2r32 B)
{
    if(V2r32_Length(A) < V2r32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2r32_GreaterThan(v2r32 A, v2r32 B)
{
    if(V2r32_Length(A) > V2r32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V2R32_H_
#endif