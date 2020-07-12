#ifndef MATH_VOX_MATH_H_

#define EPSILON32 .0000001192092896F
#define TAU 6.283185307179586476925286766559‬0
#define PI 3.1415926535897932384626433832795

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define SWAP(A, B, T) {T Swap = (A); (A) = (B); (B) = Swap;}
#define Index2d(X, Y, MaxX) ((X) + ((Y) * (MaxX)))
#define Index3d(X, Y, Z, MaxX, MaxY) ((X) + ((MaxX) * ((Y) + ((Z) *( MaxY)))))

//NOTE: Scalar

typedef union convert_r32_s32
{
    r32 R32;
    s32 S32;
} convert_r32_s32;

inline r32
R32_Abs(r32 N)
{
    convert_r32_s32 RTS_N;
    RTS_N.R32 = N;
    convert_r32_s32 RTS_Z = {-0.0f};
    RTS_N.S32 = RTS_N.S32 & ~RTS_Z.S32;
    return RTS_N.R32;
}

inline s32
S32_Abs(s32 N)
{
    s32 Result = N * ((N > 0) - (N < 0));
    return Result;
}

inline r32
Floor(r32 N)
{
    if(N >= R32_MAX || N <= R32_MIN || N != N)
    {
        return N;
    }
    s32 I = (s32)N;
    r32 D = (r32)I;
    if(D == N || N >= 0)
    {
        return D;
    }
    return D - 1;
}

inline r32
R32_Lerp(r32 A,
         r32 B,
         r32 T)
{
    r32 Result = A + (T * (B - A));
    return Result;
}

inline r32
S32_Lerp(s32 A,
         s32 B,
         r32 T)
{
    r32 Result = (r32)A + (T * (r32)(B - A));
    return Result;
}

inline r32
Radians(r32 D)
{
    r32 Result = (D * (r32)PI) / 180.0f;
    return Result;
}

inline r32
Sin(r32 R)
{
    local_var r32 FOPI = 1.27323954473516;
    local_var r32 PIO4F = 0.7853981633974483096;
    local_var r32 DP1 = 0.78515625;
    local_var r32 DP2 = 2.4187564849853515625e-4;
    local_var r32 DP3 = 3.77489497744594108e-8;
    local_var r32 ErrThresh = 8192.;
    local_var r32 T24M1 = 16777215.;
    local_var r32 SinCof[] = {-1.9515295891E-4,
                               8.3321608736E-3,
                              -1.6666654611E-1};
    local_var r32 CosCof[] = { 2.443315711809948E-005,
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

inline r32
Cos(r32 R)
{
    local_var r32 FOPI = 1.27323954473516;
    local_var r32 PIO4F = 0.7853981633974483096;
    local_var r32 DP1 = 0.78515625;
    local_var r32 DP2 = 2.4187564849853515625e-4;
    local_var r32 DP3 = 3.77489497744594108e-8;
    local_var r32 ErrThresh = 8192.;
    local_var r32 T24M1 = 16777215.;
    local_var r32 SinCof[] = {-1.9515295891E-4,
                               8.3321608736E-3,
                              -1.6666654611E-1};
    local_var r32 CosCof[] = { 2.443315711809948E-005,
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

inline r32
Tan(r32 R)
{
    local_var float DP1 = 0.78515625;
    local_var float DP2 = 2.4187564849853515625E-4;
    local_var float DP3 = 3.77489497744594108E-8;
    local_var float FOPI = 1.27323954473516;
    local_var float ErrThresh = 8192.;
    
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

inline r32
SqrtP(r32 N,
      u32 Precision)
{
    r32 X = 50.0f;
    while(Precision--)
    {
        X = (X*0.5f) + (N/(2*X));
    }
    
    return X;
}

inline r32
Sqrt(r32 N)
{
    r32 Result = SqrtP(N, 8);
    return Result;
}

#define MATH_VOX_MATH_H_
#endif