/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define _DEBUG
#define _MSVC
#define API_ENTRY  __stdcall
#define API_IMPORT __declspec(dllimport)
#define API_EXPORT __declspec(dllexport)
#define _X64

#define global     static
#define persistant static
#define internal   static inline
#define external

#define RETURNS(...)

#define FORCE_CAST(Variable, Type)               (*(Type*)&(Variable))
#define LITERAL_CAST(OldType, Constant, NewType) (*(NewType*)&(OldType){Constant})
#define OFFSET_OF(Type, Element) ((u64)&(((Type*)0)->Element))

#ifdef _DEBUG
    #define STOP Intrin_DebugBreak()
    #define NOP Intrin_Nop();
    #define ERROR(Expression, Message) { if(!(Expression)) { Platform->Assert(__FILE__, __LINE__, Message); STOP; } }
    #define ASSERT(Expression) { if(!(Expression)) { STOP; } }
#else
    #define STOP
    #define NOP
    #define ERROR(Expression, Message) ((void)(Expression))
    #define ASSERT(...) ((void)(Expression))
#endif

#define NULL ((vptr)0)
#define FALSE 0
#define TRUE  1
#define LESS  (-1)
#define EQUAL   0
#define GREATER 1



typedef signed   __int8  s08;
typedef signed   __int16 s16;
typedef signed   __int32 s32;
typedef signed   __int64 s64;

typedef unsigned __int8  u08;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef float  r32;
typedef double r64;

typedef void* vptr;
typedef void (*fptr)(void);

typedef s08 b08;
typedef s32 b32;

typedef u08 c08;
typedef u16 c16;

#include <util/intrin.h>
#include <platform/win32.h>
#include <platform/entry.h>

s32 _fltused;