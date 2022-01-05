/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_FILE_H_
#define GAME_FILE_H_

typedef enum file_type
{
    FileType_Any,
    FileType_Bitmap,
    FileType_AssetPack,
} file_type;
#define FILE_TYPE__ANY "*.*"
#define FILE_TYPE__BITMAP "*.bmp"

typedef enum file_group
{
    FileGroup_Assets,
    FileGroup_Shaders,
} file_group;
#define FILE_GROUP__ASSETS "assets/"
#define FILE_GROUP__SHADERS "shaders/"

#define ASSET_PACK_FILENAME "assets/pack.vaa"
#define ASSET_DATA_FILENAME "assets/assetdata.json"
#define ASSET_PACK_MAGIC_NUMBER MAGIC_4('V', 'A', 'A', ' ')
#define ASSET_PACK_VERSION VERSION_32(0, 0, 1)
#define ASSET_FONT_DEFAULT_HEIGHT 36.0f

typedef enum asset_group_id
{
    AssetGroupID_Font_Cour,
    
    AssetGroupID_Count
} asset_group_id;

// Strings should just be macro-enumerated, like the IDs
typedef enum asset_tag_type
{
    AssetTagType_Bool,
    AssetTagType_R32,
    AssetTagType_S32,
    AssetTagType_U32,
    AssetTagType_Arr,
} asset_tag_type;

#define ENUM_GROUP_TAG_IDS \
    ENUM(Ascent,    AssetTagType_R32) \
    ENUM(Descent,   AssetTagType_R32) \
    ENUM(LineGap,   AssetTagType_R32) \

#define ENUM_ASSET_TAG_IDS \
    ENUM(Codepoint, AssetTagType_S32) \
    ENUM(Advance,   AssetTagType_Arr) \
    ENUM(BearingX,  AssetTagType_R32) \
    ENUM(BearingY,  AssetTagType_R32) \

typedef enum asset_tag_id
{
    #define ENUM(Name, Value) \
        AssetTagID_##Name,
    ENUM_GROUP_TAG_IDS
    ENUM_ASSET_TAG_IDS
    #undef ENUM
    
    AssetTagID_Count
} asset_tag_id;

global asset_tag_type AssetTagTypes[AssetTagID_Count] = {
    #define ENUM(Name, Value) \
        Value,
    ENUM_GROUP_TAG_IDS
    ENUM_ASSET_TAG_IDS
    #undef ENUM
};

typedef struct asset_tag
{
    asset_tag_id ID;
    union
    {
        b08 Bool;
        r32 R32;
        s32 S32;
        u32 U32;
        u32 Mem; // Offset from Tags Overflow
    } Value;
} asset_tag;

typedef struct asset_info
{
    v2u16 Offset;
    v2u16 Size;
    u32 AtlasIndex;
    u32 TagCount;
    
    union
    {
        asset_tag *Tags;
        u64 TagsOffset;
    };
} asset_info;

typedef struct asset_group
{
    asset_group_id GroupID;
    u32 AssetCount;
    u32 GroupTagCount;
    u64 GroupInfoSize;
    
    union
    {
        asset_info *Assets;
        u64 AssetsOffset;
    };
    union
    {
        asset_tag *GroupTags;
        u64 GroupTagsOffset;
    };
} asset_group;

typedef struct asset_atlas
{
    mem Pixels;
} asset_atlas;

typedef struct asset_pack
{
    u32 MagicNumber;
    u32 Version;
    u32 GroupCount;
    u32 AssetCount;
    u32 TagCount;
    u32 ExtraSize;
    u32 AtlasCount;
    
    v2u16 AtlasSize;
    u32 _Padding_;
    
    union
    {
        asset_group *Groups;
        u64 GroupsOffset;
    };
    union
    {
        asset_info *Assets;
        u64 AssetsOffset;
    };
    union
    {
        asset_tag *Tags;
        u64 TagsOffset;
    };
    union
    {
        mem Extra;
        u64 ExtraOffset;
    };
    union
    {
        mem Atlases;
        u64 AtlasesOffset;
    };
} asset_pack;

typedef struct asset_packer_node
{
    struct asset_packer_node *Next;
    v2u16 Offset;
    v2u16 Size;
    u32 AtlasIndex;
} asset_packer_node;

typedef struct asset_packer_asset
{
    v2u16 Offset;
    v2u16 Size;
    u32 AtlasIndex;
    u32 TagCount;
    union
    {
        asset_tag *Tags;
        u64 TagsOffset;
    };
    str Path;
} asset_packer_asset;

#pragma pack(push, 1)
typedef struct bitmap_header
{
    chr Signature[2];
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


#define GAME__FILE__EXPORTS \
    FUNC(str,         File_Read, str FilePath, u64 ReadLength, u64 ReadOffset) \

#define GAME__FILE__FUNCS \
    FUNC(void,        File_Write, str FilePath, str Text, u64 WriteOffset) \
    FUNC(void,        File_ReadConfig, game_config *Config, chr *ConfigFilePath) \
    FUNC(u32,         File_ReadShaders, chr *VertexShaderFilePath, chr *FragmentShaderFilePath) \
    FUNC(asset_pack*, File_ReadAssetPack, heap *Heap) \
    FUNC(void,        File_CreateAssetPack, heap *Heap) \
    FUNC(asset_info*, Asset_GetFromTags, asset_pack *Pack, asset_group_id GroupID, asset_tag *Tags, r32 *Weights, u32 TagCount) \
    FUNC(asset_tag *, Asset_GetTag, asset_tag *Tag, u32 TagCount, asset_tag_id AssetTagID) \
    FUNC(vptr,        Asset_GetExtra, asset_pack *Pack, asset_tag *Tag) \
    FUNC(void,        AssetPacker_FillTag, heap *Heap, asset_tag *Tag, asset_tag_id TagID, str ValueStr, mem Extra, u32 ExtraOffset) \


/*
pack.vaa *Outdated*

- asset_pack_header PackHeader
  [Header x1]
  - u32 Magic Number ('V' 'A' 'A' ' ')
  - u32 Version [8b 8b 16b] (0 0 1)
  - u32 GroupCount
  - u32 AssetCount
  - u32 TagCount
  - u32 AtlasCount
  - asset_group *Groups // Index from hash of tags
  - asset_info *Assets
  - asset_tag *Tags
  - asset_atlas *Atlases
  [Groups xN]
  - u32 AssetCount
  - asset *Assets
  [Assets xN]
  - v2u16 Offset
  - v2u16 Size
  - u32 AtlasIndex
  - u32 TagCount
  - asset_tag *Tags
  [Tags xN]
  - u32 ID
  - num Value
  [Tags Overflox xM]
  - mem Value
  [Atlases xN] // This will also be in pack.bmp for debug
  - mem Pixels
*/

#endif