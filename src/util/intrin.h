/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_INTRIN_H_
#define UTIL_INTRIN_H_

//TODO: Write alternatives for intrinsics that aren't supported

#if _ARCH_X86 || _ARCH_X64

typedef union __declspec(intrin_type) __declspec(align(16)) __m128i
{
    s08 S08[16];
    s16 S16[ 8];
    s32 S32[ 4];
    s64 S64[ 2];
    
    u08 U08[16];
    u16 U16[ 8];
    u32 U32[ 4];
    u64 U64[ 2];
} i128;

typedef union __declspec(intrin_type) __declspec(align(16)) __m128
{
    s08 S08[16];
    s16 S16[ 8];
    s32 S32[ 4];
    s64 S64[ 2];
    
    u08 U08[16];
    u16 U16[ 8];
    u32 U32[ 4];
    u64 U64[ 2];
    
    r32 R32[ 4];
    r64 R64[ 2];
} r128;

u08 _BitScanReverse(u32 *Index, u32 Mask);
#define BitScanReverse(u32p__Index, u32__Mask) _BitScanReverse(u32p__Index, u32__Mask)

s32 _mm_popcnt_u32(u32 Value);
#define BitPopulationCount(u32__Value) _mm_popcnt_u32(u32__Value);

i128 _mm_set1_epi8(u08 Byte);
i128 _mm_set1_epi32(u32 DWord);
#define I128_Set_1x1(u08__Value) _mm_set1_epi8(u08__Value)
#define I128_Set_1x4(u32__Value) _mm_set1_epi32(u32__Value)

r128 _mm_set_ss(r32 Index0);
#define R128_SetR32_1(r32__Index0) _mm_set_ss(r32__Index0)

r128 _mm_sqrt_ss(r128 Radicand);
#define R128_SqrtR32_1(r128__Radicand) _mm_sqrt_ss(r128__Radicand)

long _InterlockedCompareExchange(volatile s32 *Destination, s32 Exchange, s32 Comparand);
#define InterlockedCompareExchange(s32pv__Destination, s32__Exchange, s32__Comparand) _InterlockedCompareExchange(s32pv__Destination, s32__Exchange, s32__Comparand)

long _InterlockedIncrement(volatile s32 *Addend);
#define InterlockedIncrement(s32pv__Addend) _InterlockedIncrement(s32pv__Addend)

void _mm_sfence(void);
#define WriteFence() _mm_sfence()

#endif

void _WriteBarrier(void);
#define WriteBarrier() _WriteBarrier()

vptr _ReturnAddress(void);

void __nop(void);
void __debugbreak(void);

#endif