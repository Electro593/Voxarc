#ifndef MATH_VOX_V4U32_H_

#include "util/vox_defines.h"
#include "vox_math.h"
#include "vox_v3u32.h"
#include "vox_v2u32.h"

typedef union v4u32
{
    struct
    {
        u32 X;
        u32 Y;
        u32 Z;
        u32 W;
    };
    struct
    {
        v3u32 XYZ;
        u32 _XYZUnused;
    };
    struct
    {
        v2u32 XY;
        v2u32 ZW;
    };
    u32 E[4];
} v4u32, v4u;

inline v4u32
V4u32_1(u32 N)
{
    v4u32 Result;
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    Result.W = N;
    return Result;
}

inline v4u32
V4u32_3_1(v3u32 XYZ, u32 W)
{
    v4u32 Result;
    Result.XYZ = XYZ;
    Result.W = W;
    return Result;
}

inline v4u32
V4u32_2_1_1(v2u32 XY, u32 Z, u32 W)
{
    v4u32 Result;
    Result.XY = XY;
    Result.Z = Z;
    Result.W = W;
    return Result;
}

inline v4u32
V4u32_2_2(v2u32 XY, v2u32 ZW)
{
    v4u32 Result;
    Result.XY = XY;
    Result.ZW = ZW;
    return Result;
}

inline v4u32
V4u32_1_1_1_1(u32 X, u32 Y, u32 Z, u32 W)
{
    v4u32 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    return Result;
}

inline v4u32
V4u32_Add(v4u32 A, v4u32 B)
{
    v4u32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;
    return Result;
}

inline v4u32
V4u32_Subtract(v4u32 A, v4u32 B)
{
    v4u32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;
    return Result;
}

inline v4u32
V4u32_Multiply(v4u32 A, v4u32 B)
{
    v4u32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    Result.W = A.W * B.W;
    return Result;
}

inline v4u32
V4u32_MultiplyS(v4u32 V, u32 S)
{
    v4u32 Result;
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    Result.Z = V.Z * S;
    Result.W = V.W * S;
    return Result;
}

inline v4u32
V4u32_Divide(v4u32 A, v4u32 B)
{
    v4u32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    Result.W = A.W / B.W;
    return Result;
}

inline v4u32
V4u32_DivideS(v4u32 V, u32 S)
{
    v4u32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    Result.Z = V.Z / S;
    Result.W = V.W / S;
    return Result;
}

inline u32
V4u32_Dot(v4u32 A, v4u32 B)
{
    return A.X*B.X +
           A.Y*B.Y +
           A.Z*B.Z +
           A.W*B.W;
}

inline r32
V4u32_Length(v4u32 V)
{
    return Sqrt((r32)V4u32_Dot(V, V));
}

inline v4u32
V4u32_Normalize(v4u32 V)
{
    r32 Len = V4u32_Length(V);
    if(Len == 0)
        return V;
    return V4u32_DivideS(V, (u32)Len);
}

inline b08
V4u32_Equal(v4u32 A, v4u32 B)
{
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z &&
       A.W == B.W)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V4u32_LessThan(v4u32 A, v4u32 B)
{
    if(V4u32_Length(A) < V4u32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V4u32_GreaterThan(v4u32 A, v4u32 B)
{
    if(V4u32_Length(A) > V4u32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V4U32_H_
#endif