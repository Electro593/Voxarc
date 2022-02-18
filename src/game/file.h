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
} assetpack_tag_id;

typedef struct assetpack_header {
    u32 TagRegistryCount;
    u32 TagCount;
    u32 TagDataSize;
    u32 AssetCount;
    u32 AtlasCount;
} assetpack_header;

typedef struct assetpack_texture {
    u64 DataOffset;
    v2u32 Size;
    u32 AtlasIndex;
} assetpack_texture;

typedef struct assetpack_font {
    s32 AdvanceY;
} assetpack_font;

typedef union assetpack_asset {
    assetpack_texture Texture;
    assetpack_font Font;
} assetpack_asset;

typedef struct assetpack_tag {
    vptr Value; // Points to elsewhere if size > 8
    u32 AssetCount;
    assetpack_asset *Assets[];
} assetpack_tag;

typedef struct assetpack_tag_registry {
    assetpack_tag_id ID;
    type Type;
    u32 TagCount;
    assetpack_tag *Tags;
} assetpack_tag_registry;

typedef struct assetpack_atlas {
    vptr Data;
    v2u32 Size;
} assetpack_atlas;

typedef struct assetpack {
    assetpack_header *Header;
    assetpack_tag_registry *Registries;
    assetpack_tag *Tags;
    vptr TagData;
    assetpack_asset *Assets;
    assetpack_atlas *Atlases; // TODO: Consider removing
    vptr Data;
} assetpack;