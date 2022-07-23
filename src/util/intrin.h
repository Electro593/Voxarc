/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_X64) && defined(_MSVC)

typedef union __declspec(intrin_type) __declspec(align(16)) __m128 {
    r32 R32[4];
    r64 R64[2];
} r128;

void __debugbreak(void);
void __nop(void);
u64  __readgsqword(u32 Offset);
u64 __popcnt64(u64 Value);
u08 _BitScanForward64(u32 *Index, u64 Mask);
u08 _BitScanReverse(u32 *Index, u32 Mask);
u08 _BitScanReverse64(u32 *Index, u64 Mask);
r128 _mm_sqrt_ps(r128);
r128 _mm_set_ps(r32, r32, r32, r32);

#define Asm_ReadGSQWord(u32__Offset)                    RETURNS(u64)  __readgsqword(u32__Offset)
#define Intrin_DebugBreak()                             RETURNS(void) __debugbreak();
#define Intrin_Nop()                                    RETURNS(void) __nop();
#define Intrin_Popcount64(u64_Value)                    RETURNS(u64)  __popcnt64(u64_Value);
#define Intrin_BitScanForward64(u32_p_Index, u64_Value) RETURNS(b08)  _BitScanForward64(u32_p_Index, u64_Value);
#define Intrin_BitScanReverse(u32_p_Index, u32_Value)   RETURNS(b08)  _BitScanReverse(u32_p_Index, u32_Value);
#define Intrin_BitScanReverse64(u32_p_Index, u64_Value) RETURNS(b08)  _BitScanReverse(u32_p_Index, u64_Value);

#define R128_Set_4x32(_0,_1,_2,_3) _mm_set_ps(_0,_1,_2,_3)
#define R128_Sqrt32(R128) (_mm_sqrt_ps(R128))
#define R128_Get32(R128, Index) (R128.R32[Index])

typedef u08* va_list;
void __va_start(va_list *Args, ...);
#define VA_Start(Args, Last) ((void)(__va_start(&Args, Last)))
#define VA_End(Args) ((void)(Args = NULL))
#define VA_Next(Args, Type) \
    ((sizeof(Type) > 8 || (sizeof(Type) & (sizeof(Type) - 1)) != 0) /*Pointer or not a power of 2*/ \
        ? **(Type**)((Args += 8) - 8) \
        :  *(Type* )((Args += 8) - 8))

#endif