/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
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

#ifndef GAME_ASSETS_H_
#define GAME_ASSETS_H_

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
    AssetTag_BearingX,
    AssetTag_BearingY,
    AssetTag_Advance,
    
    //NOTE: This needs to be the last member
    AssetTag_Count,
} asset_tag;

str AssetTypeJSONIDs[] = {
    "Bitmap",
};

str AssetTagJSONIDs[] = {
    "Codepoint",
    "IsMonospace",
    "BearingX",
    "BearingY",
    "Advance",
};

typedef enum tag_search
{
    TagSearch_Exact,
} tag_search;

#pragma pack(push, 1)

typedef struct assets_info_header
{
    u32 MagicNumber;
    u32 Version;
    
    u32 AtlasDim;
    u32 TypeCount;
} voxai_header, assets_info_header;
//TODO: Fully convert this

typedef struct voxai_type
{
    asset_type ID;
    u32 AssetCount;
    u64 AssetsOffset;
} voxai_type;

typedef struct voxai_tag
{
    u32 ID;
    r32 Value;
} voxai_tag;

typedef struct voxai_asset
{
    u32 TagsCount;
    u64 TagsOffset;
    u64 DataOffset;
} voxai_asset;

typedef struct voxai_bitmap
{
    u32 OffsetX;
    u32 OffsetY;
    u32 Width;
    u32 Height;
} voxai_bitmap;

#pragma pack(pop)

typedef struct asset_memory
{
    memory_handle *Info;
    memory_handle *Atlas0;
    
    u32 Atlas0ID;
} asset_memory;

#define ASSETS_GET_INFO_HEADER(InfoText) (voxai_header*)Voxai

internal hvptr
Assets_LoadInfo(heap_pool *Heap,
                str Path)
{
    hvptr InfoText = File_ReadFile(Heap, Path);
    assets_info_header *Header = ASSETS_GET_INFO_HEADER(InfoText);
    if(Header->MagicNumber != VOXAI_MAGIC_NUMBER || Header->Version != VOXAI_VERSION)
    {
        //TODO: Logging
    }
    
    return InfoText;
}

internal memory_handle *
Assets_LoadAtlas(handle_pool *Pool,
                 str Path)
{
    return File_ReadFile(Pool, Path);
}

internal voxai_type *
GetVoxaiType(memory_handle *Voxai,
             asset_type Type)
{
    voxai_header *Header = GetVoxaiHeader(Voxai);
    voxai_type *CurrentType = (voxai_type*)((u08*)Voxai->Base + sizeof(voxai_header));
    
    for(u32 TypeIndex = 0;
        TypeIndex < Header->TypeCount;
        ++TypeIndex)
    {
        if(CurrentType->ID == Type)
        {
            return CurrentType;
        }
        
        ++CurrentType;
    }
    
    return NULL;
}

//TODO: Optimize this
internal voxai_asset *
GetVoxaiAssetFromExactTags(memory_handle *Voxai,
                           voxai_type *Type,
                           u32 TagsCount,
                           memory_handle **Tags)
{
    voxai_asset *CurrentAsset = (voxai_asset*)((u08*)Voxai->Base + Type->AssetsOffset);
    for(u32 AssetIndex = 0;
        AssetIndex < Type->AssetCount;
        ++AssetIndex)
    {
        u32 Matched = 0;
        for(u32 SearchIndex = 0;
            SearchIndex < TagsCount;
            ++SearchIndex)
        {
            voxai_tag *SearchTag = Tags[SearchIndex]->Base;
            
            voxai_tag *CurrentTag = (voxai_tag*)((u08*)Voxai->Base + CurrentAsset->TagsOffset);
            for(u32 TagIndex = 0;
                TagIndex < CurrentAsset->TagsCount;
                ++TagIndex)
            {
                if(SearchTag->ID == CurrentTag->ID)
                {
                    if(SearchTag->Value == CurrentTag->Value)
                    {
                        ++Matched;
                    }
                }
                
                ++CurrentTag;
            }
        }
        
        if(Matched == TagsCount)
        {
            return CurrentAsset;
        }
        
        ++CurrentAsset;
    }
    
    return NULL;
}

internal voxai_bitmap *
GetVoxaiBitmap(memory_handle *Voxai,
               tag_search SearchType,
               u32 TagsCount,
               memory_handle **Tags)
{
    voxai_type *Type = GetVoxaiType(Voxai, AssetType_Bitmap);
    if(Type)
    {
        voxai_asset *Asset = NULL;
        switch(SearchType)
        {
            case TagSearch_Exact:
            {
                Asset = GetVoxaiAssetFromExactTags(Voxai, Type, TagsCount, Tags);
            } break;
        }
        
        if(Asset)
        {
            return (voxai_bitmap*)((u08*)Voxai->Base + Asset->DataOffset);
        }
    }
    
    return NULL;
}

#endif