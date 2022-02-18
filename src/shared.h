/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define _OPENGL

#if defined(_MSVC)
    #define API_ENTRY  __stdcall
    #define API_IMPORT __declspec(dllimport)
    #define API_EXPORT __declspec(dllexport)
#elif defined(_GCC)
    #define API_ENTRY
    #define API_IMPORT
    #define API_EXPORT
#endif

#define global     static
#define persistent static
#define internal   static inline
#define external

#define RETURNS(...)

#define FORCE_CAST(Type, Variable)               (*(Type*)&(Variable))
#define LITERAL_CAST(OldType, Constant, NewType) (*(NewType*)&(OldType){Constant})
#define OFFSET_OF(Type, Element) ((u64)&(((Type*)0)->Element))
#define INDEX_2D(X, Y, MaxX) ((X) + ((Y) * (MaxX)))

#define Error(Message) Platform_Assert(__FILE__, __LINE__, "", Message)
#define Assert(Expression, ...) \
    do { \
        if(!(Expression)) { \
            Platform_Assert(__FILE__, __LINE__, #Expression, "" __VA_ARGS__); \
            STOP; \
        } \
    } while(0)

#ifdef _DEBUG
    #define STOP Intrin_DebugBreak()
    #define NOP Intrin_Nop();
#else
    #define STOP
    #define NOP
#endif

#define NULL ((vptr)0)
#define FALSE 0
#define TRUE  1
#define LESS  (-1)
#define EQUAL   0
#define GREATER 1

#define U32_MAX 0xFFFFFFFF

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

typedef struct platform_state platform_state;
typedef struct renderer_state renderer_state;
typedef struct game_state game_state;

typedef struct platform_exports platform_exports;
typedef struct renderer_exports renderer_exports;
typedef struct opengl_funcs opengl_funcs;
typedef struct game_exports game_exports;

#include <util/intrin.h>
#include <util/scalar.h>
#include <util/vector.h>
#include <util/bit.h>
#include <util/memory.h>
#include <util/string.h>
#include <renderer/renderer.h>
#include <platform/platform.h>
#include <game/game.h>

s32 _fltused;