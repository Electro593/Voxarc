/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
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

#if !defined(VOX_SHARED_H_)
#define      VOX_SHARED_H_

//SUBSECTION: Defines

#define _VOX_MODULE_PLATFORM 0
#define _VOX_MODULE_GAME     1
#if _VOX_MODULE == _VOX_MODULE_PLATFORM
    #define _VOX_PLATFORM_API   _API_EXPORT
    #define _VOX_GAME_API     _API_IMPORT
#elif _VOX_MODULE == _VOX_MODULE_GAME
    #define _VOX_PLATFORM_API _API_IMPORT
    #define _VOX_GAME_API       _API_EXPORT
#else
    #define _VOX_PLATFORM_API _API_IMPORT
    #define _VOX_GAME_API     _API_IMPORT
#endif

#define _VOX_BUILD_MODE_DEBUG    0
#define _VOX_BUILD_MODE_RELEASE  1
#define _VOX_BUILD_MODE_SHIPMENT 2
#if _VOX_BUILD_MODE == _VOX_BUILD_MODE_DEBUG
    #define _VOX_DEBUG
#elif _VOX_BUILD_MODE == _VOX_BUILD_MODE_RELEASE
    #define _VOX_RELEASE
#elif _VOX_BUILD_MODE == _VOX_BUILD_MODE_SHIPMENT
    #define _VOX_SHIPMENT
#else
    #error "Build type not supported"
#endif

#define _VOX_COMPILER_MSVC 0
#if _VOX_COMPILER == _VOX_COMPILER_MSVC
    #define _VOX_MSVC
#else
    #error "Compiler not supported"
#endif

#define _VOX_PLATFORM_WINDOWS 0
#if _VOX_PLATFORM == _VOX_PLATFORM_WINDOWS
    #define _VOX_WINDOWS
    #define _API_ENTRY __stdcall
    #define _API_IMPORT __declspec(dllimport)
    #define _API_EXPORT __declspec(dllexport)
#else
    #error "Platform not supported"
#endif

#define _VOX_ARCHITECTURE_X64 0
#if _VOX_ARCHITECTURE == _VOX_ARCHITECTURE_X64
    #define _VOX_X64
#else
    #error "Architecture not supported"
#endif

//TODO: Test inline by defining internal as static inline
#define global_var    static
#define local_persist static
#define local inline
#define internal static inline
#define external

#define UNUSED(...) ((void)__VA_ARGS__)

//TODO: Turn some of these into functions

#define PTR_JUMP(Base, Offset)    (vptr)((u08*)(Base) + (Offset))
#define OFFSET_OF(Struct, Member) ((u64)&(((Struct*)0)->Member))
#define ARRAY_COUNT(Array)        (sizeof(Array) / sizeof((Array)[0]))

#define FORCE_CAST(Variable, Type)               (*(Type*)&(Variable))
#define LITERAL_CAST(OldType, Constant, NewType) (*(NewType*)&(OldType){Constant})
#define MAKE_ARRAY(Type, ...)                    ((Type[]){__VA_ARGS__})

#define BIT_SET(Bitstring, Index)    ((Bitstring) |=  (1 << (Index)))
#define BIT_TEST(Bitstring, Index)   ((Bitstring) &   (1 << (Index)))
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

#if defined(_VOX_DEBUG)
    #define BREAK __debugbreak()
    #define ASSERT(Condition) if(!(Condition)) BREAK
    #define NOP __nop();
#else
//TODO: Put in error checking where there are asserts
    #define BREAK
    #define ASSERT(Condition)
    #define NOP
#endif

#define KIBIBYTES(Count) ((Count) * 1024ULL)
#define MEBIBYTES(Count) (KIBIBYTES(Count) * 1024ULL)
#define GIBIBYTES(Count) (MEBIBYTES(Count) * 1024ULL)
#define TEBIBYTES(Count) (GIBIBYTES(Count) * 1024ULL)

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

#define OFFSET(Type, Name) union { u64 Name##Offset; Type *Name##Ptr; }

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

#include "game/util/vox_macros.h"
#include "game/util/vox_mem.h"
#include "game/util/vox_str.h"
#include "game/util/vox_json.h"
#include "game/util/vox_intrin.h"
#include "game/util/math/vox_rand.h"
#include "game/util/math/vox_s32.h"
#include "game/util/math/vox_u32.h"
#include "game/util/math/vox_u64.h"
#include "game/util/math/vox_r32.h"
#include "game/util/math/vox_v2.h"
#include "game/util/math/vox_v3.h"
#include "game/util/math/vox_v4.h"
#include "game/util/math/vox_m3x3.h"
#include "game/util/math/vox_m4x4.h"
#include "game/util/math/vox_bit.h"

#include "platform/vox_opengl.h"
#include "platform/vox_platform.h"

#include "game/vox_file.h"
// #include "game/vox_asset.h"
#include "game/vox_mesh.h"
#include "game/vox_ui.h"
// #include "game/vox_world.h"
#include "game/vox_game.h"

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