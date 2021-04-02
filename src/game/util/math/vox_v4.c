/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
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

//TODO: SIMD


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: R32 (v4r32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v4r32
V4r32_1x1(r32 N)
{
    v4r32 Result;
    
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    Result.W = N;
    
    return Result;
}

internal v4r32
V4r32_4x1(r32 X,
          r32 Y,
          r32 Z,
          r32 W)
{
    v4r32 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    
    return Result;
}

internal v4r32
V4r32_1x3_1x1(v3r32 XYZ,
              r32 W)
{
    v4r32 Result;
    
    Result.XYZ = XYZ;
    Result.W = W;
    
    return Result;
}

internal v4r32
V4r32_Add(v4r32 A,
          v4r32 B)
{
    v4r32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;
    
    return Result;
}

internal v4r32
V4r32_Sub(v4r32 A,
          v4r32 B)
{
    v4r32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;
    
    return Result;
}

internal v4r32
V4r32_Mul(v4r32 A,
          v4r32 B)
{
    v4r32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    Result.W = A.W * B.W;
    
    return Result;
}

internal v4r32
V4r32_Div(v4r32 A,
          v4r32 B)
{
    ASSERT(B.X != 0 && B.Y != 0 && B.Z != 0 && B.W != 0);
    
    v4r32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    Result.W = A.W / B.W;
    
    return Result;
}

internal r32
V4r32_Dot(v4r32 A,
          v4r32 B)
{
    r32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y +
             A.Z * B.Z +
             A.W * B.W;
    
    return Result;
}

internal r32
V4r32_Len(v4r32 V)
{
    r32 Result;
    
    Result = (r32)V4r32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal v4r32
V4r32_Normalize(v4r32 V)
{
    v4r32 Result;
    
    r32 Len = V4r32_Len(V);
    if(Len)
    {
        Result = V4r32_Div(V, V4r32_1x1(Len));
    }
    else
    {
        Result = V;
    }
    
    return Result;
}

internal b08
V4r32_Equal(v4r32 A,
            v4r32 B)
{
    b08 Result = FALSE;
    
    if(R32_Equal(A.X, B.X) &&
       R32_Equal(A.Y, B.Y) &&
       R32_Equal(A.Z, B.Z) &&
       R32_Equal(A.W, B.W))
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V4r32_Cmp(v4r32 A,
              v4r32 B)
{
    s32 Result;
    
    r32 ALen = V4r32_Len(A);
    r32 BLen = V4r32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}

//SUBSECTION: Casts

internal bfs32
V4r32_ToBfs32_Round(v4r32 V,
                    bf_format Format)
{
    bfs32 Result;
    
    switch(Format)
    {
        case BFF_1X2_3X10:
        {
            Result = Bfs32_1x2_3x10((s32)R32_Round(V.X),
                                    (s32)R32_Round(V.Y),
                                    (s32)R32_Round(V.Z),
                                    (s32)R32_Round(V.W));
        } break;
        
        default:
        {
            STOP;
            return 0;
        }
    }
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U08 (v4u08)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v4u08
V4u08_4x1(u08 X,
          u08 Y,
          u08 Z,
          u08 W)
{
    v4u08 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U16 (v4u16)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v4u16
V4u16_1x1(u16 N)
{
    v4u16 Result;
    
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    Result.W = N;
    
    return Result;
}

internal v4u16
V4u16_4x1(u16 X,
          u16 Y,
          u16 Z,
          u16 W)
{
    v4u16 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    
    return Result;
}

internal v4u16
V4u16_2x2(v2u16 XY,
          v2u16 ZW)
{
    v4u16 Result;
    
    Result.X = XY.X;
    Result.Y = XY.Y;
    Result.Z = ZW.X;
    Result.W = ZW.Y;
    
    return Result;
}

//NOTE: Origin is bottom-left, returns {0, 0, 0, 0} if no overlap
internal v4u16
V4u16_RectOverlap(v4u16 A,
                  v4u16 B)
{
    v4u16 Result;
    
    u16 Left = MAX(A.X, B.X);
    u16 Bottom = MAX(A.Y, B.Y);
    u16 Right = MIN(A.X + A.Z, B.X + B.Z);
    u16 Top = MIN(A.Y + A.W, B.Y + B.W);
    
    if(Left < Right && Bottom < Top)
    {
        Result = V4u16_4x1(Left, Bottom, Right - Left, Top - Bottom);
    }
    else
    {
        Result = V4u16_1x1(0);
    }
    
    return Result;
}

internal b08
V4u16_Equal(v4u16 A,
            v4u16 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z &&
       A.W == B.W)
    {
        Result = TRUE;
    }
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U32 (v4u32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v4u32
V4u32_1x1(u32 N)
{
    v4u32 Result;
    
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    Result.W = N;
    
    return Result;
}

internal v4u32
V4u32_4x1(u32 X,
              u32 Y,
              u32 Z,
              u32 W)
{
    v4u32 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    
    return Result;
}

internal v4u32
V4u32_Add(v4u32 A,
          v4u32 B)
{
    v4u32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;
    
    return Result;
}

internal v4u32
V4u32_Sub(v4u32 A,
          v4u32 B)
{
    v4u32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;
    
    return Result;
}

internal v4u32
V4u32_Mul(v4u32 A,
          v4u32 B)
{
    v4u32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    Result.W = A.W * B.W;
    
    return Result;
}

internal v4u32
V4u32_Div(v4u32 A,
          v4u32 B)
{
    ASSERT(B.X != 0 && B.Y != 0 && B.Z != 0 && B.W != 0);
    
    v4u32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    Result.W = A.W / B.W;
    
    return Result;
}

internal u32
V4u32_Dot(v4u32 A,
          v4u32 B)
{
    u32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y +
             A.Z * B.Z +
             A.W * B.W;
    
    return Result;
}

internal r32
V4u32_Len(v4u32 V)
{
    r32 Result;
    
    Result = (r32)V4u32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal v4u32
V4u32_Normalize(v4u32 V)
{
    v4u32 Result;
    
    r32 Len = V4u32_Len(V);
    if(Len)
    {
        Result = V4u32_Div(V, V4u32_1x1((u32)Len));
    }
    else
    {
        Result = V;
    }
    
    return Result;
}

internal b08
V4u32_Equal(v4u32 A,
            v4u32 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z &&
       A.W == B.W)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V4u32_Cmp(v4u32 A,
              v4u32 B)
{
    s32 Result;
    
    r32 ALen = V4u32_Len(A);
    r32 BLen = V4u32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}