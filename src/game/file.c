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

#define Assetpack_FindExactTag(Assetpack, TagID, Value) _Assetpack_FindExactTag(Assetpack, TagID, FORCE_CAST(vptr, Value))
internal assetpack_tag *
_Assetpack_FindExactTag(assetpack Assetpack,
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
                     heap *Heap)
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
    
    u08 *FontData = File_Read("assets\\fonts\\arial.ttf", 0, 0).Text;
    font Font = Font_Init(FontData);
    
    r32 Scale = 60.0f / (Font.hhea->Ascent - Font.hhea->Descent);
    s32 Ascent  = (s32)((r32)Font.hhea->Ascent  * Scale);
    s32 Descent = (s32)((r32)Font.hhea->Descent * Scale);
    s32 LineGap = (s32)((r32)Font.hhea->LineGap * Scale);
    
    Header.AssetCount = 127-32;
    Assets = Heap_Allocate(Heap, (127-32)*sizeof(assetpack_texture));
    
    Header.TagCount = Header.AssetCount+1;
    Tags = Heap_Allocate(Heap, Header.TagCount*sizeof(assetpack_tag) + Header.AssetCount*sizeof(assetpack_asset*));
    
    Header.TagDataSize = sizeof(assetpack_atlas) + Header.AssetCount*sizeof(assetpack_asset*);
    TagData = Heap_Allocate(Heap, Header.TagDataSize);
    vptr TagDataCursor = TagData->Data;
    
    Header.RegistryCount = 2;
    Registries = Heap_Allocate(Heap, Header.RegistryCount*sizeof(assetpack_registry));
    ((assetpack_registry*)Registries->Data)[0].ID = TAG_CODEPOINT;
    ((assetpack_registry*)Registries->Data)[0].Type = TYPE_U32;
    ((assetpack_registry*)Registries->Data)[0].TagCount = Header.AssetCount;
    ((assetpack_registry*)Registries->Data)[0].Tags = 0;
    
    v2u32 AtlasDims = {512, 512};
    u32 AtlasSize = AtlasDims.X*AtlasDims.Y*sizeof(v4u08);
    u32 AtlasCount = 0;
    heap_handle *Atlases = Heap_Allocate(Heap, 0);
    
    binpacker_node *NullNode = Stack_Allocate(sizeof(binpacker_node));
    NullNode->AtlasIndex = 0;
    NullNode->Pos = (v2u32){0};
    NullNode->Size = (v2u32){0};
    NullNode->Next = NullNode;
    
    asset_node *NullAssetNode = Stack_Allocate(sizeof(asset_node));
    NullAssetNode->Asset = NULL;
    NullAssetNode->Next = NullAssetNode;
    NullAssetNode->Prev = NullAssetNode;
    
    assetpack_texture *Asset = (assetpack_texture*)Assets->Data;
    assetpack_tag *Tag = (assetpack_tag*)Tags->Data;
    for(c08 Codepoint = ' '; Codepoint <= '~'; Codepoint++) {
        u32 GlyphIndex = Font_GetGlyphIndex(Font, Codepoint);
        if(GlyphIndex == 0) continue;
        
        font_glyph Glyph = Font_GetGlyph(Font, Codepoint, Scale);
        
        Asset->AdvanceX = Glyph.Advance;
        Asset->Bearing = Glyph.Bearing;
        Asset->Size = Glyph.Size;
        
        Tag->ValueI = Codepoint;
        Tag->AssetCount = 1;
        Tag->Assets = (vptr)((u08*)TagDataCursor - (u64)TagData->Data);
        *(vptr*)TagDataCursor = (u08*)Asset - (u64)Assets->Data;
        (u08*)TagDataCursor += sizeof(assetpack_asset*);
        
        if(Glyph.Shape.ContourCount == 0) {
            Asset->Pos = (v2u32){0};
            Asset->AtlasIndex = 0;
            Asset->IsRotated = FALSE;
        } else {
            asset_node *AssetNode = Stack_Allocate(sizeof(asset_node));
            AssetNode->Glyph = Glyph;
            AssetNode->Size = (v2u32){Asset->Size.X+Padding, Asset->Size.Y+Padding};
            AssetNode->Asset = Asset;
            AssetNode->Prev = NullAssetNode;
            AssetNode->Next = NullAssetNode->Next;
            AssetNode->Prev->Next = AssetNode;
            AssetNode->Next->Prev = AssetNode;
        }
        
        Asset++;
        Tag++;
    }
    
    binpacker_node *BestNode, *Node;
    asset_node *BestAssetNode=NULL, *AssetNode;
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
            Node = Node->Next;
            
            AssetNode = NullAssetNode;
            while(AssetNode->Next != NullAssetNode) {
                AssetNode = AssetNode->Next;
                Asset = AssetNode->Asset;
                
                u32 ShortSideN = U32_MAX;
                u32 ShortSideR = U32_MAX;
                u32 LongSideN = U32_MAX;
                u32 LongSideR = U32_MAX;
                if(AssetNode->Size.X < Node->Size.X && AssetNode->Size.Y < Node->Size.Y) {
                    v2u32 SizeDiff = V2u32_Sub(Node->Size, AssetNode->Size);
                    ShortSideN = MIN(SizeDiff.X, SizeDiff.Y);
                    LongSideN = MAX(SizeDiff.X, SizeDiff.Y);
                }
                if(AssetNode->Size.Y < Node->Size.X && AssetNode->Size.X < Node->Size.Y) {
                    v2u32 AssetSize = (v2u32){AssetNode->Size.Y, AssetNode->Size.X};
                    v2u32 SizeDiff = V2u32_Sub(Node->Size, AssetSize);
                    // ShortSideR = MIN(SizeDiff.X, SizeDiff.Y);
                    // LongSideR = MAX(SizeDiff.X, SizeDiff.Y);
                }
                
                u32 ShortSide = MIN(ShortSideN, ShortSideR);
                u32 LongSide = MIN(LongSideN, LongSideR);
                if(ShortSide < MinShortSide) {
                    BestNode = Node;
                    BestAssetNode = AssetNode;
                    MinShortSide = ShortSide;
                    IsRotated = ShortSide != ShortSideN;
                } else if(ShortSide == MinShortSide && LongSide < MinLongSide) {
                    BestNode = Node;
                    BestAssetNode = AssetNode;
                    MinLongSide = LongSide;
                    IsRotated = LongSide != LongSideN;
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
                Mem_Set(Atlases->Data+AtlasSize*AtlasCount, 0, AtlasSize);
                AtlasCount++;
            }
        } while(Node->Next != NullNode);
        Node = BestNode;
        AssetNode = BestAssetNode;
        AssetNode->Prev->Next = AssetNode->Next;
        AssetNode->Next->Prev = AssetNode->Prev;
        Asset = AssetNode->Asset;
        Asset->IsRotated = IsRotated;
        Asset->AtlasIndex = Node->AtlasIndex;
        
        // Place the texture
        if(IsRotated) {
            SWAP(Asset->Size.X, Asset->Size.Y, u32);
            SWAP(AssetNode->Size.X, AssetNode->Size.Y, u32);
        }
        v4u08 *Bitmap = (v4u08*)(Atlases->Data + Node->AtlasIndex*AtlasSize);
        Bitmap += INDEX_2D(Node->Pos.X, Node->Pos.Y, AtlasDims.X);
        MSDF_DrawShape(AssetNode->Glyph.Shape, Scale, Asset->Size, Bitmap, Node->Pos, Node->AtlasIndex, AtlasDims);
        Asset->Pos = Node->Pos;
        
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
    
    ((assetpack_registry*)Registries->Data)[1].ID = TAG_ATLAS_DESCRIPTOR;
    ((assetpack_registry*)Registries->Data)[1].Type = TYPE_VPTR;
    ((assetpack_registry*)Registries->Data)[1].TagCount = 1;
    ((assetpack_registry*)Registries->Data)[1].Tags = (assetpack_tag*)((u08*)Tag - Tags->Data);
    Tag->ValueP = (u08*)TagDataCursor - (u64)TagData->Data;
    Tag->AssetCount = 0;
    assetpack_atlas *AtlasDescriptor = (assetpack_atlas*)TagDataCursor;
    (u08*)TagDataCursor += sizeof(assetpack_atlas);
    AtlasDescriptor->DataOffset = 0;
    AtlasDescriptor->Size = AtlasDims;
    AtlasDescriptor->Count = AtlasCount;
    
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
    
    Heap_Free(Atlases);
    Heap_Free(Assets);
    Heap_Free(TagData);
    Heap_Free(Tags);
    Heap_Free(Registries);
    
    Stack_Pop();
}