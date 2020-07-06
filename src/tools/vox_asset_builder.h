#ifndef VOX_ASSET_BUILDER_H_

#include "util/vox_defines.h"

#define VAAI_MAGIC_NUMBER (((u32)('v') << 0) | ((u32)('a') << 8) | ((u32)('a') << 16) | ((u32)('i') << 24))
#define VAAI_VERSION 0

#define ASSETS_DIR "assets/"

typedef enum asset_type
{
    AssetType_Glyph,
    
    AssetType_Count,
} asset_type;

typedef enum asset_tag
{
    //NOTE: Fonts
    AssetTag_Codepoint,
    AssetTag_IsMonospace,
    
    AssetTag_Count,
} asset_tag;

typedef struct gen_data
{
    u32 TotalTypes;
    u32 TotalAssets;
    u32 TotalTags;
} gen_data;

#pragma pack(push, 1)

typedef struct vaai_header
{
    u32 MagicNumber;
    u32 Version;
    
    u32 TypeCount;
} vaai_header;

typedef struct vaai_type
{
    asset_type ID;
    u32 AssetCount;
    size AssetsOffset;
} vaai_type;

typedef struct vaai_tag
{
    unsigned ID     :30;
    unsigned Format :2;
    union
    {
        u32 Value_U32; // 00
        s32 Value_S32; // 01
        r32 Value_R32; // 10
    };
} vaai_tag;

typedef struct vaai_asset
{
    u32 TagsCount;
    size TagsOffset;
    size DataOffset;
} vaai_asset;

typedef struct vaai_bitmap
{
    u32 Width;
    u32 Height;
    u32 OffsetX;
    u32 OffsetY;
} vaai_bitmap;

#pragma pack(pop)

#define VOX_ASSET_BUILDER_H_
#endif