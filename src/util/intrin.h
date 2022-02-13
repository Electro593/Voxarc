/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef union __declspec(intrin_type) __declspec(align(16)) __m128 {
    r32 R32[4];
    r64 R64[2];
} r128;

void __debugbreak(void);
void __nop(void);
u64  __readgsqword(u32 Offset);
r128 _mm_sqrt_ps(r128);
r128 _mm_set_ps(r32, r32, r32, r32);

#define Asm_ReadGSQWord(u32__Offset) RETURNS(u64)  __readgsqword(u32__Offset)
#define Intrin_DebugBreak()          RETURNS(void) __debugbreak();
#define Intrin_Nop()                 RETURNS(void) __nop();

#define R128_Set_4x32(_0,_1,_2,_3) _mm_set_ps(_0,_1,_2,_3)
#define R128_Sqrt_4(R128) (_mm_sqrt_ps(R128))