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
    if(Size == 0)   return Dest;
    if(Dest == Src) return Dest;
    
    s08 Dir = 1;
    u08 *Dest08 = (u08*)Dest;
    u08 *Src08 = (u08*)Src;
    
    if(Dest > Src) {
        Dir = -1;
        Dest08 += Size - 1;
        Src08 += Size - 1;
    }
    
    while(Size--) {
        *Dest08 = *Src08;
        Dest08 += Dir;
        Src08 += Dir;
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





#define GETSIZE(I) ((Handles[I].SizeU << 32) | (Handles[I].SizeL))
#define SETSIZE(I, S) \
    do { \
        Handles[I].SizeU = ((u64)S)>>32; \
        Handles[I].SizeL = (u32)(S); \
    } while(0)

internal void
Heap_Init(heap *Heap,
          u64 Size)
{
    heap_handle *NullUsedHandle = (heap_handle*)(Heap+1);
    NullUsedHandle->Data = (u08*)(Heap+1);
    NullUsedHandle->SizeU = sizeof(heap_handle) >> 32;
    NullUsedHandle->SizeL = (u32)sizeof(heap_handle);
    NullUsedHandle->Offset = Size - sizeof(heap_handle);
    NullUsedHandle->Index = 0;
    NullUsedHandle->PrevFree = 0;
    NullUsedHandle->NextFree = 0;
    NullUsedHandle->PrevUsed = 0;
    NullUsedHandle->NextUsed = 0;
    NullUsedHandle->PrevBlock = 0;
    NullUsedHandle->NextBlock = 0;
    NullUsedHandle->Anchored = TRUE;
    NullUsedHandle->Free = FALSE;
}

internal void
Heap_Defragment(heap *Heap)
{
    STOP; // Hasn't been debugged
    
    Assert(Heap);
    heap_handle *Handles = (heap_handle*)Heap;
    
    u64 Offset = 0;
    heap_handle *Block = Handles;
    do {
        Block = Handles + Block->PrevBlock;
        
        if(Block->Anchored) {
            Block->Offset += Offset;
            Offset = 0;
            continue;
        }
        
        Offset += Block->Offset;
        Block->Offset = 0;
        
        if(Offset) {
            Mem_Cpy(Block->Data+Offset, Block->Data, GETSIZE(Block->Index));
            Block->Data += Offset;
        }
    } while(Block->Index);
}

internal heap_handle *
_Heap_Allocate(heap *Heap,
               u64 Size,
               b08 Anchored)
{
    Assert(Heap);
    heap_handle *Handles = (heap_handle*)Heap;
    heap_handle *Handle = NULL;
    b08 Defragmented = FALSE;
    
    if(Anchored) Size += sizeof(heap_handle*);
    
    heap_handle *PrevUsed;
    if(Handles[0].NextFree == 0) {
        if(Handles[0].Offset < sizeof(heap_handle)) {
            Heap_Defragment(Heap);
            Defragmented = TRUE;
            Assert(Handles[0].Offset >= sizeof(heap_handle), "Not enough memory for new heap handle");
        }
        
        u16 HandleCount = (u16)(GETSIZE(0) / sizeof(heap_handle));
        Handle = Handles + HandleCount;
        Handle->Index = HandleCount;
        
        SETSIZE(0, GETSIZE(0) + sizeof(heap_handle));
        Handles[0].Offset -= sizeof(heap_handle);
        
        PrevUsed = Handles + Handles[0].PrevUsed;
    } else {
        Handle = Handles + Handles[0].NextFree;
        Handles[Handle->PrevFree].NextFree = Handle->NextFree;
        Handles[Handle->NextFree].PrevFree = Handle->PrevFree;
        
        PrevUsed = Handle;
        while(PrevUsed->Index && PrevUsed->Free) PrevUsed--;
    }
    Handle->PrevUsed = PrevUsed->Index;
    Handle->NextUsed = PrevUsed->NextUsed;
    Handle->PrevFree = 0;
    Handle->NextFree = 0;
    Handles[Handle->PrevUsed].NextUsed = Handle->Index;
    Handles[Handle->NextUsed].PrevUsed = Handle->Index;
    SETSIZE(Handle->Index, Size);
    Handle->Anchored = Anchored;
    Handle->Free = FALSE;
    
    heap_handle *PrevBlock = Handles + Handles[0].PrevBlock;
    while(PrevBlock->Index && PrevBlock->Offset < Size) {
        PrevBlock = Handles+PrevBlock->PrevBlock;
    }
    if(Handles[0].Offset < Size && !Defragmented) {
        Heap_Defragment(Heap);
        Assert(Handles[0].Offset >= Size, "Not enough memory for new heap block");
    }
    Handle->Data = PrevBlock->Data + GETSIZE(PrevBlock->Index) + PrevBlock->Offset - Size;
    PrevBlock->Offset -= Size;
    Handle->PrevBlock = PrevBlock->Index;
    Handle->NextBlock = PrevBlock->NextBlock;
    Handles[Handle->NextBlock].PrevBlock = Handle->Index;
    Handles[Handle->PrevBlock].NextBlock = Handle->Index;
    Handle->Offset = 0;
    
    if(Anchored) *(vptr*)Handle->Data = Handle;
    
    return Handle;
}

internal heap_handle *Heap_Allocate (heap *Heap, u64 Size) { return _Heap_Allocate(Heap, Size, FALSE); }
internal vptr         Heap_AllocateA(heap *Heap, u64 Size) { return _Heap_Allocate(Heap, Size, TRUE)->Data + sizeof(heap_handle*); }

internal void
Heap_Free(heap_handle *Handle)
{
    Assert(Handle);
    heap_handle *Handles = Handle - Handle->Index;
    
    Handles[Handle->PrevBlock].Offset += GETSIZE(Handle->Index) + Handle->Offset;
    Handles[Handle->PrevBlock].NextBlock = Handle->NextBlock;
    Handles[Handle->NextBlock].PrevBlock = Handle->PrevBlock;
    Handles[Handle->NextUsed].PrevUsed = Handle->PrevUsed;
    Handles[Handle->PrevUsed].NextUsed = Handle->NextUsed;
    
    if(Handle->NextUsed == 0) {
        heap_handle *PrevFree = Handles + Handle->PrevUsed;
        while(PrevFree->Index && !PrevFree->Free) PrevFree--;
        Handles[0].PrevFree = PrevFree->Index;
        PrevFree->NextFree = 0;
        
        u16 Offset = Handle->PrevUsed + 1;
        u64 NewSize = Offset*sizeof(heap_handle);
        u64 DeltaSize = GETSIZE(0) - NewSize;
        SETSIZE(0, NewSize);
        Handles[0].Offset += DeltaSize;
        Mem_Set(Handles+Offset, 0, DeltaSize);
    } else {
        heap_handle *PrevFree = Handle;
        while(PrevFree->Index && !PrevFree->Free) PrevFree--;
        
        Handle->Data = NULL;
        Handle->Offset = 0;
        Handle->Free = TRUE;
        Handle->Anchored = FALSE;
        SETSIZE(Handle->Index, 0);
        Handle->PrevFree = PrevFree->Index;
        Handle->NextFree = PrevFree->NextFree;
        Handle->PrevUsed = 0;
        Handle->NextUsed = 0;
        Handle->PrevBlock = 0;
        Handle->NextBlock = 0;
        Handles[Handle->NextFree].PrevFree = Handle->Index;
        Handles[Handle->PrevFree].NextFree = Handle->Index;
    }
}

#undef GETSIZE
#undef SETSIZE





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
    vptr *Marker = (vptr*)__Global.Stack->Cursor;
    *Marker = __Global.Stack->FirstMarker;
    __Global.Stack->FirstMarker = Marker;
    __Global.Stack->Cursor += sizeof(vptr);
}

internal vptr
Stack_Allocate(u64 Size)
{
    vptr Result = __Global.Stack->Cursor;
    __Global.Stack->Cursor += Size;
    return Result;
}

internal void
Stack_Pop(void)
{
    __Global.Stack->FirstMarker = *__Global.Stack->FirstMarker;
    __Global.Stack->Cursor = (u08*)__Global.Stack->FirstMarker + sizeof(vptr);
}