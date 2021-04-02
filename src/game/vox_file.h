/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(GAME_VOX_FILE_H_)
#define      GAME_VOX_FILE_H_

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

#define ASSET_PACK_JSON_KEY_FONT_DATA "Data"
#define ASSET_PACK_JSON_KEY_FONT_DATA_ASCENT "Ascent"
#define ASSET_PACK_JSON_KEY_FONT_DATA_DESCENT "Descent"
#define ASSET_PACK_JSON_KEY_FONT_DATA_LINEGAP "LineGap"

typedef enum asset_group_id
{
    AssetGroupID_Font_Cour,
    
    AssetGroupID_Count
} asset_group_id;

// Strings should just be macro-enumerated, like the IDs
typedef enum asset_tag_type
{
    AssetTagType_B08,
    AssetTagType_R32,
    AssetTagType_S32,
    AssetTagType_U32,
} asset_tag_type;

#define ENUM_GROUP_TAG_IDS \
    ENUM(Ascent,    AssetTagType_R32) \
    ENUM(Descent,   AssetTagType_R32) \
    ENUM(LineGap,   AssetTagType_R32) \

#define ENUM_ASSET_TAG_IDS \
    ENUM(Codepoint, AssetTagType_S32) \
    ENUM(Advance,   AssetTagType_R32) \
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

global_var asset_tag_type AssetTagTypes[AssetTagID_Count] = {
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
        b08 B08;
        r32 R32;
        s32 S32;
        u32 U32;
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
        size TagsOffset;
    };
} asset_info;

typedef struct asset_group
{
    asset_group_id GroupID;
    u32 AssetCount;
    u32 GroupTagCount;
    
    union
    {
        asset_info *Assets;
        size AssetsOffset;
    };
    union
    {
        asset_tag *GroupTags;
        size GroupTagsOffset;
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
    u32 AtlasCount;
    
    v2u16 AtlasSize;
    u32 _Padding_;
    
    union
    {
        asset_group *Groups;
        size GroupsOffset;
    };
    union
    {
        asset_info *Assets;
        size AssetsOffset;
    };
    union
    {
        asset_tag *Tags;
        size TagsOffset;
    };
    union
    {
        mem Atlases;
        size AtlasesOffset;
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
        size TagsOffset;
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
    PROC(str,         File_Read, str FilePath, size ReadLength, size ReadOffset) \

#define GAME__FILE__FUNCS \
    PROC(void,        File_Write, str FilePath, str Text, size WriteOffset) \
    PROC(void,        File_ReadConfig, game_config *Config, chr *ConfigFilePath) \
    PROC(u32,         File_ReadShaders, chr *VertexShaderFilePath, chr *FragmentShaderFilePath) \
    PROC(asset_pack*, File_ReadAssetPack, heap *Heap) \
    PROC(void,        File_CreateAssetPack, heap *Heap) \
    PROC(asset_info*, Asset_GetFromTags, asset_pack *Pack, asset_group_id GroupID, asset_tag *Tags, r32 *Weights, u32 TagCount) \
    PROC(asset_tag *, Asset_GetTag, asset_tag *Tag, u32 TagCount, asset_tag_id AssetTagID) \
    PROC(void,        AssetPacker_FillTag, asset_tag *Tag, asset_tag_id TagID, str ValueStr) \


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
  [Atlases xN] // This will also be in pack.bmp for debug
  - mem Pixels
*/

#endif