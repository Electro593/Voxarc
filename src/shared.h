/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SHARED_H_
#define SHARED_H_

//SUBSECTION: Defines

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


#if _MODE == 0 // DEBUG
#   define _MODE_DEBUG 1
#elif _MODE == 1 // RELEASE
#   define _MODE_RELEASE 1
#elif _MODE == 2 // SHIPMENT
#   define _MODE_SHIPMENT 1
#else
#   error "Unknown build mode"
#endif
// #undef _MODE


#if _COMPILER == 0 // MSVC
#   define _COMPILER_MSVC 1
#else
#   error "Unknown compiler"
#endif
// #undef _COMPILER


#if _PLATFORM == 0 // WINDOWS
#   define _PLATFORM_WINDOWS 1
#   define _API_ENTRY __stdcall
#   define _API_IMPORT __declspec(dllimport)
#   define _API_EXPORT __declspec(dllexport)
#else
#   error "Unknown OS"
#endif
// #undef _PLATFORM


#if _LOADER == 0 // WIN32
#   if !_PLATFORM_WINDOWS
#       error "Can only load with Win32 on Windows"
#   endif
#   define _LOADER_WIN32 1
#else
#   error "Unknown Loading API"
#endif
// #undef _LOADER


#if _ARCH == 0 // X64
#   define _ARCH_X64 1
#elif _ARCH == 1 // X86
#   define _ARCH_X86 1
#else
#   error "Unknown architecture"
#endif
// #undef _ARCH


#define global  static
#define persist static
#define local inline
#define internal static inline
#define external

#define UNUSED(...) ((void)__VA_ARGS__)

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

#define LOW_08(Number)  ((u08)((u64)(Number) & 0xFF))
#define HIGH_08(Number) ((u08)(((u64)(Number) >>  8) & 0xFF))
#define LOW_16(Number)  ((u16)((u64)(Number) & 0xFFFF))
#define HIGH_16(Number) ((u16)(((u64)(Number) >> 16) & 0xFFFF))
#define LOW_32(Number)  ((u32)((u64)(Number) & 0xFFFFFFFF))
#define HIGH_32(Number) ((u32)(((u64)(Number) >> 32) & 0xFFFFFFFF))
#define MERGE_32(u32_High, u32_Low) (((u64)(u32_High) << 32) | ((u64)(u32_Low) << 0))

#define ALIGN_DOWN_32(Num, Bytes) ((Num) & ~(u32)((Bytes) - 1))
#define ALIGN_UP_32(Num, Bytes)   ALIGN_DOWN_32((Num) + (Bytes) - 1, Bytes)
#define ALIGN_DOWN_64(Num, Bytes) ((Num) & ~(u64)((Bytes) - 1))
#define ALIGN_UP_64(Num, Bytes)   ALIGN_DOWN_64((Num) + (Bytes) - 1, Bytes)

#define LIST_2(Array) (Array)[0], (Array)[1]
#define LIST_3(Array) (Array)[0], (Array)[1], (Array)[2]
#define LIST_4(Array) (Array)[0], (Array)[1], (Array)[2], (Array)[3]

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define WITHIN(Min, X, Max) ((Min) <= (X) && (X) <= (Max))
#define SWAP(A, B, Type) {Type _Swap_ = (A); (A) = (B); (B) = _Swap_;}

#define INDEX_2D(X, Y, MaxX)          ((X) + ((MaxX) * (Y)))
#define INDEX_3D(X, Y, Z, MaxX, MaxY) ((X) + ((MaxX) * ((Y) + ((MaxY) * (Z)))))

#define VERSION_32(Major, Minor, Patch) (((Major & 0xFF) << 24) | ((Minor & 0xFF) << 16) | (Patch & 0xFFFF))
#define MAGIC_2(C0, C1) (((u16)C0 << 0) | ((u16)C1 << 8))
#define MAGIC_4(C0, C1, C2, C3) (((u32)C0 << 0) | ((u32)C1 << 8) | ((u32)C2 << 16) | ((u32)C3 << 24))

#define KIBIBYTES(Count) ((Count) * 1024ULL)
#define MEBIBYTES(Count) (KIBIBYTES(Count) * 1024ULL)
#define GIBIBYTES(Count) (MEBIBYTES(Count) * 1024ULL)
#define TEBIBYTES(Count) (GIBIBYTES(Count) * 1024ULL)

#if _MODE_DEBUG
#   define STOP __debugbreak()
#   define ASSERT(Condition) if(!(Condition)) STOP
#   define NOP __nop();
#else
//TODO: Put in error checking where there are asserts
#   define STOP
#   define ASSERT(Condition)
#   define NOP
#endif

#define NULL ((vptr)0)
#define LESS  (-1)
#define EQUAL   0
#define GREATER 1

#define TRUE  1
#define FALSE 0

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

// #define OFFSET(Type, Name) union { u64 Name##Offset; Type *Name##Ptr; }

//SUBSECTION: Types

typedef signed __int8  s08;
typedef signed __int16 s16;
typedef signed __int32 s32;
typedef signed __int64 s64;

typedef unsigned __int8 u08;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef u08 b08;

typedef float  r32;
typedef double r64;

typedef void* vptr;
typedef void (*fptr)(void);

typedef u08 flag08;
typedef u16 flag16;

typedef enum type
{
    TYPE_S08,
    TYPE_S16,
    TYPE_S32,
    TYPE_S64,
    
    TYPE_U08,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    
    TYPE_R32,
    TYPE_R64,
    
    TYPE_STR,
    TYPE_MEM,
} type;

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
#if 0 && _PLATFORM_WINDOWS
#   include <d3d/api.h>
#   include <d3d/d3d.h>
#else
#   include <platform/opengl/api.h>
#   include <platform/opengl/opengl.h>
#endif
#if _LOADER_WIN32
#   include <platform/win32/api.h>
#   include <platform/win32/loader.h>
#endif

#include <game/file.h>
#include <game/asset.h>
#include <game/mesh.h>
#include <game/ui.h>
// #include <game/world.h>
#include <game/game.h>

typedef struct num
{
    type Type;
    
    union
    {
        u64 Raw;
        
        s08 S08;
        s16 S16;
        s32 S32;
        s64 S64;
        
        u08 U08;
        u16 U16;
        u32 U32;
        u64 U64;
        
        r32 R32;
        r64 R64;
        
        str NumStr;
    };
} num;

s32 _fltused;

#endif