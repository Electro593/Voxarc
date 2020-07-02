#ifndef MATH_VOX_V4R32_H_

#include "util/vox_defines.h"
#include "vox_math.h"
#include "vox_v3r32.h"
#include "vox_v2r32.h"

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
        r32 R;
        r32 G;
        r32 B;
        r32 A;
    };
    struct
    {
        v3r32 XYZ;
        r32 _XYZUnused;
    };
    struct
    {
        v2r32 XY;
        v2r32 ZW;
    };
    r32 E[4];
} v4r32, v4r;

inline v4r32
V4r32_1(r32 N)
{
    v4r32 Result;
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    Result.W = N;
    return Result;
}

inline v4r32
V4r32_3_1(v3r32 XYZ, r32 W)
{
    v4r32 Result;
    Result.XYZ = XYZ;
    Result.W = W;
    return Result;
}

inline v4r32
V4r32_2_1_1(v2r32 XY, r32 Z, r32 W)
{
    v4r32 Result;
    Result.XY = XY;
    Result.Z = Z;
    Result.W = W;
    return Result;
}

inline v4r32
V4r32_2_2(v2r32 XY, v2r32 ZW)
{
    v4r32 Result;
    Result.XY = XY;
    Result.ZW = ZW;
    return Result;
}

inline v4r32
V4r32_1_1_1_1(r32 X, r32 Y, r32 Z, r32 W)
{
    v4r32 Result;
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    return Result;
}

inline v4r32
V4r32_Add(v4r32 A, v4r32 B)
{
    v4r32 Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;
    return Result;
}

inline v4r32
V4r32_Subtract(v4r32 A, v4r32 B)
{
    v4r32 Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;
    return Result;
}

inline v4r32
V4r32_Multiply(v4r32 A, v4r32 B)
{
    v4r32 Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    Result.W = A.W * B.W;
    return Result;
}

inline v4r32
V4r32_MultiplyS(v4r32 V, r32 S)
{
    v4r32 Result;
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    Result.Z = V.Z * S;
    Result.W = V.W * S;
    return Result;
}

inline v4r32
V4r32_Divide(v4r32 A, v4r32 B)
{
    v4r32 Result;
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    Result.W = A.W / B.W;
    return Result;
}

inline v4r32
V4r32_DivideS(v4r32 V, r32 S)
{
    v4r32 Result;
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    Result.Z = V.Z / S;
    Result.W = V.W / S;
    return Result;
}

inline r32
V4r32_Dot(v4r32 A, v4r32 B)
{
    return A.X*B.X +
           A.Y*B.Y +
           A.Z*B.Z +
           A.W*B.W;
}

inline r32
V4r32_Length(v4r32 V)
{
    return Sqrt(V4r32_Dot(V, V));
}

inline v4r32
V4r32_Normalize(v4r32 V)
{
    r32 Len = V4r32_Length(V);
    if(Len == 0)
        return V;
    return V4r32_DivideS(V, Len);
}

inline b08
V4r32_Equal(v4r32 A, v4r32 B)
{
    if(AbsR(A.X - B.X) < EPSILON32 &&
       AbsR(A.Y - B.Y) < EPSILON32 &&
       AbsR(A.Z - B.Z) < EPSILON32 &&
       AbsR(A.W - B.W) < EPSILON32)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V4r32_NotEqual(v4r32 A, v4r32 B)
{
    if(AbsR(A.X - B.X) < EPSILON32 &&
       AbsR(A.Y - B.Y) < EPSILON32 &&
       AbsR(A.Z - B.Z) < EPSILON32 &&
       AbsR(A.W - B.W) < EPSILON32)
    {
        return FALSE;
    }
    return TRUE;
}

inline b08
V4r32_LessThan(v4r32 A, v4r32 B)
{
    if(V4r32_Length(A) < V4r32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
V4r32_GreaterThan(v4r32 A, v4r32 B)
{
    if(V4r32_Length(A) > V4r32_Length(B))
    {
        return TRUE;
    }
    return FALSE;
}

#define MATH_VOX_V4R32_H_
#endif