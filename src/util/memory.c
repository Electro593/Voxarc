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






internal void
Heap_Init(heap *Heap,
          u64 Size)
{
    Heap->TotalSize = Size;
    Heap->HandleCount = 1;
    Heap->FirstFree = HEAP_NULL_HANDLE;
    Heap->LastFree = HEAP_NULL_HANDLE;
    
    heap_handle *NullHandle = (heap_handle*)(Heap+1);
    NullHandle->Data = (u08*)(Heap+1);
    NullHandle->Size = sizeof(heap_handle);
    NullHandle->Offset = Size - NullHandle->Size;
    NullHandle->Index = 0;
    NullHandle->Prev = 0;
    NullHandle->Next = 0;
    NullHandle->PrevBlock = 0;
    NullHandle->NextBlock = 0;
    NullHandle->Free = FALSE;
}

internal void
Heap_Defragment(heap *Heap)
{
    
}

internal heap_handle *
Heap_Allocate(heap *Heap,
              u64 Size)
{
    heap_handle *Handle, *Handles, *PrevUsed;
    b08 Defragmented;
    
    Assert(Heap);
    
    Handles = (heap_handle*)(Heap+1);
    Handle = NULL;
    Defragmented = FALSE;
    
    if(Heap->FirstFree == HEAP_NULL_HANDLE) {
        if(Handles[0].Offset < sizeof(heap_handle)) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            Assert(Handles[0].Offset >= sizeof(heap_handle),
                   "Not enough memory for new heap handle");
        }
        
        Handles[0].Size += sizeof(heap_handle);
        Handles[0].Offset -= sizeof(heap_handle);
        
        Handle = Handles + Heap->HandleCount;
        PrevUsed = Handles + Handles[0].Prev;
        Handle->Index = Heap->HandleCount;
        Heap->HandleCount++;
    } else {
        Handle = Handles + Heap->FirstFree;
        Assert(Handle->Index != 0);
        
        if(Handle->Next != HEAP_NULL_HANDLE)
            Handles[Handle->Next].Prev = HEAP_NULL_HANDLE;
        Heap->FirstFree = Handle->Next;
        if(Heap->LastFree == Handle->Index)
            Heap->LastFree = HEAP_NULL_HANDLE;
        
        PrevUsed = Handle-1;
        while(PrevUsed->Index && PrevUsed->Free) PrevUsed--;
    }
    Handle->Prev = PrevUsed->Index;
    Handle->Next = PrevUsed->Next;
    Handles[Handle->Prev].Next = Handle->Index;
    Handles[Handle->Next].Prev = Handle->Index;
    Handle->Size = Size;
    Handle->Free = FALSE;
    
    
    heap_handle *PrevBlock = Handles + Handles[0].PrevBlock;
    while(PrevBlock->Index && PrevBlock->Offset < Size)
        PrevBlock = Handles+PrevBlock->PrevBlock;
    
    if(PrevBlock->Index == 0) {
        if(!Defragmented && Handles[0].Offset < Size) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            
            Assert(Handles[0].Offset >= Size,
                   "Not enough memory for new heap block");
        }
    }
    Handle->Data = PrevBlock->Data + PrevBlock->Size + PrevBlock->Offset - Size;
    PrevBlock->Offset -= Size;
    Handle->PrevBlock = PrevBlock->Index;
    Handle->NextBlock = PrevBlock->NextBlock;
    Handles[Handle->NextBlock].PrevBlock = Handle->Index;
    Handles[Handle->PrevBlock].NextBlock = Handle->Index;
    Handle->Offset = 0;
    
    return Handle;
}

internal void
Heap_Free(heap_handle *Handle)
{
    Assert(Handle);
    Assert(!Handle->Free);
    
    heap_handle *Handles = Handle - Handle->Index;
    heap *Heap = (heap*)Handles-1;
    
    Handles[Handle->PrevBlock].Offset += Handle->Size + Handle->Offset;
    Handles[Handle->PrevBlock].NextBlock = Handle->NextBlock;
    Handles[Handle->NextBlock].PrevBlock = Handle->PrevBlock;
    Handles[Handle->Next].Prev = Handle->Prev;
    Handles[Handle->Prev].Next = Handle->Next;
    
    if(Handle->Next == HEAP_NULL_HANDLE) {
        if(Handle->Prev == HEAP_NULL_HANDLE) {
            Heap->FirstFree = HEAP_NULL_HANDLE;
            Heap->LastFree = HEAP_NULL_HANDLE;
        } else {
            heap_handle *PrevFree = Handles + Handle->Prev;
            while(PrevFree->Index) {
                PrevFree--;
                if(PrevFree->Free) {
                    PrevFree->Next = HEAP_NULL_HANDLE;
                    Heap->LastFree = PrevFree->Index;
                    goto FoundPrevFree1;
                }
            }
            Heap->FirstFree = HEAP_NULL_HANDLE;
            Heap->LastFree = HEAP_NULL_HANDLE;
            FoundPrevFree1:;
            
            Handles[Handle->Prev].Next = 0;
        }
        
        u16 Offset = Handle->Prev + 1;
        Mem_Set(Handles+Offset, 0, (Heap->HandleCount-Offset) * sizeof(heap_handle));
        Heap->HandleCount = Offset;
    } else {
        Handle->Data = NULL;
        Handle->Size = 0;
        Handle->Offset = 0;
        Handle->PrevBlock = 0;
        Handle->NextBlock = 0;
        Handle->Free = TRUE;
        
        heap_handle *PrevFree = Handle;
        while(PrevFree->Index) {
            PrevFree--;
            if(PrevFree->Free) {
                Handle->Prev = PrevFree->Index;
                Handle->Next = PrevFree->Next;
                if(PrevFree->Next != HEAP_NULL_HANDLE)
                    Handles[PrevFree->Next].Prev = Handle->Index;
                else
                    Heap->LastFree = Handle->Index;
                PrevFree->Next = Handle->Index;
                goto FoundNextFree;
            }
        }
        Handle->Prev = HEAP_NULL_HANDLE;
        Handle->Next = Heap->FirstFree;
        if(Handle->Next != HEAP_NULL_HANDLE)
            Handles[Handle->Next].Prev = Handle->Index;
        else
            Heap->LastFree = Handle->Index;
        Heap->FirstFree = Handle->Index;
        FoundNextFree:;
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