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
    
    c08 *Text = Stack_Allocate(Length+1);
    u64 BytesRead = Platform_ReadFile(FileHandle, Text, Length, Offset);
    Text[Length] = 0;
    string String = {BytesRead, Length+1, FALSE, Text};
    
    Platform_CloseFile(FileHandle);
    
    return String;
}

internal assetpack_registry *
Assetpack_FindRegistry(assetpack Assetpack,
                       assetpack_tag_id TagID)
{
    u32 Index;
    u32 Start = 0;
    u32 End = Assetpack.Header->RegistryCount;
    while(Start != End) {
        Index = Start+(End-Start)/2;
        if(Assetpack.Registries[Index].ID == TagID) break;
        if(Assetpack.Registries[Index].ID < TagID) Start = Index+1;
        else End = Index;
    }
    if(Start == End) return NULL;
    return Assetpack.Registries+Index;
}

internal assetpack_tag *
Assetpack_FindFirstTag(assetpack Assetpack,
                       assetpack_tag_id TagID)
{
    assetpack_registry *Registry = Assetpack_FindRegistry(Assetpack, TagID);
    if(Registry == NULL) return NULL;
    if(Registry->TagCount == 0) return NULL;
    return Registry->Tags+0;
}

internal assetpack_tag *
Assetpack_FindExactTag(assetpack Assetpack,
                       assetpack_tag_id TagID,
                       vptr Value)
{
    assetpack_registry *Registry = Assetpack_FindRegistry(Assetpack, TagID);
    if(Registry == NULL) return NULL;
    if(Registry->TagCount == 0) return NULL;
    
    u32 Index;
    u32 Start = 0;
    u32 End = Registry->TagCount;
    while(Start != End) {
        Index = Start+(End-Start)/2;
        s08 CmpResult;
        switch(Registry->Type.ID) {
            case TYPEID_U32: {
                if(Registry->Tags[Index].ValueI < (u64)Value) CmpResult = LESS;
                else if(Registry->Tags[Index].ValueI > (u64)Value) CmpResult = GREATER;
                else CmpResult = EQUAL;
            } break;
            
            default: {
                Assert(0, "Not implemented");
            }
        }
        if(CmpResult == EQUAL) return Registry->Tags+Index;
        if(CmpResult == LESS) Start = Index+1;
        else End = Index;
    }
    return NULL;
}

internal assetpack_asset *
Assetpack_FindExactAsset(assetpack Assetpack,
                         u32 IDCount,
                         assetpack_tag_id *IDs,
                         vptr *Values)
{
    if(IDCount == 0) return NULL;
    
    Assert(0, "Not implemented!");
    return NULL;
}

internal assetpack
File_LoadAssetpack(c08 *FileName,
                   heap *Heap)
{
    assetpack Assetpack;
    
    file_handle FileHandle;
    Platform_OpenFile(&FileHandle, FileName, FILE_READ);
    u32 Length = Platform_GetFileLength(FileHandle);
    u08 *FileBase = Heap_AllocateA(Heap, Length+1);
    u64 BytesRead = Platform_ReadFile(FileHandle, FileBase, Length, 0);
    FileBase[Length] = 0;
    Platform_CloseFile(FileHandle);
    Assetpack.FileDataHandle = Heap_GetHandle(FileBase);
    u08 *FileCursor = FileBase;
    
    Assetpack.Header = (assetpack_header*)FileCursor;
    FileCursor += sizeof(assetpack_header);
    Assetpack.Registries = (assetpack_registry*)FileCursor;
    FileCursor += sizeof(assetpack_registry) * Assetpack.Header->RegistryCount;
    Assetpack.Tags = (assetpack_tag*)FileCursor;
    FileCursor = FileBase + Assetpack.Header->TagDataOffset;
    Assetpack.TagData = FileCursor;
    FileCursor += Assetpack.Header->TagDataSize;
    Assetpack.Assets = (assetpack_asset*)FileCursor;
    FileCursor = FileBase + Assetpack.Header->AssetDataOffset;
    Assetpack.AssetData = FileCursor;
    
    for(u32 R = 0; R < Assetpack.Header->RegistryCount; R++) {
        assetpack_registry *Registry = Assetpack.Registries+R;
        (u08*)Registry->Tags += (u64)Assetpack.Tags;
        for(u32 T = 0; T < Registry->TagCount; T++) {
            assetpack_tag *Tag = Registry->Tags+T;
            if(Registry->Type.ID == TYPEID_VPTR)
                (u08*)Tag->ValueP += (u64)Assetpack.TagData;
            (u08*)Tag->Assets += (u64)Assetpack.TagData;
            for(u32 A = 0; A < Tag->AssetCount; A++)
                (u08*)(Tag->Assets[A]) += (u64)Assetpack.Assets;
        }
    }
    
    return Assetpack;
}

internal void
File_CreateAssetpack(c08 *FileName,
                     heap *Heap,
                     r32 FontSize)
{
    Stack_Push();
    
    /* TODO
       - Verify the efficiency of the packer; there's probably a bug
       - Serialize at the end?
    */
    
    assetpack_header Header;
    heap_handle *Registries;
    heap_handle *Tags;
    heap_handle *TagData;
    heap_handle *Assets;
    
    u32 Padding = 1;
    v4u08 BorderColor = {0, 0, 0, 255};
    
    u08 *FontData = File_Read("assets\\fonts\\arial.ttf", 0, 0).Text;
    font Font = Font_Init(FontData);
    
    r32 UnitScale = 1.0f/(Font.hhea->Ascent-Font.hhea->Descent);
    
    u32 GlyphAssetSize = sizeof(struct assetpack_asset_header) + sizeof(assetpack_glyph);
    u32 BlockAssetSize = sizeof(struct assetpack_asset_header);
    
    u32 CodepointCount = 127 - 32;
    u32 AssetCount = CodepointCount + BLOCK_Count;
    Header.AssetsSize = CodepointCount*GlyphAssetSize + BLOCK_Count*BlockAssetSize;
    Assets = Heap_Allocate(Heap, Header.AssetsSize);
    
    Header.TagCount = AssetCount+1+1;
    Tags = Heap_Allocate(Heap, Header.TagCount*sizeof(assetpack_tag) + AssetCount*sizeof(assetpack_asset*));
    
    Header.TagDataSize = sizeof(assetpack_atlas) + sizeof(assetpack_font) + AssetCount*sizeof(assetpack_asset*);
    TagData = Heap_Allocate(Heap, Header.TagDataSize);
    vptr TagDataCursor = TagData->Data;
    
    u32 RegistryIndex = 0;
    Header.RegistryCount = 4;
    Registries = Heap_Allocate(Heap, Header.RegistryCount*sizeof(assetpack_registry));
    ((assetpack_registry*)Registries->Data)[RegistryIndex].ID = TAG_CODEPOINT;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Type = TYPE_U32;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].TagCount = CodepointCount;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Tags = 0;
    RegistryIndex++;
    
    ((assetpack_registry*)Registries->Data)[RegistryIndex].ID = TAG_BLOCK_TEXTURE;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Type = TYPE_U32;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].TagCount = BLOCK_Count;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Tags = (assetpack_tag*)(sizeof(assetpack_tag)*CodepointCount);
    RegistryIndex++;
    
    v2u32 AtlasDims = {512, 512};
    u32 AtlasSize = AtlasDims.X*AtlasDims.Y*sizeof(v4u08);
    u32 AtlasCount = 0;
    heap_handle *Atlases = Heap_Allocate(Heap, 0);
    
    // Load in block bitmaps
    file_handle BlockHandles[BLOCK_Count];
    for(u32 BlockIndex = 0; BlockIndex < BLOCK_Count; BlockIndex++) {
        Platform_OpenFile(BlockHandles+BlockIndex, BlockTexturePaths[BlockIndex], FILE_READ);
    }
    
    binpacker_node *NullNode = Stack_Allocate(sizeof(binpacker_node));
    NullNode->AtlasIndex = 0;
    NullNode->Pos = (v2u32){0};
    NullNode->Size = (v2u32){0};
    NullNode->Next = NullNode;
    
    asset_node *NullAssetNode = Stack_Allocate(sizeof(asset_node));
    NullAssetNode->Asset = NULL;
    NullAssetNode->Next = NullAssetNode;
    NullAssetNode->Prev = NullAssetNode;
    
    assetpack_tag *Tag = (assetpack_tag*)Tags->Data;
    assetpack_asset *Asset = (assetpack_asset*)Assets->Data;
    
    for(c08 Codepoint = ' '; Codepoint <= '~'; Codepoint++) {
        u32 GlyphIndex = Font_GetGlyphIndex(Font, Codepoint);
        if(GlyphIndex == 0) continue;
        
        font_glyph Glyph = Font_GetGlyph(Font, Codepoint, UnitScale);
        
        Asset->Size = (v2u32){Glyph.Size.X*FontSize, Glyph.Size.Y*FontSize};
        Asset->Glyph.AdvanceX = Glyph.Advance;
        Asset->Glyph.Bearing = Glyph.Bearing;
        Asset->Glyph.SizeR = Glyph.Size;
        
        Tag->ValueI = Codepoint;
        Tag->AssetCount = 1;
        Tag->Assets = (vptr)((u08*)TagDataCursor - (u64)TagData->Data);
        *(vptr*)TagDataCursor = (u08*)Asset - (u64)Assets->Data;
        (u08*)TagDataCursor += sizeof(assetpack_asset*);
        
        if(Glyph.Shape.ContourCount == 0) {
            Asset->Pos = (v2u32){0};
            Asset->AtlasIndex = 0;
        } else {
            asset_node *AssetNode = Stack_Allocate(sizeof(asset_node));
            AssetNode->Type = ASSET_GLYPH;
            AssetNode->Glyph = Glyph;
            AssetNode->Size = (v2u32){Asset->Size.X+Padding, Asset->Size.Y+Padding};
            AssetNode->Asset = Asset;
            AssetNode->Prev = NullAssetNode;
            AssetNode->Next = NullAssetNode->Next;
            AssetNode->Prev->Next = AssetNode;
            AssetNode->Next->Prev = AssetNode;
        }
        
        (u08*)Asset += GlyphAssetSize;
        Tag++;
    }
    
    for(u32 BlockIndex = 0; BlockIndex < BLOCK_Count; BlockIndex++) {
        bitmap_header BitmapHeader;
        Platform_ReadFile(BlockHandles[BlockIndex], &BitmapHeader, sizeof(bitmap_header), 0);
        
        Asset->Size = (v2u32){BitmapHeader.Width, BitmapHeader.Height};
        
        Tag->ValueI = BlockIndex;
        Tag->AssetCount = 1;
        Tag->Assets = (vptr)((u08*)TagDataCursor - (u64)TagData->Data);
        *(vptr*)TagDataCursor = (u08*)Asset - (u64)Assets->Data;
        (u08*)TagDataCursor += sizeof(assetpack_asset*);
        
        asset_node *AssetNode = Stack_Allocate(sizeof(asset_node));
        AssetNode->Type = ASSET_BLOCK;
        AssetNode->HandleIndex = BlockIndex;
        AssetNode->BitmapOffset = BitmapHeader.DataOffset;
        AssetNode->Size = (v2u32){Asset->Size.X+Padding, Asset->Size.Y+Padding};
        AssetNode->Asset = Asset;
        AssetNode->Prev = NullAssetNode;
        AssetNode->Next = NullAssetNode->Next;
        AssetNode->Prev->Next = AssetNode;
        AssetNode->Next->Prev = AssetNode;
        
        (u08*)Asset += BlockAssetSize;
        Tag++;
    }
    
    while(NullAssetNode->Next != NullAssetNode) {
        // Find a fitting node
        s32 MinShortSide = S32_MAX;
        s32 MinLongSide = S32_MAX;
        binpacker_node *Node = NullNode;
        binpacker_node *BestNode = NULL;
        asset_node *BestAssetNode=NULL;
        
        do {
            Node = Node->Next;
            asset_node *AssetNode = NullAssetNode;
            
            if(Node != NullNode) {
                while(AssetNode->Next != NullAssetNode) {
                    AssetNode = AssetNode->Next;
                    
                    v2s32 SizeDiff = {Node->Size.X-AssetNode->Size.X, Node->Size.Y-AssetNode->Size.Y};
                    
                    if(SizeDiff.X < 0 || SizeDiff.Y < 0) continue;
                    
                    s32 ShortSide = MIN(SizeDiff.X, SizeDiff.Y);
                    s32 LongSide = MAX(SizeDiff.X, SizeDiff.Y);
                    
                    if(ShortSide < MinShortSide) {
                        BestNode = Node;
                        BestAssetNode = AssetNode;
                        MinShortSide = ShortSide;
                    } else if(ShortSide == MinShortSide && LongSide < MinLongSide) {
                        BestNode = Node;
                        BestAssetNode = AssetNode;
                        MinLongSide = LongSide;
                    }
                }
            }
            
            // Make a new atlas if necessary
            if(BestNode == NULL && Node->Next == NullNode) {
                binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
                NewNode->Pos = (v2u32){Padding, Padding};
                Assert(AssetNode->Size.X < AtlasDims.X && AssetNode->Size.Y < AtlasDims.Y);
                NewNode->Size = (v2u32){AtlasDims.X-Padding, AtlasDims.Y-Padding};
                NewNode->AtlasIndex = AtlasCount;
                NewNode->Next = Node->Next;
                NewNode->Prev = Node;
                NewNode->Next->Prev = NewNode;
                NewNode->Prev->Next = NewNode;
                
                Heap_Resize(Atlases, Atlases->Size + AtlasSize);
                v4u08 *Bitmap = (v4u08*)(Atlases->Data + AtlasSize*AtlasCount);
                
                //TODO: Less granular memset?
                for(u32 Y = 0; Y < AtlasDims.Y; Y++) {
                    for(u32 X = 0; X < AtlasDims.X; X++) {
                        Bitmap[INDEX_2D(X, Y, AtlasDims.X)] = BorderColor;
                    }
                }
                AtlasCount++;
            }
        } while(Node->Next != NullNode);
        
        Node = BestNode;
        asset_node *AssetNode = BestAssetNode;
        AssetNode->Prev->Next = AssetNode->Next;
        AssetNode->Next->Prev = AssetNode->Prev;
        
        Asset = AssetNode->Asset;
        Asset->AtlasIndex = Node->AtlasIndex;
        
        v4u08 *Bitmap = (v4u08*)(Atlases->Data + Node->AtlasIndex*AtlasSize);
        Bitmap += INDEX_2D(Node->Pos.X, Node->Pos.Y, AtlasDims.X);
        Asset->Pos = Node->Pos;
        
        switch(AssetNode->Type) {
            case ASSET_GLYPH: {
                MSDF_DrawShape(AssetNode->Glyph.Shape, &Asset->Pos, &Asset->Size, Bitmap, Node->Pos, Node->AtlasIndex, AtlasDims);
            } break;
            
            case ASSET_BLOCK: {
                Stack_Push();
                u32 Size = Asset->Size.X*Asset->Size.Y*sizeof(v4u08);
                v4u08 *Src = Stack_Allocate(Size);
                //TODO: Async read?
                Platform_ReadFile(BlockHandles[AssetNode->HandleIndex], Src, Size, AssetNode->BitmapOffset);
                for(u32 Y = 0; Y < Asset->Size.Y; Y++) {
                    for(u32 X = 0; X < Asset->Size.X; X++) {
                        v4u08 Color = {Src->Z, Src->Y, Src->X, 255};
                        Bitmap[INDEX_2D(X, Y, AtlasDims.X)] = Color;
                        Src++;
                    }
                }
                Stack_Pop();
            } break;
        }
        
        v2u32 Pos = Node->Pos;
        v2u32 Pos2 = V2u32_Add(Node->Pos, AssetNode->Size);
        Node = NullNode;
        while(Node->Next != NullNode) {
            Node = Node->Next;
            if(Node->AtlasIndex != Asset->AtlasIndex) continue;
            
            binpacker_node *NewNode = NULL;
            v2u32 NodePos2 = V2u32_Add(Node->Pos, Node->Size);
            if(Node->Pos.Y < Pos2.Y && Pos.Y < NodePos2.Y) {
                if(Node->Pos.X < Pos.X && Pos.X < NodePos2.X) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNode->Prev = Node;
                    NewNode->Next = Node->Next;
                    NewNode->Prev->Next = NewNode;
                    NewNode->Next->Prev = NewNode;
                    NewNode->Pos = Node->Pos;
                    NewNode->Size = (v2u32){Pos.X - Node->Pos.X, Node->Size.Y};
                } if(Node->Pos.X < Pos2.X && Pos2.X < NodePos2.X) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNode->Prev = Node;
                    NewNode->Next = Node->Next;
                    NewNode->Prev->Next = NewNode;
                    NewNode->Next->Prev = NewNode;
                    NewNode->Pos = (v2u32){Pos2.X, Node->Pos.Y};
                    NewNode->Size = (v2u32){NodePos2.X - Pos2.X, Node->Size.Y};
                }
            } if(Node->Pos.X < Pos2.X && Pos.X < NodePos2.X) {
                if(Node->Pos.Y < Pos.Y && Pos.Y < NodePos2.Y) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNode->Prev = Node;
                    NewNode->Next = Node->Next;
                    NewNode->Prev->Next = NewNode;
                    NewNode->Next->Prev = NewNode;
                    NewNode->Pos = Node->Pos;
                    NewNode->Size = (v2u32){Node->Size.X, Pos.Y - Node->Pos.Y};
                } if(Node->Pos.Y < Pos2.Y && Pos2.Y < NodePos2.Y) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = Node->AtlasIndex;
                    NewNode->Prev = Node;
                    NewNode->Next = Node->Next;
                    NewNode->Prev->Next = NewNode;
                    NewNode->Next->Prev = NewNode;
                    NewNode->Pos = (v2u32){Node->Pos.X, Pos2.Y};
                    NewNode->Size = (v2u32){Node->Size.X, NodePos2.Y - Pos2.Y};
                }
            }
            
            if(NewNode) {
                Node->Prev->Next = Node->Next;
                Node->Next->Prev = Node->Prev;
            }
        }
        //TODO: Individual Atlas1, Atlas2, ... lists so only search a specific atlas
        
        binpacker_node *Node1 = NullNode->Next;
        while(Node1 != NullNode) {
            v2u32 Node1Pos2 = V2u32_Add(Node1->Pos, Node1->Size);
            binpacker_node *Node2 = NullNode;
            while(Node2->Next != NullNode) {
                Node2 = Node2->Next;
                v2u32 Node2Pos2 = V2u32_Add(Node2->Pos, Node2->Size);
                if(Node1 == Node2) continue;
                if(Node1->AtlasIndex != Node2->AtlasIndex) continue;
                if(Node1->Pos.X <= Node2->Pos.X && Node2Pos2.X <= Node1Pos2.X && Node1->Pos.Y <= Node2->Pos.Y && Node2Pos2.Y <= Node1Pos2.Y) {
                    Node2->Prev->Next = Node2->Next;
                    Node2->Next->Prev = Node2->Prev;
                }
            }
            Node1 = Node1->Next;
        }
    }
    
    ((assetpack_registry*)Registries->Data)[RegistryIndex].ID = TAG_ATLAS_DESCRIPTOR;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Type = TYPE_VPTR;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].TagCount = 1;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Tags = (assetpack_tag*)((u08*)Tag - Tags->Data);
    RegistryIndex++;
    Tag->ValueP = (u08*)TagDataCursor - (u64)TagData->Data;
    Tag->AssetCount = 0;
    Tag++;
    assetpack_atlas *AtlasDescriptor = (assetpack_atlas*)TagDataCursor;
    (u08*)TagDataCursor += sizeof(assetpack_atlas);
    AtlasDescriptor->DataOffset = 0;
    AtlasDescriptor->Size = AtlasDims;
    AtlasDescriptor->Count = AtlasCount;
    
    ((assetpack_registry*)Registries->Data)[RegistryIndex].ID = TAG_FONT_DEF;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Type = TYPE_VPTR;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].TagCount = 1;
    ((assetpack_registry*)Registries->Data)[RegistryIndex].Tags = (assetpack_tag*)((u08*)Tag - Tags->Data);
    RegistryIndex++;
    Tag->ValueP = (u08*)TagDataCursor - (u64)TagData->Data;
    Tag->AssetCount = 0;
    Tag++;
    assetpack_font *FontDescriptor = (assetpack_font*)TagDataCursor;
    (u08*)TagDataCursor += sizeof(assetpack_font);
    FontDescriptor->Ascent  = Font.hhea->Ascent*UnitScale;
    FontDescriptor->Descent = Font.hhea->Descent*UnitScale;
    FontDescriptor->LineGap = Font.hhea->LineGap*UnitScale;
    
    bitmap_header BitmapHeader = {0};
    BitmapHeader.Signature[0] = 'B';
    BitmapHeader.Signature[1] = 'M';
    BitmapHeader.FileSize = sizeof(bitmap_header) + Atlases->Size;
    BitmapHeader.DataOffset = sizeof(bitmap_header);
    BitmapHeader.Size = 40;
    BitmapHeader.Width = AtlasDims.X;
    BitmapHeader.Height = AtlasDims.Y * AtlasCount;
    BitmapHeader.Planes = 1;
    BitmapHeader.BitsPerPixel = 32;
    BitmapHeader.ImageSize = Atlases->Size;
    
    file_handle FileHandle;
    Platform_OpenFile(&FileHandle, "assets\\pack.bmp", FILE_WRITE);
    Platform_WriteFile(FileHandle, &BitmapHeader, sizeof(bitmap_header), 0);
    Platform_WriteFile(FileHandle, Atlases->Data, Atlases->Size, sizeof(bitmap_header));
    Platform_CloseFile(FileHandle);
    
    Platform_OpenFile(&FileHandle, FileName, FILE_WRITE);
    u64 Offset = sizeof(assetpack_header);
    Platform_WriteFile(FileHandle, Registries->Data, Registries->Size, Offset);
    Offset += Registries->Size;
    Platform_WriteFile(FileHandle, Tags->Data, Tags->Size, Offset);
    Offset += Tags->Size;
    Header.TagDataOffset = Offset;
    Platform_WriteFile(FileHandle, TagData->Data, TagData->Size, Offset);
    Offset += TagData->Size;
    Platform_WriteFile(FileHandle, Assets->Data, Assets->Size, Offset);
    Offset += Assets->Size;
    Header.AssetDataOffset = Offset;
    Header.AssetDataSize = Atlases->Size;
    Platform_WriteFile(FileHandle, Atlases->Data, Atlases->Size, Offset);
    Platform_WriteFile(FileHandle, &Header, sizeof(assetpack_header), 0);
    Platform_CloseFile(FileHandle);
    
    for(u32 BlockIndex = 0; BlockIndex < BLOCK_Count; BlockIndex++) {
        Platform_CloseFile(BlockHandles[BlockIndex]);
    }
    
    Heap_Free(Atlases);
    Heap_Free(Assets);
    Heap_Free(TagData);
    Heap_Free(Tags);
    Heap_Free(Registries);
    
    Stack_Pop();
}