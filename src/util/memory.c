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

internal s32
Mem_Cmp(vptr A,
        vptr B,
        u32 Size)
{
    while(Size && *(((u08*)A)++) == *(((u08*)B)++)) Size--;
    if(!Size) return EQUAL;
    if((u64)A > (u64)B) return GREATER;
    return LESS;
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
Heap_GetHeap(heap_handle *Handle)
{
    return (heap*)(Handle-Handle->Index);
}

internal heap_handle *
Heap_GetHandle(vptr Data)
{
    return *((heap_handle**)Data-1);
}

internal heap *
Heap_Init(vptr MemBase,
          u64 Size)
{
    Assert(MemBase);
    Assert(Size > sizeof(heap_handle));
    Assert(Size - sizeof(heap_handle) < (1ULL<<46));
    
    heap_handle *NullUsedHandle = (heap_handle*)MemBase;
    NullUsedHandle->Data = (u08*)MemBase;
    NullUsedHandle->Size = sizeof(heap_handle);
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
    
    return (heap*)MemBase;
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
            Mem_Cpy((u08*)Block->Data+Offset, Block->Data, Block->Size);
            (u08*)Block->Data += Offset;
        }
    } while(Block->Index);
}

internal void
Heap_AllocateBlock(heap *Heap,
                   heap_handle *Handle,
                   u32 Size)
{
    heap_handle *Handles = (vptr)Heap;
    heap_handle *PrevBlock = Handles + Handles[0].PrevBlock;
    while(PrevBlock->Index && PrevBlock->Offset < Size)
        PrevBlock = Handles+PrevBlock->PrevBlock;
    if(PrevBlock->Offset < Size) {
        Heap_Defragment(Heap);
        Assert(PrevBlock->Offset >= Size, "Not enough memory for new heap block");
    }
    Handle->Data = (u08*)PrevBlock->Data + PrevBlock->Size + PrevBlock->Offset - Size;
    PrevBlock->Offset -= Size;
    Handle->PrevBlock = PrevBlock->Index;
    Handle->NextBlock = PrevBlock->NextBlock;
    Handles[Handle->NextBlock].PrevBlock = Handle->Index;
    Handles[Handle->PrevBlock].NextBlock = Handle->Index;
    Handle->Offset = 0;
    Handle->Size = Size;
}

internal void
Heap_FreeBlock(heap *Heap,
               heap_handle *Handle)
{
    heap_handle *Handles = (vptr)Heap;
    Handles[Handle->PrevBlock].Offset += Handle->Size + Handle->Offset;
    Handles[Handle->PrevBlock].NextBlock = Handle->NextBlock;
    Handles[Handle->NextBlock].PrevBlock = Handle->PrevBlock;
}

internal heap_handle *
_Heap_Allocate(heap *Heap,
               u32 Size,
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
        
        u16 HandleCount = (u16)(Handles[0].Size / sizeof(heap_handle));
        Handle = Handles + HandleCount;
        Handle->Index = HandleCount;
        
        Handles[0].Size += sizeof(heap_handle);
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
    Handle->Anchored = Anchored;
    Handle->Free = FALSE;
    
    Heap_AllocateBlock(Heap, Handle, Size);
    
    if(Anchored) *(heap_handle**)Handle->Data = Handle;
    
    return Handle;
}

internal heap_handle *
Heap_Allocate(heap *Heap, u64 Size)
{
    return _Heap_Allocate(Heap, Size, FALSE);
}

internal vptr
Heap_AllocateA(heap *Heap, u64 Size)
{
    return (u08*)_Heap_Allocate(Heap, Size, TRUE)->Data + sizeof(heap_handle*);
}

internal void
Heap_Resize(heap_handle *Handle,
            u32 NewSize)
{
    Assert(Handle);
    
    if(NewSize <= Handle->Size + Handle->Offset) {
        Handle->Offset += (s64)Handle->Size - (s64)NewSize;
        Handle->Size = NewSize;
    } else {
        u08 *PrevData = Handle->Data;
        u32 PrevSize = Handle->Size;
        heap *Heap = (heap*)(Handle - Handle->Index);
        Heap_FreeBlock(Heap, Handle);
        Heap_AllocateBlock(Heap, Handle, NewSize);
        Mem_Cpy(Handle->Data, PrevData, PrevSize);
    }
}

internal void
Heap_ResizeA(vptr *Data,
             u32 NewSize)
{
    heap_handle *Handle = Heap_GetHandle(*Data);
    Heap_Resize(Handle, NewSize);
    *Data = Handle->Data;
}

internal void
Heap_Free(heap_handle *Handle)
{
    Assert(Handle);
    heap_handle *Handles = Handle - Handle->Index;
    heap *Heap = (vptr)Handles;
    
    Heap_FreeBlock(Heap, Handle);
    
    Handles[Handle->NextUsed].PrevUsed = Handle->PrevUsed;
    Handles[Handle->PrevUsed].NextUsed = Handle->NextUsed;
    
    if(Handle->NextUsed == 0) {
        heap_handle *PrevFree = Handles + Handle->PrevUsed;
        while(PrevFree->Index && !PrevFree->Free) PrevFree--;
        Handles[0].PrevFree = PrevFree->Index;
        PrevFree->NextFree = 0;
        
        u16 Offset = Handle->PrevUsed + 1;
        u64 NewSize = Offset*sizeof(heap_handle);
        u64 DeltaSize = Handles[0].Size - NewSize;
        Handles[0].Size = NewSize;
        Handles[0].Offset += DeltaSize;
        Mem_Set(Handles+Offset, 0, DeltaSize);
    } else {
        heap_handle *PrevFree = Handle;
        while(PrevFree->Index && !PrevFree->Free) PrevFree--;
        
        Handle->Data = NULL;
        Handle->Offset = 0;
        Handle->Free = TRUE;
        Handle->Anchored = FALSE;
        Handle->Size = 0;
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

internal void
Heap_FreeA(vptr Data)
{
    Heap_Free(Heap_GetHandle(Data));
}

internal void
Heap_Dump(heap *Heap)
{
    file_handle FileHandle;
    Platform_OpenFile(&FileHandle, "heap_dump.txt", FILE_WRITE);
    u32 FileOffset = 0;
    
    heap_handle *Handles = (heap_handle*)Heap;
    u32 HandleCount = Handles[0].Size / sizeof(heap_handle);
    
    Stack_Push();
    {
        string NoneStr = CString("");
        
        string Line1 = NoneStr;
        string Line2 = NoneStr;
        string Line3 = NoneStr;
        string Line4 = NoneStr;
        
        heap_handle *Handle = Handles;
        do {
            if(Handle->Size == 0) goto next;
            
            u32 Index;
            Intrin_BitScanReverse(&Index, Handle->Size);
            u32 SizeWidth = (Index < 3) ? 1 : Index-1;
            
            b08 IsValid = Intrin_BitScanReverse64(&Index, Handle->Offset);
            u32 OffsetWidth = IsValid * ((Index < 3) ? 1 : Index-1);
            
            string Size   = CFString("%'d", Handle->Size);
            string Offset = CFString("%'Ld", Handle->Offset);
            
            c08 BorderChar = (Handle->Anchored) ? '=' : '-';
            
            string PlusStr = IsValid ? CString("+") : NoneStr;
            string PipeStr = IsValid ? CString("|") : NoneStr;
            string Seg1 = CFString(".%*s%s%2$-*s.", SizeWidth, NoneStr, PlusStr, OffsetWidth);
            string Seg2 = CFString(":%*s%s%2$-*s:", SizeWidth, NoneStr, PipeStr, OffsetWidth);
            string Seg3 = CFString(":%*s%-*s%s%*.*s%-*s:", (SizeWidth+Size.Length)/2, Size, (SizeWidth-Size.Length+1)/2, NoneStr, PipeStr, (OffsetWidth+Offset.Length)/2, OffsetWidth, Offset, (OffsetWidth-Offset.Length+1)/2, NoneStr);
            string Seg4 = CFString("'%*s%s%2$-*s'", SizeWidth, NoneStr, PlusStr, OffsetWidth);
            
            Mem_Set(Seg1.Text+1, BorderChar, SizeWidth);
            Mem_Set(Seg1.Text+1+SizeWidth+1, BorderChar, OffsetWidth);
            Mem_Set(Seg2.Text+1, '#', SizeWidth);
            Mem_Set(Seg4.Text+1, BorderChar, SizeWidth);
            Mem_Set(Seg4.Text+1+SizeWidth+1, BorderChar, OffsetWidth);
            
            Line1 = CFString("%s%s", Line1, Seg1);
            Line2 = CFString("%s%s", Line2, Seg2);
            Line3 = CFString("%s%s", Line3, Seg3);
            Line4 = CFString("%s%s", Line4, Seg4);
            
            next:
            Handle = Handles + Handle->NextBlock;
        } while(Handle->Index);
        
        string String = CFString("\n%s\n%s\n%s\n%s\n%s\n", Line1, Line2, Line3, Line2, Line4);
        Platform_WriteFile(FileHandle, String.Text, String.Length, 0);
        FileOffset += String.Length;
    }
    Stack_Pop();
    
    Stack_Push();
    {
        string NoneStr = CString("");
        
        string DataStr = CString("Data:");
        string FlagsStr = CString("Status:");
        string HandlesStr = CString("Handles:");
        string SizeStr = CString("Size:");
        string OffsetStr = CString("Offset:");
        u32 LabelWidthU = MAX(DataStr.Length, FlagsStr.Length);
        LabelWidthU     = MAX(HandlesStr.Length, LabelWidthU);
        LabelWidthU     = MAX(SizeStr.Length, LabelWidthU);
        LabelWidthU     = MAX(OffsetStr.Length, LabelWidthU);
        s32 LabelWidth = -(s32)LabelWidthU;
        
        string FreeStr = CString("Free");
        string UsedStr = CString("Used");
        string IsAnchoredStr = CString(", Anchored");
        
        s32 Width = 0;
        s32 CurrWidth;
        
        string String = NoneStr;
        
        for(u32 I = 0; I < HandleCount; I++) {
            string Name, Data, Size, Offset, Flags;
            
            if(I == 0) Name = CFString("HEAP (Handle 0)%n", &CurrWidth);
            else       Name = CFString("Handle %'d%n", Handles[I].Index, &CurrWidth);
            if(Width < CurrWidth) Width = CurrWidth;
            
            Data = CFString("%*s %p%n", LabelWidth, DataStr, Handles[I].Data, &CurrWidth);
            if(Width < CurrWidth) Width = CurrWidth;
            
            string UsageStr = (Handles[I].Free) ? FreeStr : UsedStr;
            string AnchoredStr = (Handles[I].Anchored) ? IsAnchoredStr : NoneStr;
            Flags = CFString("%*s %s%s%n", LabelWidth, FlagsStr, UsageStr, AnchoredStr, &CurrWidth);
            if(Width < CurrWidth) Width = CurrWidth;
            
            if(I == 0) Size = CFString("%*s %'u (%'u bytes)%n", LabelWidth, HandlesStr, HandleCount, Handles[I].Size, &CurrWidth);
            else       Size = CFString("%*s %'u bytes%n", LabelWidth, SizeStr, Handles[I].Size, &CurrWidth);
            if(Width < CurrWidth) Width = CurrWidth;
            
            Offset = CFString("%*s %'u bytes%n", LabelWidth, OffsetStr, Handles[I].Offset, &CurrWidth);
            if(Width < CurrWidth) Width = CurrWidth;
            
            string NextHandleStr = CFString("---Next %s---> %'d", UsageStr, (Handles[I].Free) ? Handles[I].NextFree : Handles[I].NextUsed);
            string PrevHandleStr = CFString("---Prev %s---> %'d", UsageStr, (Handles[I].Free) ? Handles[I].PrevFree : Handles[I].PrevUsed);
            string NextBlockStr  = CFString("---Next Block--> %'d", Handles[I].NextBlock);
            string PrevBlockStr  = CFString("---Prev Block--> %'d", Handles[I].PrevBlock);
            
            string BorderTop    = CFString(".-%*s-.", Width, NoneStr);
            string BorderBottom = CFString("'-%*s-'", Width, NoneStr);
            Mem_Set(BorderTop.Text   +2, '-', Width);
            Mem_Set(BorderBottom.Text+2, '-', Width);
            
            String = CFString("%s\n%s\n| %*4$s%*s | %12$s\n| %8$*3$s | %13$s\n| %9$*3$s | %14$s\n| %10$*3$s | %15$s\n| %11$*3$s |\n%16$s\n",
                String, BorderTop, -Width,
                (Width+Name.Length)/2, Name, (Width-Name.Length+1)/2, NoneStr,
                Data, Size, Offset, Flags,
                NextHandleStr, PrevHandleStr, NextBlockStr, PrevBlockStr,
                BorderBottom);
        }
        
        Platform_WriteFile(FileHandle, String.Text, String.Length, FileOffset);
        Platform_CloseFile(FileHandle);
    }
    Stack_Pop();
}




internal stack *
Stack_Init(vptr Mem,
           u64 Size)
{
    stack *Result = Mem;
    Result->Size = Size - sizeof(stack);
    Result->FirstMarker = NULL;
    Result->Cursor = (u08*)Mem + sizeof(stack);
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

// For unbounded allocations
internal vptr Stack_GetCursor(void) { return __Global.Stack->Cursor; }
internal void
Stack_SetCursor(vptr Cursor)
{
    Assert((u08*)Cursor <= (u08*)(__Global.Stack+1)+__Global.Stack->Size);
    __Global.Stack->Cursor = Cursor;
}

internal vptr
Stack_Allocate(u64 Size)
{
    vptr Result = __Global.Stack->Cursor;
    __Global.Stack->Cursor += Size;
    Assert(__Global.Stack->Cursor <= (u08*)(__Global.Stack+1)+__Global.Stack->Size);
    return Result;
}

internal void
Stack_Pop(void)
{
    __Global.Stack->Cursor = (u08*)__Global.Stack->FirstMarker;
    __Global.Stack->FirstMarker = *__Global.Stack->FirstMarker;
}