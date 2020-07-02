#ifndef MATH_VOX_V2R64_H_

#include "util/vox_defines.h"
#include "vox_math.h"

typedef union v2r64
{
    struct
    {
        r32 X;
        r32 Y;
    };
    r32 E[2];
} v2r64;

inline v2r64
V2r64_1(r32 N)
{
    v2r64 Result;
    Result.X = N;
    Result.Y = N;
    return Result;
}

inline v2r64
V2r64_2(r32 X, r32 Y)
{
    v2r64 Result;
    Result.X = X;
    Result.Y = Y;
    return Result;
}

inline v2r64
V2r64_Add(v2r64 A, v2r64 B)
{
    v2r64 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return Result;
}

inline v2r64
V2r64_Subtract(v2r64 A, v2r64 B)
{
    v2r64 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return Result;
}

inline v2r64
V2r64_Multiply(v2r64 A, v2r64 B)
{
    v2r64 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    return Result;
}

inline v2r64
V2r64_Divide(v2r64 A, v2r64 B)
{
    v2r64 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    return Result;
}

inline v2r64
V2r64_DivideS(v2r64 V, r32 S)
{
    v2r64 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    return Result;
}

inline void
V2r64_AddEqual(v2r64 *A, v2r64 B)
{
    *A = V2r64_Add(*A, B);
}

inline void
V2r64_SubtractEqual(v2r64 *A, v2r64 B)
{
    *A = V2r64_Subtract(*A, B);
}

inline void
V2r64_MultiplyEqual(v2r64 *A, v2r64 B)
{
    *A = V2r64_Multiply(*A, B);
}

inline void
V2r64_DivideEqual(v2r64 *A, v2r64 B)
{
    *A = V2r64_Divide(*A, B);
}

inline r32
V2r64_Dot(v2r64 A, v2r64 B)
{
    return A.X*B.X + A.Y*B.Y;
}

inline r32
V2r64_Length(v2r64 V)
{
    return Sqrt(V2r64_Dot(V, V));
}

inline v2r64
V2r64_Normalize(v2r64 V)
{
    r32 Len = V2r64_Length(V);
    if(Len == 0)
        return V;
    return V2r64_DivideS(V, Len);
}

inline b08
V2r64_Equal(v2r64 A, v2r64 B)
{
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2r64_LessThan(v2r64 A, v2r64 B)
{
    if(V2r64_Length(A) < V2r64_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2r64_GreaterThan(v2r64 A, v2r64 B)
{
    if(V2r64_Length(A) > V2r64_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V2R64_H_
#endif