#include "util/vox_defines.h"
#include "util/vox_assets.h"

inline voxai_header *
GetVoxaiHeader(memory_handle *Voxai)
{
    return (voxai_header*)Voxai->Base;
}

inline memory_handle *
LoadVoxai(handle_pool *Pool,
          c08 *Path)
{
    memory_handle *Handle = VoxReadFile(Pool, Path);
    voxai_header *Header = GetVoxaiHeader(Handle);
    if(Header->MagicNumber != VOXAI_MAGIC_NUMBER || Header->Version != VOXAI_VERSION)
    {
        //TODO: Logging
    }
    
    return Handle;
}

inline memory_handle *
LoadVoxaa(handle_pool *Pool,
          c08 *Path)
{
    return VoxReadFile(Pool, Path);
}

inline voxai_type *
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
inline voxai_asset *
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
                    if((SearchTag->Format == 0 && SearchTag->Value_S32 == CurrentTag->Value_S32) ||
                       (SearchTag->Format == 1 && SearchTag->Value_R32 == CurrentTag->Value_R32))
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

inline voxai_bitmap *
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