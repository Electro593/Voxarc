#if 0
internal u32
LoadBMP(memory_handle *Handle, str ImagePath)
{
    u08 Header[0x8A];
    u32 DataPos;
    u32 ImageSize;
    u32 Width;
    u32 Height;
    
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    memory_handle *FileHandle = AllocateHandle(HandlePool, sizeof(vptr));
    OpenFile(FileHandle, ImagePath);
    memory_handle *ImageHeader = AllocateHandle(HandlePool, 0x8A);
    ReadFile(ImageHeader, FileHandle, 0);
    
    s32 BytesPerPixel = 4;
    
    Assert(*(chr*)ImageHeader->Base == 'B' && *((chr*)ImageHeader->Base + 1) == 'M');
    Assert(*((u08*)ImageHeader + 0x1E) == 3 && *((u08*)Header + 0x1C) == 8 * BytesPerPixel);
    DataPos = *(u32*)((u08*)ImageHeader->Base + 0x0A);
    ImageSize = *(u32*)((u08*)ImageHeader->Base + 0x22);
    Width = *(u32*)((u08*)ImageHeader->Base + 0x12);
    Height = *(u32*)((u08*)ImageHeader->Base + 0x16);
    
    if(ImageSize == 0) ImageSize = Width * Height * BytesPerPixel;
    if(DataPos == 0) DataPos = 0x8A;
    
    memory_handle *ImageData = AllocateHandle(HandlePool, ImageSize);
    ReadFile(ImageData, FileHandle, DataPos);
    CloseFile(FileHandle);
    
    u32 TextureID;
    GL_GenTextures(1, &TextureID);
    GL_BindTexture(GL_TEXTURE_2D, TextureID);
    GL_TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, ImageData->Base);
    
    GL_TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    GL_TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    GL_TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL_TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    return TextureID;
}
#endif

// In File_CreateAssetPack
#if 0 // Wasted Area double-counts the overlaps
    u32 Index = 1;
    u32 WastedArea = 0;
    for(asset_packer_node *Node = FreeList;
        Node;
        Node = Node->Next)
    {
        WastedArea += V2u16_RectArea(Node->Size);
        
        v4u08 *AtlasPixel = (v4u08*)((mem)Atlases + sizeof(bitmap_header) + (BytesPerAtlas * Node->AtlasIndex));
        v4u08 Color = LITERAL_CAST(u32, 0xFF000000 + Index*0xFF102038, v4u08);
        Color = SWIZZLE(Color, SWIZZLE_ZYXW);
        
        AtlasPixel += Node->Offset.Y * MAX_ATLAS_DIM;
        AtlasPixel += Node->Offset.X;
        for(u32 Row = Node->Offset.Y;
            Row < (u32)(Node->Offset.Y + Node->Size.Y);
            ++Row)
        {
            Mem_Set(AtlasPixel, FORCE_CAST(Color, u32), Node->Size.X * BYTES_PER_PIXEL);
            
            AtlasPixel += MAX_ATLAS_DIM;
        }
        
        ++Index;
    }
#endif

#if 0
internal void
JSON_CreateFromTokens(json_write_data *WriteData,
                      str Text,
                      json_token *Tokens,
                      u32 TokenCount)
{
    Stack_Push();
    
    str Key = NULL;
    
    json_token *Token = Tokens;
    s32 ParentToken = WriteData->ParentToken;
    for(u32 Index = 0;
        Index < TokenCount;
        ++Index, ++Token)
    {
        if(Token->ParentToken != ParentToken)
        {
            ASSERT(Index > 0);
            json_token *Prev = Tokens + Index - 1;
            Prev = Tokens + Prev->ParentToken;
            if(Prev->Type == JSONToken_String)
            {
                Prev = Tokens + Prev->ParentToken;
            }
            
            switch(Prev->Type)
            {
                case JSONToken_Array:
                {
                    JSON_CreateArrayEnd(WriteData);
                } break;
                
                case JSONToken_Object:
                {
                    JSON_CreateObjectEnd(WriteData);
                } break;
            }
        }
        
        switch(Token->Type)
        {
            case JSONToken_Primitive:
            {
                str Value = Stack_Allocate(Token->EndOffset - Token->StartOffset + 1);
                Str_Sub(Value, Text, Token->StartOffset, Token->EndOffset);
                
                JSON_CreateNum(WriteData, TYPE_STR, Key, (num){.NumStr=Value});
                Key = NULL;
            } break;
            
            case JSONToken_String:
            {
                
                if(Key)
                {
                    str Value = Stack_Allocate(Token->EndOffset - Token->StartOffset + 1);
                    Str_Sub(Value, Text, Token->StartOffset, Token->EndOffset);
                    
                    JSON_CreateStr(WriteData, Key, Value);
                    Key = NULL;
                }
                else
                {
                    Key = Stack_Allocate(Token->EndOffset - Token->StartOffset + 1);
                    Str_Sub(Key, Text, Token->StartOffset, Token->EndOffset);
                }
            } break;
            
            case JSONToken_Array:
            {
                ParentToken = WriteData->ParentToken;
                JSON_CreateArrayStart(WriteData, Key);
                Key = NULL;
            } break;
            
            case JSONToken_Object:
            {
                ParentToken = WriteData->ParentToken;
                JSON_CreateObjectStart(WriteData, Key);
                Key = NULL;
            } break;
        }
        
        Platform_Print_DEBUG(WriteData->Buffer);
        Platform_Print_DEBUG("\n\n\n\n");
    }
    
    Stack_Pop();
}
#endif

#if 0

#define HANDLE_COUNT(Handle, Type) (Handle->Size / sizeof(Type))
#define HANDLE_MEMBER(Handle, Type, Index) ((Type*)Handle->Base + Index)

//TODO: Typedef this as a pointer
typedef struct memory_handle
{
    vptr Base;
    u64 Size;
    u32 HandleIndex;
} memory_handle;

typedef struct data_pool
{
    vptr Base;
    u64 Used;
    u64 Size;
} data_pool;

typedef struct handle_pool
{
    memory_handle *Handles;
    data_pool *DataPool;
    u32 HandleCount;
} handle_pool;

internal handle_pool *
GetHandlePool(memory_handle *Handle)
{
    return (handle_pool*)((u08*)(Handle - Handle->HandleIndex) - sizeof(handle_pool));
}

internal void
CopyHandles(memory_handle *Dest,
            memory_handle *Src)
{
    ASSERT(Dest->Size >= Src->Size);
    ASSERT(Dest->Base && Src->Base);
    Mem_Cpy(Dest->Base, Src->Base, Src->Size);
}

internal handle_pool *
CreateHandlePool(u64 Size,
                 u32 HandleCount)
{
    u64 HandlePoolSize = HandleCount * sizeof(memory_handle) + sizeof(handle_pool);
    u64 DataPoolSize = Size + sizeof(data_pool);
#if defined(_VOX_WINDOWS)
    vptr Base = Platform_AllocateMemory(HandlePoolSize + DataPoolSize);
#else
    vptr Base = malloc(HandlePoolSize + DataPoolSize);
#endif
    handle_pool *HandlePool = (handle_pool*)Base;
    HandlePool->Handles = (memory_handle*)(HandlePool + 1);
    HandlePool->HandleCount = HandleCount;
    
    data_pool *DataPool = (data_pool*)((u08*)Base + HandlePoolSize);
    DataPool->Base = DataPool + 1;
    DataPool->Used = 0;
    DataPool->Size = Size;
    
    HandlePool->DataPool = DataPool;
    
    memory_handle *Handle = HandlePool->Handles;
    for(u32 HandleIndex = 0;
        HandleIndex < HandleCount;
        ++HandleIndex)
    {
        Handle->Base = 0;
        Handle->Size = 0;
        Handle->HandleIndex = HandleIndex;
        Handle++;
    }
    
    return HandlePool;
}

internal void
FreeHandlePool(handle_pool *HandlePool)
{
    Platform_FreeMemory(HandlePool);
    HandlePool = NULL;
}

internal void
DefragmentMemoryPools(handle_pool *HandlePool)
{
    memory_handle *StartHandle = HandlePool->Handles;
    
    data_pool *DataPool = HandlePool->DataPool;
    
    u08 *Dest = (u08*)DataPool->Base;
    for(u32 MainLoopIndex = 0;
        MainLoopIndex < HandlePool->HandleCount;
        ++MainLoopIndex)
    {
        memory_handle *MinHandle = 0;
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
            Mem_Cpy(Dest, MinHandle->Base, MinHandle->Size);
            MinHandle->Base = Dest;
            Dest += MinHandle->Size;
        }
    }
    
    DataPool->Used = Dest - (u08*)DataPool->Base;
}

internal vptr
FindFreeMemory(handle_pool *HandlePool,
               u64 Size)
{
    data_pool *DataPool = HandlePool->DataPool;
    vptr Base = 0;
    if(DataPool->Size - DataPool->Used >= Size)
    {
        Base = (u08*)DataPool->Base + DataPool->Used;
        DataPool->Used += Size;
    }
    return Base;
}

internal memory_handle *
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

internal void
FreeHandle(memory_handle *Handle)
{
    Handle->Size = 0;
    Handle->Base = 0;
}

internal memory_handle *
AllocateHandle(handle_pool *HandlePool,
               u64 Size)
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
        
        Mem_Set(UnusedHandle->Base, 0, UnusedHandle->Size);
        return UnusedHandle;
    }
    
    Platform_ThrowError_DEBUG("Coudn't allocate memory\n");
    return 0;
}

internal memory_handle *
ResizeHandle(memory_handle *Handle,
             u64 Size)
{
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    s32 DeltaSize = (s32)Handle->Size - (s32)Size;
    if(DeltaSize > 0) // Shrink
    {
        Handle->Size = Size;
        Mem_Set((u08*)Handle->Base + Handle->Size, 0, DeltaSize);
        return Handle;
    }
    else if(DeltaSize < 0) // Grow
    {
        memory_handle *NewHandle = AllocateHandle(HandlePool, Size);
        Mem_Cpy(NewHandle->Base, Handle->Base, Handle->Size);
        FreeHandle(Handle);
        return NewHandle;
    }
    return Handle;
}
#endif
