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
//      SECTION: Generators
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//NOTE: JAI would make this so much better...
#if _VEC_GENERATORS

#define V3_GEN_CASES \
    CASE(r32, r32) CASE(r64, r64) \
    CASE(s08, s32) CASE(s16, s32) CASE(s32, s32) CASE(s64, s64) \
    CASE(u08, u32) CASE(u16, u32) CASE(u32, u32) CASE(u64, u64)

#define CASE(vt, rt) /* v3t V3t_1x1(t N) */ \
    internal v3##vt \
    V3##vt##_1x1(vt N) \
    { \
        return (v3##vt){N, N, N}; \
    }
V3_GEN_CASES
#undef CASE

#define CASE(vt, rt) /* v3t V3t_3x1(t X, t Y, t Z) */ \
    internal v3##vt \
    V3##vt##_3x1(vt X, vt Y, vt Z) \
    { \
        return (v3##vt){X, Y, Z}; \
    }
V3_GEN_CASES
#undef CASE

#define CASE(vt, rt) /* v3vt V3vt_Dot(v3vt A, v3vt B) */ \
    internal rt \
    V3##vt##_Dot(v3##vt A, v3##vt B) \
    { \
        return (rt)A.X*(rt)B.X + (rt)A.Y*(rt)B.Y * (rt)A.Z*(rt)B.Z; \
    }
V3_GEN_CASES
#undef CASE

#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: R32 (v3r32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if !_VEC_GENERATORS
internal v3r32
V3r32_1x1(r32 N)
{
    v3r32 Result;
    
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    
    return Result;
}

internal v3r32
V3r32_3x1(r32 X,
            r32 Y,
            r32 Z)
{
    v3r32 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    
    return Result;
}

internal r32
V3r32_Dot(v3r32 A,
          v3r32 B)
{
    r32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y +
             A.Z * B.Z;
    
    return Result;
}
#endif

internal v3r32
V3r32_Add(v3r32 A,
          v3r32 B)
{
    v3r32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    
    return Result;
}

internal v3r32
V3r32_Sub(v3r32 A,
          v3r32 B)
{
    v3r32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    
    return Result;
}

internal v3r32
V3r32_Mul_V_V(v3r32 A,
              v3r32 B)
{
    v3r32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    
    return Result;
}

internal v3r32
V3r32_Mul_V_S(v3r32 V,
              r32 S)
{
    v3r32 Result;
    
    Result.X = V.X * S;
    Result.Y = V.Y * S;
    Result.Z = V.Z * S;
    
    return Result;
}

internal v3r32
V3r32_Div_V_V(v3r32 A,
              v3r32 B)
{
    ASSERT(B.X != 0 && B.Y != 0 && B.Z != 0);
    
    v3r32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    
    return Result;
}

internal v3r32
V3r32_Div_V_S(v3r32 V,
              r32 S)
{
    ASSERT(S != 0);
    
    v3r32 Result;
    
    Result.X = V.X / S;
    Result.Y = V.Y / S;
    Result.Z = V.Z / S;
    
    return Result;
}

internal v3r32
V3r32_Div_S_V(r32 S,
              v3r32 V)
{
    ASSERT(V.X != 0 && V.Y != 0 && V.Z != 0);
    
    v3r32 Result;
    
    Result.X = S / V.X;
    Result.Y = S / V.Y;
    Result.Z = S / V.Z;
    
    return Result;
}

internal v3r32
V3r32_AddS(v3r32 V,
           r32 S)
{
    v3r32 Result;
    
    Result.X = V.X + S;
    Result.Y = V.Y + S;
    Result.Z = V.Z + S;
    
    return Result;
}

internal v3r32
V3r32_SubS(v3r32 V,
           r32 S)
{
    v3r32 Result;
    
    Result.X = V.X - S;
    Result.Y = V.Y - S;
    Result.Z = V.Z - S;
    
    return Result;
}

internal v3r32
V3r32_Cross(v3r32 A,
            v3r32 B)
{
    v3r32 Result;
    
    Result.X = (A.Y * B.Z) - (A.Z * B.Y);
    Result.Y = (A.Z * B.X) - (A.X * B.Z);
    Result.Z = (A.X * B.Y) - (A.Y * B.X);
    
    return Result;
}

internal r32
V3r32_Len(v3r32 V)
{
    r32 Result;
    
    Result = (r32)V3r32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal v3r32
V3r32_Normalize(v3r32 V)
{
    v3r32 Result;
    
    r32 Len = V3r32_Len(V);
    if(Len)
    {
        Result = V3r32_Div(V, V3r32_1x1(Len));
    }
    else
    {
        Result = V;
    }
    
    return Result;
}

internal v3r32
V3r32_Clamp(v3r32 Min,
            v3r32 V,
            v3r32 Max)
{
    v3r32 Result;
    
    Result.X = MAX(Min.X, MIN(V.X, Max.X));
    Result.Y = MAX(Min.Y, MIN(V.Y, Max.Y));
    Result.Z = MAX(Min.Z, MIN(V.Z, Max.Z));
    
    return Result;
}

internal b08
V3r32_Equal(v3r32 A,
            v3r32 B)
{
    b08 Result = FALSE;
    
    if(R32_Equal(A.X, B.X) &&
       R32_Equal(A.Y, B.Y) &&
       R32_Equal(A.Z, B.Z))
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V3r32_Cmp(v3r32 A,
              v3r32 B)
{
    s32 Result;
    
    r32 ALen = V3r32_Len(A);
    r32 BLen = V3r32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}

//SUBSECTION: Casts

internal v3s32
V3r32_To_V3s32(v3r32 V)
{
    v3s32 Result;
    
    Result = V3s32_3x1((s32)V.X, (s32)V.Y, (s32)V.Z);
    
    return Result;
}

internal v3u16
V3r32_RoundTo_V3u16(v3r32 V)
{
    v3u16 Result;
    
    Result = V3u16_3x1((u16)R32_Round(V.X),
                       (u16)R32_Round(V.Y),
                       (u16)R32_Round(V.Z));
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: S32 (v3s32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if !_VEC_GENERATORS
internal v3s32
V3s32_1x1(s32 N)
{
    v3s32 Result;
    
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    
    return Result;
}

internal v3s32
V3s32_3x1(s32 X,
            s32 Y,
            s32 Z)
{
    v3s32 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    
    return Result;
}

internal s32
V3s32_Dot(v3s32 A,
          v3s32 B)
{
    s32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y +
             A.Z * B.Z;
    
    return Result;
}
#endif

internal v3s32
V3s32_Add(v3s32 A,
          v3s32 B)
{
    v3s32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    
    return Result;
}

internal v3s32
V3s32_Sub(v3s32 A,
          v3s32 B)
{
    v3s32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    
    return Result;
}

internal v3s32
V3s32_Mul(v3s32 A,
          v3s32 B)
{
    v3s32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    
    return Result;
}

internal v3s32
V3s32_Div(v3s32 A,
          v3s32 B)
{
    ASSERT(B.X != 0 && B.Y != 0 && B.Z != 0);
    
    v3s32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    
    return Result;
}

internal v3s32
V3s32_Cross(v3s32 A,
            v3s32 B)
{
    v3s32 Result;
    
    Result.X = (A.Y * B.Z) - (A.Z * B.Y);
    Result.Y = (A.Z * B.X) - (A.X * B.Z);
    Result.Z = (A.X * B.Y) - (A.Y * B.X);
    
    return Result;
}

internal r32
V3s32_Len(v3s32 V)
{
    r32 Result;
    
    Result = (r32)V3s32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal b08
V3s32_Equal(v3s32 A,
            v3s32 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V3s32_Cmp(v3s32 A,
              v3s32 B)
{
    s32 Result;
    
    r32 ALen = V3s32_Len(A);
    r32 BLen = V3s32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}

//SUBSECTION: Casts

internal v3r32
V3s32_To_V3r32(v3s32 V)
{
    v3r32 Result;
    
    Result.X = (r32)V.X;
    Result.Y = (r32)V.Y;
    Result.Z = (r32)V.Z;
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U08 (v3u08)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if !_VEC_GENERATORS
internal v3u08
V3u08_3x1(u08 X,
          u08 Y,
          u08 Z)
{
    v3u08 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    
    return Result;
}

internal u32
V3u08_Dot(v3u08 A,
          v3u08 B)
{
    u32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y +
             A.Z * B.Z;
    
    return Result;
}
#endif

internal r32
V3u08_Len(v3u08 V)
{
    r32 Result;
    
    Result = (r32)V3u08_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal s32
V3u08_Cmp(v3u08 A,
          v3u08 B)
{
    s32 Result;
    
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z)
        Result = 0;
    else
    {
        r32 ALen = V3u08_Len(A);
        r32 BLen = V3u08_Len(B);
        Result = R32_Cmp(ALen, BLen);
    }
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U16 (v3u16)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if !_VEC_GENERATORS
internal v3u16
V3u16_3x1(u16 X,
          u16 Y,
          u16 Z)
{
    v3u16 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    
    return Result;
}
#endif

internal v3r32
V3u16_To_V3r32(v3u16 V)
{
    v3r32 Result;
    
    Result.X = (r32)V.X;
    Result.Y = (r32)V.Y;
    Result.Z = (r32)V.Z;
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U32 (v3u32)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if !_VEC_GENERATORS
internal v3u32
V3u32_1x1(u32 N)
{
    v3u32 Result;
    
    Result.X = N;
    Result.Y = N;
    Result.Z = N;
    
    return Result;
}

internal v3u32
V3u32_3x1(u32 X,
          u32 Y,
          u32 Z)
{
    v3u32 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    
    return Result;
}

internal u32
V3u32_Dot(v3u32 A,
          v3u32 B)
{
    u32 Result;
    
    Result = A.X * B.X +
             A.Y * B.Y +
             A.Z * B.Z;
    
    return Result;
}
#endif

internal v3u32
V3u32_Add(v3u32 A,
          v3u32 B)
{
    v3u32 Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    
    return Result;
}

internal v3u32
V3u32_Sub(v3u32 A,
          v3u32 B)
{
    v3u32 Result;
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    
    return Result;
}

internal v3u32
V3u32_Mul(v3u32 A,
          v3u32 B)
{
    v3u32 Result;
    
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    
    return Result;
}

internal v3u32
V3u32_Div(v3u32 A,
          v3u32 B)
{
    ASSERT(B.X != 0 && B.Y != 0 && B.Z != 0);
    
    v3u32 Result;
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    
    return Result;
}

internal v3u32
V3u32_Cross(v3u32 A,
            v3u32 B)
{
    v3u32 Result;
    
    Result.X = (A.Y * B.Z) - (A.Z * B.Y);
    Result.Y = (A.Z * B.X) - (A.X * B.Z);
    Result.Z = (A.X * B.Y) - (A.Y * B.X);
    
    return Result;
}

internal r32
V3u32_Len(v3u32 V)
{
    r32 Result;
    
    Result = (r32)V3u32_Dot(V, V);
    Result = R32_Sqrt(Result);
    
    return Result;
}

internal b08
V3u32_Equal(v3u32 A,
            v3u32 B)
{
    b08 Result = FALSE;
    
    if(A.X == B.X &&
       A.Y == B.Y &&
       A.Z == B.Z)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal s32
V3u32_Cmp(v3u32 A,
          v3u32 B)
{
    s32 Result;
    
    r32 ALen = V3u32_Len(A);
    r32 BLen = V3u32_Len(B);
    
    Result = R32_Cmp(ALen, BLen);
    
    return Result;
}