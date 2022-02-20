/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal string
File_Read(c08 *FileName,
          u64 Length,
          u64 Offset)
{
    file_handle FileHandle;
    Assert(Platform_OpenFile(&FileHandle, FileName, FILE_READ),
           "Invalid file name!");
    
    if(Length == 0) {
        Length = Platform_GetFileLength(FileHandle);
    }
    
    vptr Text = Stack_Allocate(Length);
    u64 BytesRead = Platform_ReadFile(FileHandle, Text, Length, Offset);
    string String = {BytesRead, Length, FALSE, Text};
    
    Platform_CloseFile(FileHandle);
    
    return String;
}

internal assetpack
File_LoadAssetpack(c08 *FileName)
{
    assetpack Assetpack;
    Stack_Push();
    
    string FileDataStr = File_Read(FileName, 0, 0);
    u08 *FileCursor = (u08*)FileDataStr.Text;
    u64 BaseOffset = (u64)FileCursor;
    
    Assetpack.Header = (assetpack_header*)FileCursor;
    FileCursor += sizeof(assetpack_header);
    Assetpack.Registries = (assetpack_tag_registry*)FileCursor;
    FileCursor += sizeof(assetpack_tag_registry) * Assetpack.Header->TagRegistryCount;
    Assetpack.Tags = (assetpack_tag*)FileCursor;
    FileCursor += sizeof(assetpack_tag) * Assetpack.Header->TagCount;
    Assetpack.TagData = FileCursor;
    FileCursor += Assetpack.Header->TagDataSize;
    Assetpack.Assets = (assetpack_asset*)FileCursor;
    FileCursor += sizeof(assetpack_asset) * Assetpack.Header->AssetCount;
    Assetpack.AssetData = FileCursor;
    
    for(u32 R = 0; R < Assetpack.Header->TagRegistryCount; R++) {
        assetpack_tag_registry *Registry = Assetpack.Registries+R;
        (u08*)Registry->Tags += BaseOffset;
        if(Assetpack.Registries[R].Type.Size > sizeof(vptr)) {
            assetpack_tag *Tag = Registry->Tags;
            for(u32 T = 0; T < Registry->TagCount; T++) {
                (u08*)Tag->ValueP += BaseOffset;
                for(u32 A = 0; A < Tag->AssetCount; A++)
                    (u08*)(Tag->Assets[A]) += BaseOffset;
                (u08*)Tag += sizeof(assetpack_tag) + Tag->AssetCount * sizeof(assetpack_asset);
            }
        }
    }
    
    Stack_Pop();
    return Assetpack;
}

internal void
File_CreateAssetpack(c08 *FileName,
                     heap *Heap)
{
    assetpack_header Header;
    heap_handle *Registries;
    heap_handle *Tags;
    heap_handle *TagData;
    heap_handle *Assets;
    
    stbtt_fontinfo FontInfo;
    u08 *FontData = File_Read("assets\\fonts\\cour.ttf", 0, 0).Text;
    stbtt_InitFont(&FontInfo, FontData, stbtt_GetFontOffsetForIndex(FontData, 0));
    r32 Scale = stbtt_ScaleForPixelHeight(&FontInfo, 36.0f);
    s32 Ascent, Descent, LineGap;
    stbtt_GetFontVMetrics(&FontInfo, &Ascent, &Descent, &LineGap);
    Ascent  = (s32)((r32)Ascent  * Scale);
    Descent = (s32)((r32)Descent * Scale);
    LineGap = (s32)((r32)LineGap * Scale);
    
    Header.AssetCount = 127-32 + 1;
    Assets = Heap_Allocate(Heap, (127-32)*sizeof(assetpack_texture) + sizeof(assetpack_font));
    
    Header.TagCount = Header.AssetCount;
    Tags = Heap_Allocate(Heap, Header.TagCount*sizeof(assetpack_tag) + Header.AssetCount*sizeof(assetpack_asset*);
    
    Header.TagDataSize = 0;
    TagData = Heap_Allocate(Heap, Header.TagDataSize);
    
    Header.TagRegistryCount = 3;
    Registries = Heap_Allocate(Heap, Header.TagRegistryCount*sizeof(assetpack_tag_registry));
    ((assetpack_tag_registry*)Registries->Data)[0].ID = TAG_CODEPOINT;
    ((assetpack_tag_registry*)Registries->Data)[0].Type = TYPE_U32;
    ((assetpack_tag_registry*)Registries->Data)[0].TagCount = Header.AssetCount;
    ((assetpack_tag_registry*)Registries->Data)[0].Tags = 0;
    
    u32 AssetNodeCount = 1;
    asset_node *AssetNodes = Stack_Allocate((Header.AssetCount+1) * sizeof(asset_node));
    AssetNodes[0].Next = NULL;
    AssetNodes[0].Prev = NULL;
    assetpack_texture *Asset = (assetpack_texture*)Assets->Data;
    assetpack_tag *Tag = (assetpack_tag*)Tags->Data;
    for(c08 Codepoint = ' '; Codepoint <= '~'; Codepoint++) {
        u32 GlyphIndex = stbtt_FindGlyphIndex(&FontInfo, Codepoint);
        if(GlyphIndex == 0) continue;
        
        s32 AdvanceX, BearingX;
        stbtt_GetGlyphHMetrics(&FontInfo, GlyphIndex, &AdvanceX, &BearingX);
        Asset->AdvanceX = (s32)((r32)AdvanceX * Scale);
        Asset->Bearing.X = (s32)((r32)BearingX * Scale);
        
        s32 SX, SY, EX, EY;
        stbtt_GetGlyphBitmapBox(&FontInfo, GlyphIndex, Scale, Scale, &SX, &SY, &EX, &EY);
        Asset->Bearing.Y = -EY;
        Asset->Size = (v2u32){EX - SX, EY - SY};
        
        // Tag->ValueI = Codepoint;
        // Tag->AssetCount = 1;
        // Tag->Assets[0] = (u08*)Asset - (u08*)Assets->Data;
        // (u08*)Tag += sizeof(assetpack_tag) + sizeof(assetpack_asset*);
        (u64*)Tag->ValueI++ = Codepoint;
        (u32*)Tag->AssetCount++ = 1;
        ((assetpack_asset*)Tag->Assets++)[0] = (u08*)Asset - Assets->Data;
        
        AssetNodes[AssetNodeCount].Size = Asset->Size;
        AssetNodes[AssetNodeCount]->Prev = AssetNodes[AssetNodeCount-1];
        AssetNodes[AssetNodeCount-1]->Next = AssetNodes[AssetNodeCount];
        AssetNodeCount++;
        
        Asset++;
    }
    AssetNodes[AssetNodeCount-1].Next = AssetNodes[0];
    AssetNodes[0].Prev = AssetNodes[AssetNodeCount-1];
    
    v2u32 AtlasDims = {1024, 1024};
    u32 AtlasSize = AtlasDims.X * AtlasDims.Y;
    u32 AtlasCount = 0;
    u08 **Atlases;
    
    
    
    // MAXRECTS-BSSF-BBF-GLOBAL
    binpacker_node *PrevNode = /*NullNode*/;
    asset_node *AssetNode = AssetNodes[0].Next;
    Asset = (assetpack_texture*)Assets->Data;
    binpacker_node *RootNode = Stack_Allocate(sizeof(binpacker_node));
    RootNode->AtlasIndex = 0;
    RootNode->Next = NULL;
    RootNode->Size = AtlasDims;
    while(AssetNode != AssetNodes[0]) {
        AssetNode = AssetNodes[0].Next;
        binpacker_node *Node = RootNode;
        
        while(AssetNode != AssetNodes[0]) {
            Node = RootNode->Next;
            
            while(Node != RootNode) {
                if(AssetNode->Size.X < Node->Size.X && AssetNode->Size.Y < Node->Size.Y) {
                    if(Node->Size.X - AssetNode->Size.X < Node->Size.Y - AssetNode->Size.Y) {
                        
                    }
                } else if(AssetNode->Size.X < Node->Size.Y && AssetNode->Size.Y < Node->Size.X) {
                    
                } else {
                    
                }
                
                
                Node = Node->Next;
            }
            
            AssetNode = AssetNode->Next;
        }
        
        if(Node == RootNode) {
            binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
            NewNode->Pos = (v2u32){0};
            NewNode->Size = AtlasDims;
            NewNode->AtlasIndex = AtlasCount;
            NewNode->Next = RootNode->Next;
            RootNode->Next = NewNode;
            AtlasCount++;
        }
        
        // Split the node into two
        if(AssetNode->Size.X < Node->Size.X) {
            binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
            NewNode->AtlasIndex = Node->AtlasIndex;
            NewNode->Pos = (v2u32){Node->Pos.X+AssetNode->Size.X, Node->Pos.Y};
            NewNode->Size = (v2u32){Node->Size.X-AssetNode->Size.X, Node->Size.Y};
            NewNode->Next = Node->Next;
            Node->Next = NewNode;
        }
        if(AssetNode->Size.Y < Node->Size.Y) {
            binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
            NewNode->AtlasIndex = Node->AtlasIndex;
            NewNode->Pos = (v2u32){Node->Pos.X, Node->Pos.Y+AssetNode->Size.Y};
            NewNode->Size = (v2u32){Node->Size.X, Node->Size.Y-AssetNode->Size.Y};
            NewNode->Next = Node->Next;
            Node->Next = NewNode;
        }
        PrevNode->Next = Node->Next;
        
        // Split nodes that intersect with assetnode
        
        
        // Remove redudant nodes
        
        
        
        
    }
    
    
    
    
    
    
    
    
    
    assetpack_font *FontDef = (assetpack_font*)Asset;
    FontDef->AdvanceY = Ascent - Descent + LineGap;
    ((assetpack_tag_registry*)Registries->Data)[1].ID = TAG_FONT_DEF;
    ((assetpack_tag_registry*)Registries->Data)[1].Type = TYPE_U32;
    ((assetpack_tag_registry*)Registries->Data)[1].TagCount = 1;
    ((assetpack_tag_registry*)Registries->Data)[1].Tags = (u08*)Tag - Tags->Data;
    Tag->ValueI = 1;
    Tag->AssetCount = 1;
    Tag->Assets[0] = (u08*)FontDef - Assets->Data;
    (u08*)Tag += sizeof(assetpack_tag) + sizeof(assetpack_asset*);
    
    
    
    
    
    
    
    
    
    
    
    
    
    file_handle FileHandle;
    Platform_OpenFile(&FileHandle, FileName, FILE_WRITE);
    u64 Offset = 0;
    Platform_WriteFile(FileHandle, &Header, sizeof(assetpack_header), Offset);
    Offset += sizeof(assetpack_header);
    Platform_WriteFile(FileHandle, Registries->Data, Registries->Size, Offset);
    Offset += Registries->Size;
    Platform_WriteFile(FileHandle, Tags->Data, Tags->Size, Offset);
    Offset += Tags->Size;
    Platform_WriteFile(FileHandle, TagData->Data, TagData->Size, Offset);
    Offset += TagData->Size;
    Platform_WriteFile(FileHandle, Assets->Data, Assets->Size, Offset);
    Offset += Assets->Size;
    for(u32 I = 0; I < AtlasCount; I++) {
        Platform_WriteFile(FileHandle, Atlases[I]->Data, Atlases[I]->Size, Offset);
        Offset += Atlases[I]->Size;
    }
    Platform_CloseFile(FileHandle);
    
    Heap_Free(Registries);
    Heap_Free(Tags);
    Heap_Free(TagData);
    Heap_Free(Assets);
    for(u32 I = 0; I < AtlasCount; I++)
        Heap_Free(Heap_GetHandle(Atlases[I]));
}