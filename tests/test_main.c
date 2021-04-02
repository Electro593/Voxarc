#include <math.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <intrin.h>

#include <gl/GL.h>

#define internal static

#define UNUSED(...) ((void)__VA_ARGS__)
#define FORCE_CAST(Variable, Type) (*(Type*)&(Variable))
#define LITERAL_CAST(OldType, Constant, NewType) (*(NewType*)&(OldType){Constant})

#define BIT_SET(Bitstring, Index)    ((Bitstring) |=  (1 << (Index)))
#define BIT_TEST(Bitstring, Index)   ((Bitstring) &   (1 << (Index)))
#define BIT_CLEAR(Bitstring, Index)  ((Bitstring) &= ~(1 << (Index)))
#define BIT_TOGGLE(Bitstring, Index) ((Bitstring) ^=  (1 << (Index)))

#define NOP {u32 _Breakpoint_ = 0; UNUSED(_Breakpoint_);}
#define FALSE 0
#define TRUE 1

#define R32_BIT_NAN_SIGNAL 22
#define R32_BIT_SIGN       31

#define R32_MASK_MANTISSA 0x007FFFFF
#define R32_MASK_EXPONENT 0x7F800000
#define R32_MASK_SIGN     0x80000000

#define R32_HEX_DEN     0x00000001
#define R32_HEX_MIN     0x00800000
#define R32_HEX_EPSILON 0x34000000
#define R32_HEX_MAX     0x7F7FFFFF
#define R32_HEX_INF     0x7F800000
#define R32_HEX_QNAN    0x7F800001
#define R32_HEX_SNAN    0x7FC00000
#define R32_HEX_NZERO   0x80000000
#define R32_HEX_IND     0xFFC00000

#define R32_DEN      LITERAL_CAST(u32, R32_HEX_DEN,     r32) // 0 00000000 00000000000000000000001
#define R32_MIN      LITERAL_CAST(u32, R32_HEX_MIN,     r32) // 0 00000001 00000000000000000000000
#define R32_EPSILON  LITERAL_CAST(u32, R32_HEX_EPSILON, r32) // 0 01101000 00000000000000000000000
#define R32_MAX      LITERAL_CAST(u32, R32_HEX_MAX,     r32) // 0 11111110 11111111111111111111111
#define R32_INF      LITERAL_CAST(u32, R32_HEX_INF,     r32) // 0 11111111 00000000000000000000000
#define R32_QNAN     LITERAL_CAST(u32, R32_HEX_QNAN,    r32) // 0 11111111 00000000000000000000001
#define R32_SNAN     LITERAL_CAST(u32, R32_HEX_SNAN,    r32) // 0 11111111 10000000000000000000000
#define R32_IND      LITERAL_CAST(u32, R32_HEX_IND,     r32) // 1 11111111 10000000000000000000000

#define R128_SetR32_1(r32_Index0)     _mm_set_ss(r32_Index0)
#define R128_SqrtR32_1(r128_Radicand) _mm_sqrt_ss(r128_Radicand)

typedef __int32 s32;
typedef __m128 r128;
typedef unsigned __int8 u08;
typedef unsigned __int32 u32;
typedef float r32;
typedef u08 b08;
typedef u32 b32;

#include <Windows.h>

typedef __declspec(dllimport) void Testfunc(void);

void
main(void)
{
    HMODULE Dll = LoadLibraryA("Test2.dll");
    
    Testfunc();
    
#if 0
    r32 _Root0  = sqrtf( 0.0);
    r32 _Root1  = sqrtf(-0.0);
    r32 _Root2  = sqrtf( R32_MAX);
    r32 _Root3  = sqrtf(-R32_MAX);
    r32 _Root4  = sqrtf( R32_INF);
    r32 _Root5  = sqrtf(-R32_INF);
    r32 _Root6  = sqrtf( R32_SNAN);
    r32 _Root7  = sqrtf(-R32_SNAN);
    r32 _Root8  = sqrtf( R32_QNAN);
    r32 _Root9  = sqrtf(-R32_QNAN);
    r32 _Root10 = sqrtf(R32_IND);
    r32 _Root11 = sqrtf( R32_EPS);
    r32 _Root12 = sqrtf(-R32_EPS);
    r32 _Root13 = sqrtf( R32_MIN);
    r32 _Root14 = sqrtf(-R32_MIN);
    r32 _Root15 = sqrtf( R32_DEN);
    r32 _Root16 = sqrtf(-R32_DEN);
    r32 _Root17 = sqrtf( 16.0);
    r32 _Root18 = sqrtf( 9.0);
    r32 _Root19 = sqrtf(-9.0);
    
    
    r32 Root0  = R32_Sqrt( 0.0);
    r32 Root1  = R32_Sqrt(-0.0);
    r32 Root2  = R32_Sqrt( R32_MAX);
    r32 Root3  = R32_Sqrt(-R32_MAX);
    r32 Root4  = R32_Sqrt( R32_INF);
    r32 Root5  = R32_Sqrt(-R32_INF);
    r32 Root6  = R32_Sqrt( R32_SNAN);
    r32 Root7  = R32_Sqrt(-R32_SNAN);
    r32 Root8  = R32_Sqrt( R32_QNAN);
    r32 Root9  = R32_Sqrt(-R32_QNAN);
    r32 Root10 = R32_Sqrt(R32_IND);
    r32 Root11 = R32_Sqrt( R32_EPS);
    r32 Root12 = R32_Sqrt(-R32_EPS);
    r32 Root13 = R32_Sqrt( R32_MIN);
    r32 Root14 = R32_Sqrt(-R32_MIN);
    r32 Root15 = R32_Sqrt( R32_DEN);
    r32 Root16 = R32_Sqrt(-R32_DEN);
    r32 Root17 = R32_Sqrt( 16.0);
    r32 Root18 = R32_Sqrt( 9.0);
    r32 Root19 = R32_Sqrt(-9.0);
#endif
    
    NOP;
}