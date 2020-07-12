#ifndef VOX_ASSET_PACKER_H_

#include "util/vox_defines.h"
#include "util/vox_assets.h"

#define MAX_ASSETS 256
#define MAX_TAGS 256
#define MAX_ATLAS_BYTES (MAX_ATLAS_DIM * MAX_ATLAS_DIM * BYTES_PER_PIXEL)

typedef struct packer_node
{
    b08 Used;
    u32 Width;
    u32 Height;
    u32 OffsetX;
    u32 OffsetY;
    struct packer_node *Right;
    struct packer_node *Down;
} packer_node;

typedef struct packer
{
    u32 NodeCount;
    packer_node *Nodes;
} packer;

#pragma pack(push, 1)

typedef struct bitmap_header
{
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

#pragma pack(pop)

typedef struct gen_asset
{
    u32 TagCount;
    u32 StartTag;
    c08 Path[MAX_PATH_SIZE];
    asset_type Type;
    union
    {
        voxai_bitmap Bitmap;
    };
} gen_asset;

typedef struct gen_type
{
    asset_type Type;
    u32 AssetCount;
    u32 StartAsset;
} gen_type;

typedef struct gen_data
{
    u32 TotalTypes;
    u32 TotalAssets;
    u32 TotalTags;
    gen_type Types[AssetType_Count];
    gen_asset *Assets;
    voxai_tag *Tags;
} gen_data;

typedef struct file_data
{
    u32 Size;
    vptr Data;
} file_data;

#define VOX_ASSET_PACKER_H_
#endif