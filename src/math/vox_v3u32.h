#ifndef MATH_VOX_V3U32_H_

#include "util/vox_defines.h"
#include "math/vox_math.h"
#include "math/vox_v2u32.h"

typedef union v3u32
{
    struct
    {
        u32 X;
        u32 Y;
        u32 Z;
    };
    struct
    {
        v2u32 XY;
        u32 _XYUnused;
    };
    u32 E[3];
} v3u32, v3u;

inline v3u32
V3u32_1(u32 N)
{
    v3u32 Result;
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    return Result;
}

inline v3u32
V3u32_2(v2u XY, u32 Z)
{
    v3u32 Result;
    Result.XY = XY;
    Result.Z = Z;
    return Result;
}

inline v3u32
V3u32_3(u32 X, u32 Y, u32 Z)
{
    v3u32 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    return Result;
}

inline v3u32
V3u32_Add(v3u32 A, v3u32 B)
{
    v3u32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    return Result;
}

inline v3u32
V3u32_AddS(v3u32 V, u32 S)
{
    v3u32 Result;
    Result.X = V.X + S;
    Result.Y = V.Y + S;
    Result.Z = V.Z + S;
    return Result;
}

inline v3u32
V3u32_Subtract(v3u32 A, v3u32 B)
{
    v3u32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    return Result;
}

inline v3u32
V3u32_Multiply(v3u32 A, v3u32 B)
{
    v3u32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    return Result;
}

inline v3u32
V3u32_MultiplyS(v3u32 V, u32 S)
{
    v3u32 Result;
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    Result.Z = V.Z * S;
    return Result;
}

inline v3u32
V3u32_Divide(v3u32 A, v3u32 B)
{
    v3u32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    return Result;
}

inline v3u32
V3u32_DivideS(v3u32 V, u32 S)
{
    v3u32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    Result.Z = V.Z / S;
    return Result;
}

inline void
V3u32_AddEqual(v3u32 *A, v3u32 B)
{
    *A = V3u32_Add(*A, B);
}

inline void
V3u32_AddEqualS(v3u32 *A, u32 B)
{
    *A = V3u32_AddS(*A, B);
}

inline void
V3u32_SubtractEqual(v3u32 *A, v3u32 B)
{
    *A = V3u32_Subtract(*A, B);
}

inline void
V3u32_MultiplyEqual(v3u32 *A, v3u32 B)
{
    *A = V3u32_Multiply(*A, B);
}

inline void
V3u32_DivideEqual(v3u32 *A, v3u32 B)
{
    *A = V3u32_Divide(*A, B);
}

inline u32
V3u32_Dot(v3u32 A, v3u32 B)
{
    return A.X*B.X +
           A.Y*B.Y +
           A.Z*B.Z;
}

inline v3u32
V3u32_Cross(v3u32 A, v3u32 B)
{
    v3u32 Result;
    Result.X = A.Y*B.Z - A.Z*B.Y;
    Result.Y = A.Z*B.X - A.X*B.Z;
    Result.Z = A.X*B.Y - A.Y*B.X;
    return Result;
}

inline r32
V3u32_Length(v3u32 V)
{
    return Sqrt((r32)V3u32_Dot(V, V));
}

inline v3u32
V3u32_Normalize(v3u32 V)
{
    r32 Len = V3u32_Length(V);
    if(Len == 0)
        return V;
    return V3u32_DivideS(V, (u32)Len);
}

inline b08
V3u32_Equal(v3u32 A, v3u32 B)
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
V3u32_LessThan(v3u32 A, v3u32 B)
{
    if(V3u32_Length(A) < V3u32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V3u32_GreaterThan(v3u32 A, v3u32 B)
{
    if(V3u32_Length(A) > V3u32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V3U32_H_
#endif