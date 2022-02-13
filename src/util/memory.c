/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal vptr
Mem_Set(vptr Dest,
        s32 Data,
        u64 Size)
{
    u08 *Dest08 = (u08*)Dest;
    u08 Data08 = (u08)Data;
    while(Size--) {
        *Dest08++ = Data08;
    }
    return Dest;
}

internal vptr
Mem_Cpy(vptr Dest,
        vptr Src,
        u64 Size)
{
    u08 *Dest08 = (u08*)Dest;
    u08 *Src08 = (u08*)Src;
    while(Size--) {
        *Dest08++ = *Src08++;
    }
    return Dest;
}

internal u64
Mem_BytesUntil(u08 *Data,
               u08 Byte)
{
    u64 Length = 0;
    while(*Data++ != Byte) Length++;
    return Length;
}






internal heap *
Heap_Create(vptr Memory,
            u64 Size)
{
    heap *Heap = (heap*)Memory;
    
    Heap->TotalSize = Size;
    Heap->HandleCount = 0;
    Heap->FreeList = HEAP_NULL_HANDLE;
    Heap->UsedList = HEAP_NULL_HANDLE;
    Heap->BlockList = HEAP_NULL_HANDLE;
    Heap->DataCursor = (u08*)Memory + Size;
    
    return Heap;
}

internal void
Heap_Defragment(heap *Heap)
{
    
}

internal heap_handle *
Heap_Allocate(heap *Heap,
              u64 Size)
{
    Assert(Heap);
    
    heap_handle *Handles = (heap_handle*)(Heap+1);
    heap_handle *Handle = NULL;
    b08 Defragmented = FALSE;
    
    if(Heap->FreeList = HEAP_NULL_HANDLE) {
        Heap->HandleCount++;
        
        if((u08*)(Handles + Heap->HandleCount) > Heap->DataCursor) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            
            Assert((u08*)(Handles + Heap->HandleCount) <= Heap->DataCursor,
                   "Not enough memory for new heap handle");
        }
        
        Handle = Handles + Heap->HandleCount-1;
        Handle->Index = Heap->HandleCount - 1;
    } else {
        Handle = Handles + Heap->FreeList;
        Assert(Handle->Free);
        Heap->FreeList = Handle->Next;
        if(Handle->Next != HEAP_NULL_HANDLE) {
            Handles[Handle->Next].Prev = HEAP_NULL_HANDLE;
        }
    }
    Handle->Free = FALSE;
    Handle->Size = Size;
    Handle->Next = Heap->UsedList;
    Heap->UsedList = Handle->Index;
    Handles[Handle->Next].Prev = Handle->Index;
    
    u16 Index = Heap->BlockList;
    while(Index != HEAP_NULL_HANDLE && Handles[Index].Offset < Size) {
        Index = Handles[Index]->NextBlock;
    }
    
    if(Index == HEAP_NULL_HANDLE) {
        if(!Defragmented && (u08*)(Handles + Heap->HandleCount) > Heap->DataCursor - Size) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            
            Assert((u08*)(Handles + Heap->HandleCount) <= Heap->DataCursor - Size,
                   "Not enough memory for new heap block");
        }
        
        Heap->DataCursor -= Size;
        Handle->Data = Heap->DataCursor;
        Handle->PrevBlock = HEAP_NULL_HANDLE;
        Handle->NextBlock = Heap->BlockList;
        if(Heap->BlockList != HEAP_NULL_HANDLE) {
            Handles[Heap->BlockList].PrevBlock = Handle->Index;
        }
    } else {
        heap_handle *NextBlock = Handles[Index];
        heap_handle *PrevBlock = NextBlock->PrevBlock;
        NextBlock->Offset -= Size;
        NextBlock->PrevBlock = Handle->Index;
        PrevBlock->NextBlock = Handle->Index;
        Handle->PrevBlock = PrevBlock->Index;
        Handle->NextBlock = NextBlock->Index;
        Handle->Data = NextBlock->Data - NextBlock->Offset;
    }
    Handle->Offset = 0;
    
    return Handle;
}

internal void
Heap_Free(heap_handle *Handle)
{
    heap *Heap;
    heap_handle *Handles, *PrevUsed, *PrevFree, *Iter;
    
    Assert(Handle);
    
    Handles = Handle - Handle->Index;
    Heap = (heap*)Handles-1;
    
    Handles[Handle->NextBlock].Offset += Handle->Size + Handle->Offset;
    Handles[Handle->PrevBlock].NextBlock = Handle->NextBlock;
    
    if(Handle->Next == HEAP_NULL_HANDLE) {
        if(Handle->Prev == HEAP_NULL_HANDLE) {
            Heap->UsedList = HEAP_NULL_HANDLE;
            Heap->FreeList = HEAP_NULL_HANDLE;
            Heap->HandleCount = 0;
            Mem_Set(Heap+1, 0, Heap->HandleCount * sizeof(heap_handle));
        } else {
            PrevUsed = Handles[Handle->Prev];
            
            PrevUsed->Next = HEAP_NULL_HANDLE;
            Mem_Set(PrevUsed+1, 0, (Heap->HandleCount-PrevUsed->Index-1)*sizeof(heap_handle));
            Heap->HandleCount = PrevUsed->Index + 1;
            
            PrevFree = PrevUsed;
            while(PrevFree->Index) {
                PrevFree--;
                if(PrevFree->Free) {
                    PrevFree->Next = HEAP_NULL_HANDLE;
                    goto SkipJankIf1;
                }
            }
            Heap->FreeList = HEAP_NULL_HANDLE;
            SkipJankIf1:
        }
    } else {
        Handle->Data = NULL;
        Handle->Size = 0;
        Handle->Offset = 0;
        Handle->PrevBlock = HEAP_NULL_HANDLE;
        Handle->NextBlock = HEAP_NULL_HANDLE;
        Handle->Free = TRUE;
        
        if(Handle->Prev == HEAP_NULL_HANDLE) {
            Heap->UsedList = Handle->Next;
        } else {
            Handles[Handle->Prev]->Next = Handle->Next;
        }
        
        PrevFree = Handle;
        while(PrevFree->Index) {
            PrevFree--;
            if(PrevFree->Free) {
                Handle->Next = PrevFree->Next;
                Handle->Prev = PrevFree->Index;
                PrevFree->Next = Handle->Index;
                goto SkipJankIf2;
            }
        }
        Handle->Next = Heap->FreeList;
        Handle->Prev = HEAP_NULL_HANDLE;
        Heap->FreeList = Handle->Index;
        SkipJankIf2:
    }
}





internal stack *
Stack_Init(vptr Mem,
           u64 Size)
{
    stack *Result = Mem;
    Result->Size = Size - sizeof(stack);
    Result->FirstMarker = (vptr*)&Result->FirstMarker;
    Result->Cursor = (u08*)Mem + sizeof(stack) + sizeof(vptr);
    return Result;
}

internal void
Stack_Push(void)
{
    vptr *Marker = (vptr*)Context.Stack->Cursor;
    *Marker = Context.Stack->FirstMarker;
    Context.Stack->FirstMarker = Marker;
    Context.Stack->Cursor += sizeof(vptr);
}

internal vptr
Stack_Allocate(u64 Size)
{
    vptr Result = Context.Stack->Cursor;
    Context.Stack->Cursor += Size;
    return Result;
}

internal void
Stack_Pop(void)
{
    Context.Stack->FirstMarker = *Context.Stack->FirstMarker;
    Context.Stack->Cursor = (u08*)Context.Stack->FirstMarker + sizeof(vptr);
}