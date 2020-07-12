#ifndef MATH_VOX_V2U32_H_

#include "util/vox_defines.h"

union v2u32
{
    struct
    {
        u32 X;
        u32 Y;
    };
    u32 E[2];
};

inline v2u32
V2u32_1(u32 N)
{
    v2u32 Result;
    Result.X = N;
    Result.Y = N;
    return Result;
}

inline v2u32
V2u32_1_1(u32 X,
          u32 Y)
{
    v2u32 Result;
    Result.X = X;
    Result.Y = Y;
    return Result;
}

inline u32
V2u32_Dot(v2u32 A,
          v2u32 B)
{
    return A.X*B.X + A.Y*B.Y;
}

inline r32
V2u32_Length(v2u32 V)
{
    return Sqrt((r32)V2u32_Dot(V, V));
}

inline b08
V2u32_Equal(v2u32 A,
            v2u32 B)
{
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2u32_LessThan(v2u32 A,
               v2u32 B)
{
    if(V2u32_Length(A) < V2u32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V2u32_GreaterThan(v2u32 A,
                  v2u32 B)
{
    if(V2u32_Length(A) > V2u32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline v2u32
V2u32_Add(v2u32 A,
          v2u32 B)
{
    v2u32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return Result;
}

inline v2u32
V2u32_Subtract(v2u32 A,
               v2u32 B)
{
    v2u32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return Result;
}

inline v2u32
V2u32_Multiply(v2u32 A,
               v2u32 B)
{
    v2u32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    return Result;
}

inline v2u32
V2u32_Divide(v2u32 A,
             v2u32 B)
{
    v2u32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    return Result;
}

inline v2u32
V2u32_DivideS(v2u32 V,
              u32 S)
{
    v2u32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    return Result;
}

#define MATH_VOX_V2U32_H_
#endif