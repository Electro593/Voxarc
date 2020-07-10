#ifndef VOX_ASSET_PACKER_H_

#include "util/vox_defines.h"

#define MAX_ASSETS 256
#define MAX_TAGS 256
#define MAX_PATH_SIZE 256
#define BYTES_PER_PIXEL 4
#define ASSET_DIR "assets/"
#define ASSET_INFO_FILENAME "assets/info.voxai"
#define ASSET_ATLAS_FILENAME "assets/atlas0.bmp"
#define VOXAI_MAGIC_NUMBER (((u32)('v') << 0) | ((u32)('a') << 8) | ((u32)('a') << 16) | ((u32)('i') << 24))
#define VOXAI_VERSION 0

#define ASSETS_DIR "assets/"

typedef enum asset_type
{
    AssetType_Bitmap,
    
    AssetType_Count,
} asset_type;

typedef enum asset_tag
{
    //NOTE: Fonts
    AssetTag_Codepoint,
    AssetTag_IsMonospace,
    
    AssetTag_Count,
} asset_tag;

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

typedef struct voxai_header
{
    u32 MagicNumber;
    u32 Version;
    
    u32 TypeCount;
} voxai_header;

typedef struct voxai_type
{
    asset_type ID;
    u32 AssetCount;
    size AssetsOffset;
} voxai_type;

typedef struct voxai_tag
{
    unsigned ID     :31;
    unsigned Format :1;
    union
    {
        s32 Value_S32; // 0
        r32 Value_R32; // 1
    };
} voxai_tag;

typedef struct voxai_asset
{
    u32 TagsCount;
    size TagsOffset;
    size DataOffset;
} voxai_asset;

typedef struct voxai_bitmap
{
    u32 Width;
    u32 Height;
    u32 OffsetX;
    u32 OffsetY;
} voxai_bitmap;

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