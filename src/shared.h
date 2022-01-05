/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//
// Defines
//
#if _MODULE == 0 // PLATFORM
#   define _MODULE_PLATFORM 1
#   define   _API_PLATFORM   _API_EXPORT
#   define _API_GAME       _API_IMPORT
#   define _API_UTIL       _API_IMPORT
#elif _MODULE == 1 // GAME
#   define _MODULE_GAME 1
#   define _API_PLATFORM   _API_IMPORT
#   define   _API_GAME       _API_EXPORT
#   define _API_UTIL       _API_IMPORT
#elif _MODULE == 2 // UTIL
#   define _MODULE_UTIL 1
#   define _API_PLATFORM   _API_IMPORT
#   define _API_GAME       _API_IMPORT
#   define   _API_UTIL       _API_EXPORT

#else
#   error "Unknown module ID"
#endif
// #undef _MODULE


#if _BUILD_MODE == 0
#   define _DEBUG
#else
#   error "Unsupported Build Mode"
#endif

#if _COMPILER == 0
#   define _MSVC
#else
#   error "Unsupported Compiler"
#endif

#if _PLATFORM == 0
#   define _API_ENTRY  __stdcall
#   define _API_IMPORT __declspec(dllimport)
#   define _API_EXPORT __declspec(dllexport)
#else
#   error "Unsupported Platform"
#endif

#if _ARCHITECTURE == 0
#   define _X64
#else
#   error "Unsupported Architecture"
#endif



#define global  static
#define persist static
#define local inline
#define internal static inline
#define external

#define UNUSED(...) ((void)__VA_ARGS__)
#define RETURNS(...)

#define PTR_JUMP(Base, Offset)    (vptr)((u08*)(Base) + (Offset))
#define OFFSET_OF(Struct, Member) ((u64)&(((Struct*)0)->Member))
#define ARRAY_COUNT(Array)        (sizeof(Array) / sizeof((Array)[0]))

#define FORCE_CAST(Variable, Type)               (*(Type*)&(Variable))
#define LITERAL_CAST(OldType, Constant, NewType) (*(NewType*)&(OldType){Constant})
#define MAKE_ARRAY(Type, ...)                    ((Type[]){__VA_ARGS__})

#define BIT_SET(Bitstring, Index)    ((Bitstring) |=  (1 << (Index)))
#define BIT_GET(Bitstring, Index)   (((Bitstring) &   (1 << (Index))) >> (Index))
#define BIT_CLEAR(Bitstring, Index)  ((Bitstring) &= ~(1 << (Index)))
#define BIT_TOGGLE(Bitstring, Index) ((Bitstring) ^=  (1 << (Index)))

#define FLAG_SET(Bitstring, Flag)    ((Bitstring) |=  (Flag))
#define FLAG_TEST(Bitstring, Flag)   ((Bitstring) &   (Flag))
#define FLAG_CLEAR(Bitstring, Flag)  ((Bitstring) &= ~(Flag))
#define FLAG_TOGGLE(Bitstring, Flag) ((Bitstring) ^=  (Flag))

#define LOW_16(Number)  ((u16)((u64)(Number) & 0xFFFF))
#define HIGH_16(Number) ((u16)(((u64)(Number) >> 16) & 0xFFFF))
#define MERGE_32(u32_High, u32_Low) (((u64)(u32_High) << 32) | ((u64)(u32_Low) << 0))

#define ALIGN_DOWN_32(Num, Bytes) ((Num) & ~(u32)((Bytes) - 1))
#define ALIGN_UP_32(Num, Bytes)   ALIGN_DOWN_32((Num) + (Bytes) - 1, Bytes)
#define ALIGN_DOWN_64(Num, Bytes) ((Num) & ~(u64)((Bytes) - 1))
#define ALIGN_UP_64(Num, Bytes)   ALIGN_DOWN_64((Num) + (Bytes) - 1, Bytes)

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define WITHIN(Min, X, Max) ((Min) <= (X) && (X) <= (Max))
#define SWAP(A, B, Type) {Type _Swap_ = (A); (A) = (B); (B) = _Swap_;}

#define INDEX_2D(X, Y, MaxX)          ((X) + ((MaxX) * (Y)))
#define INDEX_3D(X, Y, Z, MaxX, MaxY) ((X) + ((MaxX) * ((Y) + ((MaxY) * (Z)))))

#define VERSION_32(Major, Minor, Patch) (((Major & 0xFF) << 24) | ((Minor & 0xFF) << 16) | (Patch & 0xFFFF))
#define MAGIC_4(C0, C1, C2, C3) (((u32)C0 << 0) | ((u32)C1 << 8) | ((u32)C2 << 16) | ((u32)C3 << 24))

#define KIBIBYTES(Count) ((Count) * 1024ULL)
#define MEBIBYTES(Count) (KIBIBYTES(Count) * 1024ULL)

#ifdef _DEBUG
#   define STOP __debugbreak()
#   define ASSERTP(Expression, Message) { if(!(Expression)) { Platform->Assert(__FILE__, __LINE__, Message); STOP; } }
#   define ASSERT(Expression) { if(!(Expression)) { STOP; } }
#   define NOP __nop();
#else
//TODO: Put in error checking where there are asserts
#   define STOP
#   define ASSERTP(...)
#   define ASSERT(...)
#   define NOP
#endif

#define NULL ((vptr)0)
#define FALSE 0
#define TRUE  1
#define LESS  (-1)
#define EQUAL   0
#define GREATER 1

#define S08_MIN 0x80
#define S16_MIN 0x8000
#define S64_MIN 0x8000000000000000
#define S08_MAX 0x7F
#define S16_MAX 0x7FFF
#define S64_MAX 0x7FFFFFFFFFFFFFFF
#define U08_MAX 0xFF
#define U16_MAX 0xFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFF

#define MAX_PATH_SIZE 256
#define MAX_ATLAS_DIM 128
#define BYTES_PER_PIXEL 4



//
// Types
//
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

typedef u08 flag08;
typedef u16 flag16;

typedef u08* str;



//
// Generics
//
#define TYPES \
    ENUM(R32, r32) \
    ENUM(S32, s32) \
    ENUM(U32, u32) \
    ENUM(Str, str) \
    ENUM(Ptr, vptr) \

typedef enum type_name {
    #define ENUM(Name, Type) \
        Type_##Name,
    TYPES
    #undef ENUM
} type_name;

typedef struct type {
    type_name Name;
    u32 Size;
} type;

#define ENUM(Name, Type) \
    global type Name = {Type_##Name, sizeof(Type)};
TYPES
#undef ENUM

typedef struct num
{
    type Type;
    
    union
    {
        #define ENUM(Name, Type) \
            Type Name;
        TYPES
        #undef ENUM
    };
} num;

#undef TYPES



#define _VEC_GENERATORS 1

typedef struct platform_exports platform_exports;
typedef struct platform_state   platform_state;
typedef struct platform_module  platform_module;
typedef struct game_exports     game_exports;
typedef struct game_state       game_state;
typedef struct game_module      game_module;
typedef struct util_exports     util_exports;
typedef struct util_state       util_state;
typedef struct util_module      util_module;

#include <util/macros.h>
#include <util/mem.h>
#include <util/str.h>
#include <util/json.h>
#include <util/intrin.h>
#include <util/math/rand.h>
#include <util/math/v1.h>
#include <util/math/v2.h>
#include <util/math/v3.h>
#include <util/math/v4.h>
#include <util/math/m3x3.h>
#include <util/math/m4x4.h>
#include <util/math/bit.h>

#include <platform/shared.h>
#include <platform/opengl/api.h>
#include <platform/opengl/opengl.h>
#if _WIN32
#   include <platform/win32/api.h>
#   include <platform/win32/loader.h>
#endif

#include <game/file.h>
#include <game/asset.h>
#include <game/mesh.h>
#include <game/ui.h>
// #include <game/world.h>
#include <game/game.h>



#define INCLUDE_HEADER
//     typedef struct game game;
//     typedef struct util util;
//     typedef struct platform platform;
//     #include <game/module.c>
//     #include <util/module.c>
//     #include <platform/module.c>
#undef  INCLUDE_HEADER

s32 _fltused;