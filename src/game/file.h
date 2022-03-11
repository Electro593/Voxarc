/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal string File_Read(c08 *FileName, u64 Length, u64 Offset);

typedef enum assetpack_tag_id {
    TAG_CODEPOINT=1,
    TAG_ATLAS_DESCRIPTOR,
    TAG_FONT_DEF,
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
    u32 TagDataOffset;
    u32 TagDataSize;
    u32 AssetCount;
    u32 AssetDataOffset;
    u32 AssetDataSize;
} assetpack_header;

typedef struct assetpack_texture {
    v2u32 Pos;
    v2u32 Size;
    v2r32 SizeR;
    v2r32 Bearing;
    r32 AdvanceX;
    u16 AtlasIndex;
    b08 IsRotated;
    u08 _Unused[1];
} assetpack_texture;

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

typedef union assetpack_asset {
    assetpack_texture Texture;
    assetpack_font Font;
} assetpack_asset;

typedef struct assetpack_tag {
    union {
        u64 ValueI;
        r64 ValueR;
        vptr ValueP;
    };
    u32 AssetCount;
    assetpack_asset **Assets;
} assetpack_tag;

typedef struct assetpack_registry {
    assetpack_tag_id ID;
    type Type;
    u32 TagCount;
    assetpack_tag *Tags;
} assetpack_registry;

typedef struct assetpack {
    assetpack_header *Header;
    assetpack_registry *Registries;
    assetpack_tag *Tags;
    u08 *TagData;
    assetpack_asset *Assets;
    u08 *AssetData;
    
    heap_handle *FileDataHandle;
} assetpack;
#pragma pack(pop)

typedef struct asset_node {
    font_glyph Glyph;
    v2u32 Size;
    assetpack_texture *Asset;
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

internal assetpack_tag *Assetpack_FindFirstTag(assetpack Assetpack, assetpack_tag_id TagID);