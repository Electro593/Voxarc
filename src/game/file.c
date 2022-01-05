/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//NOTE: Set ReadLength to 0 to read to the end of the file
internal str
File_Read(str FilePath,
          u64 ReadLength,
          u64 ReadOffset)
{
    mem FileHandle;
    Platform_OpenFile(&FileHandle, FilePath, FileMode_Read);
    if(!FileHandle)
    {
        return NULL;
    }
    
    if(!ReadLength)
    {
        ReadLength = Platform_GetFileLength(&FileHandle);
        ASSERT(ReadLength < U32_MAX);
        
        if(ReadOffset)
        {
            ASSERT(ReadLength >= ReadOffset);
            ReadLength -= ReadOffset;
        }
    }
    
    str Text;
    Platform_ReadFile(Str_Create(&Text, NULL, (u32)ReadLength), &FileHandle, ReadLength, ReadOffset);
    Platform_CloseFile(&FileHandle);
    
    return Text;
}

internal void
File_Write(str FilePath,
           str Text,
           u64 WriteOffset)
{
    mem FileHandle;
    Platform_OpenFile(&FileHandle, FilePath, FileMode_Write);
    Platform_WriteFile(Text, &FileHandle, Str_Len(Text), WriteOffset);
    Platform_CloseFile(&FileHandle);
}

internal void
File_ReadConfig(game_config *Config,
                str ConfigFilePath)
{
    str Name = Str_Create(NULL, NULL, 128);
    str Value = Str_Create(NULL, NULL, 128);
    str Data = File_Read(ConfigFilePath, 0, 0);
    
    u08 *C = (u08*)Data;
    while(TRUE)
    {
        u08 *NI = (u08*)Name;
        u08 *VI = (u08*)Value;
        
        while(Chr_IsWhitespace(*C)) C++;
        while(*C == '#')
        {
            while(*C != '\n') C++;
            C++;
        }
        
        if(*C == '\0') break;
        while(Chr_IsWhitespace(*C)) C++;
        while(Chr_IsPrintableASCII(*C) && *C != '=') *NI++ = *C++;
        *NI++ = '\0';
        while(Chr_IsWhitespace(*C)) C++;
        ASSERT(*C == '=');
        C++;
        
        while(Chr_IsWhitespace(*C)) C++;
        while(Chr_IsPrintableASCII(*C)) *VI++ = *C++;
        *VI++ = '\0';
        
        ASSERT(Name[0] != '\0' && Value[0] != '\0');
        
        #define CASE(Label) (Mem_Cmp(Name, #Label, sizeof(#Label)-1) == EQUAL)
        
        if(CASE(VisionBitsPerChannel))
            Config->VisionBitsPerChannel = Str_To_U32(Value);
        else if(CASE(VisionChannelBinary))
            Config->VisionChannelBinary = Str_To_U32(Value);
        else if(CASE(FOV))
            Config->FOV = Str_To_U32(Value);
        else if(CASE(Sensitivity))
            Config->Sensitivity = Str_To_R32(Value);
        else if(CASE(RenderDistance))
            Config->RenderDistance = V3u32_1x1(Str_To_U32(Value));
        else if(CASE(MoveLeft))
            Config->MoveLeft = Str_To_U32(Value);
        else if(CASE(MoveForward))
            Config->MoveForward = Str_To_U32(Value);
        else if(CASE(MoveRight))
            Config->MoveRight = Str_To_U32(Value);
        else if(CASE(MoveBack))
            Config->MoveBack = Str_To_U32(Value);
        else if(CASE(MoveUp))
            Config->MoveUp = Str_To_U32(Value);
        else if(CASE(MoveDown))
            Config->MoveDown = Str_To_U32(Value);
        
        #undef CASE
    }
    
    Str_Free(Name);
    Str_Free(Value);
    Str_Free(Data);
}

internal u32
File_ReadShaders(chr *VertexShaderFilePath,
                 chr *FragmentShaderFilePath)
{
    s32 Result = FALSE;
    s32 InfoLogLength;
    
    u32 VertexShaderID = GL_CreateShader(GL_VERTEX_SHADER);
    str VertexShaderCode = File_Read(VertexShaderFilePath, 0, 0);
    GL_ShaderSource(VertexShaderID, 1, &VertexShaderCode, 0);
    GL_CompileShader(VertexShaderID);
    
    GL_GetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    GL_GetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    str VertexShaderErrorMessage = Str_Create(NULL, NULL, InfoLogLength);
    GL_GetShaderInfoLog(VertexShaderID, InfoLogLength, 0, VertexShaderErrorMessage);
    if(!Result)
    {
        Platform_ThrowError_DEBUG(VertexShaderErrorMessage);
        //TODO: Logging
    }
    Str_Free(VertexShaderErrorMessage);
    Str_Free(VertexShaderCode);
    
    u32 FragmentShaderID = GL_CreateShader(GL_FRAGMENT_SHADER);
    str FragmentShaderCode = File_Read(FragmentShaderFilePath, 0, 0);
    GL_ShaderSource(FragmentShaderID, 1, &FragmentShaderCode, 0);
    GL_CompileShader(FragmentShaderID);
    
    GL_GetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    GL_GetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    str FragmentShaderErrorMessage = Str_Create(NULL, NULL, InfoLogLength);
    GL_GetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, FragmentShaderErrorMessage);
    if(!Result)
    {
        Platform_ThrowError_DEBUG(FragmentShaderErrorMessage);
        //TODO: Logging
    }
    Str_Free(FragmentShaderErrorMessage);
    Str_Free(FragmentShaderCode);
    
    u32 ProgramID = GL_CreateProgram();
    GL_AttachShader(ProgramID, VertexShaderID);
    GL_AttachShader(ProgramID, FragmentShaderID);
    GL_LinkProgram(ProgramID);
    GL_GetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    GL_GetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    str ProgramErrorMessage = Str_Create(NULL, NULL, InfoLogLength);
    GL_GetProgramInfoLog(ProgramID, InfoLogLength, 0, ProgramErrorMessage);
    if(!Result)
    {
        Platform_ThrowError_DEBUG(ProgramErrorMessage);
        //TODO: Logging
    }
    Str_Free(ProgramErrorMessage);
    
    GL_DeleteShader(VertexShaderID);
    GL_DeleteShader(FragmentShaderID);
    
    return ProgramID;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Assets
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal asset_pack *
File_ReadAssetPack(heap *Heap)
{
    str TempStr;
    vptr FileHandle;
    
    Platform_OpenFile(&FileHandle, Str_Create(&TempStr, ASSET_PACK_FILENAME, 0), FileMode_Read);
    Str_Free(TempStr);
    
    u32 HeaderSize = sizeof(asset_pack);
    asset_pack *AssetPack = Heap_Allocate(Heap, HeaderSize);
    Platform_ReadFile(AssetPack, &FileHandle, HeaderSize, 0);
    ASSERT(AssetPack->MagicNumber == ASSET_PACK_MAGIC_NUMBER);
    ASSERT(AssetPack->Version == ASSET_PACK_VERSION);
    
    u32 GroupsSize = AssetPack->GroupCount * sizeof(asset_group);
    u32 AssetsSize = AssetPack->AssetCount * sizeof(asset_info);
    u32 TagsSize = AssetPack->TagCount * sizeof(asset_tag);
    u32 ExtraSize = AssetPack->ExtraSize;
    u32 AtlasesSize = AssetPack->AtlasCount * V2u16_RectArea(AssetPack->AtlasSize) * BYTES_PER_PIXEL;
    Heap_Resize(&AssetPack, HeaderSize + GroupsSize + AssetsSize + TagsSize + ExtraSize + AtlasesSize);
    
    u64 GroupsOffset = AssetPack->GroupsOffset;
    u64 AssetsOffset = AssetPack->AssetsOffset;
    u64 TagsOffset = AssetPack->TagsOffset;
    u64 ExtraOffset = AssetPack->ExtraOffset;
    u64 AtlasesOffset = AssetPack->AtlasesOffset;
    
    AssetPack->Groups = (asset_group*)((mem)AssetPack + AssetPack->GroupsOffset);
    AssetPack->Assets = (asset_info*)((mem)AssetPack + AssetPack->AssetsOffset);
    AssetPack->Tags = (asset_tag*)((mem)AssetPack + AssetPack->TagsOffset);
    AssetPack->Extra = (mem)AssetPack + AssetPack->ExtraOffset;
    AssetPack->Atlases = (mem)AssetPack + AssetPack->AtlasesOffset;
    
    Platform_ReadFile(AssetPack->Groups, &FileHandle, GroupsSize, GroupsOffset);
    Platform_ReadFile(AssetPack->Assets, &FileHandle, AssetsSize, AssetsOffset);
    Platform_ReadFile(AssetPack->Tags, &FileHandle, TagsSize, TagsOffset);
    Platform_ReadFile(AssetPack->Extra, &FileHandle, ExtraSize, ExtraOffset);
    Platform_ReadFile(AssetPack->Atlases, &FileHandle, AtlasesSize, AtlasesOffset);
    
    Platform_CloseFile(&FileHandle);
    
    // Convert sub-struct offsets to pointers
    
    for(u32 GroupIndex = 0;
        GroupIndex < AssetPack->GroupCount;
        ++GroupIndex)
    {
        asset_group *Group = AssetPack->Groups + GroupIndex;
        Group->Assets = (asset_info*)((mem)AssetPack + Group->AssetsOffset);
        Group->GroupTags = (asset_tag*)((mem)AssetPack + Group->GroupTagsOffset);
    }
    
    for(u32 AssetIndex = 0;
        AssetIndex < AssetPack->AssetCount;
        ++AssetIndex)
    {
        asset_info *Asset = AssetPack->Assets + AssetIndex;
        Asset->Tags = (asset_tag*)((mem)AssetPack + Asset->TagsOffset);
    }
    
    return AssetPack;
}

internal void
File_CreateAssetPack(heap *Heap)
{
    Stack_Push();
    
    //NOTE: Load the tags
    
    str TempStr;
    str FileText = File_Read(ASSET_DATA_FILENAME, 0, 0);
    
    json_token *Tokens = JSON_Parse(Heap, FileText);
    
    ASSERT((s32)Tokens[0].ChildCount >= 0);
    u32 AssetCount = Tokens[2].ChildCount - 1; // For group data
    asset_packer_asset *GenAssets = Stack_Allocate(sizeof(asset_packer_asset) * AssetCount);
    u08 *Atlases = Heap_Allocate(Heap, 0);
    u32 BytesPerAtlas = MAX_ATLAS_DIM * MAX_ATLAS_DIM * BYTES_PER_PIXEL;
    u32 TotalTagCount = 0;
    
    u32 StartIndex = Tokens[1].StartOffset;
    u32 EndIndex = Tokens[1].EndOffset;
    u32 AssetGroupID = Str_To_U32(Str_Sub(&TempStr, FileText, StartIndex, EndIndex)); Str_Free(TempStr);
    
    //TODO: Only handles one group currently
    u32 GroupCount = 1;
    u32 TokenIndex = 4;
    u32 GroupsSize = sizeof(asset_group) * GroupCount;
    asset_group *Groups = Heap_AllocateZeroed(Heap, GroupsSize);
    Groups[0].GroupTagCount = Tokens[TokenIndex++].ChildCount;
    TotalTagCount += Groups[0].GroupTagCount;
    Groups[0].GroupTags = Stack_Allocate(sizeof(asset_tag) * Groups[0].GroupTagCount);
    
    for(u32 TagIndex = 0;
        TagIndex < Groups[0].GroupTagCount;
        ++TagIndex)
    {
        str IDStr = Str_Sub(NULL, FileText, Tokens[TokenIndex].StartOffset, Tokens[TokenIndex].EndOffset);
        str ValueStr = Str_Sub(NULL, FileText, Tokens[TokenIndex+1].StartOffset, Tokens[TokenIndex+1].EndOffset);
        
        #define ENUM(Name, Value) \
            if(Mem_Cmp(IDStr, #Name, sizeof(#Name)) == EQUAL) \
            { \
                AssetPacker_FillTag(Heap, Groups[0].GroupTags + TagIndex, AssetTagID_##Name, ValueStr, 0, 0); \
            } else
        ENUM_GROUP_TAG_IDS
        #undef ENUM
            { // Continuing the last else
                Platform_Print_DEBUG("Unknown asset tag ");
                Platform_Print_DEBUG(IDStr);
                Platform_Print_DEBUG(" with a value of ");
                Platform_Print_DEBUG(ValueStr);
                Platform_Print_DEBUG("\n");
            }
        
        Str_Free(IDStr);
        Str_Free(ValueStr);
        
        TokenIndex += 2;
    }
    
    u32 ExtraSize = 0;
    mem Extra = Heap_Allocate(Heap, ExtraSize);
    for(u32 AssetIndex = 0;
        AssetIndex < AssetCount;
        ++AssetIndex)
    {
        ASSERT(Tokens[TokenIndex+0].Type == JSONToken_String &&
               Tokens[TokenIndex+1].Type == JSONToken_Object);
        
        StartIndex = Tokens[TokenIndex].StartOffset;
        EndIndex = Tokens[TokenIndex].EndOffset;
        Str_Sub(&GenAssets[AssetIndex].Path, FileText, StartIndex, EndIndex);
        
        u32 TagCount = Tokens[TokenIndex+1].ChildCount;
        u32 *ExtraStack = Stack_Allocate((TagCount+1) * sizeof(u32));
        Mem_Zero(ExtraStack, TagCount * sizeof(u32));
        for(u32 ChildIndex = 0;
            ChildIndex < TagCount;
            ++ChildIndex)
        {
            json_token Token = Tokens[TokenIndex+3+2*ChildIndex];
            if(Token.Type == JSONToken_Array)
            {
                json_token *ChildToken = Tokens + TokenIndex+3+2*ChildIndex+1;
                switch(ChildToken->Type)
                {
                    case JSONToken_Integer:
                    case JSONToken_Float:
                    {
                        ExtraStack[ChildIndex+1] = Token.ChildCount * 4;
                    } break;
                    case JSONToken_String:
                    {
                        for(u32 ChildIndex1 = 0;
                            ChildIndex1 < Token.ChildCount;
                            ++ChildIndex1)
                        {
                            ExtraStack[ChildIndex+1] += ChildToken->EndOffset - ChildToken->StartOffset + 1;
                            ++ChildToken;
                        }
                    } break;
                    default:
                    {
                        STOP;
                    }
                }
                
                ExtraStack[ChildIndex+1] += ExtraStack[ChildIndex];
            }
        }
        Heap_Resize(&Extra, ExtraSize + ExtraStack[TagCount]);
        GenAssets[AssetIndex].Tags = Stack_Allocate(sizeof(asset_tag) * TagCount);
        GenAssets[AssetIndex].TagCount = TagCount;
        TotalTagCount += TagCount;
        
        TokenIndex += 2;
        for(u32 TagIndex = 0;
            TagIndex < TagCount;
            ++TagIndex, TokenIndex += 2)
        {
            StartIndex = Tokens[TokenIndex].StartOffset;
            EndIndex = Tokens[TokenIndex].EndOffset;
            str TagStr = Str_Sub(NULL, FileText, StartIndex, EndIndex);
            
            asset_tag *Tag = GenAssets[AssetIndex].Tags + TagIndex;
            StartIndex = Tokens[TokenIndex+1].StartOffset;
            EndIndex = Tokens[TokenIndex+1].EndOffset;
            str ValueStr = Str_Sub(NULL, FileText, StartIndex, EndIndex);
            
            #define ENUM(Name, Value) \
                if(Mem_Cmp(TagStr, #Name, sizeof(#Name)) == 0) \
                { \
                    AssetPacker_FillTag(Heap, Tag, AssetTagID_##Name, ValueStr, Extra, ExtraSize + ExtraStack[TagIndex]); \
                } else
            ENUM_ASSET_TAG_IDS
            #undef ENUM
                { // Continuing the last else
                    Platform_Print_DEBUG("Unknown asset tag ");
                    Platform_Print_DEBUG(TagStr);
                    Platform_Print_DEBUG(" with a value of ");
                    Platform_Print_DEBUG(ValueStr);
                    Platform_Print_DEBUG("\n");
                }
            
            Str_Free(TagStr);
            Str_Free(ValueStr);
            
            switch(Tokens[TokenIndex+1].Type)
            {
                case JSONToken_Array:
                {
                    ASSERT(Tokens[TokenIndex+2].Type != JSONToken_Object);
                    TokenIndex += Tokens[TokenIndex+1].ChildCount;
                } break;
                case JSONToken_Object:
                {
                    STOP;
                } break;         
            }
        }
        
        ExtraSize += ExtraStack[TagCount];
    }
    
    Heap_Free(Tokens);
    Str_Free(FileText);
    
    //NOTE: Get data from files
    
    mem FileHandle;
    for(u32 AssetIndex = 0;
        AssetIndex < AssetCount;
        ++AssetIndex)
    {
        asset_packer_asset *Asset = GenAssets + AssetIndex;
        
        if(Asset->Path[0] != '\0')
        {
            str BitmapText = File_Read(Asset->Path, sizeof(bitmap_header), 0);
            bitmap_header *BitmapHeader = (bitmap_header*)BitmapText;
            
            ASSERT(BitmapHeader->Signature[0] == 'B' && BitmapHeader->Signature[1] == 'M');
            ASSERT(BitmapHeader->BitsPerPixel == 32);
            
            ASSERT(BitmapHeader->Width <= 0xFFFF && BitmapHeader->Height <= 0xFFFF);
            Asset->Size = V2u16_2x1((u16)BitmapHeader->Width, (u16)BitmapHeader->Height);
            
            Str_Free(BitmapText);
        }
    }
    
    //NOTE: Sort the assets
    
    //CREDIT: https://medium.com/basecs/pivoting-to-understand-quicksort-part-1-75178dfb9313
    //        https://www.geeksforgeeks.org/iterative-quick-sort/
    //        http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.98.3502&rep=rep1&type=pdf
    //        http://pds25.egloos.com/pds/201504/21/98/RectangleBinPack.pdf
    //        https://github.com/dagmike/BinPacking/blob/cea4a10fea7a89bfe880f6b61aab3f4f869c3cf9/src/Algorithms/BestShortSideFit.php#L9
    
    if(AssetCount >= 2)
    {
        Stack_Push();
        
        StartIndex = 0;
        EndIndex = AssetCount - 1;
        s32 StackPtr = 0;
        
        u32 *SortStack = Stack_Allocate(AssetCount * sizeof(u32));
        SortStack[StackPtr++] = StartIndex;
        SortStack[StackPtr++] = EndIndex;
        
        while(StackPtr > 0)
        {
            EndIndex = SortStack[--StackPtr];
            StartIndex = SortStack[--StackPtr];
            
            u32 LowerIndex = StartIndex;
            u32 PivotArea = V2u16_RectArea(GenAssets[EndIndex].Size);
            
            for(u32 UpperIndex = StartIndex;
                UpperIndex <= EndIndex - 1;
                ++UpperIndex)
            {
                u32 LowerArea = V2u16_RectArea(GenAssets[LowerIndex].Size);
                u32 UpperArea = V2u16_RectArea(GenAssets[UpperIndex].Size);
                
                if(UpperArea <= PivotArea)
                {
                    SWAP(GenAssets[LowerIndex], GenAssets[UpperIndex], asset_packer_asset);
                    
                    ++LowerIndex;
                }
            }
            
            SWAP(GenAssets[LowerIndex], GenAssets[EndIndex], asset_packer_asset);
            
            if(LowerIndex - 1 > StartIndex)
            {
                SortStack[StackPtr++] = StartIndex;
                SortStack[StackPtr++] = LowerIndex - 1;
            }
            if(LowerIndex + 1 < EndIndex)
            {
                SortStack[StackPtr++] = LowerIndex + 1;
                SortStack[StackPtr++] = EndIndex;
            }
        }
        
        Stack_Pop();
    }
    
    //NOTE: Pack the bitmaps
    
    u32 AtlasCount = 0;
    asset_packer_node *FreeList = NULL;
    for(s32 AssetIndex = AssetCount - 1;
        AssetIndex >= 0; // Largest to smallest
        --AssetIndex)
    {
        asset_packer_asset *Asset = GenAssets + AssetIndex;
        if(Asset->Path[0] == '\0')
        {
            continue;
        }
        
        u16 BestShortSide = U16_MAX;
        u16 BestLongSide = U16_MAX;
        asset_packer_node *BestNode = NULL;
        b08 BestNodeRotated = FALSE;
        
        asset_packer_node **BestNodeRef = NULL;
        asset_packer_node **NodeRef = &FreeList;
        asset_packer_node *Node = FreeList;
        while(Node)
        {
            v2u16 FillRect = Asset->Size;
            v2u16 FreeRect = Node->Size;
            b08 RectFits = V2u16_IsWithin(FreeRect, FillRect);
            b08 Rotated = FALSE;
            
            if(!RectFits)
            {
                FillRect = V2u16_Swap(FillRect);
                RectFits = V2u16_Equal(FreeRect, FillRect);
                Rotated = TRUE;
            }
            if(RectFits)
            {
                u16 RemainingWidth = FreeRect.X - FillRect.X;
                u16 RemainingHeight = FreeRect.Y - FillRect.Y;
                u16 ShortSide = MIN(RemainingWidth, RemainingHeight);
                u16 LongSide = MAX(RemainingWidth, RemainingHeight);
                
                if(ShortSide < BestShortSide ||
                   (ShortSide == BestShortSide && LongSide < BestLongSide))
                {
                    BestNodeRef = NodeRef;
                    BestNode = Node;
                    BestNodeRotated = Rotated;
                    
                    if(Rotated)
                    {
                        BestNode->Size = V2u16_Swap(BestNode->Size);
                    }
                    
                    BestShortSide = ShortSide;
                    BestLongSide = LongSide;
                }
            }
            
            NodeRef = &Node->Next;
            Node = Node->Next;
        }
        
        if(BestNode == NULL)
        {
            BestNode = Stack_Allocate(sizeof(asset_packer_node));
            BestNode->Offset = V2u16_1x1(0);
            BestNode->Size = V2u16_1x1(MAX_ATLAS_DIM); //TODO: Use GL_GetIntegerv for max size
            BestNode->AtlasIndex = AtlasCount;
            
            BestNode->Next = FreeList;
            BestNodeRef = &FreeList;
            FreeList = BestNode;
            
            Heap_Resize(&Atlases, Heap_GetSize(Atlases) + BytesPerAtlas);
            Mem_Zero(Atlases + (BytesPerAtlas * AtlasCount), BytesPerAtlas);
            ++AtlasCount;
        }
        
        //NOTE: Place the bitmap
        
        Asset->Offset = BestNode->Offset;
        Asset->AtlasIndex = BestNode->AtlasIndex;
        
        str Bitmap = File_Read(Asset->Path, 0, sizeof(bitmap_header));
        v4u08 *BitmapPixel = (v4u08*)Bitmap;
        v4u08 *AtlasPixel = (v4u08*)((mem)Atlases + (BytesPerAtlas * BestNode->AtlasIndex));
        
        AtlasPixel += BestNode->Offset.Y * MAX_ATLAS_DIM;
        AtlasPixel += BestNode->Offset.X;
        for(u32 Row = BestNode->Offset.Y;
            Row < (u32)(BestNode->Offset.Y + Asset->Size.Y);
            ++Row)
        {
            for(u32 Col = 0;
                Col < Asset->Size.X;
                ++Col)
            {
                v4u08 *Dest = AtlasPixel + Col;
                v4u08 *Src = BitmapPixel + Col;
                u08 Gray = (u08)(((r32)Src->X + (r32)Src->Y + (r32)Src->Z) / 3.0f);
                *Dest = V4u08_4x1(255, 255, 255, Gray);
                // *Dest = V4u08_4x1(Gray, Gray, Gray, Gray);
                
                // if(Str_Cmp(Asset->Path, Str_Create(&TempStr, "assets/fonts/arial/89.bmp", 0)) == 0)
                // {
                //     *Dest = V4u08_4x1(255, 0, 0, 255);
                // }
                // Str_Free(TempStr);
            }
            
            // Mem_Cpy(AtlasPixel, BitmapPixel, Asset->Size.X * sizeof(v4u08));
            // v4u08 Color = V4U08_4X1((u08)((255.0f/(AssetCount+2))*(AssetIndex+3)), 0, 0, 255);
            // Color = (AssetIndex == 9 ? V4U08_4X1(255, 255, 255, 255) : V4U08_4X1(0, 0, 0, 0));
            // Color = SWIZZLE(Color, SWIZZLE_ZYXW);
            // Mem_Set(AtlasPixel, FORCE_CAST(Color, u32), Asset->Size.X);
            
            BitmapPixel += Asset->Size.X;
            AtlasPixel += MAX_ATLAS_DIM;
        }
        #if 0
        str Bitmap = File_Read(Asset->Path, 0, sizeof(bitmap_header));
        v3u08 *BitmapPixel = (v3u08*)Bitmap;
        v3u08 *AtlasPixel = (v3u08*)((mem)Atlases + (BytesPerAtlas * BestNode->AtlasIndex));
        
        AtlasPixel += BestNode->Offset.Y * MAX_ATLAS_DIM;
        AtlasPixel += BestNode->Offset.X;
        for(u32 Row = BestNode->Offset.Y;
            Row < (u32)(BestNode->Offset.Y + Asset->Size.Y);
            ++Row)
        {
            Mem_Cpy(AtlasPixel, BitmapPixel, Asset->Size.X * sizeof(v3u08));
            
            BitmapPixel += Asset->Size.X;
            AtlasPixel += MAX_ATLAS_DIM;
        }
        #endif
        
        Str_Free(Bitmap);
        
        //NOTE: Split the node
        
        if(BestNode->Size.X > Asset->Size.X)
        { // Extra width
            asset_packer_node *NewNode = Stack_Allocate(sizeof(asset_packer_node));
            NewNode->Offset = V2u16_2x1(BestNode->Offset.X + Asset->Size.X,
                                        BestNode->Offset.Y);
            NewNode->Size = V2u16_2x1(BestNode->Size.X - Asset->Size.X,
                                      BestNode->Size.Y);
            NewNode->AtlasIndex = BestNode->AtlasIndex;
            
            NewNode->Next = BestNode->Next; // Append NewNode after BestNode
            BestNode->Next = NewNode;
        }
        if(BestNode->Size.Y > Asset->Size.Y)
        { // Extra height
            asset_packer_node *NewNode = Stack_Allocate(sizeof(asset_packer_node));
            NewNode->Offset = V2u16_2x1(BestNode->Offset.X,
                                        BestNode->Offset.Y + Asset->Size.Y);
            NewNode->Size = V2u16_2x1(BestNode->Size.X,
                                      BestNode->Size.Y - Asset->Size.Y);
            NewNode->AtlasIndex = BestNode->AtlasIndex;
            
            NewNode->Next = BestNode->Next; // Append NewNode after BestNode
            BestNode->Next = NewNode;
        }
        
        *BestNodeRef = BestNode->Next;
        
        //NOTE: Fix intersecting nodes by splitting
        
        Node = FreeList;
        asset_packer_node **PrevRef = &FreeList;
        while(Node)
        {
            v4u16 Intersection = V4u16_RectOverlap(V4u16_2x2(Node->Offset, Node->Size),
                                                   V4u16_2x2(Asset->Offset, Asset->Size));
            
            if(V2u16_RectArea(Intersection.ZW) != 0)
            {
                asset_packer_node *NewNodes[4];
                u32 AddedCount = 0;
                
                if(Intersection.X > Node->Offset.X)
                { // Free space on left
                    asset_packer_node *NewNode = Stack_Allocate(sizeof(asset_packer_node));
                    NewNode->Offset = Node->Offset; 
                    NewNode->Size = V2u16_2x1(Intersection.X - Node->Offset.X, Node->Size.Y);
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNodes[AddedCount++] = NewNode;
                }
                if(Intersection.Y > Node->Offset.Y)
                { // Free space on bottom
                    asset_packer_node *NewNode = Stack_Allocate(sizeof(asset_packer_node));
                    NewNode->Offset = Node->Offset; 
                    NewNode->Size = V2u16_2x1(Node->Size.X, Intersection.Y - Node->Offset.Y);
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNodes[AddedCount++] = NewNode;
                }
                if(Intersection.X + Intersection.Z < Node->Offset.X + Node->Size.X)
                { // Free space on right
                    asset_packer_node *NewNode = Stack_Allocate(sizeof(asset_packer_node));
                    NewNode->Offset = V2u16_2x1(Intersection.X + Intersection.Z, Node->Offset.Y); 
                    NewNode->Size = V2u16_2x1((Node->Offset.X + Node->Size.X) - (Intersection.X + Intersection.Z), Node->Size.Y);
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNodes[AddedCount++] = NewNode;
                }
                if(Intersection.Y + Intersection.W < Node->Offset.Y + Node->Size.Y)
                { // Free space on top
                    asset_packer_node *NewNode = Stack_Allocate(sizeof(asset_packer_node));
                    NewNode->Offset = V2u16_2x1(Node->Offset.X, Intersection.Y + Intersection.W); 
                    NewNode->Size = V2u16_2x1(Node->Size.X, (Node->Offset.Y + Node->Size.Y) - (Intersection.Y + Intersection.W));
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNodes[AddedCount++] = NewNode;
                }
                
                asset_packer_node *PrevNode = Node;
                
                for(u32 Index = 0;
                    Index < AddedCount;
                    ++Index)
                {
                    NewNodes[Index]->Next = Node->Next;
                    Node->Next = NewNodes[Index];
                    Node = Node->Next;
                }
                
                *PrevRef = PrevNode->Next; // Pop PrevNode
            }
            
            PrevRef = &Node->Next;
            Node = Node->Next;
        }
        
        //NOTE: Remove extra nodes
        
        NodeRef = &FreeList;
        Node = FreeList;
        while(Node)
        {
            asset_packer_node **NextRef = &Node->Next;
            asset_packer_node *Next = Node->Next;
            
            while(Next && Next->AtlasIndex == Node->AtlasIndex)
            {
                v4u16 CurrRect = V4u16_2x2(Node->Offset, Node->Size);
                v4u16 NextRect = V4u16_2x2(Next->Offset, Next->Size);
                v4u16 Overlap = V4u16_RectOverlap(CurrRect, NextRect);
                
                if(V4u16_Equal(Overlap, CurrRect))
                { // Curr is completely within Next
                    *NodeRef = Node->Next;
                    break;
                }
                else if(V4u16_Equal(Overlap, NextRect))
                { // Next is completely within Curr
                    *NextRef = Next->Next;
                }
                
                NextRef = &Next->Next;
                Next = Next->Next;
            }
            
            NodeRef = &Node->Next;
            Node = Node->Next;
        }
    }
    
    //NOTE: Write file
    
    #ifdef _DEBUG
        bitmap_header OutHeader = {0};
        OutHeader.Signature[0] = 'B';
        OutHeader.Signature[1] = 'M';
        OutHeader.FileSize = sizeof(bitmap_header) + (AtlasCount * BytesPerAtlas);
        OutHeader.DataOffset = sizeof(bitmap_header);
        OutHeader.Size = 40;
        OutHeader.Width = MAX_ATLAS_DIM;
        OutHeader.Height = AtlasCount * MAX_ATLAS_DIM;
        OutHeader.Planes = 1;
        OutHeader.BitsPerPixel = 32;
        
        str OutFileName = Str_Create(NULL, "assets/pack_DEBUG.bmp", 0);
        Platform_OpenFile(&FileHandle, OutFileName, FileMode_Write);
        Platform_WriteFile(&OutHeader, &FileHandle, sizeof(bitmap_header), 0);
        Platform_WriteFile(Atlases, &FileHandle, AtlasCount * BytesPerAtlas, sizeof(bitmap_header));
        Platform_CloseFile(&FileHandle);
        Str_Free(OutFileName);
    #endif
    
    asset_pack AssetPack = {0};
    AssetPack.MagicNumber = ASSET_PACK_MAGIC_NUMBER;
    AssetPack.Version = ASSET_PACK_VERSION;
    AssetPack.GroupCount = GroupCount;
    AssetPack.AssetCount = AssetCount;
    AssetPack.TagCount = TotalTagCount;
    AssetPack.ExtraSize = ExtraSize;
    AssetPack.AtlasCount = AtlasCount;
    AssetPack.AtlasSize = V2u16_1x1(MAX_ATLAS_DIM);
    
    u64 AssetsOffset = sizeof(asset_pack) + GroupsSize;
    u32 AssetsSize = AssetCount * sizeof(asset_info);
    asset_info *Assets = Heap_AllocateZeroed(Heap, AssetsSize);
    
    u64 TagsOffset = sizeof(asset_pack) + GroupsSize + AssetsSize;
    u32 TagsSize = TotalTagCount * sizeof(asset_tag);
    asset_tag *Tags = Heap_AllocateZeroed(Heap, TagsSize);
    
    mem TagCursor = (mem)Tags;
    mem AssetCursor = (mem)Assets;
    
    Groups[0].AssetCount = AssetCount;
    Groups[0].AssetsOffset = AssetsOffset;
    Groups[0].GroupID = AssetGroupID;
    Mem_Cpy(TagCursor, Groups[0].GroupTags, sizeof(asset_tag) * Groups[0].GroupTagCount);
    Groups[0].GroupTagsOffset = (u64)TagCursor - (u64)Tags + TagsOffset;
    TagCursor += sizeof(asset_tag) * Groups[0].GroupTagCount;
    
    for(u32 AssetIndex = 0;
        AssetIndex < AssetCount;
        ++AssetIndex)
    {
        asset_packer_asset *GenAsset = GenAssets + AssetIndex;
        
        Mem_Cpy(TagCursor, GenAsset->Tags, sizeof(asset_tag) * GenAsset->TagCount);
        GenAsset->TagsOffset = (u64)TagCursor - (u64)Tags + TagsOffset;
        TagCursor += sizeof(asset_tag) * GenAsset->TagCount;
        
        Mem_Cpy(AssetCursor, GenAsset, sizeof(asset_info));
        AssetCursor += sizeof(asset_info);
    }
    
    Platform_OpenFile(&FileHandle, ASSET_PACK_FILENAME, FileMode_Write);
    u64 WriteCursor = sizeof(asset_pack);
    
    Platform_WriteFile(Groups, &FileHandle, GroupsSize, WriteCursor); // Groups
    AssetPack.GroupsOffset = WriteCursor;
    WriteCursor += GroupsSize;
    
    Platform_WriteFile(Assets, &FileHandle, AssetsSize, WriteCursor); // Assets
    AssetPack.AssetsOffset = WriteCursor;
    WriteCursor += AssetsSize;
    
    Platform_WriteFile(Tags, &FileHandle, TagsSize, WriteCursor); // Tags
    AssetPack.TagsOffset = WriteCursor;
    WriteCursor += TagsSize;
    
    Platform_WriteFile(Extra, &FileHandle, ExtraSize, WriteCursor); // Extra
    AssetPack.ExtraOffset = WriteCursor;
    WriteCursor += ExtraSize;
    
    Platform_WriteFile(Atlases, &FileHandle, AtlasCount * BytesPerAtlas, WriteCursor); // Atlases
    AssetPack.AtlasesOffset = WriteCursor;
    
    Platform_WriteFile(&AssetPack, &FileHandle, sizeof(asset_pack), 0); // Header
    
    Platform_CloseFile(&FileHandle);
    
    Heap_Free(Groups);
    Heap_Free(Assets);
    Heap_Free(Tags);
    Heap_Free(Extra);
    Heap_Free(Atlases);
    for(u32 AssetIndex = 0;
        AssetIndex < AssetCount;
        ++AssetIndex)
    {
        Heap_Free(GenAssets[AssetIndex].Path);
    }
    
    Stack_Pop();
}

#if 0
internal u32
Asset_GetAssetIndex(asset_tag *Tags,
                    u32 TagCount,
                    u32 GroupCount)
{
    //CREDIT: http://www.cse.yorku.ca/~oz/hash.html
    
    // Get hash
    
    u64 Hash = 5381;
    
    asset_tag *Tag = Tags;
    for(u32 TagIndex = 0;
        TagIndex < TagCount;
        ++TagIndex, ++Tag)
    {
        // u64 Value = (FORCE_CAST(Tag->Value, u32) * Tag->ID);
        u64 Value = FORCE_CAST(Tag->Value, u32);
        
        Hash = ((Hash << 5) + Hash) ^ Value;
        // Hash = Value + (Hash << 6) + (Hash << 16) - Hash;
    }
    
    // Get index
    
    u32 Index = (u32)(Hash % GroupCount);
    
    return Index;
}
#endif

internal asset_info *
Asset_GetFromTags(asset_pack *Pack,
                  asset_group_id GroupID,
                  asset_tag *Tags,
                  r32 *Weights,
                  u32 TagCount)
{
    asset_group *Group = NULL;
    for(u32 GroupIndex = 0;
        GroupIndex < Pack->GroupCount;
        ++GroupIndex)
    {
        if(Pack->Groups[GroupIndex].GroupID == GroupID)
        {
            Group = Pack->Groups + GroupIndex;
            break;
        }
    }
    ASSERT(Group);
    
    r32 BestDiff = R32_MAX;
    asset_info *BestAsset = NULL;
    
    for(u32 AssetIndex = 0;
        AssetIndex < Group->AssetCount;
        ++AssetIndex)
    {
        asset_info *Asset = Group->Assets + AssetIndex;
        
        r32 WeightedDiff = 0.0f;
        for(u32 SearchTagIndex = 0;
            SearchTagIndex < TagCount;
            ++SearchTagIndex)
        {
            asset_tag *SearchTag = Tags + SearchTagIndex;
            r32 Weight = Weights[SearchTagIndex];
            
            for(u32 AssetTagIndex = 0;
                AssetTagIndex < Asset->TagCount;
                ++AssetTagIndex)
            {
                asset_tag *AssetTag = Asset->Tags + AssetTagIndex;
                
                if(SearchTag->ID == AssetTag->ID)
                {
                    switch(AssetTagTypes[AssetTag->ID])
                    {
                        case AssetTagType_Bool:
                        {
                            ASSERT(WITHIN(0, AssetTag->Value.Bool, 1));
                            ASSERT(WITHIN(0, SearchTag->Value.Bool, 1));
                            WeightedDiff += Weight * (r32)(AssetTag->Value.Bool ^ SearchTag->Value.Bool);
                        } break;
                        
                        case AssetTagType_R32:
                        {
                            WeightedDiff += Weight * R32_Abs(SearchTag->Value.R32 - AssetTag->Value.R32);
                        } break;
                        
                        case AssetTagType_S32:
                        {
                            WeightedDiff += Weight * (r32)S32_Abs(AssetTag->Value.S32 - SearchTag->Value.S32);
                        } break;
                        
                        case AssetTagType_U32:
                        {
                            u32 Max = MAX(AssetTag->Value.U32, SearchTag->Value.U32);
                            u32 Min = MIN(AssetTag->Value.U32, SearchTag->Value.U32);
                            WeightedDiff += Weight * (r32)(Max - Min);
                        }
                        
                        default:
                        {
                            STOP;
                        }
                    }
                    
                    break;
                }
            }
        }
        
        if(BestDiff > WeightedDiff)
        {
            BestDiff = WeightedDiff;
            BestAsset = Asset;
        }
    }
    
    ASSERT(BestAsset);
    return BestAsset;
}

internal asset_tag *
Asset_GetTag(asset_tag *Tags,
             u32 TagCount,
             asset_tag_id AssetTagID)
{
    for(u32 TagIndex = 0;
        TagIndex < TagCount;
        ++TagIndex)
    {
        asset_tag *Tag = Tags + TagIndex;
        
        if(Tag->ID == AssetTagID)
        {
            return Tag;
        }
    }
    
    return NULL;
}

internal vptr
Asset_GetExtra(asset_pack *Pack,
               asset_tag *Tag)
{
    return Pack->Extra + Tag->Value.Mem;
}

internal void
AssetPacker_FillTag(heap *Heap,
                    asset_tag *Tag,
                    asset_tag_id TagID,
                    str ValueStr,
                    mem Extra,
                    u32 ExtraOffset)
{
    Tag->ID = TagID;
    
    switch(AssetTagTypes[Tag->ID])
    {
        case AssetTagType_Bool:
        {
            Tag->Value.Bool = Str_To_Bool(ValueStr);
        } break;
        
        case AssetTagType_R32:
        {
            Tag->Value.R32 = Str_To_R32(ValueStr);
        } break;
        
        case AssetTagType_S32:
        {
            Tag->Value.S32 = Str_To_S32(ValueStr);
        } break;
        
        case AssetTagType_Arr:
        {
            Extra += ExtraOffset;
            json_token *Tokens = JSON_Parse(Heap, ValueStr);
            json_token_type Type = Tokens[1].Type;
            for(u32 ChildIndex = 0;
                ChildIndex < Tokens[0].ChildCount;
                ++ChildIndex)
            {
                json_token Token = Tokens[ChildIndex+1];
                u32 SubLen = Token.EndOffset - Token.StartOffset;
                str SubValue = Str_Sub(0, ValueStr, Token.StartOffset, Token.EndOffset);
                switch(Type)
                {
                    case JSONToken_Integer:
                    {
                        ((s32*)Extra)[ChildIndex] = Str_To_S32(SubValue);
                    } break;
                    case JSONToken_Float:
                    {
                        ((r32*)Extra)[ChildIndex] = Str_To_R32(SubValue);
                    } break;
                    case JSONToken_String:
                    {
                        Mem_Cpy(Extra, SubValue, SubLen+1);
                        Extra += SubLen+1;
                    } break;
                    default:
                    {
                        STOP;
                    }
                }
                Str_Free(SubValue);
            }
            
            Heap_Free(Tokens);
            Tag->Value.Mem = ExtraOffset;
        } break;
        
        default:
        {
            STOP;
        }
    }
}