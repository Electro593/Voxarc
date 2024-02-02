/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef INCLUDE_HEADER

typedef enum asset_type {
   ASSET_INVALID, // Should only be for the null node
   ASSET_TEXTURE,
   ASSET_GLYPH,
   ASSET_FONT,
   ASSET_ATLAS,
} asset_type;



#define GUI_TEXTURE_PREFIX "assets\\gui\\"

#define GUI_TEXTURES \
   ENUM(BLOCK_HIGHLIGHT, block_highlight) \

typedef enum gui_texture {
   #define ENUM(EnumName, FileName) \
      GUI_TEXTURE_##EnumName,
   GUI_TEXTURES
   #undef ENUM
   
   GUI_TEXTURE_Count
} gui_texture;

global c08 *GUITexturePaths[GUI_TEXTURE_Count] = {
   #define ENUM(EnumName, FileName) \
      GUI_TEXTURE_PREFIX #FileName ".bmp",
   GUI_TEXTURES
   #undef ENUM
};



typedef enum font_style {
   FONT_MONOSPACE,
} font_style;

typedef enum assetpack_tag_id {
   TAG_INVALID,
   TAG_CODEPOINT,
   TAG_BLOCK_TEXTURE,
   TAG_UI_TEXTURE,
   TAG_ATLAS_DESC,
   TAG_FONT_DESC,
   TAG_FONT_NAME,
   TAG_FONT_STYLE,
   
   TAG_Count
} assetpack_tag_id;




#pragma pack(push, 1)
typedef struct bitmap_header {
   c08 Signature[2];
   u32 FileSize;
   u32 Reserved;
   u32 DataOffset;
   u32 Size;
   u32 Width;
   u32 Height;
   u16 Planes;
   u16 BitsPerPixel;
   u32 Compression;
   u32 ImageSize;
   u32 XPixelsPerM;
   u32 YPixelsPerM;
   u32 ColorsUsed;
   u32 ImportantColors;
} bitmap_header;

typedef struct assetpack_header {
   u32 RegistryCount;
   u32 TagCount;
   u32 TagDataSize;
   u32 AssetsSize;
   u32 AssetCount;
   u32 AssetDataOffset;
   u32 AssetDataSize;
} assetpack_header;

typedef struct assetpack_glyph {
   r32 AdvanceX;
   v2r32 SizeR;
   v2r32 Bearing;
} assetpack_glyph;

typedef struct assetpack_font {
   r32 Ascent;
   r32 Descent;
   r32 LineGap;
} assetpack_font;

typedef struct assetpack_atlas {
   u64 DataOffset;
   v2u32 Size;
   u32 Count;
} assetpack_atlas;

//NOTE: sizeof(assetpack_asset) MUST be a multiple of 4
//TODO: Shouldn't AtlasIndex be specific to glyph and texture?
//TODO: Actually implement the hash
typedef struct assetpack_asset {
   struct assetpack_asset_header {
      v2u32 Pos;
      v2u32 Size;
      u16 AtlasIndex;
      u16 _Unused;
   };
   union {
      assetpack_glyph Glyph;
      assetpack_font  Font;
      assetpack_atlas Atlas;
   };
} assetpack_asset;

typedef struct assetpack_tag {
   union {
      u64 ValueI;
      r64 ValueR;
      vptr ValueP;
   };
   u32 AssetCount;
   union {
      assetpack_asset **Assets;
      HEAP(assetpack_asset*) AssetsGen;
   };
} assetpack_tag;

typedef struct assetpack_registry {
   assetpack_tag_id ID;
   type Type;
   u32 TagCount;
   union {
      assetpack_tag *Tags;
      HEAP(assetpack_tag) TagsGen;
   };
} assetpack_registry;

typedef struct assetpack {
   assetpack_header *Header;
   assetpack_registry *Registries;
   assetpack_tag *Tags;
   u08 *TagData;
   assetpack_asset *Assets;
   u08 *AssetData;
   
   // The heap handle for the actual file in memory
   heap_handle *FileDataHandle;
} assetpack;

typedef struct asset_node {
   asset_type Type;
   font_glyph Glyph;
   file_handle FileHandle;
   u16 BitmapOffset;
   v2u32 Size;
   assetpack_asset *Asset;
   struct asset_node *Prev;
   struct asset_node *Next;
} asset_node;

typedef struct binpacker_node {
   v2u32 Pos;
   v2u32 Size;
   u32 AtlasIndex;
   struct binpacker_node *Prev;
   struct binpacker_node *Next;
} binpacker_node;

typedef struct assetpack_gen {
   assetpack_header Header;
   HEAP(assetpack_registry) Registries;
   HEAP(assetpack_tag) Tags;
   HEAP(u08) TagData;
   HEAP(assetpack_asset) Assets;
   HEAP(u08) AssetData;
   
   u32 RegistryMap[TAG_Count];
   
   v2u32 Padding;
   v2u32 AtlasDims;
   u32 AtlasCount;
   u32 AtlasOffset;
   asset_node *NullAssetNode;
   binpacker_node *NullPackerNode;
} assetpack_gen;
#pragma pack(pop)

#define GAME_FILE_FUNCS \
   EXPORT(assetpack_asset*, FindFirstAssetFromExactTag, assetpack Assetpack, assetpack_tag_id TagID, vptr TagValue) \

#endif



#ifdef INCLUDE_SOURCE

internal s08
CompareAssetpackTagString(
   vptr A,
   vptr B,
   vptr Param)
{
   u64 Unserialize = *(u64*)Param;
   string *AStr = (vptr)(*(u08**)A + Unserialize);
   string *BStr = *(string**)B;
   c08 *AText = AStr->Text + Unserialize;
   u32 Len = MIN(AStr->Length, BStr->Length);
   return Mem_Cmp(AText, BStr->Text, Len);
}

internal assetpack_registry *
FindTagRegistry(
   assetpack Assetpack,
   assetpack_tag_id TagID)
{
   type Type = MakeMemberType(TYPEID_U32, 0, sizeof(assetpack_registry));
   return BinarySearchArray(&Assetpack.Registries, 0, Assetpack.Header->RegistryCount, &TagID, Type, NULL, NULL, NULL);
}

internal assetpack_tag *
FindTagEntry(
   assetpack Assetpack,
   assetpack_tag_id TagID,
   vptr TagValue)
{
   assetpack_registry *Registry = FindTagRegistry(Assetpack, TagID);
   if(!Registry) return NULL;
   
   cmp_func *Func = NULL;
   u64 Param = 0;
   type Type = MakeMemberType(Registry->Type.ID, 0, sizeof(assetpack_tag));
   if(Registry->Type.ID == TYPEID_STR)
      Func = CompareAssetpackTagString;
   
   return BinarySearchArray(&Registry->Tags, 0, Registry->TagCount, TagValue, Type, Func, &Param, NULL);
}

internal assetpack_asset **
FindAssetsFromPartialTags(
   assetpack Assetpack,
   u32 TypeCount,
   assetpack_tag_id *TagIDs,
   vptr *TagValues,
   u32 *MatchedCountOut)
{
   if(TypeCount == 0) {
      if(MatchedCountOut) *MatchedCountOut = Assetpack.Header->AssetCount;
      return &Assetpack.Assets;
   } else if(TypeCount == 1) {
      assetpack_tag *Entry = FindTagEntry(Assetpack, TagIDs[0], TagValues[0]);
      if(MatchedCountOut) *MatchedCountOut = Entry->AssetCount;
      return Entry->Assets;
   } else {
   	// Get the tag entries for each registry.
   	assetpack_tag **SelectedTags = Stack_Allocate(TypeCount * sizeof(assetpack_tag*));
   	for(u32 I = 0; I < TypeCount; I++) {
   		SelectedTags[I] = FindTagEntry(Assetpack, TagIDs[I], TagValues[I]);
   	}
   	
   	// Find the tag with the fewest assets.
   	assetpack_tag *StartTag = SelectedTags[0];
   	for(u32 I = 1; I < TypeCount; I++) {
   		if(SelectedTags[I]->AssetCount < StartTag->AssetCount)
   			StartTag = SelectedTags[I];
   	}
   	
   	assetpack_asset **Matched = Stack_Allocate(StartTag->AssetCount * sizeof(assetpack_asset*));
   	u32 MatchedCount = 0;
   	
       // For each asset (Asset) in the first tag, if every other tag
       // contains it, it's a match.
   	for(u32 I = 0; I < StartTag->AssetCount; I++) {
   		assetpack_asset *Asset = StartTag->Assets[I];
   		
   		u32 J = 0;
   		for(; J < TypeCount; J++) {
   			assetpack_tag *CurrTag = SelectedTags[J];
   			if(CurrTag == StartTag) continue;
   			
   			assetpack_asset *Match = BinarySearchArray((vptr*)&CurrTag->Assets, 0, CurrTag->AssetCount, &Asset, TYPE_VPTR, NULL, NULL, NULL);
   			if(!Match) break;
   		}
   		
   		// Asset was in all of the other sets, so it's a match
   		if(J == TypeCount) Matched[MatchedCount++] = Asset;
   	}
   	
      if(MatchedCountOut) *MatchedCountOut = MatchedCount;
   	return Matched;
   }
}

internal assetpack_asset *
FindFirstAssetFromPartialTags(
   assetpack Assetpack,
   u32 TypeCount,
   assetpack_tag_id *TagIDs,
   vptr *TagValues)
{
   Stack_Push();
   
   u32 MatchedCount;
   assetpack_asset **Matches = FindAssetsFromPartialTags(Assetpack, TypeCount, TagIDs, TagValues, &MatchedCount);
   assetpack_asset *Result = MatchedCount ? Matches[0] : NULL;
   
   Stack_Pop();
   
   return Result;
}

internal assetpack_asset *
FindFirstAssetFromExactTags(
   assetpack Assetpack,
   u32 TypeCount,
   assetpack_tag_id *TagIDs,
   vptr *TagValues)
{
   // Since hashing isn't implemented, just do this
   return FindFirstAssetFromPartialTags(Assetpack, TypeCount, TagIDs, TagValues);
}

internal assetpack_asset *
FindFirstAssetFromExactTag(
   assetpack Assetpack,
   assetpack_tag_id TagID,
   vptr TagValue)
{
   assetpack_tag *Tag = FindTagEntry(Assetpack, TagID, TagValue);
   if(!Tag || Tag->AssetCount == 0) return NULL;
   return Tag->Assets[0];
}

internal assetpack
File_LoadAssetpack(
   c08 *FileName,
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
   Assetpack.FileDataHandle = Heap_GetHandleA(FileBase);
   u08 *FileCursor = FileBase;
   
   Assetpack.Header = (assetpack_header*)FileCursor;
   FileCursor += sizeof(assetpack_header);
   
   Assetpack.Registries = (assetpack_registry*)FileCursor;
   FileCursor += sizeof(assetpack_registry) * Assetpack.Header->RegistryCount;
   
   Assetpack.Tags = (assetpack_tag*)FileCursor;
   FileCursor += sizeof(assetpack_tag) * Assetpack.Header->TagCount;
   
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
         
         switch(Registry->Type.ID) {
            case TYPEID_VPTR: {
               (u08*)Tag->ValueP += (u64)Assetpack.TagData;
            } break;
            
            case TYPEID_STR: {
               (u08*)Tag->ValueP += (u64)Assetpack.TagData;
               string *String = (string*)Tag->ValueP;
               (u08*)String->Text += (u64)Assetpack.TagData;
            } break;
            
            case TYPEID_U32: break;
            
            default: {
               Assert(FALSE, "Unsupported type!");
            }
         }
         
         (u08*)Tag->Assets += (u64)Assetpack.TagData;
         
         for(u32 A = 0; A < Tag->AssetCount; A++)
            (u08*)(Tag->Assets[A]) += (u64)Assetpack.Assets;
      }
   }
   
   return Assetpack;
}

/* TODO
  - Verify the efficiency of the packer; there's probably a bug
  - Maybe save RegistryMap to file, so it can be used instead
    of binary searches
  - Have each asset point to an array of tag pointers in AssetData,
    so you can find the full set of tags if the query was partial or
    random
*/

internal void
BeginAssetpack(
   assetpack_gen *Assetpack,
   heap *Heap)
{
   Assetpack->Header = (assetpack_header){0};
   Assetpack->Registries = Heap_Allocate(Heap, 0);
   Assetpack->Tags = Heap_Allocate(Heap, 0);
   Assetpack->TagData = Heap_Allocate(Heap, 0);
   Assetpack->Assets = Heap_Allocate(Heap, 0);
   Assetpack->AssetData = Heap_Allocate(Heap, 0);
   
   Mem_Set(Assetpack->RegistryMap, U32_MAX, TAG_Count*sizeof(u32));
   
   Assetpack->Padding = (v2u32){1, 1};
   Assetpack->AtlasDims = (v2u32){512, 512};
   Assetpack->AtlasCount = 0;
   Assetpack->AtlasOffset = 0;
   
   Assetpack->NullAssetNode = Stack_Allocate(sizeof(asset_node));
   Assetpack->NullAssetNode->Asset = NULL;
   Assetpack->NullAssetNode->Next  = Assetpack->NullAssetNode;
   Assetpack->NullAssetNode->Prev  = Assetpack->NullAssetNode;
   
   Assetpack->NullPackerNode = Stack_Allocate(sizeof(binpacker_node));
   Assetpack->NullPackerNode->AtlasIndex = 0;
   Assetpack->NullPackerNode->Pos        = (v2u32){0};
   Assetpack->NullPackerNode->Size       = (v2u32){0};
   Assetpack->NullPackerNode->Next       = Assetpack->NullPackerNode;
}

internal void
EndAssetpack(
   assetpack_gen *Assetpack,
   c08 *FileName)
{
   assetpack_registry *Registries = Assetpack->Registries->Data;
   Heap_Resize(Assetpack->Tags, Assetpack->Header.TagCount*sizeof(assetpack_tag));
   assetpack_tag *Tags = Assetpack->Tags->Data;
   u32 TagOffset = 0;
   for(u32 I = 0; I < Assetpack->Header.RegistryCount; I++) {
      assetpack_registry *Registry = Registries+I;
      Mem_Cpy(Tags+TagOffset, Registry->TagsGen->Data, Registry->TagsGen->Size);
      Heap_Free(Registry->TagsGen);
      Registry->Tags = (vptr)(TagOffset*sizeof(assetpack_tag));
      TagOffset += Registry->TagCount;
   }
   
   u32 PrevSize = Assetpack->TagData->Size;
   Assetpack->Header.TagDataSize += PrevSize;
   Heap_Resize(Assetpack->TagData, Assetpack->Header.TagDataSize);
   u08 *TagData = (u08*)Assetpack->TagData->Data + PrevSize;
   for(u32 I = 0; I < Assetpack->Header.TagCount; I++) {
      u32 Size = sizeof(assetpack_asset*);
      Mem_Cpy(TagData, Tags[I].AssetsGen->Data, Size);
      Heap_Free(Tags[I].AssetsGen);
      Tags[I].Assets = (vptr)((u64)TagData - (u64)Assetpack->TagData->Data);
      TagData += Size;
   }
   
   u32 AtlasSize = Assetpack->AtlasDims.X*Assetpack->AtlasDims.Y*sizeof(v4u08);
   bitmap_header BitmapHeader = {0};
   BitmapHeader.Signature[0] = 'B';
   BitmapHeader.Signature[1] = 'M';
   BitmapHeader.FileSize = sizeof(bitmap_header) + AtlasSize;
   BitmapHeader.DataOffset = sizeof(bitmap_header);
   BitmapHeader.Size = 40;
   BitmapHeader.Width = Assetpack->AtlasDims.X;
   BitmapHeader.Height = Assetpack->AtlasDims.Y * Assetpack->AtlasCount;
   BitmapHeader.Planes = 1;
   BitmapHeader.BitsPerPixel = 32;
   BitmapHeader.ImageSize = AtlasSize;
   
   //TODO: This doesn't work with additional asset data
   file_handle FileHandle;
   Platform_OpenFile(&FileHandle, "assets\\pack.bmp", FILE_WRITE);
   {
      Platform_WriteFile(FileHandle, &BitmapHeader, sizeof(bitmap_header), 0);
      Platform_WriteFile(FileHandle, (u08*)Assetpack->AssetData->Data + Assetpack->AtlasOffset, AtlasSize, sizeof(bitmap_header));
   }
   Platform_CloseFile(FileHandle);
   
   Platform_OpenFile(&FileHandle, FileName, FILE_WRITE);
   {
      u64 Offset = sizeof(assetpack_header);
      
      Platform_WriteFile(FileHandle, Assetpack->Registries->Data, Assetpack->Registries->Size, Offset);
      Offset += Assetpack->Registries->Size;
      
      Platform_WriteFile(FileHandle, Assetpack->Tags->Data, Assetpack->Tags->Size, Offset);
      Offset += Assetpack->Tags->Size;
      
      Platform_WriteFile(FileHandle, Assetpack->TagData->Data, Assetpack->TagData->Size, Offset);
      Offset += Assetpack->TagData->Size;
      
      Platform_WriteFile(FileHandle, Assetpack->Assets->Data, Assetpack->Assets->Size, Offset);
      Offset += Assetpack->Assets->Size;
      
      Assetpack->Header.AssetDataOffset = Offset;
      Platform_WriteFile(FileHandle, Assetpack->AssetData->Data, Assetpack->AssetData->Size, Offset);
      
      Platform_WriteFile(FileHandle, &Assetpack->Header, sizeof(assetpack_header), 0);
   }
   Platform_CloseFile(FileHandle);
   
   Heap_Free(Assetpack->Registries);
   Heap_Free(Assetpack->Tags);
   Heap_Free(Assetpack->TagData);
   Heap_Free(Assetpack->Assets);
   Heap_Free(Assetpack->AssetData);
}

internal void
DefineRegistry(
   assetpack_gen *Assetpack,
   assetpack_tag_id TagType,
   type TagValueType)
{
   // Make sure it hasn't already been defined
   Assert(Assetpack->RegistryMap[TagType] == U32_MAX, "Registry already exists!");
   
   Heap_Resize(Assetpack->Registries, Assetpack->Registries->Size + sizeof(assetpack_registry));
   
   assetpack_registry *Registries = Assetpack->Registries->Data;
   
   u32 Index;
   u32 Count = Assetpack->Header.RegistryCount;
   type Type = MakeMemberType(TYPEID_U32, 0, sizeof(assetpack_registry));
   BinarySearchArray(&Registries, 0, Count, &TagType, Type, NULL, NULL, &Index);
   
   Mem_Cpy(Registries+Index+1, Registries+Index, (Count-Index)*sizeof(assetpack_registry));
   
   Registries[Index].ID = TagType;
   Registries[Index].Type = TagValueType;
   Registries[Index].TagCount = 0;
   Registries[Index].TagsGen = Heap_Allocate(Heap_GetHeap(Assetpack->Registries), 0);
   
   Assetpack->RegistryMap[TagType] = Index;
   for(u32 I = TagType+1; I < TAG_Count; I++) {
      if(Assetpack->RegistryMap[I] != U32_MAX)
         Assetpack->RegistryMap[I]++;
   }
   
   Assetpack->Header.RegistryCount++;
}

internal assetpack_asset *
AddAsset(
   assetpack_gen *Assetpack,
   asset_type Type)
{
   u32 Size = 0;
   switch(Type) {
      case ASSET_TEXTURE: {
         Size = sizeof(struct assetpack_asset_header);
      } break;
      
      case ASSET_GLYPH: {
         Size = sizeof(struct assetpack_asset_header) + sizeof(assetpack_glyph);
      } break;
      
      case ASSET_FONT: {
         Size = sizeof(struct assetpack_asset_header) + sizeof(assetpack_font);
      } break;
      
      case ASSET_ATLAS: {
         Size = sizeof(struct assetpack_asset_header) + sizeof(assetpack_atlas);
      } break;
      
      default: {
         Assert(FALSE, "Asset type not supported!");
      }
   }
   
   Heap_Resize(Assetpack->Assets, Assetpack->Assets->Size + Size);
   
   assetpack_asset *Assets = Assetpack->Assets->Data;
   assetpack_asset *Asset = (vptr)((u08*)Assets + Assetpack->Header.AssetsSize);
   
   Assetpack->Header.AssetsSize += Size;
   Assetpack->Header.AssetCount++;
   
   return Asset;
}

internal void
AddTag(
   assetpack_gen *Assetpack,
   assetpack_asset *Asset,
   assetpack_tag_id TagID,
   vptr TagValue)
{
   Assert(Assetpack->RegistryMap[TagID] != U32_MAX, "Registry doesn't exist!");
   assetpack_registry *Registries = Assetpack->Registries->Data;
   assetpack_registry *Registry = Registries + Assetpack->RegistryMap[TagID];
   
   // Woo... polymorphism in C. Yay.........
   // Yeah, this is a mess.
   u32 Index;
   u32 Count = Registry->TagCount;
   assetpack_tag *Tag;
   cmp_func *Func = NULL;
   vptr Param = &Assetpack->TagData->Data;
   type Type = MakeMemberType(Registry->Type.ID, 0, sizeof(assetpack_tag));
   if(Registry->Type.ID == TYPEID_STR)
      Func = CompareAssetpackTagString;
   Tag = BinarySearchArray(&Registry->TagsGen->Data, 0, Count, TagValue, Type, Func, Param, &Index);
   
   if(!Tag) {
      Heap_Resize(Registry->TagsGen, Registry->TagsGen->Size + sizeof(assetpack_tag));
      assetpack_tag *Tags = Registry->TagsGen->Data;
      Mem_Cpy(Tags+Index+1, Tags+Index, (Count-Index)*sizeof(assetpack_tag));
      
      Tag = Tags+Index;
      Tag->AssetCount = 1;
      Tag->AssetsGen = Heap_Allocate(Heap_GetHeap(Assetpack->TagData), sizeof(assetpack_asset*));
      assetpack_asset **Assets = Tag->AssetsGen->Data;
      Assets[0] = (vptr)((u64)Asset - (u64)Assetpack->Assets->Data);
      
      switch(Registry->Type.ID) {
         case TYPEID_U32: {
            Tag->ValueI = *(u32*)TagValue;
         } break;
         
         case TYPEID_STR: {
            string Str = **(string**)TagValue;
            u64 PrevSize = Assetpack->TagData->Size;
            Heap_Resize(Assetpack->TagData, PrevSize + sizeof(string) + Str.Length);
            u08 *TagData = (u08*)Assetpack->TagData->Data + PrevSize;
            Mem_Cpy(TagData+sizeof(string), Str.Text, Str.Length);
            Str.Text = TagData+sizeof(string) - (u64)Assetpack->TagData->Data;
            Str.Capacity = Str.Length;
            Mem_Cpy(TagData, &Str, sizeof(string));
            Tag->ValueI = (u64)TagData - (u64)Assetpack->TagData->Data;
         } break;
         
         case TYPEID_VPTR: {
            Tag->ValueP = *(vptr*)TagValue;
         } break;
         
         default: {
            Assert(FALSE, "Type not supported!");
         }
      }
      
      Registry->TagCount++;
      Assetpack->Header.TagCount++;
   } else {
      Count = Tag->AssetCount;
      assetpack_asset *Match = BinarySearchArray(&Tag->AssetsGen->Data, 0, Count, &Asset, TYPE_VPTR, NULL, NULL, &Index);
      Assert(!Match, "Asset has already been added to the tag!");
      Heap_Resize(Tag->AssetsGen, Tag->AssetsGen->Size + sizeof(assetpack_asset*));
      assetpack_asset **Assets = Tag->AssetsGen->Data;
      Mem_Cpy(Assets+Index+1, Assets+Index, (Count-Index)*sizeof(assetpack_asset*));
      Assets[Index] = (vptr)((u64)Asset - (u64)Assetpack->Assets->Data);
      
      Tag->AssetCount++;
   }
   
   Assetpack->Header.TagDataSize += sizeof(assetpack_asset*);
}

internal void
AddToAtlas(
   assetpack_gen *Assetpack,
   assetpack_asset *Asset,
   asset_type Type,
   font_glyph *Glyph,
   file_handle FileHandle,
   u32 BitmapOffset)
{
   asset_node *AssetNode = Stack_Allocate(sizeof(asset_node));
   AssetNode->Type = Type;
   if(Glyph)
      AssetNode->Glyph = *Glyph;
   AssetNode->FileHandle = FileHandle;
   AssetNode->BitmapOffset = BitmapOffset;
   AssetNode->Size = (v2u32){
      Asset->Size.X + Assetpack->Padding.X,
      Asset->Size.Y + Assetpack->Padding.Y
   };
   AssetNode->Asset = (vptr)((u64)Asset - (u64)Assetpack->Assets->Data);
   AssetNode->Prev = Assetpack->NullAssetNode;
   AssetNode->Next = Assetpack->NullAssetNode->Next;
   AssetNode->Prev->Next = AssetNode;
   AssetNode->Next->Prev = AssetNode;
}

internal void
MakeAtlas(
   assetpack_gen *Assetpack,
   v4u08 BorderColor)
{
   asset_node *NullAssetNode = Assetpack->NullAssetNode;
   binpacker_node *NullNode = Assetpack->NullPackerNode;
   v2u32 Padding = Assetpack->Padding;
   
   u32 AtlasCount = 0;
   v2u32 AtlasDims = Assetpack->AtlasDims;
   u32 AtlasSize = AtlasDims.X * AtlasDims.Y * sizeof(v4u08);
   u32 AtlasesOffset = Assetpack->AssetData->Size;
   u08 *Atlases = (u08*)Assetpack->AssetData->Data + AtlasesOffset;
   
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
            NewNode->Pos = Padding;
            Assert(AssetNode->Size.X < AtlasDims.X && AssetNode->Size.Y < AtlasDims.Y);
            NewNode->Size = V2u32_Sub(AtlasDims, Padding);
            NewNode->AtlasIndex = AtlasCount;
            NewNode->Next = Node->Next;
            NewNode->Prev = Node;
            NewNode->Next->Prev = NewNode;
            NewNode->Prev->Next = NewNode;
            
            Heap_Resize(Assetpack->AssetData, Assetpack->AssetData->Size + AtlasSize);
            Atlases = (u08*)Assetpack->AssetData->Data + AtlasesOffset;
            v4u08 *Bitmap = (v4u08*)(Atlases + AtlasSize*AtlasCount);
            
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
      
      // Asset was serialized since its handle was probably resized,
      // so we need to turn it back into a pointer.
      assetpack_asset *Asset = (vptr)((u64)AssetNode->Asset + (u64)Assetpack->Assets->Data);
      Asset->AtlasIndex = Node->AtlasIndex;
      
      v4u08 *Bitmap = (v4u08*)(Atlases + Node->AtlasIndex*AtlasSize);
      Bitmap += INDEX_2D(Node->Pos.X, Node->Pos.Y, AtlasDims.X);
      Asset->Pos = Node->Pos;
      
      Stack_Push();
      switch(AssetNode->Type) {
         case ASSET_GLYPH: {
            MSDF_DrawShape(AssetNode->Glyph.Shape, &Asset->Pos, &Asset->Size, Bitmap, Node->Pos, Node->AtlasIndex, AtlasDims);
         } break;
         
         case ASSET_TEXTURE: {
            u32 Size = Asset->Size.X*Asset->Size.Y*sizeof(v4u08);
            v4u08 *Src = Stack_Allocate(Size);
            //TODO: Async read?
            Platform_ReadFile(AssetNode->FileHandle, Src, Size, AssetNode->BitmapOffset);
            for(u32 Y = 0; Y < Asset->Size.Y; Y++) {
               for(u32 X = 0; X < Asset->Size.X; X++) {
                  v4u08 Color = {Src->Z, Src->Y, Src->X, 255};
                  Bitmap[INDEX_2D(X, Y, AtlasDims.X)] = Color;
                  Src++;
               }
            }
            Platform_CloseFile(AssetNode->FileHandle);
         } break;
         
         default: {
            Assert(FALSE, "Unsupported asset type!");
         }
      }
      Stack_Pop();
      
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
            }
            
            if(Node->Pos.X < Pos2.X && Pos2.X < NodePos2.X) {
               NewNode = Stack_Allocate(sizeof(binpacker_node));
               NewNode->AtlasIndex = Node->AtlasIndex;
               NewNode->Prev = Node;
               NewNode->Next = Node->Next;
               NewNode->Prev->Next = NewNode;
               NewNode->Next->Prev = NewNode;
               NewNode->Pos = (v2u32){Pos2.X, Node->Pos.Y};
               NewNode->Size = (v2u32){NodePos2.X - Pos2.X, Node->Size.Y};
            }
         }
         
         if(Node->Pos.X < Pos2.X && Pos.X < NodePos2.X) {
             if(Node->Pos.Y < Pos.Y && Pos.Y < NodePos2.Y) {
                NewNode = Stack_Allocate(sizeof(binpacker_node));
                NewNode->AtlasIndex = Node->AtlasIndex;
                NewNode->Prev = Node;
                NewNode->Next = Node->Next;
                NewNode->Prev->Next = NewNode;
                NewNode->Next->Prev = NewNode;
                NewNode->Pos = Node->Pos;
                NewNode->Size = (v2u32){Node->Size.X, Pos.Y - Node->Pos.Y};
             }
             
             if(Node->Pos.Y < Pos2.Y && Pos2.Y < NodePos2.Y) {
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
   
   assetpack_asset *Asset = AddAsset(Assetpack, ASSET_ATLAS);
   Asset->Atlas.DataOffset = 0;
   Asset->Atlas.Size       = AtlasDims;
   Asset->Atlas.Count      = AtlasCount;
   AddTag(Assetpack, Asset, TAG_ATLAS_DESC, &(u32){0});
   
   Assetpack->AtlasCount = AtlasCount;
   Assetpack->AtlasOffset = AtlasesOffset;
   Assetpack->Header.AssetDataSize = Assetpack->AssetData->Size;
}

internal void
File_CreateAssetpack(
   c08 *FileName,
   heap *Heap,
   r32 FontSize)
{
   Stack_Push();
   
   assetpack_gen Assetpack;
   
   
   font_style FontStyle = FONT_MONOSPACE;
   string _FontName = CString("arial");
   string *FontName = &_FontName;
   u08 *FontData = File_Read("assets\\fonts\\arial.ttf", 0, 0).Text;
   font Font = Font_Init(FontData);
   
   r32 UnitScale = 1.0f/(Font.hhea->Ascent-Font.hhea->Descent);
   
   
   BeginAssetpack(&Assetpack, Heap);
   {
      DefineRegistry(&Assetpack, TAG_FONT_STYLE,    TYPE_U32);
      DefineRegistry(&Assetpack, TAG_FONT_NAME,     TYPE_STR);
      DefineRegistry(&Assetpack, TAG_FONT_DESC,     TYPE_U32);
      DefineRegistry(&Assetpack, TAG_ATLAS_DESC,    TYPE_U32);
      DefineRegistry(&Assetpack, TAG_CODEPOINT,     TYPE_U32);
      DefineRegistry(&Assetpack, TAG_UI_TEXTURE,    TYPE_U32);
      DefineRegistry(&Assetpack, TAG_BLOCK_TEXTURE, TYPE_U32);
      
      assetpack_asset *Asset;
      
      Asset = AddAsset(&Assetpack, ASSET_FONT);
      Asset->Font.Ascent  = Font.hhea->Ascent  *  UnitScale;
      Asset->Font.Descent = Font.hhea->Descent *  UnitScale;
      Asset->Font.LineGap = Font.hhea->LineGap *  UnitScale;
      AddTag(&Assetpack, Asset, TAG_FONT_NAME, &FontName);
      AddTag(&Assetpack, Asset, TAG_FONT_STYLE, &FontStyle);
      AddTag(&Assetpack, Asset, TAG_FONT_DESC, &(u32){0});
      
      for(u32 Codepoint = ' '; Codepoint <= '~'; Codepoint++) {
         u32 GlyphIndex = Font_GetGlyphIndex(Font, Codepoint);
         if(GlyphIndex == 0) continue;
         
         font_glyph Glyph = Font_GetGlyph(Font, Codepoint, UnitScale);
         
         Asset = AddAsset(&Assetpack, ASSET_GLYPH);
         Asset->Size = (v2u32){Glyph.Size.X*FontSize, Glyph.Size.Y*FontSize};
         Asset->Glyph.AdvanceX = Glyph.Advance;
         Asset->Glyph.Bearing = Glyph.Bearing;
         Asset->Glyph.SizeR = Glyph.Size;
         
         if(Codepoint != ' ')
             AddToAtlas(&Assetpack, Asset, ASSET_GLYPH, &Glyph, 0, 0);
         
         AddTag(&Assetpack, Asset, TAG_CODEPOINT, &Codepoint);
         AddTag(&Assetpack, Asset, TAG_FONT_NAME, &FontName);
         AddTag(&Assetpack, Asset, TAG_FONT_STYLE, &FontStyle);
      }
      
      for(u32 I = 1; I < BLOCK_Count; I++) {
         bitmap_header BitmapHeader;
         file_handle Handle;
         Platform_OpenFile(&Handle, BlockTexturePaths[I], FILE_READ);
         Platform_ReadFile(Handle, &BitmapHeader, sizeof(bitmap_header), 0);
         
         Asset = AddAsset(&Assetpack, ASSET_TEXTURE);
         Asset->Size = (v2u32){BitmapHeader.Width, BitmapHeader.Height};
         
         u32 HandleIndex = 0;
         AddToAtlas(&Assetpack, Asset, ASSET_TEXTURE, NULL, Handle, BitmapHeader.DataOffset);
         
         AddTag(&Assetpack, Asset, TAG_BLOCK_TEXTURE, &I);
      }
      
      for(u32 I = 0; I < GUI_TEXTURE_Count; I++) {
         bitmap_header BitmapHeader;
         file_handle Handle;
         Platform_OpenFile(&Handle, GUITexturePaths[I], FILE_READ);
         Platform_ReadFile(Handle, &BitmapHeader, sizeof(bitmap_header), 0);
         
         Asset = AddAsset(&Assetpack, ASSET_TEXTURE);
         Asset->Size = (v2u32){BitmapHeader.Width, BitmapHeader.Height};
         
         u32 HandleIndex = 0;
         AddToAtlas(&Assetpack, Asset, ASSET_TEXTURE, NULL, Handle, BitmapHeader.DataOffset);
         
         AddTag(&Assetpack, Asset, TAG_UI_TEXTURE, &I);
      }
      
      v4u08 BorderColor = {0, 0, 0, 255};
      MakeAtlas(&Assetpack, BorderColor);
   }
   EndAssetpack(&Assetpack, FileName);
   
   Stack_Pop();
}

#endif