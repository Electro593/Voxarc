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
//      SECTION: R32 (v2r32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v2r32
V2r32_1x1(r32 N)
{
    v2r32 Result;
    
    Result.X = N;
    Result.Y = N;
    
    return Result;
}

internal v2r32
V2r32_2x1(r32 X,
          r32 Y)
{
    v2r32 Result;
    
    Result.X = X;
    Result.Y = Y;
    
    return Result;
}

internal v2r32
V2r32_Add(v2r32 A,
          v2r32 B)
{
    v2r32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    
    return Result;
}

internal v2r32
V2r32_Sub(v2r32 A,
          v2r32 B)
{
    v2r32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    
    return Result;
}

internal v2r32
V2r32_Mul(v2r32 V,
          r32 S)
{
    v2r32 Result;
    
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    
    return Result;
}

internal v2r32
V2r32_Div(v2r32 A,
          v2r32 B)
{
    ASSERT(B.X != 0 && B.Y != 0);
    
    v2r32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    
    return Result;
}

internal v2r32
V2r32_DivS(v2r32 V,
           r32 S)
{
    v2r32 Result;
    
    ASSERT(S != 0);
    
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    
    return Result;
}

internal r32
V2r32_Dot(v2r32 A,
          v2r32 B)
{
    r32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y;
    
    return Result;
}

internal r32
V2r32_Len(v2r32 V)
{
    r32 Result;
    
    Result = (r32)V2r32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal r32
V2r32_Cross(v2r32 A,
            v2r32 B)
{
    return (A.X * B.Y) - (A.Y * B.X);
}

internal v2r32
V2r32_Normalize(v2r32 V)
{
    v2r32 Result;
    
    r32 Len = V2r32_Len(V);
    if(Len)
    {
        Result = V2r32_DivS(V, Len);
    }
    else
    {
        Result = V;
    }
    
    return Result;
}

internal b08
V2r32_Equal(v2r32 A,
            v2r32 B)
{
    b08 Result = FALSE;
    
    if(R32_Equal(A.X, B.X) &&
       R32_Equal(A.Y, B.Y))
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V2r32_Cmp(v2r32 A,
              v2r32 B)
{
    s32 Result;
    
    r32 ALen = V2r32_Len(A);
    r32 BLen = V2r32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: S16 (v2s16)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v2s16
V2s16_1x1(s16 N)
{
    v2s16 Result;
    
    Result.X = N;
    Result.Y = N;
    
    return Result;
}

internal v2s16
V2s16_2x1(s16 X,
          s16 Y)
{
    v2s16 Result;
    
    Result.X = X;
    Result.Y = Y;
    
    return Result;
}

internal v2s16
V2s16_Add(v2s16 A,
          v2s16 B)
{
    v2s16 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    
    return Result;
}

internal v2s16
V2s16_Sub(v2s16 A,
          v2s16 B)
{
    v2s16 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    
    return Result;
}

internal v2s16
V2s16_Lerp(v2s16 A,
           v2s16 B,
           pct32 T)
{
    v2s16 Result;
    
    v2s16 Diff = V2s16_Sub(B, A);
    v2s16 Frac = V2s16_2x1((s16)((r32)Diff.X * T), (s16)((r32)Diff.Y * T));
    Result = V2s16_Add(A, Frac);
    
    return Result;
}

internal s32
V2s16_Dot(v2s16 A,
          v2s16 B)
{
    s32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y;
    
    return Result;
}

internal r32
V2s16_Len(v2s16 V)
{
    r32 Result;
    
    Result = (r32)V2s16_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal r32
V2s16_Cross(v2s16 A,
            v2s16 B)
{
    r32 Result;
    
    v2r32 NormA = V2r32_Normalize(V2s16_To_V2r32(A));
    v2r32 NormB = V2r32_Normalize(V2s16_To_V2r32(B));
    
    Result = (NormA.X * NormB.Y) - (NormA.Y * NormB.X);
    
    return Result;
}

//SUBSECTION: Conversions

internal v2r32
V2s16_To_V2r32(v2s16 V)
{
    v2r32 Result;
    
    Result.X = (r32)V.X;
    Result.Y = (r32)V.Y;
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: S32 (v2s32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v2s32
V2s32_1x1(s32 N)
{
    v2s32 Result;
    
    Result.X = N;
    Result.Y = N;
    
    return Result;
}

internal v2s32
V2s32_2x1(s32 X,
          s32 Y)
{
    v2s32 Result;
    
    Result.X = X;
    Result.Y = Y;
    
    return Result;
}

internal v2s32
V2s32_Add(v2s32 A,
          v2s32 B)
{
    v2s32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    
    return Result;
}

internal v2s32
V2s32_Sub(v2s32 A,
          v2s32 B)
{
    v2s32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    
    return Result;
}

internal v2s32
V2s32_Mul(v2s32 A,
          v2s32 B)
{
    v2s32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    
    return Result;
}

internal v2s32
V2s32_Div(v2s32 A,
          v2s32 B)
{
    ASSERT(B.X != 0 && B.Y != 0);
    
    v2s32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    
    return Result;
}

internal s32
V2s32_Dot(v2s32 A,
          v2s32 B)
{
    s32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y;
    
    return Result;
}

internal r32
V2s32_Len(v2s32 V)
{
    r32 Result;
    
    Result = (r32)V2s32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal b08
V2s32_Equal(v2s32 A,
            v2s32 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V2s32_Cmp(v2s32 A,
          v2s32 B)
{
    s32 Result;
    
    r32 ALen = V2s32_Len(A);
    r32 BLen = V2s32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U16 (v2u16)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v2u16
V2u16_1x1(u16 N)
{
    v2u16 Result;
    
    Result.X = N;
    Result.Y = N;
    
    return Result;
}

internal v2u16
V2u16_2x1(u16 X,
          u16 Y)
{
    v2u16 Result;
    
    Result.X = X;
    Result.Y = Y;
    
    return Result;
}

internal v2u16
V2u16_Swap(v2u16 V)
{
    v2u16 Result;
    
    Result.X = V.Y;
    Result.Y = V.X;
    
    return Result;
}

internal u32
V2u16_RectArea(v2u16 V)
{
    u16 Result;
    
    Result = V.X * V.Y;
    
    return Result;
}

internal b08
V2u16_IsWithin(v2u16 A,
               v2u16 B)
{
    b08 Result = FALSE;
    
    if(A.X >= B.X &&
       A.Y >= B.Y)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal b08
V2u16_Equal(v2u16 A,
            v2u16 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        Result = TRUE;
    }
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U32 (v2u32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal v2u32
V2u32_1x1(u32 N)
{
    v2u32 Result;
    
    Result.X = N;
    Result.Y = N;
    
    return Result;
}

internal v2u32
V2u32_2x1(u32 X,
          u32 Y)
{
    v2u32 Result;
    
    Result.X = X;
    Result.Y = Y;
    
    return Result;
}

internal v2u32
V2u32_Add(v2u32 A,
          v2u32 B)
{
    v2u32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    
    return Result;
}

internal v2u32
V2u32_Sub(v2u32 A,
          v2u32 B)
{
    v2u32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    
    return Result;
}

internal v2u32
V2u32_Mul(v2u32 A,
          v2u32 B)
{
    v2u32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    
    return Result;
}

internal v2u32
V2u32_Div(v2u32 A,
          v2u32 B)
{
    ASSERT(B.X != 0 && B.Y != 0);
    
    v2u32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    
    return Result;
}

internal u32
V2u32_Dot(v2u32 A,
          v2u32 B)
{
    u32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y;
    
    return Result;
}

internal r32
V2u32_Len(v2u32 V)
{
    r32 Result;
    
    Result = (r32)V2u32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal b08
V2u32_Equal(v2u32 A,
            v2u32 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V2u32_Cmp(v2u32 A,
              v2u32 B)
{
    s32 Result;
    
    r32 ALen = V2u32_Len(A);
    r32 BLen = V2u32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}

//SUBSECTION: Conversions

internal v2r32
V2u32_To_V2r32(v2u32 V)
{
    v2r32 Result;
    
    Result.X = (r32)V.X;
    Result.Y = (r32)V.Y;
    
    return Result;
}