/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal r32
R32_Floor(r32 N)
{
    if(N > 0) return (r32)((s32)N);
    if(N - (s32)N == 0) return N;
    return (r32)((s32)N - 1);
}

internal r32
R32_Ceil(r32 N)
{
    if(N < 0) return (r32)((s32)N);
    if(N - (s32)N == 0) return N;
    return (r32)((s32)N + 1);
}

internal r32
R32_Abs(r32 N)
{
    u32 Binary = FORCE_CAST(u32, N);
    Binary &= 0x7FFFFFFF;
    return FORCE_CAST(r32, Binary);
}

internal r32
R32_Sign(r32 N)
{
    u32 Binary = FORCE_CAST(u32, N);
    Binary &= 0x80000000;
    Binary |= 0x3F800000;
    return FORCE_CAST(r32, Binary);
}

internal r32
R32_Round(r32 N)
{
    return (r32)(s32)(N+R32_Sign(N)*0.5);
}

internal s32
R32_Exponent(r32 N)
{
    u32 Binary = FORCE_CAST(u32, N);
    Binary &= 0x7F800000;
    Binary >>= 23;
    s32 Signed = (s32)Binary - 127;
    return Signed;
}

// TODO make defines for these (like with the vectors)
internal r32
R32_Max(r32 A, r32 B)
{
    return (A < B) ? B : A;
}

internal r32
R32_Min(r32 A, r32 B)
{
    return (A < B) ? A : B;
}

internal r32
R32_Median(r32 A, r32 B, r32 C)
{
    return R32_Max(R32_Min(A, B), R32_Min(R32_Max(A, B), C));
}

internal r32
R32_Lerp(r32 S, r32 E, r32 t)
{
    return S + t*(E-S);
}

internal r32
R32_Remainder(r32 Dividend, r32 Divisor)
{
    return Dividend - ((s32)(Dividend/Divisor)*Divisor);
}

internal b08
R32_Within(r32 Num, r32 Start, r32 End, r32 Epsilon)
{
    return (Start-Epsilon < Num && Num < End+Epsilon);
}

internal r32
R32_Clamp(r32 N, r32 S, r32 E)
{
    if(N < S) return S;
    if(N > E) return E;
    return N;
}

internal r32
R32_sqrt(r32 N)
{
    r32 Result;
    Result = R128_Get32(R128_Sqrt32(R128_Set_4x32(N,N,N,N)), 0);
    return Result;
}

#define R32_CBRT_ITERATIONS 10
internal r32
R32_cbrt(r32 N)
{
    u32 Binary = FORCE_CAST(u32, N);
    s32 Exponent = (R32_Exponent(N) / 3) + 127;
    Binary = (Binary & ~R32_EXPONENT_MASK) | (Exponent << 23);
    r32 X = FORCE_CAST(r32, Binary);
    for(u32 I = 0; I < R32_CBRT_ITERATIONS; I++)
        X = (N/(X*X) + 2*X)/3;
    return X;
}

internal r32
R32_sin(r32 R)
{
    r32 D, F, R1, R2, Result, *LUT;
    u32 I;
    b08 N=FALSE;
    
    D = R*180/R32_PI;
    if(D < 0) {
        N = !N;
        D = -D;
    }
    if(D >= 360) D = (u32)D%360;
    if(D >= 270) {
        N = !N;
        D = 360 - D;
    }
    if(D >= 180) {
        N = !N;
        D -= 180;
    }
    if(D > 90) D = 180 - D;
    if(D > 45) {
        D = 90 - D;
        LUT = CosLookupTable;
    } else LUT = SinLookupTable;
    
    I = (u32)D;
    F = D - (r32)I;
    R1 = LUT[I];
    R2 = LUT[I+1];
    Result = R32_Lerp(R1, R2, F);
    
    return (N) ? -Result : Result;
}

internal r32
R32_cos(r32 R)
{
    r32 D, F, R1, R2, Result, *LUT;
    u32 I;
    b08 N=FALSE;
    
    D = R*180/R32_PI;
    if(D < 0) D = -D;
    if(D >= 360) D = (u32)D%360;
    if(D >= 270) D = 360 - D;
    if(D >= 180) {
        N = TRUE;
        D -= 180;
    }
    if(D > 90) {
        N = TRUE;
        D = 180 - D;
    }
    if(D > 45) {
        D = 90 - D;
        LUT = SinLookupTable;
    } else LUT = CosLookupTable;
    
    I = (u32)D;
    F = D - (r32)I;
    R1 = LUT[I];
    R2 = LUT[I+1];
    Result = R32_Lerp(R1, R2, F);
    
    return (N) ? -Result : Result;
}

internal r32
R32_tan(r32 R)
{
    //TODO: Make this use a lookup table
    return R32_sin(R) / R32_cos(R);
}

internal r32
R32_arccos(r32 N)
{
    if(N > 1) return R32_INF;
    if(N < -1) return -R32_INF;
    
    r32 F, R1, R2, Result;
    u32 I;
    b08 S=FALSE;
    
    if(N < 0) {
        S = TRUE;
        N = -N;
    }
    
    N *= sizeof(ArccosLookupTable)/sizeof(ArccosLookupTable[0]) - 1;
    I = (u32)N;
    F = N - (r32)I;
    R1 = ArccosLookupTable[I];
    R2 = ArccosLookupTable[I+1];
    Result = R32_Lerp(R1, R2, F);
    
    return (S) ? R32_PI - Result : Result;
}

internal u32
R32_SolveLinear(r32 C1, r32 C0,
                r32 *Roots)
{
    if(C1 == 0) return 0;
    C0 /= C1;
    
    Roots[0] = -C0;
    return 1;
}

internal u32
R32_SolveQuadratic(r32 C2, r32 C1, r32 C0,
                   r32 *Roots)
{
    if(C2 == 0) return R32_SolveLinear(C1, C0, Roots);
    C0 /= C2;
    C1 /= C2;
    
    r32 C1C1 = C1*C1;
    r32 D = C1C1 - 4*C0;
    if(D == 0) {
        Roots[0] = -C1 / 2;
        return 1;
    }
    if(D > 0) {
        r32 sqrtD = R32_sqrt(D);
        Roots[0] = (-C1 + sqrtD) / 2;
        Roots[1] = (-C1 - sqrtD) / 2;
        return 2;
    }
    return 0;
}

internal u32
R32_SolveCubic(r32 C3, r32 C2, r32 C1, r32 C0,
               r32 *Roots)
{
    if(C3 == 0) return R32_SolveQuadratic(C2, C1, C0, Roots);
    C0 /= C3;
    C1 /= C3;
    C2 /= C3;
    
    r32 C2C2 = C2*C2;
    r32 Q = (3*C1 - C2C2) / 9;
    r32 R = (9*C2*C1 - 27*C0 - 2*C2C2*C2) / 54;
    r32 QQQ = Q*Q*Q;
    r32 D = QQQ + R*R;
    r32 C2d3 = C2 / 3;
    if(D > 0) {
        r32 S = R32_cbrt(R + R32_sqrt(D));
        r32 T = R32_cbrt(R - R32_sqrt(D));
        Roots[0] = S+T - C2d3;
        return 1;
    }
    if(D == 0) {
        r32 SpT = 2*R32_cbrt(R);
        Roots[0] = SpT - C2d3;
        if(SpT == 0) return 1;
        Roots[1] = -SpT/2 - C2d3;
        return 2;
    }
    r32 Theta = R32_arccos(R/R32_sqrt(-QQQ));
    r32 sqrtnQt2 = 2*R32_sqrt(-Q);
    Roots[0] = sqrtnQt2*R32_cos(Theta/3) - C2d3;
    Roots[1] = sqrtnQt2*R32_cos((Theta+2*R32_PI)/3) - C2d3;
    Roots[2] = sqrtnQt2*R32_cos((Theta+4*R32_PI)/3) - C2d3;
    return 3;
}

internal s32
S32_Sign(s32 N)
{
    return 1 + 2*(N >> 31);
}

internal s32
S32_Abs(s32 N)
{
    return N * S32_Sign(N);
}

internal s32
S32_Clamp(s32 N, s32 S, s32 E)
{
    if(N < S) return S;
    if(N > E) return E;
    return N;
}

internal u08
U08_Max(u08 A, u08 B)
{
    return (A < B) ? B : A;
}

internal u08
U08_Min(u08 A, u08 B)
{
    return (A < B) ? A : B;
}

internal u08
U08_Median(u08 A, u08 B, u08 C)
{
    return U08_Max(U08_Min(A, B), U08_Min(U08_Max(A, B), C));
}

internal u08
U08_Lerp(u08 A, u08 B, r32 T)
{
    return A + (u08)(T * ((r32)B - A));
}





internal random
Rand_Init(u32 Seed)
{
    random Random;
    Random.LFSR32 = Seed % U32_MAX;                      // 32 Bit
    Random.LFSR31 = (Seed + 0x2F2F2F2F) % (U32_MAX / 2); // 31 Bit
    return Random;
}

//TODO: Rework this

internal u32
Rand_Int(random *Random)
{
    s32 FeedBack;
    u32 LFSR32 = Random->LFSR32;
    
    FeedBack = LFSR32 & 1;
    LFSR32 >>= 1;
    if(FeedBack == 1)
        LFSR32 ^= RAND_POLYNOMIAL_MASK32;
    FeedBack = LFSR32 & 1;
    LFSR32 >>= 1;
    if(FeedBack == 1)
        LFSR32 ^= RAND_POLYNOMIAL_MASK32;
    Random->LFSR32 = LFSR32;
    
    u32 LFSR31 = Random->LFSR31;
    FeedBack = LFSR31 & 1;
    LFSR31 >>= 1;
    if(FeedBack == 1)
        LFSR31 ^= RAND_POLYNOMIAL_MASK31;
    Random->LFSR31 = LFSR31;
    
    return (LFSR32 ^ LFSR31) & 0xFFFF;
}

//NOTE: Max is exclusive, so Min <= Result < Max
internal s32
Rand_IntRange(random *Random, s32 Min, s32 Max)
{
    Assert(Min != Max);
    s32 Result = Rand_Int(Random) % (Max - Min) + Min;
    return Result;
}