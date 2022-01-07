/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void __debugbreak(void);
void __nop(void);
u64  __readgsqword(u32 Offset);

#define Asm_ReadGSQWord(u32__Offset) RETURNS(u64)  __readgsqword(u32__Offset)
#define Intrin_DebugBreak()          RETURNS(void) __debugbreak();
#define Intrin_Nop()                 RETURNS(void) __nop();