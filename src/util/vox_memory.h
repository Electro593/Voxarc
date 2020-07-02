#ifndef VOX_MEMORY_H_

#include "vox_defines.h"
#include <intrin.h>

#define HANDLES_PER_MEGABYTE 8

typedef struct memory_handle
{
    size Size;
    vptr Base;
    u32 HandleIndex;
    u32 _Unused[3];
} memory_handle;

typedef struct data_pool
{
    vptr Base;
    size Used;
    size Size;
    u32 _Unused[2];
} data_pool;

typedef struct handle_pool
{
    memory_handle *Handles;
    data_pool *DataPool;
    u32 HandleCount;
    u32 _Unused[3];
} handle_pool;

inline void
SetMemory(vptr Dest, u32 Data, size Size)
{
    u08 Data08 = (u08)Data;
    __m128i Data128 = _mm_set1_epi8(Data08);
    
    size Alignment = Align16(Size - 15);
    u08 *Dest08 = (u08*)Dest;
    while(Size > Alignment)
    {
        Size--;
        *Dest08++ = Data08;
    }
    
    __m128i *Dest128 = (__m128i*)Dest08;
    while(Size > sizeof(__m128i))
    {
        Size -= sizeof(__m128i);
        *Dest128++ = Data128;
    }
    
    Dest08 = Dest128->m128i_u8;
    while(Size)
    {
        Size--;
        *Dest08++ = Data08;
    }
    //TODO(andrew): Make this in assembly?
    // https://github.com/macmade/LibC-String-Optimisations/blob/master/source/memset.64.s
}

inline void
CopyMemory(vptr Dest, cvptr Src, size Size)
{
    size Alignment = Align16(Size - 15);
    u08 *Src08 = (u08*)Src;
    u08 *Dest08 = (u08*)Dest;
    while(Size > Alignment)
    {
        Size--;
        *Dest08++ = *Src08++;
    }
    
    __m128i *Src128 = (__m128i*)Src08;
    __m128i *Dest128 = (__m128i*)Dest08;
    while(Size > sizeof(__m128i))
    {
        Size -= sizeof(__m128i);
        *Dest128++ = *Src128++;
    }
    
    Src08 = Src128->m128i_u8;
    Dest08 = Dest128->m128i_u8;
    while(Size)
    {
        Size--;
        *Dest08++ = *Src08++;
    }
}

inline handle_pool *
GetHandlePool(memory_handle *Handle)
{
    return (handle_pool*)((u08*)(Handle - Handle->HandleIndex) - sizeof(handle_pool));
}

inline void
CopyHandles(memory_handle *Dest, memory_handle *Src)
{
    Assert(Dest->Size >= Src->Size);
    Assert(Dest->Base && Src->Base);
    CopyMemory(Dest->Base, Src->Base, Src->Size);
}

inline handle_pool *
CreateHandlePool(vptr Base, size Size)
{
    u32 MemoryHandleCount = (u32)((Size * HANDLES_PER_MEGABYTE) / (1024*1024));
    if(MemoryHandleCount < 4)
    {
        MemoryHandleCount = 4;
    }
    size HandlePoolSize = MemoryHandleCount * sizeof(memory_handle) + sizeof(handle_pool);
    size DataPoolSize = Size - HandlePoolSize - sizeof(data_pool);
    
    handle_pool *HandlePool = (handle_pool*)Base;
    HandlePool->Handles = (memory_handle*)(HandlePool + 1);
    HandlePool->HandleCount = MemoryHandleCount;
    
    data_pool *DataPool = (data_pool*)((u08*)Base + HandlePoolSize);
    DataPool->Base = DataPool + 1;
    DataPool->Used = 0;
    DataPool->Size = DataPoolSize;
    
    HandlePool->DataPool = DataPool;
    
    memory_handle *Handle = HandlePool->Handles;
    for(u32 HandleIndex = 0;
        HandleIndex < MemoryHandleCount;
        ++HandleIndex)
    {
        Handle->Base = 0;
        Handle->Size = 0;
        Handle->HandleIndex = HandleIndex;
        Handle++;
    }
    
    return HandlePool;
}

inline void
DefragmentMemoryPools(handle_pool *HandlePool)
{
    memory_handle *StartHandle = HandlePool->Handles;
    
    data_pool *DataPool = HandlePool->DataPool;
    
    u08 *Dest = (u08*)DataPool->Base;
    memory_handle *MinHandle = 0;
    for(u32 MainLoopIndex = 0;
        MainLoopIndex < HandlePool->HandleCount;
        ++MainLoopIndex)
    {
        u08 *MaxBase = (u08*)DataPool->Base + DataPool->Size;
        
        for(u32 SubLoopIndex = 0;
            SubLoopIndex < HandlePool->HandleCount;
            ++SubLoopIndex)
        {
            memory_handle *TestHandle = StartHandle + SubLoopIndex;
            if(TestHandle->Base &&
               (u08*)TestHandle->Base >= Dest &&
               (u08*)TestHandle->Base < MaxBase)
            {
                MaxBase = (u08*)TestHandle->Base;
                MinHandle = TestHandle;
            }
        }
        
        if(MinHandle)
        {
            CopyMemory(Dest, MinHandle->Base, MinHandle->Size);
            MinHandle->Base = Dest;
            Dest += MinHandle->Size;
        }
    }
    
    DataPool->Used = Dest - (u08*)DataPool->Base;
}

inline vptr
FindFreeMemory(handle_pool *HandlePool, size Size)
{
    data_pool *DataPool = HandlePool->DataPool;
    vptr Base = 0;
    if(DataPool->Size - DataPool->Used > Size)
    {
        Base = (u08*)DataPool->Base + DataPool->Used;
        DataPool->Used += Size;
    }
    return Base;
}

inline memory_handle *
FindUnusedHandle(handle_pool *HandlePool)
{
    memory_handle *NextHandle = HandlePool->Handles;
    u32 HandleCount = HandlePool->HandleCount;
    while(HandleCount--)
    {
        if(!NextHandle->Base)
        {
            return NextHandle;
        }
        NextHandle++;
    }
    
    return 0;
}

inline memory_handle *
AllocateMemory(handle_pool *HandlePool, size Size)
{
    vptr Base = FindFreeMemory(HandlePool, Size);
    memory_handle *UnusedHandle = FindUnusedHandle(HandlePool);
    if(!Base || !UnusedHandle)
    {
        DefragmentMemoryPools(HandlePool);
        if(!Base)
        {
            Base = FindFreeMemory(HandlePool, Size);
        }
        if(!UnusedHandle)
        {
            UnusedHandle = FindUnusedHandle(HandlePool);
        }
    }
    
    if(UnusedHandle && Base)
    {
        UnusedHandle->Base = Base;
        UnusedHandle->Size = Size;
        
        SetMemory(UnusedHandle->Base, 0, UnusedHandle->Size);
        return UnusedHandle;
    }
    
    Assert(!"Coudn't allocate memory!");
    return 0;
}

inline void
FreeMemory(memory_handle *Handle)
{
    SetMemory(Handle->Base, 0, Handle->Size);
    Handle->Size = 0;
    Handle->Base = 0;
}

inline memory_handle *
ResizeMemory(memory_handle *Handle, size Size)
{
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    s32 DeltaSize = (s32)Handle->Size - (s32)Size;
    if(DeltaSize > 0) // Shrink
    {
        Handle->Size = Size;
        SetMemory((u08*)Handle->Base + Handle->Size, 0, DeltaSize);
        return Handle;
    }
    else if(DeltaSize < 0) // Grow
    {
        memory_handle *NewHandle = AllocateMemory(HandlePool, Size);
        CopyMemory(NewHandle->Base, Handle->Base, Handle->Size);
        FreeMemory(Handle);
        return NewHandle;
    }
    return Handle;
}

inline s32
CompareMemory(vptr A, vptr B, size Size)
{
    u08 *ByteA = (u08*)A;
    u08 *ByteB = (u08*)B;
    while(Size && *ByteA == *ByteB) {
        Size--;
        ByteA++;
        ByteB++;
    }
    return Size ? *ByteA - *ByteB : 0;
}

inline s32
CompareArrays(c08 *A, c08 *B)
{
    while(*A && *B && *A == *B)
    {
        A++;
        B++;
    }
    return (*A == *B) ? 0 : *A - *B;
}

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
inline u32
PointerCount(void *Ptr)
{
    u32 Size = 0;
    u08 *Byte = (u08*)Ptr;
    while(*Byte++ != 0)
    {
        Size++;
    }
    return Size;
}

#define VOX_MEMORY_H_
#endif