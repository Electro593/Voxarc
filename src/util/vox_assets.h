#ifndef UTIL_VOX_ASSETS_H_

#include "util/vox_defines.h"

enum asset_type
{
    AssetType_Bitmap,
    
    AssetType_Count,
};

enum asset_tag
{
    //NOTE: Fonts
    AssetTag_Codepoint,
    AssetTag_IsMonospace,
    
    AssetTag_Count,
};

enum tag_search
{
    TagSearch_Exact,
};

#pragma pack(push, 1)

struct voxai_header
{
    u32 MagicNumber;
    u32 Version;
    
    u32 AtlasDim;
    u32 TypeCount;
};

struct voxai_type
{
    asset_type ID;
    u32 AssetCount;
    u32 AssetsOffset;
};

struct voxai_tag
{
    unsigned ID     :31;
    unsigned Format :1;
    union
    {
        s32 Value_S32; // 0
        r32 Value_R32; // 1
    };
};

struct voxai_asset
{
    u32 TagsCount;
    u32 TagsOffset;
    u32 DataOffset;
};

struct voxai_bitmap
{
    u32 Width;
    u32 Height;
    u32 OffsetX;
    u32 OffsetY;
};

#pragma pack(pop)

struct asset_memory
{
    memory_handle *Info;
    memory_handle *Atlas0;
    
    u32 Atlas0ID;
};

#define UTIL_VOX_ASSETS_H_
#endif