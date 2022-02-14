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
    Heap->DataCursor = (u08*)Heap + Size;
    Heap->HandleCount = 0;
    Heap->FirstFree = HEAP_NULL_HANDLE;
    Heap->LastFree = HEAP_NULL_HANDLE;
    Heap->FirstUsed = HEAP_NULL_HANDLE;
    Heap->LastUsed = HEAP_NULL_HANDLE;
    Heap->FirstBlock = HEAP_NULL_HANDLE;
    Heap->LastBlock = HEAP_NULL_HANDLE;
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
        Heap->HandleCount++;
        
        if((u08*)(Handles + Heap->HandleCount) > Heap->DataCursor) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            
            Assert((u08*)(Handles + Heap->HandleCount) <= Heap->DataCursor,
                   "Not enough memory for new heap handle");
        }
        
        Handle = Handles + Heap->HandleCount-1;
        Handle->Index = Heap->HandleCount - 1;
        Handle->Prev = Heap->LastUsed;
        Handle->Next = HEAP_NULL_HANDLE;
        if(Heap->LastUsed != HEAP_NULL_HANDLE)
            Handles[Heap->LastUsed].Next = Handle->Index;
        Heap->LastUsed = Handle->Index;
        if(Heap->FirstUsed == HEAP_NULL_HANDLE)
            Heap->FirstUsed = Handle->Index;
    } else {
        Handle = Handles + Heap->FirstFree;
        
        if(Handle->Next != HEAP_NULL_HANDLE)
            Handles[Handle->Next].Prev = HEAP_NULL_HANDLE;
        Heap->FirstFree = Handle->Next;
        if(Heap->LastFree == Handle->Index)
            Heap->LastFree = HEAP_NULL_HANDLE;
        
        PrevUsed = Handle;
        while(PrevUsed->Index) {
            PrevUsed--;
            if(!PrevUsed->Free) {
                Handle->Next = PrevUsed->Next;
                Handle->Prev = PrevUsed->Index;
                if(PrevUsed->Next != HEAP_NULL_HANDLE)
                    Handles[PrevUsed->Next].Prev = Handle->Index;
                else
                    Heap->LastUsed = Handle->Index;
                PrevUsed->Next = Handle->Index;
                goto FoundNextUsed;
            }
        }
        Handle->Prev = HEAP_NULL_HANDLE;
        Handle->Next = Heap->FirstUsed;
        if(Heap->FirstUsed != HEAP_NULL_HANDLE)
            Handles[Heap->FirstUsed].Prev = Handle->Index;
        Heap->FirstUsed = Handle->Index;
        if(Heap->LastUsed < Handle->Index)
            Heap->LastUsed = Handle->Index;
        FoundNextUsed:;
    }
    Handle->Size = Size;
    Handle->Free = FALSE;
    
    u16 PrevBlockIndex = Heap->LastBlock;
    while(PrevBlockIndex != HEAP_NULL_HANDLE && Handles[PrevBlockIndex].Offset < Size) {
        PrevBlockIndex = Handles[PrevBlockIndex].PrevBlock;
    }
    if(PrevBlockIndex == HEAP_NULL_HANDLE) {
        if(!Defragmented && (u08*)(Handles + Heap->HandleCount) > Heap->DataCursor - Size) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            
            Assert((u08*)(Handles + Heap->HandleCount) <= Heap->DataCursor - Size,
                   "Not enough memory for new heap block");
        }
        
        Heap->DataCursor -= Size;
        Handle->Data = Heap->DataCursor;
        Handle->PrevBlock = HEAP_NULL_HANDLE;
        Handle->NextBlock = Heap->FirstBlock;
        if(Heap->FirstBlock != HEAP_NULL_HANDLE)
            Handles[Heap->FirstBlock].PrevBlock = Handle->Index;
        else
            Heap->LastBlock = Handle->Index;
        Heap->FirstBlock = Handle->Index;
    } else {
        heap_handle *PrevBlock = Handles + PrevBlockIndex;
        
        if(PrevBlock->NextBlock != HEAP_NULL_HANDLE)
            Handles[PrevBlock->NextBlock].PrevBlock = Handle->Index;
        else
            Heap->LastBlock = Handle->Index;
        
        Handle->Data = PrevBlock->Data + PrevBlock->Size + PrevBlock->Offset - Size;
        Handle->PrevBlock = PrevBlock->Index;
        Handle->NextBlock = PrevBlock->NextBlock;
        PrevBlock->NextBlock = Handle->Index;
        PrevBlock->Offset -= Size;
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
    Assert(!Handle->Free);
    
    Handles = Handle - Handle->Index;
    Heap = (heap*)Handles-1;
    
    if(Handle->PrevBlock != HEAP_NULL_HANDLE) {
        Handles[Handle->PrevBlock].NextBlock = Handle->NextBlock;
        Handles[Handle->PrevBlock].Offset += Handle->Size + Handle->Offset;
    } else {
        Heap->FirstBlock = Handle->NextBlock;
        Heap->DataCursor += Handle->Size + Handle->Offset;
    }
    
    if(Handle->NextBlock != HEAP_NULL_HANDLE)
        Handles[Handle->NextBlock].PrevBlock = Handle->PrevBlock;
    else
        Heap->LastBlock = Handle->PrevBlock;
    
    if(Handle->Next == HEAP_NULL_HANDLE) {
        if(Handle->Prev == HEAP_NULL_HANDLE) {
            Heap->FirstFree = HEAP_NULL_HANDLE;
            Heap->LastFree = HEAP_NULL_HANDLE;
            Heap->FirstUsed = HEAP_NULL_HANDLE;
            Heap->LastUsed = HEAP_NULL_HANDLE;
            Mem_Set(Heap+1, 0, Heap->HandleCount * sizeof(heap_handle));
            Heap->HandleCount = 0;
        } else {
            Heap->LastUsed = Handle->Prev;
            PrevUsed = Handles + Handle->Prev;
            PrevUsed->Next = HEAP_NULL_HANDLE;
            Mem_Set(PrevUsed+1, 0, (Heap->HandleCount-PrevUsed->Index-1)*sizeof(heap_handle));
            Heap->HandleCount = PrevUsed->Index + 1;
            
            PrevFree = PrevUsed;
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
        }
    } else {
        Handle->Data = NULL;
        Handle->Size = 0;
        Handle->Offset = 0;
        Handle->PrevBlock = HEAP_NULL_HANDLE;
        Handle->NextBlock = HEAP_NULL_HANDLE;
        Handle->Free = TRUE;
        
        if(Handle->Prev != HEAP_NULL_HANDLE)
            Handles[Handle->Prev].Next = Handle->Next;
        else
            Heap->FirstUsed = Handle->Next;
        
        Handles[Handle->Next].Prev = Handle->Prev;
        
        PrevFree = Handle;
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