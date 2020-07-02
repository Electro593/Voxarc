#ifndef MATH_VOX_V3R32_H_

#include "util/vox_defines.h"
#include "vox_math.h"
#include "vox_v2r32.h"

typedef union v3r32
{
    struct
    {
        r32 X;
        r32 Y;
        r32 Z;
    };
    struct
    {
        v2r32 XY;
        r32 _XYUnused;
    };
    r32 E[3];
} v3r32, v3r;

inline v3r32
V3r32_1(r32 N)
{
    v3r32 Result;
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    return Result;
}

inline v3r32
V3r32_2_1(v2r32 XY, r32 Z)
{
    v3r32 Result;
    Result.XY = XY;
    Result.Z = Z;
    return Result;
}

inline v3r32
V3r32_1_1_1(r32 X, r32 Y, r32 Z)
{
    v3r32 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    return Result;
}

inline v3r32
V3r32_Add(v3r32 A, v3r32 B)
{
    v3r32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    return Result;
}

inline v3r32
V3r32_Subtract(v3r32 A, v3r32 B)
{
    v3r32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    return Result;
}

inline v3r32
V3r32_Multiply(v3r32 A, v3r32 B)
{
    v3r32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    return Result;
}

inline v3r32
V3r32_MultiplyS(v3r32 V, r32 S)
{
    v3r32 Result;
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    Result.Z = V.Z * S;
    return Result;
}

inline v3r32
V3r32_Divide(v3r32 A, v3r32 B)
{
    v3r32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    return Result;
}

inline v3r32
V3r32_DivideS(v3r32 V, r32 S)
{
    v3r32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    Result.Z = V.Z / S;
    return Result;
}

inline r32
V3r32_Dot(v3r32 A, v3r32 B)
{
    return A.X*B.X +
           A.Y*B.Y +
           A.Z*B.Z;
}

inline v3r32
V3r32_Cross(v3r32 A, v3r32 B)
{
    v3r32 Result;
    Result.X = A.Y*B.Z - A.Z*B.Y;
    Result.Y = A.Z*B.X - A.X*B.Z;
    Result.Z = A.X*B.Y - A.Y*B.X;
    return Result;
}

inline r32
V3r32_Length(v3r32 V)
{
    return Sqrt(V3r32_Dot(V, V));
}

inline v3r32
V3r32_Normalize(v3r32 V)
{
    r32 Len = V3r32_Length(V);
    if(Len == 0)
        return V;
    return V3r32_DivideS(V, Len);
}

inline b08
V3r32_Equal(v3r32 A, v3r32 B)
{
    if(R32_Abs(A.X - B.X) < EPSILON32 &&
       R32_Abs(A.Y - B.Y) < EPSILON32 &&
       R32_Abs(A.Z - B.Z) < EPSILON32)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V3r32_LessThan(v3r32 A, v3r32 B)
{
    if(V3r32_Length(A) < V3r32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V3r32_GreaterThan(v3r32 A, v3r32 B)
{
    if(V3r32_Length(A) > V3r32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V3R32_H_
#endif