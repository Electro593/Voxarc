#ifndef MATH_VOX_V3S32_H_

#include "util/vox_defines.h"
#include "vox_math.h"
#include "vox_v2s32.h"

typedef union v3s32
{
    struct
    {
        s32 X;
        s32 Y;
        s32 Z;
    };
    struct
    {
        v2s32 XY;
        s32 _XYUnused;
    };
    s32 E[3];
} v3s32, v3s;

inline v3s32
V3s32_1(s32 N)
{
    v3s32 Result;
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    return Result;
}

inline v3s32
V3s32_2_1(v2s XY, s32 Z)
{
    v3s32 Result;
    Result.XY = XY;
    Result.Z = Z;
    return Result;
}

inline v3s32
V3s32_3(s32 X, s32 Y, s32 Z)
{
    v3s32 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    return Result;
}

inline v3s32
V3s32_Add(v3s32 A, v3s32 B)
{
    v3s32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    return Result;
}

inline v3s32
V3s32_Subtract(v3s32 A, v3s32 B)
{
    v3s32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    return Result;
}

inline v3s32
V3s32_Multiply(v3s32 A, v3s32 B)
{
    v3s32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    return Result;
}

inline v3s32
V3s32_MultiplyS(v3s32 V, s32 S)
{
    v3s32 Result;
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    Result.Z = V.Z * S;
    return Result;
}

inline v3s32
V3s32_Divide(v3s32 A, v3s32 B)
{
    v3s32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    return Result;
}

inline v3s32
V3s32_DivideS(v3s32 V, s32 S)
{
    v3s32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    Result.Z = V.Z / S;
    return Result;
}

inline s32
V3s32_Dot(v3s32 A, v3s32 B)
{
    return A.X*B.X +
           A.Y*B.Y +
           A.Z*B.Z;
}

inline v3s32
V3s32_Cross(v3s32 A, v3s32 B)
{
    v3s32 Result;
    Result.X = A.Y*B.Z - A.Z*B.Y;
    Result.Y = A.Z*B.X - A.X*B.Z;
    Result.Z = A.X*B.Y - A.Y*B.X;
    return Result;
}

inline r32
V3s32_Length(v3s32 V)
{
    return Sqrt((r32)V3s32_Dot(V, V));
}

inline v3s32
V3s32_Normalize(v3s32 V)
{
    r32 Len = V3s32_Length(V);
    if(Len == 0)
        return V;
    return V3s32_DivideS(V, (s32)Len);
}

inline b08
V3s32_Equal(v3s32 A, v3s32 B)
{
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V3s32_LessThan(v3s32 A, v3s32 B)
{
    if(V3s32_Length(A) < V3s32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V3s32_GreaterThan(v3s32 A, v3s32 B)
{
    if(V3s32_Length(A) > V3s32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V3S32_H_
#endif