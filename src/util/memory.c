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
    Heap->DataCursor = (u08*)Memory + Size
    
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
    
    if(Heap->FreeList = HEAP_NULL_HANDLE) {
        Heap->HandleCount++;
        
        if(Handles[Heap->HandleCount] > Heap->DataCursor) {
            Heap_Defragment(Heap);
            
            Assert(Handles[Heap->HandleCount] <= Heap->DataCursor,
                   "Out of memory!");
        }
        
        Handle = Handles + Heap->HandleCount - 1;
        Assert(Handle->Free);
        
        Handle->Free = FALSE;
        Handle->Size = Size;
        Heap->FirstFree = Handle->Next;
        Handle->Next = Heap->UsedList;
        Heap->UsedList = Handle.Index;
    }
    
    if(Heap->BlockList = HEAP_NULL_HANDLE) {
        
    }
}

internal void
Heap_Free(vptr Data)
{
    
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