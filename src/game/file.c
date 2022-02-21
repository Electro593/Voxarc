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
    Stack_Push();
    
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
    Tags = Heap_Allocate(Heap, Header.TagCount*sizeof(assetpack_tag) + Header.AssetCount*sizeof(assetpack_asset*));
    
    Header.TagDataSize = 0;
    TagData = Heap_Allocate(Heap, Header.TagDataSize);
    
    Header.TagRegistryCount = 3;
    Registries = Heap_Allocate(Heap, Header.TagRegistryCount*sizeof(assetpack_tag_registry));
    ((assetpack_tag_registry*)Registries->Data)[0].ID = TAG_CODEPOINT;
    ((assetpack_tag_registry*)Registries->Data)[0].Type = TYPE_U32;
    ((assetpack_tag_registry*)Registries->Data)[0].TagCount = Header.AssetCount;
    ((assetpack_tag_registry*)Registries->Data)[0].Tags = 0;
    
    v2u32 AtlasDims = {256, 256};
    u32 AtlasSize = AtlasDims.X*AtlasDims.Y*sizeof(v4u08);
    u32 AtlasCount = 0;
    heap_handle *Atlases = Heap_Allocate(Heap, 0);
    
    binpacker_node *NullNode = Stack_Allocate(sizeof(binpacker_node));
    NullNode->AtlasIndex = 0;
    NullNode->Pos = (v2u32){0};
    NullNode->Size = (v2u32){0};
    NullNode->Next = NullNode;
    
    asset_node *NullAssetNode = Stack_Allocate(sizeof(asset_node));
    NullAssetNode->GlyphIndex = 0;
    NullAssetNode->Asset = NULL;
    NullAssetNode->Next = NullAssetNode;
    
    assetpack_texture *Asset = (assetpack_texture*)Assets->Data;
    assetpack_tag *Tag = (assetpack_tag*)Tags->Data;
    // for(c08 Codepoint = ' '; Codepoint <= 38; Codepoint++) {
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
        
        Tag->ValueI = Codepoint;
        Tag->AssetCount = 1;
        Tag->Assets[0] = (vptr)((u08*)Asset - (u64)Assets->Data);
        (u08*)Tag += sizeof(assetpack_tag) + sizeof(assetpack_asset*);
        
        asset_node *AssetNode = Stack_Allocate(sizeof(asset_node));
        AssetNode->GlyphIndex = GlyphIndex;
        AssetNode->Asset = Asset;
        AssetNode->Next = NullAssetNode->Next;
        NullAssetNode->Next = AssetNode;
        
        Asset++;
    }
    
    binpacker_node *BestNode, *PrevBestNode=NULL, *Node, *PrevNode;
    asset_node *BestAssetNode=NULL, *PrevBestAssetNode=NULL, *AssetNode, *PrevAssetNode=NULL;
    u32 MinShortSide, MinLongSide;
    b08 IsRotated;
    while(NullAssetNode->Next != NullAssetNode) {
        // Find a fitting node
        IsRotated = FALSE;
        MinShortSide = U32_MAX;
        MinLongSide = U32_MAX;
        BestNode = NULL;
        AssetNode = NullAssetNode;
        
        Node = NullNode;
        do {
            PrevNode = Node;
            Node = Node->Next;
            
            AssetNode = NullAssetNode;
            while(AssetNode->Next != NullAssetNode) {
                PrevAssetNode = AssetNode;
                AssetNode = AssetNode->Next;
                Asset = AssetNode->Asset;
                
                u32 ShortSideN = U32_MAX;
                u32 ShortSideR = U32_MAX;
                u32 LongSideN = U32_MAX;
                u32 LongSideR = U32_MAX;
                if(Asset->Size.X < Node->Size.X && Asset->Size.Y < Node->Size.Y) {
                    v2u32 SizeDiff = V2u32_Sub(Node->Size, Asset->Size);
                    ShortSideN = MIN(SizeDiff.X, SizeDiff.Y);
                    LongSideN = MAX(SizeDiff.X, SizeDiff.Y);
                }
                if(Asset->Size.Y < Node->Size.X && Asset->Size.X < Node->Size.Y) {
                    v2u32 AssetSize = (v2u32){Asset->Size.Y, Asset->Size.X};
                    v2u32 SizeDiff = V2u32_Sub(Node->Size, AssetSize);
                    ShortSideR = MIN(SizeDiff.X, SizeDiff.Y);
                    LongSideR = MAX(SizeDiff.X, SizeDiff.Y);
                }
                
                u32 ShortSide = MIN(ShortSideN, ShortSideR);
                u32 LongSide = MIN(LongSideN, LongSideR);
                if(ShortSide < MinShortSide) {
                    BestNode = Node;
                    PrevBestNode = PrevNode;
                    PrevBestAssetNode = PrevAssetNode;
                    BestAssetNode = AssetNode;
                    MinShortSide = ShortSide;
                    IsRotated = ShortSide != ShortSideN;
                } else if(ShortSide == MinShortSide && LongSide < MinLongSide) {
                    BestNode = Node;
                    PrevBestNode = PrevNode;
                    PrevBestAssetNode = PrevAssetNode;
                    BestAssetNode = AssetNode;
                    MinLongSide = LongSide;
                    IsRotated = LongSide != LongSideN;
                }
            }
            
            // Make a new atlas if necessary
            if(BestNode == NULL && Node->Next == NullNode) {
                binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
                NewNode->Pos = (v2u32){0};
                Assert(Asset->Size.X < AtlasDims.X && Asset->Size.Y < AtlasDims.Y);
                NewNode->Size = AtlasDims;
                NewNode->AtlasIndex = AtlasCount;
                NewNode->Next = Node->Next;
                Node->Next = NewNode;
                Heap_Resize(Atlases, Atlases->Size + AtlasSize);
                Mem_Set(Atlases->Data+AtlasSize*AtlasCount, 0, AtlasSize);
                AtlasCount++;
            }
        } while(Node->Next != NullNode);
        PrevNode = PrevBestNode;
        Node = BestNode;
        PrevAssetNode = PrevBestAssetNode;
        AssetNode = BestAssetNode;
        PrevAssetNode->Next = AssetNode->Next;
        Asset = AssetNode->Asset;
        Asset->IsRotated = IsRotated;
        
        //TODO abstract binpacker for... reasons? Maybe?
        //TODO state-wise, like BinPacker_Start, _AddAsset, _Build, etc.?
        
        // Place the texture
        u08 *Bitmap = Heap_AllocateA(Heap, Asset->Size.X*Asset->Size.Y);
        stbtt_MakeGlyphBitmap(&FontInfo, Bitmap, Asset->Size.X, Asset->Size.Y, Asset->Size.X, Scale, Scale, AssetNode->GlyphIndex);
        if(IsRotated) SWAP(Asset->Size.X, Asset->Size.Y, u32);
        for(u32 Y = 0; Y < Asset->Size.Y; Y++) {
            for(u32 X = 0; X < Asset->Size.X; X++) {
                u08 Gray;
                if(!IsRotated) {
                    Gray = Bitmap[INDEX_2D(X, Asset->Size.Y-Y-1, Asset->Size.X)];
                } else {
                    Gray = Bitmap[INDEX_2D(Asset->Size.Y-Y-1, Asset->Size.X-X-1, Asset->Size.Y)];
                }
                
                v4u08 *Atlas = (v4u08*)(Atlases->Data + Node->AtlasIndex*AtlasSize);
                v4u08 *Pixel = Atlas + INDEX_2D(X+Node->Pos.X, Y+Node->Pos.Y, AtlasDims.X);
                *Pixel = (v4u08){Gray, Gray, Gray, 255};
            }
        }
        Asset->DataOffset = INDEX_2D(Node->Pos.X, Node->Pos.Y, AtlasDims.X) * sizeof(v4u08);
        Heap_FreeA(Bitmap);
        
        // Split the node
        if(Asset->Size.X < Node->Size.X) {
            binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
            NewNode->AtlasIndex = Node->AtlasIndex;
            NewNode->Pos = (v2u32){Node->Pos.X+Asset->Size.X, Node->Pos.Y};
            NewNode->Size = (v2u32){Node->Size.X-Asset->Size.X, Node->Size.Y};
            NewNode->Next = Node->Next;
            Node->Next = NewNode;
        }
        if(Asset->Size.Y < Node->Size.Y) {
            binpacker_node *NewNode = Stack_Allocate(sizeof(binpacker_node));
            NewNode->AtlasIndex = Node->AtlasIndex;
            NewNode->Pos = (v2u32){Node->Pos.X, Node->Pos.Y+Asset->Size.Y};
            NewNode->Size = (v2u32){Node->Size.X, Node->Size.Y-Asset->Size.Y};
            NewNode->Next = Node->Next;
            Node->Next = NewNode;
        }
        PrevNode->Next = Node->Next;
        Node->Size = Asset->Size;
        
        //TODO: Individual Atlas1, Atlas2, ... lists so
        // only search a specific atlas
        
        // Split nodes that intersect the asset
        binpacker_node *PrevFreeNode;
        binpacker_node *FreeNode = NullNode;
        v2u32 NodePos2 = V2u32_Add(Node->Pos, Node->Size);
        while(FreeNode->Next != NullNode) {
            PrevFreeNode = FreeNode;
            FreeNode = FreeNode->Next;
            if(FreeNode->AtlasIndex != Node->AtlasIndex) continue;
            
            binpacker_node *NewNode = NULL;
            v2u32 FreeNodePos2 = V2u32_Add(FreeNode->Pos, FreeNode->Size);
            if(FreeNode->Pos.Y < NodePos2.Y && Node->Pos.Y < FreeNodePos2.Y) {
                if(FreeNode->Pos.X < Node->Pos.X && Node->Pos.X < FreeNodePos2.X) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = FreeNode->AtlasIndex;
                    NewNode->Pos = (v2u32){FreeNode->Pos.X, FreeNode->Pos.Y};
                    NewNode->Size = (v2u32){Node->Pos.X - FreeNode->Pos.X, FreeNode->Size.Y};
                    NewNode->Next = FreeNode->Next;
                    FreeNode->Next = NewNode;
                } if(FreeNode->Pos.X < NodePos2.X && NodePos2.X < FreeNodePos2.X) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = FreeNode->AtlasIndex;
                    NewNode->Pos = (v2u32){NodePos2.X, FreeNode->Pos.Y};
                    NewNode->Size = (v2u32){FreeNodePos2.X - NodePos2.X, FreeNode->Size.Y};
                    NewNode->Next = FreeNode->Next;
                    FreeNode->Next = NewNode;
                }
            } if(FreeNode->Pos.X < NodePos2.X && Node->Pos.X < FreeNodePos2.X) {
                if(FreeNode->Pos.Y < Node->Pos.Y && Node->Pos.Y < FreeNodePos2.Y) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = FreeNode->AtlasIndex;
                    NewNode->Pos = (v2u32){FreeNode->Pos.X, FreeNode->Pos.Y};
                    NewNode->Size = (v2u32){FreeNode->Size.X, Node->Pos.Y - FreeNode->Pos.Y};
                    NewNode->Next = FreeNode->Next;
                    FreeNode->Next = NewNode;
                } if(FreeNode->Pos.Y < NodePos2.Y && NodePos2.Y < FreeNodePos2.Y) {
                    NewNode = Stack_Allocate(sizeof(binpacker_node));
                    NewNode->AtlasIndex = FreeNode->AtlasIndex;
                    NewNode->Pos = (v2u32){FreeNode->Pos.X, NodePos2.Y};
                    NewNode->Size = (v2u32){FreeNode->Size.X, FreeNodePos2.Y - NodePos2.Y};
                    NewNode->Next = FreeNode->Next;
                    FreeNode->Next = NewNode;
                }
            }
            
            if(NewNode) PrevFreeNode->Next = FreeNode->Next;
        }
        
        // Remove redudant nodes
        binpacker_node *Node1 = NullNode->Next;
        while(Node1 != NullNode) {
            v2u32 Node1Pos2 = V2u32_Add(Node1->Pos, Node1->Size);
            binpacker_node *PrevNode2;
            binpacker_node *Node2 = NullNode;
            while(Node2->Next != NullNode) {
                PrevNode2 = Node2;
                Node2 = Node2->Next;
                v2u32 Node2Pos2 = V2u32_Add(Node2->Pos, Node2->Size);
                if(Node1 == Node2) continue;
                if(Node1->AtlasIndex != Node2->AtlasIndex) continue;
                if(Node1->Pos.X <= Node2->Pos.X && Node2Pos2.X <= Node1Pos2.X && Node1->Pos.Y <= Node2->Pos.Y && Node2Pos2.Y <= Node1Pos2.Y) {
                    PrevNode2->Next = Node2->Next;
                }
            }
            Node1 = Node1->Next;
        }
    }
    
    #if 0
    u32 NodeCount = 0;
    Node = NullNode;
    while(Node->Next != NullNode) {
        Node = Node->Next;
        NodeCount++;
        for(u32 Y = 0; Y < Node->Size.Y; Y++) {
            for(u32 X = 0; X < Node->Size.X; X++) {
                v4u08 *Atlas = (v4u08*)(Atlases->Data + Node->AtlasIndex*AtlasSize);
                v4u08 *Pixel = Atlas + INDEX_2D(X+Node->Pos.X, Y+Node->Pos.Y, AtlasDims.X);
                *Pixel = (v4u08){0, 0, 255, 255};
            }
        }
    }
    string NodeCountStr = U32_ToString(NodeCount, 10);
    Error(NodeCountStr.Text);
    #endif
    
    // assetpack_font *FontDef = (assetpack_font*)Asset;
    // FontDef->AdvanceY = Ascent - Descent + LineGap;
    // ((assetpack_tag_registry*)Registries->Data)[1].ID = TAG_FONT_DEF;
    // ((assetpack_tag_registry*)Registries->Data)[1].Type = TYPE_U32;
    // ((assetpack_tag_registry*)Registries->Data)[1].TagCount = 1;
    // ((assetpack_tag_registry*)Registries->Data)[1].Tags = (u08*)Tag - Tags->Data;
    // Tag->ValueI = 1;
    // Tag->AssetCount = 1;
    // Tag->Assets[0] = (u08*)FontDef - Assets->Data;
    // (u08*)Tag += sizeof(assetpack_tag) + sizeof(assetpack_asset*);
    
    
    
    
    
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
    Platform_OpenFile(&FileHandle, FileName, FILE_WRITE);
    Platform_WriteFile(FileHandle, &BitmapHeader, sizeof(bitmap_header), 0);
    Platform_WriteFile(FileHandle, Atlases->Data, Atlases->Size, sizeof(bitmap_header));
    Platform_CloseFile(FileHandle);
    
    
    // file_handle FileHandle;
    // Platform_OpenFile(&FileHandle, FileName, FILE_WRITE);
    // u64 Offset = 0;
    // Platform_WriteFile(FileHandle, &Header, sizeof(assetpack_header), Offset);
    // Offset += sizeof(assetpack_header);
    // Platform_WriteFile(FileHandle, Registries->Data, Registries->Size, Offset);
    // Offset += Registries->Size;
    // Platform_WriteFile(FileHandle, Tags->Data, Tags->Size, Offset);
    // Offset += Tags->Size;
    // Platform_WriteFile(FileHandle, TagData->Data, TagData->Size, Offset);
    // Offset += TagData->Size;
    // Platform_WriteFile(FileHandle, Assets->Data, Assets->Size, Offset);
    // Offset += Assets->Size;
    // for(u32 I = 0; I < AtlasCount; I++) {
    //     Platform_WriteFile(FileHandle, Atlases[I]->Data, Atlases[I]->Size, Offset);
    //     Offset += Atlases[I]->Size;
    // }
    // Platform_CloseFile(FileHandle);
    
    Heap_Free(Atlases);
    Heap_Free(Assets);
    Heap_Free(TagData);
    Heap_Free(Tags);
    Heap_Free(Registries);
    
    Stack_Pop();
}