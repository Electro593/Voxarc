#ifndef MATH_VOX_M3X3R32_H_

#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "vox_v3r32.h"

typedef union m3x3r32
{
    v3r32 V[3];
    r32 M[3][3];
    r32 E[9];
} m3x3r32, m3r;

inline m3x3r32
M3x3r32_0()
{
    m3x3r32 Result;
    SetMemory(&Result, 0, sizeof(m3x3r32));
    Result.M[0][0] = 1;
    Result.M[1][1] = 1;
    Result.M[2][2] = 1;
    return Result;
}

inline m3x3r32
M3x3r32_3(v3r32 Col1, v3r32 Col2, v3r32 Col3)
{
    m3x3r32 Result;
    Result.V[0] = Col1;
    Result.V[1] = Col2;
    Result.V[2] = Col3;
    return Result;
}

inline v3r32
M3x3r32_Multiply(m3x3r32 M, v3r32 V)
{
    v3r32 Result;
    Result.E[0] = V.E[0]*M.M[0][0] + V.E[1]*M.M[1][0] + V.E[2]*M.M[2][0];
    Result.E[1] = V.E[0]*M.M[0][1] + V.E[1]*M.M[1][1] + V.E[2]*M.M[2][1];
    Result.E[2] = V.E[0]*M.M[0][2] + V.E[1]*M.M[1][2] + V.E[2]*M.M[2][2];
    return Result;
};

inline m3x3r32
Inverse(m3x3r32 M)
{
    r32 C = 1 / (M.M[0][0]*(M.M[1][1]*M.M[2][2] - M.M[2][1]*M.M[1][2]) -
                 M.M[1][0]*(M.M[0][1]*M.M[2][2] - M.M[2][1]*M.M[0][2]) +
                 M.M[2][0]*(M.M[0][1]*M.M[1][2] - M.M[1][1]*M.M[0][2]));
    m3x3r32 Result;
    Result.M[0][0] = +(M.M[1][1]*M.M[2][2] - M.M[2][1]*M.M[1][2]) * C;
    Result.M[1][0] = -(M.M[1][0]*M.M[2][2] - M.M[2][0]*M.M[1][2]) * C;
    Result.M[2][0] = +(M.M[1][0]*M.M[2][1] - M.M[2][0]*M.M[1][1]) * C;
    Result.M[0][1] = -(M.M[0][1]*M.M[2][2] - M.M[2][1]*M.M[0][2]) * C;
    Result.M[1][1] = +(M.M[0][0]*M.M[2][2] - M.M[2][0]*M.M[0][2]) * C;
    Result.M[2][1] = -(M.M[0][0]*M.M[2][1] - M.M[2][0]*M.M[0][1]) * C;
    Result.M[0][2] = +(M.M[0][1]*M.M[1][2] - M.M[1][1]*M.M[0][2]) * C;
    Result.M[1][2] = -(M.M[0][0]*M.M[1][2] - M.M[1][0]*M.M[0][2]) * C;
    Result.M[2][2] = +(M.M[0][0]*M.M[1][1] - M.M[1][0]*M.M[0][1]) * C;
    return Result;
}

#define MATH_VOX_M3X3R32_H_
#endif