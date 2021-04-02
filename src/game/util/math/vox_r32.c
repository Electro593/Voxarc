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

//NOTE: In sin() and similar transcendentals, sin(pi) != 0 because pi is irrational.
//      Instead, it gives you pi's error, so sin(R32_PI) + R32_PI == 0

//SUBSECTION: Comparisons

//TODO: Check the CRT on these, as well as R32_Abs
internal r32
R32_Min(r32 A,
        r32 B)
{
    r32 Result = A;
    
    if(A > B)
    {
        Result = B;
    }
    
    return Result;
}

internal r32
R32_Max(r32 A,
        r32 B)
{
    r32 Result = A;
    
    if(A < B)
    {
        Result = B;
    }
    
    return Result;
}

internal b08
R32_EqualApprox(r32 A,
                r32 B,
                r32 Epsilon)
{
    b08 Result;
    
    r32 AbsA = R32_Abs(A);
    r32 AbsB = R32_Abs(B);
    r32 AbsDiff = R32_Abs(A - B);
    
    Result = AbsDiff <= (R32_Max(AbsA, AbsB) * Epsilon);
    
    return Result;
}

internal b08
R32_EqualStrict(r32 A,
                r32 B,
                r32 Epsilon)
{
    b08 Result;
    
    r32 AbsA = R32_Abs(A);
    r32 AbsB = R32_Abs(B);
    r32 AbsDiff = R32_Abs(A - B);
    
    Result = AbsDiff <= (R32_Min(AbsA, AbsB) * Epsilon);
    
    return Result;
}

internal s32
R32_Cmp(r32 A,
        r32 B)
{
    s32 Result;
    
    r32 AbsA = R32_Abs(A);
    r32 AbsB = R32_Abs(B);
    
    if((B - A) > (R32_Max(AbsA, AbsB) * R32_EPSILON))
    {
        Result = LESS;
    }
    else if((A - B) > (R32_Max(AbsA, AbsB) * R32_EPSILON))
    {
        Result = GREATER;
    }
    else
    {
        Result = EQUAL;
    }
    
    return Result;
}

//SUBSECTION: Calculations

internal r32
R32_Round(r32 N)
{
    r32 Result;
    
    Result = R32_Floor(N + 0.5f);
    
    return Result;
}

internal r32
R32_Abs(r32 N)
{
    r32 Result;
    
    u32 Binary = FORCE_CAST(N, u32);
    BIT_CLEAR(Binary, R32_BIT_SIGN);
    
    Result = FORCE_CAST(Binary, r32);
    
    return Result;
}

internal r32
R32_Mod(r32 Dividend,
        r32 Divisor)
{
    r32 Result;
    
    Result = Dividend / Divisor;
    s32 I = (s32)Result;
    Result -= I;
    
    return Result;
}

internal r32
R32_Lerp(r32 Start,
         r32 End,
         pct32 Percent)
{
    r32 Result;
    
    Result = Start + (Percent * (End - Start));
    
    return Result;
}

internal r32
R32_Floor(r32 N)
{
    u32 Binary = FORCE_CAST(N, u32);
    u32 M = Binary;
    
    b08 IsNegative = R32_GET_SIGN(Binary);
    if(IsNegative)
    {
        BIT_CLEAR(Binary, R32_BIT_SIGN);
    }
    
    if(Binary >= R32_HEX_MAXINT) // 23 - Exponent would be negative after this
    {
        if(Binary >= R32_HEX_QNAN) // Make the NaN an SNaN
        {
            BIT_SET(M, R32_BIT_NAN_SIGNAL);
        }
        return FORCE_CAST(M, r32);
    }
    
    if(Binary < R32_HEX_ONE)
    {
        if(IsNegative)
        {
            return -1.0f;
        }
        return 0.0f;
    }
    
    u32 Exponent = R32_GET_EXPONENT(M) - 0x7F;
    u32 Mask = (1 << (R32_BITS_MANTISSA - Exponent)) - 1;
    M &= ~Mask; // Mask away the decimal bits
    
    r32 Result = FORCE_CAST(M, r32);
    if(IsNegative)
    {
        if(N != Result)
        {
            Result--;
        }
    }
    
    return Result;
}

internal r32
R32_Ceil(r32 N)
{
    u32 Binary = FORCE_CAST(N, u32);
    u32 M = Binary;
    
    b08 IsNegative = R32_GET_SIGN(Binary);
    if(IsNegative)
    {
        BIT_CLEAR(Binary, R32_BIT_SIGN);
    }
    
    if(Binary >= R32_HEX_MAXINT)
    {
        if(Binary >= R32_HEX_QNAN)
        {
            BIT_SET(M, R32_BIT_NAN_SIGNAL);
        }
        return FORCE_CAST(M, r32);
    }
    
    if(Binary < R32_HEX_ONE)
    {
        if(Binary == R32_HEX_ZERO)
        {
            return 0.0f;
        }
        if(IsNegative)
        {
            return -0.0f;
        }
        return 1.0f;
    }
    
    u32 Exponent = R32_GET_EXPONENT(M) - 0x7F;
    u32 Mask = (1 << (R32_BITS_MANTISSA - Exponent)) - 1;
    M &= ~Mask;
    
    r32 Result = FORCE_CAST(M, r32);
    if(!IsNegative)
    {
        if(N != Result)
        {
            Result++;
        }
    }
    
    return Result;
}

internal r32
R32_Sqrt(r32 N)
{
    u32 Binary = FORCE_CAST(N, u32);
    b32 IsNegative = R32_GET_SIGN(Binary);
    
    if((Binary & R32_MASK_EXPONENT) == R32_HEX_INF)
    {
        if((Binary & R32_MASK_MANTISSA) != 0) // NaN
        {
            BIT_SET(Binary, R32_BIT_NAN_SIGNAL);
            
            return FORCE_CAST(Binary, r32);
        }
        
        if(IsNegative)
        {
            return R32_IND;
        }
    }
    
    if(IsNegative)
    {
        if(Binary != R32_HEX_NEG_ZERO)
        {
            return R32_IND;
        }
    }
    
    r128 Result = R128_SqrtR32_1(R128_SetR32_1(N));
    return Result.R32[0];
}

//TODO: Fix/improve these
internal r32
Sin(rad32 R)
{
    local_persist r32 FOPI = 1.27323954473516;
    local_persist r32 PIO4F = 0.7853981633974483096;
    local_persist r32 DP1 = 0.78515625;
    local_persist r32 DP2 = 2.4187564849853515625e-4;
    local_persist r32 DP3 = 3.77489497744594108e-8;
    local_persist r32 ErrThresh = 8192.;
    local_persist r32 T24M1 = 16777215.;
    local_persist r32 SinCof[] = {-1.9515295891E-4,
                               8.3321608736E-3,
                              -1.6666654611E-1};
    local_persist r32 CosCof[] = { 2.443315711809948E-005,
                              -1.388731625493765E-003,
                               4.166664568298827E-002};
    r32 *P;
    r32 X, Y, Z;
    u32 J;
    s32 Sign;
    
    Sign = 1;
    X = R;
    if(R < 0)
    {
        Sign = -1;
        X = -R;
    }
    
    if(X > T24M1)
    {
        return 0.0f;
    }
    
    J = (u32)(FOPI * X);
    Y = (r32)J;
    
    if(J & 1)
    {
        J += 1;
        Y += 1.0;
    }

    J &= 7;
    
    if(J > 3)
    {
        Sign = -Sign;
        J -= 4;
    }
    
    if(X > ErrThresh)
    {
        X = X - Y * PIO4F;
    }
    else
    {
        X = ((X - Y * DP1) - Y * DP2) - Y * DP3;
    }
    
    Z = X * X;
    if(J == 1 || J == 2)
    {
        P = CosCof;
        Y = *P++;
        Y = Y * Z + *P++;
        Y = Y * Z + *P++;
        Y *= Z * Z;
        Y -= 0.5f * Z;
        Y += 1.0f;
    }
    else
    {
        P = SinCof;
        Y = *P++;
        Y = Y * Z + *P++;
        Y = Y * Z + *P++;
        Y *= Z * X;
        Y += X;
    }
    
    if(Sign < 0)
    {
        Y = -Y;
    }
    
    return Y;
}

internal r32
Cos(rad32 R)
{
    local_persist r32 FOPI = 1.27323954473516;
    local_persist r32 PIO4F = 0.7853981633974483096;
    local_persist r32 DP1 = 0.78515625;
    local_persist r32 DP2 = 2.4187564849853515625e-4;
    local_persist r32 DP3 = 3.77489497744594108e-8;
    local_persist r32 ErrThresh = 8192.;
    local_persist r32 T24M1 = 16777215.;
    local_persist r32 SinCof[] = {-1.9515295891E-4,
                               8.3321608736E-3,
                              -1.6666654611E-1};
    local_persist r32 CosCof[] = { 2.443315711809948E-005,
                              -1.388731625493765E-003,
                               4.166664568298827E-002};
    r32 X, Y, Z;
    s32 J, Sign;
    
    Sign = 1;
    X = R;
    if(X < 0)
    {
        X = -X;
    }
    
    if(X > T24M1)
    {
        return 0.0f;
    }
    
    J = (u32)(FOPI * X);
    Y = (r32)J;
    
    if(J & 1)
    {
        J += 1;
        Y += 1.0;
    }

    J &= 7;
    
    if(J > 3)
    {
        J -= 4;
        Sign = -Sign;
    }
    
    if(J > 1)
    {
        Sign = -Sign;
    }
    
    if(X > ErrThresh)
    {
        X = X - Y * PIO4F;
    }
    else
    {
        X = ((X - Y * DP1) - Y * DP2) - Y * DP3;
    }
    
    Z = X * X;
    
    if(J == 1 || J == 2)
    {
        Y = (((-1.9515295891E-4f * Z +
                8.3321608736E-3f) * Z -
                1.6666654611E-1f) * Z * X) + X;
    }
    else
    {
        Y = ((2.443315711809948E-005f * Z -
              1.388731625493765E-003f) * Z +
              4.166664568298827E-002f) * Z * Z;
        Y -= 0.5f * Z;
        Y += 1.0f;
    }
    
    if(Sign < 0)
    {
        Y = -Y;
    }
    
    return Y;
}

internal r32
Tan(rad32 R)
{
    local_persist float DP1 = 0.78515625;
    local_persist float DP2 = 2.4187564849853515625E-4;
    local_persist float DP3 = 3.77489497744594108E-8;
    local_persist float FOPI = 1.27323954473516;
    local_persist float ErrThresh = 8192.;
    
    r32 X = R;
    s32 Sign = 1;
    if(R < 0.0f)
    {
        X = -R;
        Sign = -1;
    }
    
    if(X > ErrThresh)
    {
        return 0.0f;
    }
    
    u32 J = (u32)(FOPI * X);
    r32 Y = (r32)J;
    
    if(J & 1)
    {
        J += 1;
        Y += 1.0f;
    }
    
    r32 Z = ((X - Y * DP1) - Y * DP2) - Y * DP3;
    r32 ZZ = Z * Z;
    
    Y = Z;
    if(X > 1.0E-4)
    {
        Y = (((((9.38540185543E-3f * ZZ +
                 3.11992232697E-3f) * ZZ +
                 2.44301354525E-2f) * ZZ +
                 5.34112807005E-2f) * ZZ +
                 1.33387994085E-1f) * ZZ +
                 3.33331568548E-1f) * ZZ * Z + Z;
    }
    
    if(J & 2)
    {
        Y = -1.0f / Y;
    }
    
    if(Sign < 0)
    {
        Y = -Y;
    }
    
    return Y;
}

//SUBSECTION: Conversions

internal rad32
R32_Radians(deg32 Degrees)
{
    rad32 Result;
    
    Result = (Degrees * R32_PI) / (R32_DEGMAX / 2);
    
    return Result;
}

//CREDIT: https://stackoverflow.com/a/6930407
internal rgb32
R32_HSVToRGB(rad32 Hue, pct32 Saturation, pct32 Value)
{
    rgb32 Result;
    
    ASSERT(WITHIN(0.0f, Hue, 2 * R32_PI));
    ASSERT(WITHIN(0.0f, Saturation, 1.0f));
    ASSERT(WITHIN(0.0f, Value, 1.0f));
    
    r32 C = Value * Saturation;
    r32 HueSection = Hue / (R32_PI / 3); // H / (2pi / 6)
    r32 X = C * (1 - R32_Abs(R32_Mod(HueSection, 2) - 1));
    
    if(HueSection <= 1.0f)
    {
        Result.R = C;
        Result.G = X;
        Result.B = 0;
    }
    else if(HueSection <= 2.0f)
    {
        Result.R = X;
        Result.G = C;
        Result.B = 0;
    }
    else if(HueSection <= 3.0f)
    {
        Result.R = 0;
        Result.G = C;
        Result.B = X;
    }
    else if(HueSection <= 4.0f)
    {
        Result.R = 0;
        Result.G = X;
        Result.B = C;
    }
    else if(HueSection <= 5.0f)
    {
        Result.R = X;
        Result.G = 0;
        Result.B = C;
    }
    else
    {
        Result.R = C;
        Result.G = 0;
        Result.B = X;
    }
    
    r32 M = Value - C;
    Result.R += M;
    Result.G += M;
    Result.B += M;
    
    return Result;
}

internal v3u08
R32_RGBToV3u08(rgb32 RGB)
{
    v3u08 Result;
    
    ASSERT(WITHIN(0.0f, RGB.R, 1.0f));
    ASSERT(WITHIN(0.0f, RGB.G, 1.0f));
    ASSERT(WITHIN(0.0f, RGB.B, 1.0f));
    
    Result.X = (u08)(RGB.R * (r32)U08_MAX);
    Result.Y = (u08)(RGB.G * (r32)U08_MAX);
    Result.Z = (u08)(RGB.B * (r32)U08_MAX);
    
    return Result;
}

//SUBSECTION: Casts

internal str
R32_ToStr(str *Dest,
          r32 N)
{
    b08 Negative = FALSE;
    u32 NumLen = 0;
    u32 DecLen = R32_DIGITS_PRECISION_ROUNDING + 1;
    
    if(N < 0)
    {
        Negative = TRUE;
        N = -N;
    }
    if((s32)N == 0)
    {
        NumLen = 1;
    }
    
    r32 Num = N;
    while(Num >= 1)
    {
        Num /= 10;
        ++NumLen;
    }
    
    str Str = Str_Create(Dest, NULL, NumLen + DecLen + Negative);
    if(Negative)
    {
        Str[0] = '-';
    }
    Str[NumLen + Negative] = '.';
    
    s32 NumI = (s32)N;
    for(s32 Index = NumLen + Negative - 1;
        Index >= Negative;
        --Index)
    {
        Str[Index] = '0' + (NumI % 10);
        NumI /= 10;
    }
    
    r32 NumF = N - (s32)N;
    for(u32 Index = 1;
        Index < DecLen;
        ++Index)
    {
        NumF *= 10;
        Str[NumLen + Negative + Index] = '0' + ((s32)NumF % 10);
    }
    
    return Str;
}