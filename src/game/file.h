/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal string File_Read(c08 *FileName, u64 Length, u64 Offset);

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



#define TAG_TYPES \
    ENUM(TAG_INVALID,       TYPE_NONE) \
    ENUM(TAG_CODEPOINT,     TYPE_U32) \
    ENUM(TAG_BLOCK_TEXTURE, TYPE_U32) \
    ENUM(TAG_UI_TEXTURE,    TYPE_U32) \
    ENUM(TAG_ATLAS,         TYPE_VPTR) \
    ENUM(TAG_FONT_NAME,     TYPE_STR) \

typedef enum assetpack_tag_id {
    #define ENUM(Name, Type) \
        Name,
    TAG_TYPES
    #undef ENUM
    
    TAG_Count
} assetpack_tag_id;

// global type TagTypes[] = {
//     #define ENUM(Name, Type) \
//         Type,
//     TAG_TYPES
//     #undef ENUM
// };



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

internal assetpack_tag *Assetpack_FindFirstTag(assetpack Assetpack, assetpack_tag_id TagID);