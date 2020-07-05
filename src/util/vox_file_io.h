#ifndef UTIL_VOX_FILE_IO_H_

#include "math/vox_v3u32.h"

typedef struct config
{
    u32 VisionBitsPerChannel;
    u32 VisionChannelBinary;
    u32 FOV;
    r32 Sensitivity;
    v3u RenderDistance;
    u32 MoveLeft;
    u32 MoveForward;
    u32 MoveRight;
    u32 MoveBack;
    u32 MoveUp;
    u32 MoveDown;
} config;

typedef enum asset_type
{
    AssetType_Glyph,
    
    AssetType_Count,
} asset_type;

typedef enum asset_tag
{
    AssetTag_Codepoint,
    
    AssetTag_Count,
} asset_tag;

#pragma pack(push, 1)

#define VAI_MAGIC_NUMBER (((u32)('v') << 0) | ((u32)('a') << 8) | ((u32)('i') << 16) | ((u32)('f') << 24))
#define VAI_VERSION 0
#define MAX_TAGS 256
#define MAX_ASSETS 1024
#define BITMAP_BPP 4

typedef struct vai_header
{
    u32 MagicNumber;
    u32 Version;
    
    u32 TypeCount;
} vai_header;

typedef struct vai_type
{
    asset_type ID;
    u32 AssetCount;
    size AssetsOffset;
} vai_type;

typedef struct vai_tag
{
    unsigned ID     :30;
    unsigned Format :2;
    union
    {
        u32 Value_U32; // 00
        s32 Value_S32; // 01
        r32 Value_R32; // 10
    };
} vai_tag;

typedef struct vai_asset
{
    u32 TagsCount;
    size TagsOffset;
    size DataOffset;
} vai_asset;

typedef struct vai_bitmap
{
    u32 Width;
    u32 Height;
} vai_bitmap;

#pragma pack(pop)

#define UTIL_VOX_FILE_IO_H_
#endif