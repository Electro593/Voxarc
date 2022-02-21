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
    TAG_CODEPOINT,
    TAG_ATLAS_DEF,
    TAG_FONT_DEF,
} assetpack_tag_id;

#pragma pack(push, 1)
typedef struct assetpack_header {
    u32 TagRegistryCount;
    u32 TagCount;
    u32 TagDataSize;
    u32 AssetCount;
    u32 AssetDataSize;
} assetpack_header;

typedef struct assetpack_texture {
    u64 DataOffset;
    v2s32 Bearing;
    v2u32 Size;
    s32 AdvanceX;
    u32 AtlasIndex;
    b08 IsRotated;
} assetpack_texture;

typedef struct assetpack_font {
    s32 AdvanceY;
} assetpack_font;

typedef struct assetpack_atlas {
    vptr Data;
    v2u32 Size;
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
    assetpack_asset *Assets[];
} assetpack_tag;

typedef struct assetpack_tag_registry {
    assetpack_tag_id ID;
    type Type;
    u32 TagCount;
    assetpack_tag *Tags;
} assetpack_tag_registry;

typedef struct assetpack {
    assetpack_header *Header;
    assetpack_tag_registry *Registries;
    assetpack_tag *Tags;
    vptr TagData;
    assetpack_asset *Assets;
    u08 *AssetData;
} assetpack;
#pragma pack(pop)

typedef struct asset_node {
    v2u32 Size;
    struct asset_node *Next;
} asset_node;

typedef struct binpacker_node {
    v2u32 Pos;
    v2u32 Size;
    u32 AtlasIndex;
    struct binpacker_node *Next;
} binpacker_node;

#define BINPACKER_USE_BSSF 1
#define BINPACKER_USE_ROTATION 1
#define BINPACKER_USE_BBF 1
#define BINPACKER_USE_GLOBAL 1