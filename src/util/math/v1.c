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
//      SECTION: R32
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//NOTE: In sin() and similar transcendentals, sin(pi) != 0 because pi is irrational.
//      Instead, it gives you pi's error, so sin(R32_PI) + R32_PI == 0

//SUBSECTION: Comparisons

internal b08
R32_IsNAN(r32 N)
{
    b08 Result;
    
    u32 Binary = FORCE_CAST(N, u32);
    Result = (b08)BIT_GET(Binary, R32_BIT_NAN_SIGNAL);
    
    return Result;
}

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
    u32 IsNegative = R32_GET_SIGN(Binary);
    
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

internal r32
R32_Cbrt(r32 N)
{
    STOP;
    return N;
}

// Formula: Ax + B = 0
internal u32
R32_RootsDeg1(r32 *Roots,
              r32 A,
              r32 B)
{
    if(R32_Cmp(A, 0) == EQUAL) // B = 0
    {
        if(R32_Cmp(B, 0) == EQUAL) // 0 = 0
            return U32_MAX;
        
        return 0;
    }
    
    Roots[0] = -B / A;
    return 1;
}

// Formula: Ax^2 + Bx + C = 0
internal u32
R32_RootsDeg2(r32 *Roots,
              r32 A,
              r32 B,
              r32 C)
{
    if(R32_Cmp(A, 0) == EQUAL)
        return R32_RootsDeg1(Roots, B, C);
    
    r32 Diff = B*B - 4*A*C;
    if(R32_Cmp(Diff, 0) == GREATER)
    { // To not use sqrt if necessary
        Diff = R32_Sqrt(Diff);
        Roots[0] = (-B + Diff) / (2*A);
        Roots[1] = (-B - Diff) / (2*A);
        return 2;
    }
    if(R32_Cmp(Diff, 0) == EQUAL)
    {
        Roots[0] = -B / (2*A);
        return 1;
    }
    return 0;
}

internal u32
R32_RootsDeg3(r32 *Roots,
              r32 A,
              r32 B,
              r32 C,
              r32 D)
{
    if(R32_Cmp(A, 0) == EQUAL)
        return R32_RootsDeg2(Roots, B, C, D);
    
    B /= A;
    C /= A;
    D /= A;
    r32 BB = B*B;
    r32 Q = (3*C - BB) / 9;
    r32 R = (B*(-2*BB + 9*C) - 27*D) / 54;
    r32 QQQ = Q*Q*Q;
    r32 D1 = QQQ + R*R;
    B /= 3;
    s32 Cmp = R32_Cmp(D1, 0);
    if(Cmp == LESS)
    {
        r32 Theta = R32_Acos(R / R32_Sqrt(-QQQ));
        Q = 2 * R32_Sqrt(-Q);
        Theta /= 3;
        Roots[0] = Q*R32_Cos(Theta) - B;
        Roots[1] = Q*R32_Cos(Theta + LITERAL_CAST(u32, 0x40060A92/* 2PI/3 */, r32)) - B;
        Roots[2] = Q*R32_Cos(Theta + LITERAL_CAST(u32, 0x40860A92/* 4PI/3 */, r32)) - B;
        return 3;
    }
    
    r32 T;
    D1 = R32_Sqrt(D1);
    r32 S = R32_Cbrt(R + D1); //TODO: Generalized pow function
    if(R32_Cmp(D, 0) == EQUAL)
        T = S;
    else
        T = R32_Cbrt(R - D1);
    r32 SpT = S + T;
    r32 SmT = S - T;
    Roots[0] = SpT - B;
    if(Cmp == GREATER)
    {
        ASSERT(!R32_IsNAN(Roots[0]));
        return 1;
    }
    
    Roots[1] = -.5f*SpT - B;
    ASSERT(!R32_IsNAN(Roots[1]));
    return 2;
}

//TODO: Fix/improve these
internal r32
Sin(rad32 R)
{
    persist r32 FOPI = 1.27323954473516;
    persist r32 PIO4F = 0.7853981633974483096;
    persist r32 DP1 = 0.78515625;
    persist r32 DP2 = 2.4187564849853515625e-4;
    persist r32 DP3 = 3.77489497744594108e-8;
    persist r32 ErrThresh = 8192.;
    persist r32 T24M1 = 16777215.;
    persist r32 SinCof[] = {-1.9515295891E-4,
                               8.3321608736E-3,
                              -1.6666654611E-1};
    persist r32 CosCof[] = { 2.443315711809948E-005,
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
    persist r32 FOPI = 1.27323954473516;
    persist r32 PIO4F = 0.7853981633974483096;
    persist r32 DP1 = 0.78515625;
    persist r32 DP2 = 2.4187564849853515625e-4;
    persist r32 DP3 = 3.77489497744594108e-8;
    persist r32 ErrThresh = 8192.;
    persist r32 T24M1 = 16777215.;
    persist r32 SinCof[] = {-1.9515295891E-4,
                               8.3321608736E-3,
                              -1.6666654611E-1};
    persist r32 CosCof[] = { 2.443315711809948E-005,
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
    persist float DP1 = 0.78515625;
    persist float DP2 = 2.4187564849853515625E-4;
    persist float DP3 = 3.77489497744594108E-8;
    persist float FOPI = 1.27323954473516;
    persist float ErrThresh = 8192.;
    
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

internal r32
Acos(rad32 R)
{
    ASSERT(-1 <= R && R <= 1);
    
    r32 Summation = 0;
    for(u32 N = 0; N < 4; ++N)
    {
        r32 RN = R;
        for(u32 Pow = 1; Pow < 2*N+1; ++Pow)
            RN *= R;
        
        r32 Numer = U32_Factorial[2*N] * RN;
        r32 Denom = ((1 << (2*N)) * (r32)U32_Factorial[N]*U32_Factorial[N]) * (2*N + 1);
        Summation += Numer / Denom;
    }
    
    return R32_PI/2 - Summation;
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

internal str
R32_To_Str(str *Dest,
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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: S16
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal s16
S16_Abs(s16 N)
{
    s16 Result;
    
    s16 Mask = N >> S16_SHIFT_SIGN;
    Result = (N + Mask) ^ Mask;
    
    return Result;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: S32
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//NOTE: Returns -1 if N < 0, 1 if N > 0
internal s32
S32_Sign(s32 N)
{
    s32 Result;
    
    s32 Sign = N >> S32_SHIFT_SIGN; // -1 or 0
    Result = (Sign * 2) + 1;
    
    return Result;
}

internal s32
S32_Lerp(s32 A,
         s32 B,
         r32 Percent)
{
    s32 Result;
    
    Result = (s32)((r32)A + (Percent * (r32)(B - A)));
    
    return Result;
}

internal s32
S32_Abs(s32 N)
{
    s32 Result;
    
    s32 Mask = N >> S32_SHIFT_SIGN;
    Result = (N + Mask) ^ Mask;
    
    return Result;
}

//SUBSECTION: Conversions

internal str
S32_To_Str(str *Dest,
          s32 N)
{
    u32 NumLen = 0;
    b08 Negative = FALSE;
    
    if(N < 0)
    {
        N = -N;
        Negative = TRUE;
    }
    if(N == 0)
    {
        NumLen = 1;
    }
    else
    {
        s32 Num = N;
        while(Num)
        {
            Num /= 10;
            ++NumLen;
        }
    }
    
    str Str = Str_Create(Dest, NULL, NumLen + Negative);
    if(Negative)
    {
        Str[0] = '-';
    }
    
    do
    {
        Str[--NumLen + Negative] = '0' + (N % 10);
        N /= 10;
    }
    while(N);
    
    ASSERT(NumLen == 0);
    
    return Str;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U32
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal r32
U32_InvLerp(u32 Start,
            u32 End,
            u32 X)
{
    r32 Result;
    
    ASSERT(X >= Start && X <= End);
    ASSERT(End > Start);
    
    Result = (r32)(X - Start) / (r32)(End - Start);
    
    return Result;
}

internal u32
U32_DivUp(u32 Dividend,
          u32 Divisor)
{
    r32 Quotient = (r32)Dividend / (r32)Divisor;
    u32 Result = (u32)R32_Ceil(Quotient);
    
    return Result;
}

//SUBSECTION: Conversions

internal str
U32_To_Str(str *Dest,
          u32 N)
{
    u32 NumLen = 0;
    
    if(N == 0)
    {
        NumLen = 1;
    }
    else
    {
        u32 Num = N;
        while(Num)
        {
            Num /= 10;
            ++NumLen;
        }
    }
    
    str Str = Str_Create(Dest, NULL, NumLen);
    
    do
    {
        Str[--NumLen] = '0' + (N % 10);
        N /= 10;
    }
    while(N);
    
    ASSERT(NumLen == 0);
    
    return Str;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: U64
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: Conversions

//TODO: Use the num type instead of individual u64, u32, etc?
//      A Num_To_Str()

internal str
U64_To_Str(str *Dest,
          u64 N)
{
    u32 NumLen = 0;
    u64 Num = N;
    while(Num)
    {
        Num /= 10;
        ++NumLen;
    }
    
    str Str = Str_Create(Dest, NULL, NumLen);
    
    do
    {
        Str[NumLen--] = '0' + (N % 10);
        N /= 10;
    }
    while(N);
    
    return Str;
}

internal str
U64_To_Str_Hex(str *Dest,
              u64 N)
{
    u32 NumLen = U64_MAX_CHARS_HEX - 1;
    
    //TODO: Test this
    str Str = Str_Create(Dest, "0x", NumLen);
    
    while(NumLen > 2) // Length of "0x"
    {
        if(N % 16 < 10)
        {
            Str[--NumLen] = '0' + (N % 16);
        }
        else
        {
            Str[--NumLen] = 'A' + (N % 16 - 10);
        }
        
        N /= 16;
    }
    
    return Str;
}