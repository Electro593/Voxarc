/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: R32 (m4x4r32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal m4x4r32
M4x4r32_1x1(r32 N)
{
    m4x4r32 Result = {0};
    
    Result.M[0][0] = N;
    Result.M[1][1] = N;
    Result.M[2][2] = N;
    Result.M[3][3] = N;
    
    return Result;
}

internal m4x4r32
M4x4r32_4x4(v4r32 Col0,
            v4r32 Col1,
            v4r32 Col2,
            v4r32 Col3)
{
    m4x4r32 Result;
    
    Result.V[0] = Col0;
    Result.V[1] = Col1;
    Result.V[2] = Col2;
    Result.V[3] = Col3;
    
    return Result;
}

internal v4r32
M4x4r32_Mul_1x4(m4x4r32 M,
                v4r32   V)
{
    v4r32 Result;
    
    Result.E[0] = (M.M[0][0] * V.E[0]) + (M.M[1][0] * V.E[1]) + (M.M[2][0] * V.E[2]) + (M.M[3][0] * V.E[3]);
    Result.E[1] = (M.M[0][1] * V.E[0]) + (M.M[1][1] * V.E[1]) + (M.M[2][1] * V.E[2]) + (M.M[3][1] * V.E[3]);
    Result.E[2] = (M.M[0][2] * V.E[0]) + (M.M[1][2] * V.E[1]) + (M.M[2][2] * V.E[2]) + (M.M[3][2] * V.E[3]);
    Result.E[3] = (M.M[0][3] * V.E[0]) + (M.M[1][3] * V.E[1]) + (M.M[2][3] * V.E[2]) + (M.M[3][3] * V.E[3]);
    
    return Result;
};

//TODO: SIMD

internal m4x4r32
M4x4r32_Mul_1x4m4(m4x4r32 A,
                  m4x4r32 B)
{
    m4x4r32 Result;
    
#if 1 //NOTE: Untested
    Result.V[0] = M4x4r32_Mul_1x4(A, B.V[0]);
    Result.V[1] = M4x4r32_Mul_1x4(A, B.V[1]);
    Result.V[2] = M4x4r32_Mul_1x4(A, B.V[2]);
    Result.V[3] = M4x4r32_Mul_1x4(A, B.V[3]);
#else
    Result.E[ 0] = (A.E[ 0] * B.E[ 0]) + (A.E[ 4] * B.E[ 1]) + (A.E[ 8] * B.E[ 2]) + (A.E[12] * B.E[ 3]);
    Result.E[ 1] = (A.E[ 1] * B.E[ 0]) + (A.E[ 5] * B.E[ 1]) + (A.E[ 9] * B.E[ 2]) + (A.E[13] * B.E[ 3]);
    Result.E[ 2] = (A.E[ 2] * B.E[ 0]) + (A.E[ 6] * B.E[ 1]) + (A.E[10] * B.E[ 2]) + (A.E[14] * B.E[ 3]);
    Result.E[ 3] = (A.E[ 3] * B.E[ 0]) + (A.E[ 7] * B.E[ 1]) + (A.E[11] * B.E[ 2]) + (A.E[15] * B.E[ 3]);
    Result.E[ 4] = (A.E[ 0] * B.E[ 4]) + (A.E[ 4] * B.E[ 5]) + (A.E[ 8] * B.E[ 6]) + (A.E[12] * B.E[ 7]);
    Result.E[ 5] = (A.E[ 1] * B.E[ 4]) + (A.E[ 5] * B.E[ 5]) + (A.E[ 9] * B.E[ 6]) + (A.E[13] * B.E[ 7]);
    Result.E[ 6] = (A.E[ 2] * B.E[ 4]) + (A.E[ 6] * B.E[ 5]) + (A.E[10] * B.E[ 6]) + (A.E[14] * B.E[ 7]);
    Result.E[ 7] = (A.E[ 3] * B.E[ 4]) + (A.E[ 7] * B.E[ 5]) + (A.E[11] * B.E[ 6]) + (A.E[15] * B.E[ 7]);
    Result.E[ 8] = (A.E[ 0] * B.E[ 8]) + (A.E[ 4] * B.E[ 9]) + (A.E[ 8] * B.E[10]) + (A.E[12] * B.E[11]);
    Result.E[ 9] = (A.E[ 1] * B.E[ 8]) + (A.E[ 5] * B.E[ 9]) + (A.E[ 9] * B.E[10]) + (A.E[13] * B.E[11]);
    Result.E[10] = (A.E[ 2] * B.E[ 8]) + (A.E[ 6] * B.E[ 9]) + (A.E[10] * B.E[10]) + (A.E[14] * B.E[11]);
    Result.E[11] = (A.E[ 3] * B.E[ 8]) + (A.E[ 7] * B.E[ 9]) + (A.E[11] * B.E[10]) + (A.E[15] * B.E[11]);
    Result.E[12] = (A.E[ 0] * B.E[12]) + (A.E[ 4] * B.E[13]) + (A.E[ 8] * B.E[14]) + (A.E[12] * B.E[15]);
    Result.E[13] = (A.E[ 1] * B.E[12]) + (A.E[ 5] * B.E[13]) + (A.E[ 9] * B.E[14]) + (A.E[13] * B.E[15]);
    Result.E[14] = (A.E[ 2] * B.E[12]) + (A.E[ 6] * B.E[13]) + (A.E[10] * B.E[14]) + (A.E[14] * B.E[15]);
    Result.E[15] = (A.E[ 3] * B.E[12]) + (A.E[ 7] * B.E[13]) + (A.E[11] * B.E[14]) + (A.E[15] * B.E[15]);
#endif
    
    return Result;
}

internal m4x4r32
M4x4r32_Translation(r32 X,
                    r32 Y,
                    r32 Z)
{
    m4x4r32 Result = M4x4r32_1x1(1.0f);
    
    Result.M[3][0] = X;
    Result.M[3][1] = Y;
    Result.M[3][2] = Z;
    
    return Result;
}

internal m4x4r32
M4x4r32_Scaling(r32 X,
                r32 Y,
                r32 Z)
{
    m4x4r32 Result = {0};
    
    Result.M[0][0] = X;
    Result.M[1][1] = Y;
    Result.M[2][2] = Z;
    Result.M[3][3] = 1;
    
    return Result;
}

internal m4x4r32
M4x4r32_Perspective(r32 FOV,
                    r32 AspectRatio,
                    r32 Near,
                    r32 Far)
{
    m4x4r32 Result = {0};
    
    //TODO: Replace this with R32_Tan
    r32 F = 1.0f / Tan(FOV / 2.0f);
    
    Result.M[0][0] = F / AspectRatio;
    Result.M[1][1] = -F;
    Result.M[2][2] = Far / (Near - Far);
    Result.M[2][3] = -1.0f;
    Result.M[3][2] = (Near * Far) / (Near - Far);
    
    return Result;
}

internal m4x4r32
M4x4r32_Orthographic(r32 Left,
                     r32 Right,
                     r32 Top,
                     r32 Bottom,
                     r32 Near,
                     r32 Far)
{
    m4x4r32 Result = {0};
    
    Result.M[0][0] =  2.0f / (Right - Left);
    Result.M[1][1] =  2.0f / (Top - Bottom);
    Result.M[2][2] = -2.0f / (Far - Near);
    Result.M[3][0] = -(Right + Left) / (Right - Left);
    Result.M[3][1] = -(Top + Bottom) / (Top - Bottom);
    Result.M[3][2] = -(Far + Near) / (Far - Near);
    Result.M[3][3] =  1.0f;
    
    return Result;
}

//TODO: Pre-multiply movel and view matrices before drawing, do
//      this with r64 matrices to maintain precision

internal m4x4r32
M4x4r32_LookAt(v3r32 Eye,
               v3r32 Center,
               v3r32 Up)
{
    m4x4r32 Result = M4x4r32_1x1(1.0f);
    
    v3r32 ZAxis = V3r32_Normalize(V3r32_Sub(Center, Eye));
    v3r32 XAxis = V3r32_Normalize(V3r32_Cross(ZAxis, Up));
    v3r32 YAxis = V3r32_Cross(XAxis, ZAxis);
    
    Result.M[0][0] =  XAxis.X;
    Result.M[1][0] =  XAxis.Y;
    Result.M[2][0] =  XAxis.Z;
    Result.M[0][1] =  YAxis.X;
    Result.M[1][1] =  YAxis.Y;
    Result.M[2][1] =  YAxis.Z;
    Result.M[0][2] = -ZAxis.X;
    Result.M[1][2] = -ZAxis.Y;
    Result.M[2][2] = -ZAxis.Z;
    Result.M[3][0] = -Eye.X;
    Result.M[3][1] = -Eye.Y;
    Result.M[3][2] =  Eye.Z;
    
    return Result;
}

//TODO: Rotation
internal m4x4r32
M4x4r32_ModelMatrix(v3r32 Translation,
                    v3r32 Scaling)
{
    m4x4r32 Result = {0};
    
    Result.M[0][0] = Scaling.X;
    Result.M[1][1] = Scaling.Y;
    Result.M[2][2] = Scaling.Z;
    Result.M[3][0] = Translation.X;
    Result.M[3][1] = Translation.Y;
    Result.M[3][2] = Translation.Z;
    Result.M[3][3] = 1.0f;
    
    return Result;
}