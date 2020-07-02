#ifndef MATH_VOX_M4X4R32_H_

#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "vox_v4r32.h"
#include "vox_v3r32.h"

typedef union m4x4r32
{
    v4r32 V[4];
    r32 M[4][4];
    r32 E[16];
} m4x4r32, m4r;

inline m4x4r32
M4x4r32_0()
{
    m4x4r32 Result;
    SetMemory(&Result, 0, sizeof(m4x4r32));
    Result.M[0][0] = 1;
    Result.M[1][1] = 1;
    Result.M[2][2] = 1;
    Result.M[3][3] = 1;
    return Result;
}

inline m4x4r32
M4x4r32_4_4_4_4(v4r32 Col1, v4r32 Col2, v4r32 Col3, v4r32 Col4)
{
    m4x4r32 Result;
    Result.V[0] = Col1;
    Result.V[1] = Col2;
    Result.V[2] = Col3;
    Result.V[3] = Col4;
    return Result;
}

inline m4x4r32
M4x4r32_Multiply(m4x4r32 A, m4x4r32 B)
{
    m4x4r32 Result;
    Result.E[ 0] = A.E[ 0]*B.E[ 0] + A.E[ 4]*B.E[ 1] + A.E[ 8]*B.E[ 2] + A.E[12]*B.E[ 3];
    Result.E[ 1] = A.E[ 1]*B.E[ 0] + A.E[ 5]*B.E[ 1] + A.E[ 9]*B.E[ 2] + A.E[13]*B.E[ 3];
    Result.E[ 2] = A.E[ 2]*B.E[ 0] + A.E[ 6]*B.E[ 1] + A.E[10]*B.E[ 2] + A.E[14]*B.E[ 3];
    Result.E[ 3] = A.E[ 3]*B.E[ 0] + A.E[ 7]*B.E[ 1] + A.E[11]*B.E[ 2] + A.E[15]*B.E[ 3];
    Result.E[ 4] = A.E[ 0]*B.E[ 4] + A.E[ 4]*B.E[ 5] + A.E[ 8]*B.E[ 6] + A.E[12]*B.E[ 7];
    Result.E[ 5] = A.E[ 1]*B.E[ 4] + A.E[ 5]*B.E[ 5] + A.E[ 9]*B.E[ 6] + A.E[13]*B.E[ 7];
    Result.E[ 6] = A.E[ 2]*B.E[ 4] + A.E[ 6]*B.E[ 5] + A.E[10]*B.E[ 6] + A.E[14]*B.E[ 7];
    Result.E[ 7] = A.E[ 3]*B.E[ 4] + A.E[ 7]*B.E[ 5] + A.E[11]*B.E[ 6] + A.E[15]*B.E[ 7];
    Result.E[ 8] = A.E[ 0]*B.E[ 8] + A.E[ 4]*B.E[ 9] + A.E[ 8]*B.E[10] + A.E[12]*B.E[11];
    Result.E[ 9] = A.E[ 1]*B.E[ 8] + A.E[ 5]*B.E[ 9] + A.E[ 9]*B.E[10] + A.E[13]*B.E[11];
    Result.E[10] = A.E[ 2]*B.E[ 8] + A.E[ 6]*B.E[ 9] + A.E[10]*B.E[10] + A.E[14]*B.E[11];
    Result.E[11] = A.E[ 3]*B.E[ 8] + A.E[ 7]*B.E[ 9] + A.E[11]*B.E[10] + A.E[15]*B.E[11];
    Result.E[12] = A.E[ 0]*B.E[12] + A.E[ 4]*B.E[13] + A.E[ 8]*B.E[14] + A.E[12]*B.E[15];
    Result.E[13] = A.E[ 1]*B.E[12] + A.E[ 5]*B.E[13] + A.E[ 9]*B.E[14] + A.E[13]*B.E[15];
    Result.E[14] = A.E[ 2]*B.E[12] + A.E[ 6]*B.E[13] + A.E[10]*B.E[14] + A.E[14]*B.E[15];
    Result.E[15] = A.E[ 3]*B.E[12] + A.E[ 7]*B.E[13] + A.E[11]*B.E[14] + A.E[15]*B.E[15];
    return Result;
}

inline v4r32
M4x4r32_MultiplyV(m4x4r32 M, v4r32 V)
{
    v4r32 Result;
    Result.E[0] = M.M[0][0]*V.E[0] + M.M[1][0]*V.E[1] + M.M[2][0]*V.E[2] + M.M[3][0]*V.E[3];
    Result.E[1] = M.M[0][1]*V.E[0] + M.M[1][1]*V.E[1] + M.M[2][1]*V.E[2] + M.M[3][1]*V.E[3];
    Result.E[2] = M.M[0][2]*V.E[0] + M.M[1][2]*V.E[1] + M.M[2][2]*V.E[2] + M.M[3][2]*V.E[3];
    Result.E[3] = M.M[0][3]*V.E[0] + M.M[1][3]*V.E[1] + M.M[2][3]*V.E[2] + M.M[3][3]*V.E[3];
    return Result;
};

local_func m4x4r32
M4x4r32_Inverse(m4x4r32 M)
{
    m4x4r32 Result;
    
    r32 Sub00 = M.M[2][2]*M.M[3][3] - M.M[3][2]*M.M[2][3];
    r32 Sub01 = M.M[2][1]*M.M[3][3] - M.M[3][1]*M.M[2][3];
    r32 Sub02 = M.M[2][1]*M.M[3][2] - M.M[3][1]*M.M[2][2];
    r32 Sub03 = M.M[2][0]*M.M[3][3] - M.M[3][0]*M.M[2][3];
    r32 Sub04 = M.M[2][0]*M.M[3][2] - M.M[3][0]*M.M[2][2];
    r32 Sub05 = M.M[2][0]*M.M[3][1] - M.M[3][0]*M.M[2][1];
    r32 Sub06 = M.M[1][2]*M.M[3][3] - M.M[3][2]*M.M[1][3];
    r32 Sub07 = M.M[1][1]*M.M[3][3] - M.M[3][1]*M.M[1][3];
    r32 Sub08 = M.M[1][1]*M.M[3][2] - M.M[3][1]*M.M[1][2];
    r32 Sub09 = M.M[1][0]*M.M[3][3] - M.M[3][0]*M.M[1][3];
    r32 Sub10 = M.M[1][0]*M.M[3][2] - M.M[3][0]*M.M[1][2];
    r32 Sub11 = M.M[1][0]*M.M[3][1] - M.M[3][0]*M.M[1][1];
    r32 Sub12 = M.M[1][2]*M.M[2][3] - M.M[2][2]*M.M[1][3];
    r32 Sub13 = M.M[1][1]*M.M[2][3] - M.M[2][1]*M.M[1][3];
    r32 Sub14 = M.M[1][1]*M.M[2][2] - M.M[2][1]*M.M[1][2];
    r32 Sub15 = M.M[1][0]*M.M[2][3] - M.M[2][0]*M.M[1][3];
    r32 Sub16 = M.M[1][0]*M.M[2][2] - M.M[2][0]*M.M[1][2];
    r32 Sub17 = M.M[1][0]*M.M[2][1] - M.M[2][0]*M.M[1][1];
    
    r32 Determinant = (M.M[0][0]*(M.M[1][1]*Sub00 - M.M[1][2]*Sub08 + M.M[1][3]*Sub02) -
                       M.M[0][1]*(M.M[1][0]*Sub00 - M.M[1][2]*Sub03 + M.M[1][3]*Sub04) +
                       M.M[0][2]*(M.M[1][0]*Sub08 - M.M[1][1]*Sub03 + M.M[1][3]*Sub05) -
                       M.M[0][3]*(M.M[1][0]*Sub02 - M.M[1][1]*Sub04 + M.M[1][2]*Sub05));
    if(Determinant)
    {
        Determinant = 1 / Determinant;
    }
    
    Result.M[0][0] = +(M.M[1][1]*Sub00 - M.M[1][2]*Sub08 + M.M[1][3]*Sub02) * Determinant;
    Result.M[0][1] = -(M.M[1][0]*Sub00 - M.M[1][2]*Sub03 + M.M[1][3]*Sub04) * Determinant;
    Result.M[0][2] = +(M.M[1][0]*Sub08 - M.M[1][1]*Sub03 + M.M[1][3]*Sub05) * Determinant;
    Result.M[0][3] = -(M.M[1][0]*Sub02 - M.M[1][1]*Sub04 + M.M[1][2]*Sub05) * Determinant;
    Result.M[1][0] = -(M.M[0][1]*Sub00 - M.M[0][2]*Sub08 + M.M[0][3]*Sub02) * Determinant;
    Result.M[1][1] = +(M.M[0][0]*Sub00 - M.M[0][2]*Sub03 + M.M[0][3]*Sub04) * Determinant;
    Result.M[1][2] = -(M.M[0][0]*Sub08 - M.M[0][1]*Sub03 + M.M[0][3]*Sub05) * Determinant;
    Result.M[1][3] = +(M.M[0][0]*Sub02 - M.M[0][1]*Sub04 + M.M[0][2]*Sub05) * Determinant;
    Result.M[2][0] = +(M.M[0][1]*Sub06 - M.M[0][2]*Sub07 + M.M[0][3]*Sub08) * Determinant;
    Result.M[2][1] = -(M.M[0][0]*Sub06 - M.M[0][2]*Sub09 + M.M[0][3]*Sub10) * Determinant;
    Result.M[2][2] = +(M.M[0][0]*Sub07 - M.M[0][1]*Sub09 + M.M[0][3]*Sub11) * Determinant;
    Result.M[2][3] = -(M.M[0][0]*Sub08 - M.M[0][1]*Sub10 + M.M[0][2]*Sub11) * Determinant;
    Result.M[3][0] = -(M.M[0][1]*Sub12 - M.M[0][2]*Sub13 + M.M[0][3]*Sub14) * Determinant;
    Result.M[3][1] = +(M.M[0][0]*Sub12 - M.M[0][2]*Sub15 + M.M[0][3]*Sub16) * Determinant;
    Result.M[3][2] = -(M.M[0][0]*Sub13 - M.M[0][1]*Sub15 + M.M[0][3]*Sub17) * Determinant;
    Result.M[3][3] = +(M.M[0][0]*Sub14 - M.M[0][1]*Sub16 + M.M[0][2]*Sub17) * Determinant;
    
    return Result;
}

inline m4x4r32
M4x4r32_Translate(m4x4r32 M, r32 X, r32 Y, r32 Z)
{
    m4x4r32 Result;
    Result.M[0][0] = M.E[ 0]+X*M.E[12];
    Result.M[1][0] = M.E[ 1]+X*M.E[13];
    Result.M[2][0] = M.E[ 2]+X*M.E[14];
    Result.M[3][0] = M.E[ 3]+X*M.E[15];
    Result.M[0][1] = M.E[ 4]+Y*M.E[12];
    Result.M[1][1] = M.E[ 5]+Y*M.E[13];
    Result.M[2][1] = M.E[ 6]+Y*M.E[14];
    Result.M[3][1] = M.E[ 7]+Y*M.E[15];
    Result.M[0][2] = M.E[ 8]+Z*M.E[12];
    Result.M[1][2] = M.E[ 9]+Z*M.E[13];
    Result.M[2][2] = M.E[10]+Z*M.E[14];
    Result.M[3][2] = M.E[11]+Z*M.E[15];
    Result.M[0][3] = M.E[12];
    Result.M[1][3] = M.E[13];
    Result.M[2][3] = M.E[14];
    Result.M[3][3] = M.E[15];
    return Result;
}

inline m4x4r32
M4x4r32_Translation(r32 X, r32 Y, r32 Z)
{
    m4x4r32 Result = M4x4r32_0();
    Result.M[3][0] = X;
    Result.M[3][1] = Y;
    Result.M[3][2] = Z;
    return Result;
}

inline m4x4r32
M4x4r32_RotateX(m4x4r32 M, r32 T)
{
    r32 C = Cos(T);
    r32 S = Sin(T);
    
    m4x4r32 Result;
    Result.M[0][0] = M.E[0];
    Result.M[1][0] = M.E[1];
    Result.M[2][0] = M.E[2];
    Result.M[3][0] = M.E[3];
    Result.M[1][1] = C*M.E[5] - S*M.E[9];
    Result.M[0][1] = C*M.E[4] - S*M.E[8];
    Result.M[2][1] = C*M.E[6] - S*M.E[10];
    Result.M[3][1] = C*M.E[7] - S*M.E[11];
    Result.M[0][2] = S*M.E[4] + C*M.E[8];
    Result.M[1][2] = S*M.E[5] + C*M.E[9];
    Result.M[2][2] = S*M.E[6] + C*M.E[10];
    Result.M[3][2] = S*M.E[7] + C*M.E[11];
    Result.M[0][3] = M.E[12];
    Result.M[1][3] = M.E[13];
    Result.M[2][3] = M.E[14];
    Result.M[3][3] = M.E[15];
    return Result;
}

inline m4x4r32
M4x4r32_RotateY(m4x4r32 M, r32 T)
{
    r32 C = Cos(T);
    r32 S = Sin(T);
    
    m4x4r32 Result;
    Result.M[0][0] = C*M.E[0] + S*M.E[8];
    Result.M[1][0] = C*M.E[1] + S*M.E[9];
    Result.M[2][0] = C*M.E[2] + S*M.E[10];
    Result.M[3][0] = C*M.E[3] + S*M.E[11];
    Result.M[0][1] = M.E[4];
    Result.M[1][1] = M.E[5];
    Result.M[2][1] = M.E[6];
    Result.M[3][1] = M.E[7];
    Result.M[0][2] = -S*M.E[0] + C*M.E[8];
    Result.M[1][2] = -S*M.E[1] + C*M.E[9];
    Result.M[2][2] = -S*M.E[2] + C*M.E[10];
    Result.M[3][2] = -S*M.E[3] + C*M.E[11];
    Result.M[0][3] = M.E[12];
    Result.M[1][3] = M.E[13];
    Result.M[2][3] = M.E[14];
    Result.M[3][3] = M.E[15];
    return Result;
}

inline m4x4r32
M4x4r32_Perspective(r32 FOV, r32 AspectRatio, r32 ZNear, r32 ZFar)
{
    r32 R = ZFar - ZNear;
    r32 F = 1.0f / Tan(FOV / 2);
    m4x4r32 Result;
    SetMemory(&Result, 0, sizeof(Result));
    Result.M[0][0] = F / AspectRatio;
    Result.M[1][1] = F;
    Result.M[2][2] = -(ZFar + ZNear) / R;
    Result.M[2][3] = -1;
    Result.M[3][2] = -(2 * ZFar * ZNear) / R;
    return Result;
}

inline m4x4r32
M4x4r32_LookAt(v3r32 Position, v3r32 Target, v3r32 WorldUp)
{
    v3r32 ZAxis = V3r32_Normalize(V3r32_Subtract(Position, Target));
    v3r32 XAxis = V3r32_Normalize(V3r32_Cross(V3r32_Normalize(WorldUp), ZAxis));
    v3r32 YAxis = V3r32_Cross(ZAxis, XAxis);
    
    m4x4r32 Translation = M4x4r32_0();
    SetMemory(&Translation, 0, sizeof(Translation));
    Translation.M[3][0] = -Position.X;
    Translation.M[3][1] = -Position.Y;
    Translation.M[3][2] = -Position.Z;
    m4x4r32 Rotation = M4x4r32_0();
    SetMemory(&Rotation, 0, sizeof(Rotation));
    Rotation.M[0][0] = XAxis.X;
    Rotation.M[1][0] = XAxis.Y;
    Rotation.M[2][0] = XAxis.Z;
    Rotation.M[0][1] = YAxis.X;
    Rotation.M[1][1] = YAxis.Y;
    Rotation.M[2][1] = YAxis.Z;
    Rotation.M[0][2] = ZAxis.X;
    Rotation.M[1][2] = ZAxis.Y;
    Rotation.M[2][2] = ZAxis.Z;
    return M4x4r32_Multiply(Rotation, Translation);
}

#define MATH_VOX_M4X4R32_H_
#endif