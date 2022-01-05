/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_MEM_H_
#define UTIL_MEM_H_


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
//      NOTE: Is defragmentation necessary?
//TODO: Make heap allocation traverse free list from back to front,
//      to minimize fragmentation

#if 0
#define HDATA(Handle, Type) ((Type*)Handle->Data)

//TODO: Consider a binary search for freeing
typedef struct heap1_handle
{
    mem Data;
    u32 Size; // Size of data block
    u32 Offset; // Offset from end of lower block to start of current block
    u16 Index; // Consecutive index in handle block
    u16 NextIndex; // Free or unmapped index, depending on State
    u16 NextBlockIndex;
    b08 Mapped;
    b08 Anchored; // The data pointer can't be changed, e.g. if used in an external API
} heap1_handle;

typedef struct heap1
{
    mem DataCursor; // Top down
    u64 Size; // Excluding the header
    u16 HandleCount; // Bottom up, mapped and unmapped
    u16 FirstMappedIndex; // From HandleCount down
    u16 FirstUnmappedIndex;
    u16 FirstBlockIndex; // Lowest up
} heap1;
#endif

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
    u64 Size; // Includes sizeof(stack)
    
    u08 *Cursor;
    stack_marker *NewestMarker;
} stack;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#ifdef _DEBUG
    #define UTIL__MEM_DEBUG__EXPORTS \
        FUNC(void, _Heap_Print_DEBUG, heap *Heap, heap_data_DEBUG *DebugData) \
    
    #define UTIL__MEM_DEBUG__FUNCS \
        FUNC(void, _Heap_Mark_DEBUG, heap *Heap, heap_data_DEBUG *DebugData) \
        FUNC(void, _Heap_Check_DEBUG, heap *Heap, heap_data_DEBUG *DebugData) \
        FUNC(b08,  _Heap_IsCorrupted_DEBUG, heap *Heap) \
    
    #define Heap_Print_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData) _Heap_Print_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Mark_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData) _Heap_Mark_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Check_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData) _Heap_Check_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_IsCorrupted_DEBUG(heap_p__Heap) _Heap_IsCorrupted_DEBUG(heap_p__Heap)
#else
    #define UTIL__MEM_DEBUG__EXPORTS
    #define UTIL__MEM_DEBUG__FUNCS
    
    #define Heap_Print_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Mark_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_Check_DEBUG(heap_p__Heap, heap_data_DEBUG_p__DebugData)
    #define Heap_IsCorrupted_DEBUG(heap_p__Heap)
#endif


#define UTIL__MEM__EXPORTS \
    UTIL__MEM_DEBUG__EXPORTS \
    FUNC(vptr,         Mem_Set, vptr Dest, s32 Data, u64 Size) \
    FUNC(vptr,         Mem_Cpy, vptr Dest, vptr Src, u64 Size) \
    FUNC(s32,          Mem_Cmp, vptr A, vptr B, u64 Size) \
    FUNC(heap*,        Heap_Create, vptr Base, u32 Size) \
    FUNC(hvptr,        Heap_AllocateAligned, heap *Heap, u32 Size, u32 Alignment, b08 ShouldZeroMemory) \
    FUNC(void,         Heap_Resize, hvptr *Data, u32 Size) \
    FUNC(void,         Heap_Free, hvptr Data) \
    FUNC(u32,          Heap_GetSize, hvptr Data) \
    FUNC(void,         Stack_Init, vptr Base, u64 Size) \
    FUNC(void,         Stack_Push, void) \
    FUNC(void,         Stack_Pop, void) \
    FUNC(svptr,        Stack_Allocate, u64 Size) \
    
    // FUNC(heap1*,        Heap1_Create, vptr Base, u64 Size) \
    // FUNC(void,         Heap1_Defrag, heap1 *Heap1) \
    // FUNC(heap1_handle*, Heap1_Alloc, heap1 *Heap1, u32 Size) \
    // FUNC(void,         Heap1_Resize, heap1_handle *Handle, u32 Size) \
    // FUNC(void,         Heap1_Free, heap1_handle *Handle) \

#define UTIL__MEM__FUNCS \
    UTIL__MEM_DEBUG__FUNCS \
    FUNC(b08,          Heap_IsSentry, heap_header *Header) \
    FUNC(heap_header*, Heap_GetHeader, hvptr Data) \
    FUNC(hvptr,        Heap_GetData, heap_header *Header) \
    FUNC(heap_header*, Heap_GetNext, heap_header *Header) \
    FUNC(heap_header*, Heap_GetFirst, heap *Heap) \
    FUNC(heap*,        Heap_GetHeap, heap_header *Header) \
    FUNC(void,         Heap_Copy, hvptr Dest, hvptr Src, b08 ShouldZeroDest) \
    FUNC(b08,          Stack_HasOverflowed, void) \
    FUNC(b08,          Stack_IsCorrupted, void) \
    
    // FUNC(heap1*,        Heap1_GetHeap1, heap1_handle *Handle) \
    // FUNC(heap1_handle*, Heap1_GetHandle, heap1 *Heap1, u16 HandleIndex) \


#define Mem_Zero(Dest, Size) Mem_Set(Dest, 0, Size)
#define Heap_AllocateZeroed(Heap, u32_Size) Heap_AllocateAligned(Heap, u32_Size, 1, TRUE)
#define Heap_Allocate(Heap, u32_Size)       Heap_AllocateAligned(Heap, u32_Size, 1, FALSE)

#endif