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
    
    heap_handle *NullUsedHandle = (heap_handle*)(Heap+1);
    NullUsedHandle->Data = (u08*)(Heap+1);
    NullUsedHandle->Size = 2*sizeof(heap_handle);
    NullUsedHandle->Offset = Size - NullUsedHandle->Size;
    NullUsedHandle->Index = 0;
    NullUsedHandle->Prev = 0;
    NullUsedHandle->Next = 0;
    NullUsedHandle->PrevBlock = 0;
    NullUsedHandle->NextBlock = 0;
    NullUsedHandle->Free = FALSE;
    
    heap_handle *NullFreeHandle = NullUsedHandle+1;
    NullFreeHandle->Data = 0;
    NullFreeHandle->Size = 0;
    NullFreeHandle->Offset = 0;
    NullFreeHandle->Index = 1;
    NullFreeHandle->Prev = 1;
    NullFreeHandle->Next = 1;
    NullFreeHandle->PrevBlock = 1;
    NullFreeHandle->NextBlock = 1;
    NullFreeHandle->Free = TRUE;
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
    
    heap_handle *PrevUsed;
    if(Handles[1].Next == 1) {
        if(Handles[0].Offset < sizeof(heap_handle)) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            Assert(Handles[0].Offset >= sizeof(heap_handle), "Not enough memory for new heap handle");
        }
        
        u16 HandleCount = (u16)(Handles[0].Size / sizeof(heap_handle));
        Handle = Handles + HandleCount;
        Handle->Index = HandleCount;
        
        Handles[0].Size += sizeof(heap_handle);
        Handles[0].Offset -= sizeof(heap_handle);
        
        PrevUsed = Handles + Handles[0].Prev;
    } else {
        Handle = Handles + Handles[1].Next;
        Handles[Handle->Prev].Next = Handle->Next;
        Handles[Handle->Next].Prev = Handle->Prev;
        
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
    while(PrevBlock->Index && PrevBlock->Offset < Size) {
        PrevBlock = Handles+PrevBlock->PrevBlock;
    }
    if(Handles[0].Offset < Size && !Defragmented) {
        Heap_Defragment(Heap);
        Assert(Handles[0].Offset >= Size, "Not enough memory for new heap block");
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
    
    if(Handle->Next == 0) {
        heap_handle *PrevFree;
        if(Handle->Prev == 0) {
            PrevFree = Handles+1;
        } else {
            PrevFree = Handles + Handle->Prev;
            while(PrevFree->Index > 1 && !PrevFree->Free) PrevFree--;
        }
        Handles[1].Prev = PrevFree->Index;
        PrevFree->Next = 1;
        
        u16 Offset = Handle->Prev + 1;
        u64 DeltaSize = Handles[0].Size - Offset*sizeof(heap_handle);
        Handles[0].Size -= DeltaSize;
        Handles[0].Offset += DeltaSize;
        Mem_Set(Handles+Offset, 0, DeltaSize);
    } else {
        Handle->Data = NULL;
        Handle->Size = 0;
        Handle->Offset = 0;
        Handle->PrevBlock = 0;
        Handle->NextBlock = 0;
        Handle->Free = TRUE;
        
        heap_handle *PrevFree = Handle;
        while(PrevFree->Index && !PrevFree->Free) PrevFree--;
        Handle->Prev = PrevFree->Index;
        Handle->Next = PrevFree->Next;
        Handles[Handle->Next].Prev = Handle->Index;
        Handles[Handle->Prev].Next = Handle->Index;
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