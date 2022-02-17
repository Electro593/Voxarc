/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//        SECTION: General Memory Operations
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//TODO: If we run out of memory, maybe have a notification that tells the user to free some?

//TODO: Multithread these
internal vptr
Mem_Set(vptr Dest,
        s32 Data,
        u64 Size)
{
    #if 1
    u08 Data08 = (u08)Data;
    i128 Data128 = I128_Set_1x4(Data08);
    
    u64 Alignment = (u64)Dest - ALIGN_DOWN_64((u64)Dest, sizeof(i128));
    u08 *Dest08 = (u08*)Dest;
    while(Size && Alignment)
    {
        Size--, Alignment--;
        *Dest08++ = Data08;
    }
    
    i128 *Dest128 = (i128*)Dest08;
    while(Size >= sizeof(i128))
    {
        Size -= sizeof(i128);
        *Dest128++ = Data128;
    }
    
    Dest08 = (u08*)Dest128;
    while(Size)
    {
        Size--;
        *Dest08++ = Data08;
    }
    
    return Dest;
    
    //TODO: Make this in assembly? At least verify it with the crt
    // https://github.com/macmade/LibC-String-Optimisations/blob/master/source/memset.64.s
    #else
    i128 Data128 = I128_Set_1x4(Data);
    
    u64 Alignment = ALIGN_DOWN_64(Size, 4);
    u08 *Dest08 = (u08*)Dest;
    while(Size > Alignment)
    {
        *Dest08++ = (u08)Data;
        --Size;
    }
    
    Alignment = ALIGN_DOWN_64(Size, 16);
    u32 *Dest32 = (u32*)Dest08;
    while(Size > Alignment)
    {
        *Dest32++ = Data;
        Size -= 4;
    }
    
    i128 *Dest128 = (i128*)Dest32;
    while(Size >= sizeof(i128))
    {
        *Dest128++ = Data128;
        Size -= sizeof(i128);
    }
    
    ASSERT(Size == 0);
    // Dest32 = (u32*)Dest128;
    // while(Size)
    // {
    //     *Dest32++ = Data;
    //     Size -= 4;
    // }
    
    return Dest;
    #endif
}

internal vptr
Mem_Cpy(vptr Dest,
        vptr Src,
        u64 Size)
{
    if(!Src || !Dest || Size == 0)
        return Dest;
    
    s32 Delta = 1;
    u64 StartOffset = 0;
    u64 Offset128 = 0;
    u64 Alignment = (u64)Src - ALIGN_DOWN_64((u64)Src, sizeof(i128));
    if((u64)Src < (u64)Dest &&
       (u64)Src + Size >= (u64)Dest)
    {
        Delta = -1;
        StartOffset = Size - 1;
        Offset128 = sizeof(i128) - 1;
        
        u64 UpperBound = (u64)Dest + Size;
        Alignment = UpperBound - ALIGN_DOWN_64(UpperBound, sizeof(i128));
    }
    
    u08 *Src08 = (u08*)Src + StartOffset;
    u08 *Dest08 = (u08*)Dest + StartOffset;
    while(Size && Alignment)
    {
        Size--, Alignment--;
        *Dest08 = *Src08;
        Dest08 += Delta;
        Src08 += Delta;
    }
    
    i128 *Src128 = (i128*)(Src08 - Offset128);
    i128 *Dest128 = (i128*)(Dest08 - Offset128);
    while(Size >= sizeof(i128))
    {
        Size -= sizeof(i128);
        *Dest128 = *Src128;
        Dest128 += Delta;
        Src128 += Delta;
    }
    
    Src08 = (u08*)Src128 + Offset128;
    Dest08 = (u08*)Dest128 + Offset128;
    while(Size)
    {
        Size--;
        *Dest08 = *Src08;
        Dest08 += Delta;
        Src08 += Delta;
    }
    
    return Dest;
}

//TODO: Vectorize this
internal s32
Mem_Cmp(vptr A,
        vptr B,
        u64 Size)
{
    u08 *ByteA = (u08*)A;
    u08 *ByteB = (u08*)B;
    while(Size && *ByteA == *ByteB)
    {
        ++ByteA;
        ++ByteB;
        --Size;
    }
    
    return (Size == 0) ? 0 : 1;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Heap Allocator
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if 0
internal heap1 *
Heap1_GetHeap1(heap1_handle *Handle)
{
    heap1 *Heap1;
    ASSERT(Handle);
    
    heap1_handle *First = Handle - Handle->Index;
    Heap1 = (heap1*)First - 1;
    
    return Heap1;
}

internal heap1_handle *
Heap1_GetHandle(heap1 *Heap1,
                u16 HandleIndex)
{
    ASSERT(Heap1);
    if(HandleIndex == U16_MAX)
    {
        return NULL;
    }
    
    heap1_handle *First = (heap1_handle*)(Heap1 + 1);
    return First + HandleIndex;
}

internal heap1 *
Heap1_Create(vptr Base,
             u64 Size)
{
    ASSERT(Base);
    ASSERT(Size >= sizeof(heap1) + sizeof(heap1_handle));
    
    heap1 *Heap1 = (heap1*)Base;
    Heap1->DataCursor = (mem)Base + Size;
    Heap1->Size = Size - sizeof(heap1);
    Heap1->HandleCount = 0;
    Heap1->FirstMappedIndex = U16_MAX;
    Heap1->FirstUnmappedIndex = U16_MAX;
    Heap1->FirstBlockIndex = U16_MAX;
    
    return Heap1;
}

internal void
Heap1_Defrag(heap1 *Heap1)
{
    Platform_ThrowError_DEBUG("Defragmentation not implemented yet");
    UNUSED(Heap1);
}

internal heap1_handle *
Heap1_Alloc(heap1 *Heap1,
               u32 Size)
{
    ASSERT(Heap1);
    
    heap1_handle *BlockHandle = Heap1_GetHandle(Heap1, Heap1->FirstBlockIndex);
    
    // Find a valid handle
    heap1_handle *Handle = Heap1_GetHandle(Heap1, Heap1->FirstUnmappedIndex);
    if(Handle)
    { // Guaranteed to be before a mapped handle, all after it are mapped
        ASSERT(Handle->Mapped == FALSE);
        ASSERT(Handle->Index < Heap1->HandleCount - 1);
        
        Heap1->FirstUnmappedIndex = Handle->NextIndex;
        
        u16 *PrevNextIndex = &Heap1->FirstMappedIndex;
        while(*PrevNextIndex != U16_MAX &&
              *PrevNextIndex > Handle->Index)
        {
            PrevNextIndex = &Heap1_GetHandle(Heap1, *PrevNextIndex)->NextIndex;
        }
        Handle->NextIndex = *PrevNextIndex;
        *PrevNextIndex = Handle->Index;
    }
    else
    {
        Handle = Heap1_GetHandle(Heap1, Heap1->HandleCount);
        if((u64)(Handle + 1) > (u64)Heap1->DataCursor)
        {
            //TODO: Try to defragment
            Platform_ThrowError_DEBUG("Heap segment collision (handle overflow)");
        }
        
        Handle->Index = Heap1->HandleCount;
        Handle->NextIndex = Heap1->FirstMappedIndex;
        Heap1->FirstMappedIndex = Handle->Index;
        ++Heap1->HandleCount;
    }
    
    // Find a valid block
    u16 *PrevNextBlockIndex = &Heap1->FirstBlockIndex;
    while(BlockHandle &&
          BlockHandle->Offset < Size)
    {
        PrevNextBlockIndex = &BlockHandle->NextBlockIndex;
        BlockHandle = Heap1_GetHandle(Heap1, BlockHandle->NextBlockIndex);
    }
    
    if(BlockHandle)
    {
        Handle->Data = BlockHandle->Data + BlockHandle->Size + BlockHandle->Offset - Size;
        BlockHandle->Offset -= Size;
        Handle->NextBlockIndex = BlockHandle->NextBlockIndex;
        BlockHandle->NextBlockIndex = Handle->Index;
    }
    else
    {
        ASSERT((u64)(Heap1->DataCursor - Size) >= Heap1->HandleCount * sizeof(heap1_handle));
        
        Heap1->DataCursor -= Size;
        Handle->Data = Heap1->DataCursor;
        Handle->NextBlockIndex = Heap1->FirstBlockIndex;
        Heap1->FirstBlockIndex = Handle->Index;
    }
    
    Handle->Size = Size;
    Handle->Offset = 0;
    Handle->Mapped = TRUE;
    Handle->Anchored = FALSE;
    
    return Handle;
}

internal void
Heap1_Resize(heap1_handle *Handle,
             u32 Size)
{
    ASSERT(Handle);
    
    if(Handle->Size == Size)
        return;
    
    if(Size <= Handle->Size + Handle->Offset)
    {
        Handle->Offset += (s32)(Handle->Size - Size);
        Handle->Size = Size;
    }
    else
    {
        if(Handle->Anchored)
        {
            Platform_ThrowError_DEBUG("Cannot increase the size of an anchored handle.");
            return;
        }
        
        heap1 *Heap1 = Heap1_GetHeap1(Handle);
        heap1_handle *NewHandle = Heap1_Alloc(Heap1, Size);
        Mem_Cpy(NewHandle->Data, Handle->Data, Handle->Size);
        
        heap1_handle Temp = *Handle;
        Handle->Data = NewHandle->Data;
        Handle->Size = NewHandle->Size;
        Handle->Offset = NewHandle->Offset;
        
        NewHandle->Data = Temp.Data;
        NewHandle->Size = Temp.Size;
        NewHandle->Offset = Temp.Offset;
        
        u16 *PrevHandleBlockIndex = NULL;
        u16 *PrevNewHandleBlockIndex = NULL;
        u16 *PrevNextBlockIndex = &Heap1->FirstBlockIndex;
        while(*PrevNextBlockIndex != U16_MAX &&
              (PrevHandleBlockIndex == NULL ||
               PrevNewHandleBlockIndex == NULL))
        {
            if(*PrevNextBlockIndex == Handle->Index)
                PrevHandleBlockIndex = PrevNextBlockIndex;
            if(*PrevNextBlockIndex == NewHandle->Index)
                PrevNewHandleBlockIndex = PrevNextBlockIndex;
            
            PrevNextBlockIndex = &Heap1_GetHandle(Heap1, *PrevNextBlockIndex)->NextBlockIndex;
        }
        ASSERT(PrevHandleBlockIndex && PrevNewHandleBlockIndex);
        
        *PrevHandleBlockIndex = NewHandle->Index;
        *PrevNewHandleBlockIndex = Handle->Index;
        Handle->NextBlockIndex = NewHandle->NextBlockIndex;
        NewHandle->NextBlockIndex = Temp.NextBlockIndex;
        
        Heap1_Free(NewHandle);
    }
}

internal void
Heap1_Free(heap1_handle *Handle)
{
    ASSERT(Handle);
    ASSERT(Handle->Data);
    ASSERT(Handle->Mapped == TRUE);
    heap1 *Heap1 = Heap1_GetHeap1(Handle);
    
    u16 *PrevNextMappedIndex = &Heap1->FirstMappedIndex;
    while(*PrevNextMappedIndex != U16_MAX &&
          *PrevNextMappedIndex != Handle->Index)
    {
        PrevNextMappedIndex = &Heap1_GetHandle(Heap1, *PrevNextMappedIndex)->NextIndex;
    }
    *PrevNextMappedIndex = Handle->NextIndex;
    Handle->Mapped = FALSE;
    
    // Rightmost/highest unmapped handles can be deleted
    if(Handle->Index == Heap1->HandleCount - 1)
    {
        Handle->NextIndex = Heap1->FirstUnmappedIndex;
        
        heap1_handle *Iter = Handle;
        while(!Iter->Mapped)
        {
            --Heap1->HandleCount;
            if(Iter->Index == 0)
                break;
            
            --Iter;
        }
        
        if(!Iter->Mapped)
        {
            Heap1->FirstUnmappedIndex = Iter->NextIndex;
        }
    }
    else
    {
        u16 *PrevNextIndex = &Heap1->FirstUnmappedIndex;
        while(*PrevNextIndex != U16_MAX &&
              *PrevNextIndex > Handle->Index)
        {
            PrevNextIndex = &Heap1_GetHandle(Heap1, *PrevNextIndex)->NextIndex;
        }
        
        Handle->NextIndex = *PrevNextIndex;
        *PrevNextIndex = Handle->Index;
    }
    
    // Leftmost/lowest free data blocks can be deleted
    if(Handle->Data == Heap1->DataCursor)
    {
        //TODO: Add the offset, too
        Heap1->DataCursor += Handle->Size;
        Heap1->FirstBlockIndex = Handle->NextBlockIndex;
    }
    else
    {
        // Pop the handle from the block list
        u16 *PrevNextBlockIndex = &Heap1->FirstBlockIndex;
        while(*PrevNextBlockIndex != Handle->Index)
        {
            PrevNextBlockIndex = &Heap1_GetHandle(Heap1, *PrevNextBlockIndex)->NextBlockIndex;
            ASSERT(*PrevNextBlockIndex != U16_MAX);
        }
        heap1_handle *PrevBlockHandle = (heap1_handle*)((mem)PrevNextBlockIndex - OFFSET_OF(heap1_handle, NextBlockIndex));
        *PrevNextBlockIndex = Handle->NextBlockIndex;
        
        // Must be a previous block, otherwise it would be leftmost
        PrevBlockHandle->Offset += Handle->Size + Handle->Offset;
    }
    
    Handle->Data = NULL;
    Handle->Size = 0;
    Handle->Offset = 0;
    Handle->NextBlockIndex = U16_MAX;
}
#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Heap Allocator 0
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//TODO: Implement segregated lists
//TODO: Reconsider alignment

internal b08
Heap_IsSentry(heap_header *Header)
{
    b08 Result;
    
    ASSERT(Header);
    
    if(Header->Used == HEAP_MAGIC_NUMBER)
    {
        Result = TRUE;
    }
    else
    {
        Result = FALSE;
    }
    
    return Result;
}

internal u32
Heap_GetSize(hvptr Data)
{
    u32 Result;
    
    ASSERT(Data);
    
    Result = Heap_GetHeader(Data)->Size;
    
    return Result;
}

internal heap_header *
Heap_GetHeader(hvptr Data)
{
    heap_header *Result;
    
    ASSERT(Data);
    
    Result = (heap_header*)((u08*)Data - sizeof(heap_header));
    
    return Result;
}

internal hvptr
Heap_GetData(heap_header *Header)
{
    hvptr Result;
    
    ASSERT(Header);
    
    Result = PTR_JUMP(Header, sizeof(heap_header));
    
    return Result;
}

internal heap_header *
Heap_GetNext(heap_header *Header)
{
    heap_header *Result;
    
    ASSERT(Header);
    
    Result = (heap_header*)PTR_JUMP(Header, sizeof(heap_header) + Header->Size);
    
    return Result;
}

internal heap_header *
Heap_GetFirst(heap *Heap)
{
    heap_header *Result;
    
    ASSERT(Heap);
    
    Result = (heap_header*)PTR_JUMP(Heap, sizeof(heap));
    
    return Result;
}

internal heap *
Heap_GetHeap(heap_header *Header)
{
    heap *Result;
    
    ASSERT(Header);
    
    //TODO: Make this jump to a used block if it can, then it'll be one jump away from the sentry
    while(!Heap_IsSentry(Header))
    {
        Header = Header->NextFree;
    }
    Result = (heap*)Header;
    
    return Result;
}

internal heap *
Heap_Create(vptr Base,
            u32 Size)
{
#pragma warning(push)
#pragma warning(disable:4127)
    ASSERT(sizeof(heap_header) == sizeof(heap));
#pragma warning(pop)
    ASSERT(Size > sizeof(heap) + sizeof(heap_header));
    
    ASSERT(Base);
    //TODO: For different platforms, make sure this is zeroed
    
    heap *Heap = (heap*)Base;
    Heap->BlockCount = 1;
    Heap->MagicNumber = HEAP_MAGIC_NUMBER;
    
    Heap->FreeList = (heap_header*)PTR_JUMP(Base, sizeof(heap));
    Heap->FreeList->Size = Size - sizeof(heap) - sizeof(heap_header);
    Heap->FreeList->Used = FALSE;
    Heap->FreeList->NextFree = (heap_header*)Heap;
    
    return Heap;
}

internal hvptr
Heap_AllocateAligned(heap *Heap,
                     u32 Size,
                     u32 Alignment,
                     b08 ShouldZeroMemory)
{
    hvptr Result;
    
    ASSERT(Heap);
    
    if(Heap_IsSentry(Heap->FreeList))
    {
        STOP;
        return NULL;
    }
    
    heap_header *Prev = (heap_header*)Heap;
    heap_header *Curr = Heap->FreeList;
    heap_header *AllocatedBlock = NULL;
    
    while(!Heap_IsSentry(Curr))
    {
        ASSERT(!Curr->Used);
        
        vptr ExpectedBase = (u08*)Heap_GetNext(Curr) - Size;
        vptr AlignedBase = (vptr)ALIGN_DOWN_64((u64)ExpectedBase, Alignment);
        u32 Padding = (u32)((u64)ExpectedBase - (u64)AlignedBase);
        u32 SplitThreshold = (u32)(Size + Padding + sizeof(heap_header));
        
        if(Padding == 0 &&
           Curr->Size >= Size &&
           Curr->Size <= SplitThreshold)
        {
            Prev->NextFree = Curr->NextFree;
            
            AllocatedBlock = Curr;
            AllocatedBlock->NextFree = (heap_header*)Heap;
            AllocatedBlock->Used = TRUE;
            
            break;
        }
        else if(Curr->Size > SplitThreshold)
        {
            AllocatedBlock = (heap_header*)((u08*)AlignedBase - sizeof(heap_header));
            AllocatedBlock->Size = Size + Padding;
            AllocatedBlock->NextFree = (heap_header*)Heap;
            AllocatedBlock->Used = TRUE;
            
            Curr->Size -= Size + Padding + sizeof(heap_header);
            Heap->BlockCount++;
            
            break;
        }
        
        Prev = Curr;
        Curr = Curr->NextFree;
    }
    
    if(AllocatedBlock)
    {
        Result = Heap_GetData(AllocatedBlock);
        
        if(ShouldZeroMemory)
        {
            Mem_Zero(Result, AllocatedBlock->Size);
        }
    }
    else
    {
        //TODO: Handle this, e.g. defragmentation
        STOP;
        Result = NULL;
    }
    
    return Result;
}

internal void
Heap_Copy(hvptr Dest,
          hvptr Src,
          b08 ShouldZeroDest)
{
    if(!Dest ||
       !Src)
    {
        return;
    }
    
    heap_header *DestHeader = Heap_GetHeader(Dest);
    heap_header *SrcHeader = Heap_GetHeader(Src);
    ASSERT(DestHeader->Size >= SrcHeader->Size);
    
    Mem_Cpy(Dest, Src, SrcHeader->Size);
    
    if(ShouldZeroDest)
    {
        Mem_Zero((u08*)Dest + SrcHeader->Size, DestHeader->Size - SrcHeader->Size);
    }
}

internal void
Heap_Free(hvptr Data)
{
    //TODO: Maybe implement a binary search?
    
    if(!Data)
    {
        return;
    }
    
    heap_header *Header = Heap_GetHeader(Data);
    heap *Heap = Heap_GetHeap(Header);
    
    ASSERT(!Heap_IsSentry(Heap->FreeList));
    ASSERT(Heap->BlockCount > 0);
    
    ASSERT(Header->Used);
    Header->Used = FALSE;
    
    heap_header *PrevFree = (heap_header*)Heap;
    heap_header *Prev = NULL;
    heap_header *Curr = Heap_GetFirst(Heap);
    heap_header *Next = Heap_GetNext(Curr);
    
    for(u32 BlockIndex = 0;
        BlockIndex < Heap->BlockCount;
        ++BlockIndex)
    {
        if(Curr == Header)
        {
            if(BlockIndex == Heap->BlockCount - 1)
            {
                Next = NULL;
            }
            
            if(Prev && !Prev->Used) // P
            {
                ASSERT(Prev == PrevFree);
                Prev->Size += Header->Size + sizeof(heap_header);
                Header = Prev;
                
                Heap->BlockCount--;
                
                if(Next &&
                   !Next->Used) // PN
                {
                    ASSERT(PrevFree->NextFree == Next);
                    Header->Size += Next->Size + sizeof(heap_header);
                    Header->NextFree = Next->NextFree;
                    
                    Heap->BlockCount--;
                }
            }
            else // P'
            {
                if(Next &&
                   !Next->Used) // P'N
                {
                    ASSERT(PrevFree->NextFree == Next);
                    Header->Size += Next->Size + sizeof(heap_header);
                    Header->NextFree = Next->NextFree;
                    
                    Heap->BlockCount--;
                }
                else // P'N'
                {
                    Header->NextFree = PrevFree->NextFree;
                }
                
                PrevFree->NextFree = Header;
            }
            
            
            break;
        }
        
        if(!Curr->Used)
        {
            PrevFree = Curr;
        }
        
        Prev = Curr;
        Curr = Next;
        Next = Heap_GetNext(Next);
    }
}

typedef struct json_token_test
{
    json_token_type Type;
    s32 StartOffset;
    s32 EndOffset;
    s32 ParentToken;
    u32 ChildCount;
} json_token_test;

//TODO: Alignment
internal void
Heap_Resize(hvptr *Data,
            u32 Size)
{
    ASSERT(Data && *Data);
    
    heap_header *Header = Heap_GetHeader(*Data);
    heap *Heap = Heap_GetHeap(Header);
    
    if(Header->Size > Size + sizeof(heap_header)) // Split into two
    {
        heap_header *PrevFree = (heap_header*)Heap;
        
        while(PrevFree->NextFree != (heap_header*)Heap &&
              (u64)PrevFree->NextFree < (u64)Header)
        {
            PrevFree = PrevFree->NextFree;
        } // PrevFree < Header < PrevFree->NextFree
        
        heap_header *FreedBlock = (heap_header*)PTR_JUMP(*Data, Size);
        FreedBlock->Size = Header->Size - sizeof(heap_header) - Size;
        FreedBlock->Used = TRUE;
        FreedBlock->NextFree = (heap_header*)Heap;
        ++Heap->BlockCount;
        
        Header->Size = Size;
        
        Heap_Free(Heap_GetData(FreedBlock));
        
        *Data = Heap_GetData(Header);
    }
    else if(Header->Size < Size) // Allocate new block
    {
        u32 PrevSize = Header->Size;
        
        hvptr AllocatedData = Heap_Allocate(Heap, Size);
        Mem_Cpy(AllocatedData, *Data, PrevSize);
        Heap_Free(*Data);
        
        *Data = AllocatedData;
    }
}


//SUBSECTION: Heap Debug


#if _MODE_DEBUG
internal void
_Heap_Mark_DEBUG(heap *Heap,
                 heap_data_DEBUG *DebugData)
{
    ASSERT(Heap_IsCorrupted_DEBUG(Heap) == FALSE);
    
    if(!DebugData->DebugHeap)
    {
        u32 HeapSize = MEBIBYTES(2);
        hvptr HeapBase = Platform_AllocateMemory(HeapSize);
        DebugData->DebugHeap = Heap_Create(HeapBase, HeapSize);
    }
    
    Heap_Free(DebugData->List);
    
    DebugData->List = Heap_Allocate(DebugData->DebugHeap, (Heap->BlockCount + 1) * sizeof(heap_header));
    heap_header *Block = Heap_GetFirst(Heap);
    for(u32 BlockIndex = 0;
        BlockIndex < Heap->BlockCount;
        ++BlockIndex)
    {
        DebugData->List[BlockIndex] = *Block;
        Block = Heap_GetNext(Block);
    }
    
    DebugData->BlockCount = Heap->BlockCount;
}

internal void
_Heap_Check_DEBUG(heap *Heap,
                  heap_data_DEBUG *DebugData)
{
    ASSERT(DebugData->List);
    ASSERT(DebugData->BlockCount == Heap->BlockCount);
    ASSERT(Heap_IsCorrupted_DEBUG(Heap) == FALSE);
    
    heap_header *Block = Heap_GetFirst(Heap);
    for(u32 BlockIndex = 0;
        BlockIndex < DebugData->BlockCount;
        ++BlockIndex)
    {
        ASSERT(Mem_Cmp(DebugData->List + BlockIndex, Block, sizeof(heap_header)) == 0);
        Block = Heap_GetNext(Block);
    }
}

internal b08
_Heap_IsCorrupted_DEBUG(heap *Heap)
{
    b08 Result = FALSE;
    
    if(!Heap ||
       !Heap->BlockCount ||
       !Heap->FreeList ||
       Heap->FreeList == (heap_header*)Heap ||
       Heap->MagicNumber != HEAP_MAGIC_NUMBER)
    {
        Result = TRUE;
    }
    
    u32 SizeTotal = sizeof(heap);
    u32 PrevUsed = TRUE;
    heap_header *Header = Heap_GetFirst(Heap);
    u32 BlockIndex = 0;
    while(BlockIndex < Heap->BlockCount && !Heap_IsSentry(Header))
    {
        if((Header->Used != 0 && Header->Used != 1) ||
           !Header->NextFree ||
           (u64)Header != (u64)Heap + SizeTotal)
        {
            Result = TRUE;
        }
        
        if((Header->Used == 1 && Header->NextFree != (heap_header*)Heap) ||
           (Header->Used == 0 && Header->NextFree->Used != 0 && Header->NextFree->Used != HEAP_MAGIC_NUMBER) ||
           (Header->Used == 0 && BlockIndex + 1 < Heap->BlockCount && Heap_GetNext(Header)->Used == 0) ||
           (Header->Used == 0 && PrevUsed == 0))
        {
            Result = TRUE;
        }
        
        BlockIndex++;
        PrevUsed = Header->Used;
        SizeTotal += sizeof(heap_header) + Header->Size;
        Header = Heap_GetNext(Header);
    }
    
    return Result;
}

internal void
_Heap_Print_DEBUG(heap *Heap,
                  heap_data_DEBUG *DebugData)
{
    ASSERT(Heap);
    
    //TODO: Custom printf
    
    heap_data_DEBUG TempDebugData = {0};
    TempDebugData.DebugHeap = DebugData->DebugHeap;
    heap *StrHeap = Str_GetHeap();
    Str_SetHeap(DebugData->DebugHeap);
    Heap_Mark_DEBUG(DebugData->DebugHeap, &TempDebugData);
    
    str TempStr;
    str Str = Str_Create(NULL, "\n\nHeap:\n", 0);
    str Header = Str_Create(NULL, "__________________", 0);
    str Section = Str_Create(NULL, "| __ __ __ __ __ |", 0);
    str UsedSeg = Str_Create(NULL, "|      Used      |", 0);
    str FreeSeg = Str_Create(NULL, "|      Free      |", 0);
    str Footer = Str_Create(NULL, "|________________|", 0);
    str FreeIndent = Str_Create(NULL, "        ", 0);
    str UsedIndent = Str_Create(NULL, "            ", 0);
    
    heap_header *NextFree = Heap->FreeList;
    
    Str_Cat(&Str, FreeIndent);
    Str_Cat(&Str, Header);
    Str_CatC(&Str, "\n");
    
    u32 LineOffset = Str_Len(Str);
    Str_Cat(&Str, FreeIndent);
    Str_Cat(&Str, Footer);
    Str_CatC(&Str, " ");
    Str_Cat(&Str, U32_To_Str(&TempStr, sizeof(heap))); Str_Free(TempStr);
    Str_CatC(&Str, " bytes\n");
    if(NextFree)
    {
        Mem_Cpy(Str + LineOffset + 2, "+---", 4);
    }
    
    heap_header *Curr = Heap_GetFirst(Heap);
    for(u32 BlockIndex = 0;
        BlockIndex < Heap->BlockCount;
        ++BlockIndex)
    {
        if(BlockIndex == 140)
            NOP;
        
        str Indent;
        if(Curr->Used)
        {
            Str_Create(&Indent, UsedIndent, 0);
        }
        else
        {
            Str_Create(&Indent, FreeIndent, 0);
        }
        if(NextFree && Curr->Used)
        {
            Mem_Cpy(Indent + 2, "|", 1);
        }
        
        LineOffset = Str_Len(Str);
        Str_Cat(&Str, Indent);
        Str_Cat(&Str, Header);
        Str_CatC(&Str, "\n");
        if(Curr == NextFree)
        {
            Mem_Cpy(Str + LineOffset + 2, "|", 1);
        }
        
        LineOffset = Str_Len(Str);
        Str_Cat(&Str, Indent);
        Str_Cat(&Str, Section);
        Str_CatC(&Str, " ");
        Str_Cat(&Str, U32_To_Str(&TempStr, sizeof(heap))); Str_Free(TempStr);
        Str_CatC(&Str, " + ");
        Str_Cat(&Str, U32_To_Str(&TempStr, Curr->Size)); Str_Free(TempStr);
        Str_CatC(&Str, " bytes\n");
        if(Curr == NextFree)
        {
            Mem_Cpy(Str + LineOffset + 2, "+-->", 4);
        }
        
        if(Str_Len(Str) > 2610)
            NOP;
        
        Str_Cat(&Str, Indent);
        if(Curr->Used)
        {
            Str_Cat(&Str, UsedSeg);
        }
        else
        {
            Str_Cat(&Str, FreeSeg);
        }
        Str_CatC(&Str, " ");
        Str_Cat(&Str, U64_To_Str_Hex(&TempStr, (u64)Heap_GetData(Curr))); Str_Free(TempStr);
        Str_CatC(&Str, "\n");
        
        LineOffset = Str_Len(Str);
        Str_Cat(&Str, Indent);
        Str_Cat(&Str, Footer);
        Str_CatC(&Str, "\n");
        if(Curr->NextFree)
        {
            Mem_Cpy(Str + LineOffset + 2, "+---", 4);
        }
        
        Str_Free(Indent);
        if(Curr == NextFree)
        {
            NextFree = Curr->NextFree;
        }
        Curr = Heap_GetNext(Curr);
    }
    
    mem FileHandle;
    Platform_OpenFile(&FileHandle, "build\\DEBUGOutput.txt", FileMode_Write);
    Platform_WriteFile(Str, &FileHandle, Str_Len(Str), 0);
    Platform_CloseFile(&FileHandle);
    
    Str_Free(Str);
    Str_Free(Header);
    Str_Free(Section);
    Str_Free(UsedSeg);
    Str_Free(FreeSeg);
    Str_Free(Footer);
    Str_Free(FreeIndent);
    Str_Free(UsedIndent);
    
    Heap_Check_DEBUG(DebugData->DebugHeap, &TempDebugData);
    Heap_Free(TempDebugData.List);
    Str_SetHeap(StrHeap);
}
#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Stack Allocator
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//TODO: implement extending/switching stacks
internal void
Stack_Init(vptr Base,
           u64 Size)
{
    UtilState->Stack = Base;
    UtilState->Stack->Cursor = (u08*)Base + sizeof(stack);
    UtilState->Stack->Size = Size;
    UtilState->Stack->NewestMarker = NULL;
}

internal b08
Stack_HasOverflowed(void)
{
    b08 Result = FALSE;
    
    if((u64)UtilState->Stack->Cursor - (u64)UtilState->Stack > UtilState->Stack->Size)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal b08
Stack_IsCorrupted(void)
{
    b08 Result = FALSE;
    
    if(!UtilState->Stack ||
       !UtilState->Stack->Cursor ||
       !UtilState->Stack->Size)
    {
        ASSERT(UtilState->Stack);
        ASSERT(UtilState->Stack->Cursor);
        ASSERT(UtilState->Stack->Size);
        Result = TRUE;
    }
    else if(UtilState->Stack->NewestMarker && (u64)UtilState->Stack->Cursor < (u64)UtilState->Stack->NewestMarker + sizeof(stack_marker))
    {
        Result = TRUE;
    }
    else
    {
        stack_marker *CurrMarker = UtilState->Stack->NewestMarker;
        
        while(CurrMarker && CurrMarker->PrevMarker)
        {
            if((u64)CurrMarker < (u64)CurrMarker->PrevMarker + sizeof(stack_marker))
            {
                Result = TRUE;
            }
            
            CurrMarker = CurrMarker->PrevMarker;
        }
    }
    
    return Result;
}

internal void
Stack_Push(void)
{
    ASSERT(UtilState->Stack);
    
    stack_marker *Marker = (stack_marker*)UtilState->Stack->Cursor;
    Marker->PrevMarker = UtilState->Stack->NewestMarker;
    
    UtilState->Stack->NewestMarker = Marker;
    UtilState->Stack->Cursor += sizeof(stack_marker);
    
    ASSERT(!Stack_HasOverflowed());
    ASSERT(!Stack_IsCorrupted());
}

internal void
Stack_Pop(void)
{
    ASSERT(UtilState->Stack);
    ASSERT(UtilState->Stack->NewestMarker->PrevMarker < UtilState->Stack->NewestMarker);
    
    if(UtilState->Stack->NewestMarker)
    {
        UtilState->Stack->Cursor = (u08*)UtilState->Stack->NewestMarker;
    }
    else
    {
        UtilState->Stack->Cursor = (u08*)UtilState->Stack + sizeof(stack);
    }
    UtilState->Stack->NewestMarker = UtilState->Stack->NewestMarker->PrevMarker;
    
    ASSERT(!Stack_IsCorrupted());
}

internal svptr
Stack_Allocate(u64 Size)
{
    svptr Result;
    
    ASSERT(UtilState->Stack);
    
    Result = UtilState->Stack->Cursor;
    UtilState->Stack->Cursor += Size;
    
    ASSERT(!Stack_HasOverflowed());
    ASSERT(!Stack_IsCorrupted());
    
    return Result;
}

//TODO: This needs to be redone
#define PointerCount(Ptr, Type) PointerCount_(Ptr, sizeof(Type))
//NOTE: Requires zeroed memory
internal u32
PointerCount_(void *Ptr,
              u32 UnitSize)
{
    u32 Size = 0;
    u08 *Unit = (u08*)Ptr;
    while(TRUE)
    {
        for(u32 ByteIndex = 0;
            ByteIndex < UnitSize;
            ++ByteIndex)
        {
            if(*Unit++ != 0)    
            {
                Size++;
                Unit += UnitSize - ByteIndex - 1;
                goto cont;
            }
        }
        break;
        
        cont:
        continue;
    }
    return Size;
}