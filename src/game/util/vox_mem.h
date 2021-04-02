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

#if !defined(GAME_UTIL_VOX_MEM_H_)
#define      GAME_UTIL_VOX_MEM_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//TODO: Remove these?
#define MEM_COUNT(Size, Type)         (Size / sizeof(Type))
#define MEM_MEMBER(Data, Type, Index) ((Type*)Data + Index)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: Mem

typedef u08* mem;


//SUBSECTION: Heap

//TODO: When adding heap defragmentation, make the handle system,
//      similar to the original version
//TODO: Make heap allocation traverse free list from back to front,
//      to minimize fragmentation


// For annotation purposes
typedef vptr hvptr; // For function parameters
typedef mem hmem; // For increments, etc

//TODO: Pass a header index as a bitfield in Used?
typedef struct heap_header
{
    struct heap_header *NextFree; // Points to the state struct when Used == TRUE
    u32 Size; // Excluding size of struct
    u32 Used;
} heap_header;

typedef struct heap
{
    heap_header *FreeList;
    u32 BlockCount;
    
    #define HEAP_MAGIC_NUMBER MAGIC_4('h', 'e', 'a', 'p')
    u32 MagicNumber;
} heap;

typedef struct heap_data_DEBUG
{
    heap_header *List;
    heap *DebugHeap;
    u32 BlockCount;
} heap_data_DEBUG;


//SUBSECTION: Stack

//TODO: Use _ReturnAddress to only need Stack_Allocate and Stack_Pop
//      Pop instead of push because it wouldn't know when a function returns

typedef vptr svptr;
typedef mem smem;

//TODO: Make this a reverse offset?
typedef struct stack_marker
{
    struct stack_marker *PrevMarker;
} stack_marker;

typedef struct stack
{
    size Size; // Includes sizeof(stack)
    
    u08 *Cursor;
    stack_marker *NewestMarker;
} stack;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if defined(_VOX_DEBUG)
    #define GAME_UTIL__MEM_DEBUG__EXPORTS \
        PROC(void, _Heap_Print_DEBUG, heap *Heap, heap_data_DEBUG *DebugData) \
    
    #define GAME_UTIL__MEM_DEBUG__FUNCS \
        PROC(void, _Heap_Mark_DEBUG, heap *Heap, heap_data_DEBUG *DebugData) \
        PROC(void, _Heap_Check_DEBUG, heap *Heap, heap_data_DEBUG *DebugData) \
        PROC(b08,  _Heap_IsCorrupted_DEBUG, heap *Heap) \
    
    #define Heap_Print_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData) _Heap_Print_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Mark_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData) _Heap_Mark_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Check_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData) _Heap_Check_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_IsCorrupted_DEBUG(heap_p__Heap) _Heap_IsCorrupted_DEBUG(heap_p__Heap)
#else
    #define GAME_UTIL__MEM_DEBUG__EXPORTS
    #define GAME_UTIL__MEM_DEBUG__FUNCS
    
    #define Heap_Print_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Mark_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Check_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_IsCorrupted_DEBUG(heap_p__Heap)
#endif


#define GAME_UTIL__MEM__EXPORTS \
    GAME_UTIL__MEM_DEBUG__EXPORTS \
    PROC(vptr,         Mem_Set, vptr Dest, s32 Data, size Size) \
    PROC(vptr,         Mem_Cpy, vptr Dest, vptr Src, size Size) \
    PROC(heap*,        Heap_Create, vptr Base, u32 Size) \
    PROC(hvptr,        Heap_AllocateAligned, heap *Heap, u32 Size, u32 Alignment, b08 ShouldZeroMemory) \
    PROC(void,         Heap_Resize, hvptr *Data, u32 Size) \
    PROC(void,         Heap_Free, hvptr Data) \
    PROC(u32,          Heap_GetSize, hvptr Data) \
    PROC(void,         Stack_Init, vptr Base, size Size) \

#define GAME_UTIL__MEM__FUNCS \
    GAME_UTIL__MEM_DEBUG__FUNCS \
    PROC(s32,          Mem_Cmp, vptr A, vptr B, size Size) \
    PROC(b08,          Heap_IsSentry, heap_header *Header) \
    PROC(heap_header*, Heap_GetHeader, hvptr Data) \
    PROC(hvptr,        Heap_GetData, heap_header *Header) \
    PROC(heap_header*, Heap_GetNext, heap_header *Header) \
    PROC(heap_header*, Heap_GetFirst, heap *Heap) \
    PROC(heap*,        Heap_GetHeap, heap_header *Header) \
    PROC(void,         Heap_Copy, hvptr Dest, hvptr Src, b08 ShouldZeroDest) \
    PROC(b08,          Stack_HasOverflowed, void) \
    PROC(b08,          Stack_IsCorrupted, void) \
    PROC(void,         Stack_Push, void) \
    PROC(void,         Stack_Pop, void) \
    PROC(svptr,        Stack_Allocate, size Size) \


#define Mem_Zero(Dest, Size) Mem_Set(Dest, 0, Size)
#define Heap_AllocateZeroed(Heap, u32_Size) Heap_AllocateAligned(Heap, u32_Size, 1, TRUE)
#define Heap_Allocate(Heap, u32_Size)       Heap_AllocateAligned(Heap, u32_Size, 1, FALSE)



#if 0
//TODO: Multithreading support

//SUBSECTION: Defines

#define STACK_GET_DATA(Header)  ((u08*)(Header) + sizeof(stack_header))
#define STACK_GET_HEADER(Data)  ((stack_header*)((u08*)(Data) - sizeof(stack_header)))
#define STACK_GET_NEXT(Header)  ((stack_header*)((u08*)(Header) + sizeof(stack_header) + (Header)->Size))
#define STACK_GET_PREV(Header)  ((stack_header*)((u08*)(Header) - sizeof(stack_header) - (Header)->PrevSize))
#define STACK_GET_TOP(Stack)    ((stack_header*)((u08*)(Stack) + sizeof(stack_state) + (Stack)->Cursor - sizeof(stack_header)))
#define STACK_GET_BOTTOM(Stack) ((stack_header*)((u08*)(Stack) + sizeof(stack_state)))

//SUBSECTION: Types

typedef vptr svptr; //NOTE: Stack Memory

typedef struct stack_header
{
    u32 PrevSize;
    u32 Size;
    s32 Level;
} stack_header;

typedef struct stack_state
{
    u32 Size;
    u32 Cursor;
    s32 CurrLevel;
} stack_state;

//SUBSECTION: Variables

global_var stack_state *__Stack = NULL;

//SUBSECTION: Functions

internal stack_state * Stack_Create(vptr Base, u32 Size);

internal void Stack_SetGlobal(stack_state *Stack);
internal stack_state * Stack_GetGlobal(void);

internal svptr _Stack_Push(u32 Size, b08 ShouldZeroMemory);
#define Stack_PushZeroed(Size_u32) _Stack_Push(Size_u32, TRUE)
#define Stack_Push(Size_u32)       _Stack_Push(Size_u32, FALSE)

internal void Stack_Pop(void);

internal void Stack_PushLevel(void);

internal void Stack_PopLevel(void);

//SUBSECTION: Debug

#if defined(_VOX_DEBUG)
internal void _Stack_Print_DEBUG(void);

#define Stack_Print_DEBUG() _Stack_Print_DEBUG()
#else
#define Stack_Print_DEBUG()
#endif
#endif


#endif