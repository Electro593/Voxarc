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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//        SECTION: General Memory Operations
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//TODO: If we run out of memory, maybe have a notification that tells the user to free some?

//TODO: Multithread these
internal vptr
Mem_Set(vptr Dest,
        s32 Data,
        size Size)
{
    #if 0
    u08 Data08 = (u08)Data;
    i128 Data128 = I128_SetI08_1(Data08);
    
    size Alignment = ALIGN_DOWN_64(Size, 16);
    u08 *Dest08 = (u08*)Dest;
    while(Size > Alignment)
    {
        Size--;
        *Dest08++ = Data08;
    }
    
    i128 *Dest128 = (i128*)Dest08;
    while(Size >= sizeof(i128))
    {
        Size -= sizeof(i128);
        *Dest128++ = Data128;
    }
    
    Dest08 = Dest128->U08;
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
    
    size Alignment = ALIGN_DOWN_64(Size, 4);
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
        size Size)
{
    if(Src && Dest && Size > 0)
    {
        s32 Delta = 1;
        size StartOffset = 0;
        size Offset128 = 0;
        if((size)Src < (size)Dest &&
           (size)Src + Size >= (size)Dest)
        {
            Delta = -1;
            StartOffset = Size - 1;
            Offset128 = sizeof(i128) - 1;
        }
        //TODO: Alignment should be by address, not by size
        size Alignment = ALIGN_DOWN_64(Size, 16);
        u08 *Src08 = (u08*)Src + StartOffset;
        u08 *Dest08 = (u08*)Dest + StartOffset;
        while(Size > Alignment)
        {
            Size--;
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
        
        ASSERT(Size == 0);
        // Src08 = Src128->U08;
        // Dest08 = Dest128->U08;
        // while(Size)
        // {
        //     Size--;
        //     *Dest08 = *Src08;
        //     Dest08 += Delta;
        //     Src08 += Delta;
        // }
    }
    
    return Dest;
}

//TODO: Vectorize this
internal s32
Mem_Cmp(vptr A,
        vptr B,
        size Size)
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

//TODO: Segregated lists for allocation to decrease looping
//      Size and PrevSize to remove looping when freeing

#else

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
        vptr AlignedBase = (vptr)ALIGN_DOWN_64((size)ExpectedBase, Alignment);
        u32 Padding = (u32)((size)ExpectedBase - (size)AlignedBase);
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
              (size)PrevFree->NextFree < (size)Header)
        {
            PrevFree = PrevFree->NextFree;
        } // PrevFree < Header < PrevFree->NextFree
        
        heap_header *FreedBlock = (heap_header*)PTR_JUMP(*Data, Size);
        FreedBlock->Size = Header->Size - sizeof(heap_header) - Size;
        FreedBlock->Used = TRUE;
        FreedBlock->NextFree = (heap_header*)Heap;
        
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


#if 1
#if defined(_VOX_DEBUG)
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
           (size)Header != (size)Heap + SizeTotal)
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
    Str_Cat(&Str, U32_ToStr(&TempStr, sizeof(heap))); Str_Free(TempStr);
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
        Str_Cat(&Str, U32_ToStr(&TempStr, sizeof(heap))); Str_Free(TempStr);
        Str_CatC(&Str, " + ");
        Str_Cat(&Str, U32_ToStr(&TempStr, Curr->Size)); Str_Free(TempStr);
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
        Str_Cat(&Str, U64_ToStr_Hex(&TempStr, (u64)Heap_GetData(Curr))); Str_Free(TempStr);
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
    Platform_OpenFile(&FileHandle, "DEBUGOutput.txt", FileMode_Write);
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
    Str_SetHeap(Heap);
}
#endif
#endif

#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Stack Allocator
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//TODO: implement extending/switching stacks
internal void
Stack_Init(vptr Base,
           size Size)
{
    __Global->Stack = Base;
    __Global->Stack->Cursor = (u08*)Base + sizeof(stack);
    __Global->Stack->Size = Size;
    __Global->Stack->NewestMarker = NULL;
}

internal b08
Stack_HasOverflowed(void)
{
    b08 Result = FALSE;
    
    if((size)__Global->Stack->Cursor - (size)__Global->Stack > __Global->Stack->Size)
    {
        Result = TRUE;
    }
    
    return Result;
}

internal b08
Stack_IsCorrupted(void)
{
    b08 Result = FALSE;
    
    if(!__Global->Stack ||
       !__Global->Stack->Cursor ||
       !__Global->Stack->Size)
    {
        ASSERT(__Global->Stack);
        ASSERT(__Global->Stack->Cursor);
        ASSERT(__Global->Stack->Size);
        Result = TRUE;
    }
    else if(__Global->Stack->NewestMarker && (size)__Global->Stack->Cursor < (size)__Global->Stack->NewestMarker + sizeof(stack_marker))
    {
        Result = TRUE;
    }
    else
    {
        stack_marker *CurrMarker = __Global->Stack->NewestMarker;
        
        while(CurrMarker && CurrMarker->PrevMarker)
        {
            if((size)CurrMarker < (size)CurrMarker->PrevMarker + sizeof(stack_marker))
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
    ASSERT(__Global->Stack);
    
    stack_marker *Marker = (stack_marker*)__Global->Stack->Cursor;
    Marker->PrevMarker = __Global->Stack->NewestMarker;
    
    __Global->Stack->NewestMarker = Marker;
    __Global->Stack->Cursor += sizeof(stack_marker);
    
    ASSERT(!Stack_HasOverflowed());
    ASSERT(!Stack_IsCorrupted());
}

internal void
Stack_Pop(void)
{
    ASSERT(__Global->Stack);
    ASSERT(__Global->Stack->NewestMarker->PrevMarker < __Global->Stack->NewestMarker);
    
    if(__Global->Stack->NewestMarker)
    {
        __Global->Stack->Cursor = (u08*)__Global->Stack->NewestMarker;
    }
    else
    {
        __Global->Stack->Cursor = (u08*)__Global->Stack + sizeof(stack);
    }
    __Global->Stack->NewestMarker = __Global->Stack->NewestMarker->PrevMarker;
    
    ASSERT(!Stack_IsCorrupted());
}

internal svptr
Stack_Allocate(size Size)
{
    svptr Result;
    
    ASSERT(__Global->Stack);
    
    Result = __Global->Stack->Cursor;
    __Global->Stack->Cursor += Size;
    
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