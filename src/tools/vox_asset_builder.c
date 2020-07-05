#include <stdio.h>
#include "util/vox_defines.h"
#include "util/vox_file_io.h"
#include "util/vox_memory.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "tools/stb_truetype.h"

typedef struct gen_asset
{
    u32 TagCount;
    u32 StartTag;
    c08 *Path;
    asset_type Type;
    s32 Codepoint;
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
    gen_asset Assets[MAX_ASSETS];
    vai_tag Tags[MAX_TAGS];
} gen_data;

typedef struct file_data
{
    u32 Size;
    vptr Data;
} file_data;

local_func file_data
ReadEntireFile(c08 *Path)
{
    file_data Data = {0};
    
    FILE *File = fopen(Path, "rb");
    if(File)
    {
        fseek(File, 0, SEEK_END);
        Data.Size = ftell(File);
        fseek(File, 0, SEEK_SET);
        Data.Data = malloc(Data.Size);
        fread(Data.Data, Data.Size, 1, File);
        fclose(File);
    }
    
    return Data;
}

local_func void
BeginType(gen_data *Data, asset_type Type)
{
    Data->Types[Data->TotalTypes].Type = Type;
    Data->Types[Data->TotalTypes].StartAsset = Data->TotalAssets;
}

local_func void
BeginGlyphAsset(gen_data *Data, c08 *Path, u32 Codepoint)
{
    Data->Assets[Data->TotalAssets].Path = Path;
    Data->Assets[Data->TotalAssets].Codepoint = Codepoint;
    Data->Assets[Data->TotalAssets].StartTag = Data->TotalTags;
    Data->Assets[Data->TotalAssets].Type = AssetType_Glyph;
}

local_func void
AddTag_U32(gen_data *Data, asset_tag Tag, u32 Value)
{
    Data->Tags[Data->TotalTags].ID = Tag;
    Data->Tags[Data->TotalTags].Format = 0;
    Data->Tags[Data->TotalTags].Value_U32 = Value;
    ++Data->TotalTags;
}

local_func void
AddTag_S32(gen_data *Data, asset_tag Tag, s32 Value)
{
    Data->Tags[Data->TotalTags].ID = Tag;
    Data->Tags[Data->TotalTags].Format = 1;
    Data->Tags[Data->TotalTags].Value_S32 = Value;
    ++Data->TotalTags;
}

local_func void
AddTag_R32(gen_data *Data, asset_tag Tag, r32 Value)
{
    Data->Tags[Data->TotalTags].ID = Tag;
    Data->Tags[Data->TotalTags].Format = 2;
    Data->Tags[Data->TotalTags].Value_R32 = Value;
    ++Data->TotalTags;
}

local_func void
EndAsset(gen_data *Data)
{
    Data->Assets[Data->TotalAssets].TagCount = Data->TotalTags - Data->Assets[Data->TotalAssets].StartTag;
    ++Data->TotalAssets;
}

local_func void
EndType(gen_data *Data)
{
    Data->Types[Data->TotalTypes].AssetCount = Data->TotalAssets - Data->Types[Data->TotalTypes].StartAsset;
    ++Data->TotalTypes;
}

local_func void
WriteFiles(gen_data Data)
{
    vai_header Header = {0};
    Header.MagicNumber = VAI_MAGIC_NUMBER;
    Header.Version = VAI_VERSION;
    Header.TypeCount = Data.TotalTypes;
    
    size HeaderSize = sizeof(vai_header);
    size TypeTableSize = Data.TotalTypes * sizeof(vai_type);
    size AssetTableSize = Data.TotalAssets * sizeof(vai_asset);
    size TagTableSize = Data.TotalTags * sizeof(vai_tag);
    
    FILE *VAI = fopen(FILENAME_FONTS ".vai", "wb");
    if(!VAI)
    {
        return;
    }
    
    fwrite(&Header, sizeof(vai_header), 1, VAI);
    
    size AssetOffset = 0;
    for(u32 TypeIndex = 0;
        TypeIndex < Data.TotalTypes;
        ++TypeIndex)
    {
        vai_type Type = {0};
        Type.ID = Data.Types[TypeIndex].Type;
        Type.AssetCount = Data.Types[TypeIndex].AssetCount;
        Type.AssetsOffset = HeaderSize + TypeTableSize + AssetOffset;
        AssetOffset += Type.AssetCount * sizeof(vai_asset);
        fwrite(&Type, sizeof(vai_type), 1, VAI);
    }
    
    size TagsOffset = 0;
    size DataOffset = 0;
    for(u32 AssetIndex = 0;
        AssetIndex < Data.TotalAssets;
        ++AssetIndex)
    {
        vai_asset Asset = {0};
        Asset.TagsCount = Data.Assets[AssetIndex].TagCount;
        Asset.TagsOffset = HeaderSize + TypeTableSize + AssetTableSize + TagsOffset;
        Asset.DataOffset = HeaderSize + TypeTableSize + AssetTableSize + TagTableSize + DataOffset;
        TagsOffset += Asset.TagsCount * sizeof(vai_tag);
        switch(Data.Assets[AssetIndex].Type)
        {
            case AssetType_Glyph:
            {
                DataOffset += sizeof(vai_bitmap);
            } break;
        }
        
        fwrite(&Asset, sizeof(vai_asset), 1, VAI);
    }
    
    for(u32 TagIndex = 0;
        TagIndex < Data.TotalTags;
        ++TagIndex)
    {
        fwrite(&Data.Tags[TagIndex], sizeof(vai_tag), 1, VAI);
    }
    
    FILE *VAA = fopen(FILENAME_FONTS ".vaa", "wb");
    if(!VAA)
    {
        return;
    }
    
    u32 XOffset = 0;
    u32 YOffset = 0;
    for(u32 DataIndex = 0;
        DataIndex < Data.TotalAssets;
        ++DataIndex)
    {
        switch(Data.Assets[DataIndex].Type)
        {
            case AssetType_Glyph:
            {
                vai_bitmap Bitmap = {0};
                
                file_data VAIData = ReadEntireFile(Data.Assets[DataIndex].Path);
                if(VAIData.Size != 0)
                {
                    stbtt_fontinfo Font;
                    stbtt_InitFont(&Font, (u08*)VAIData.Data, stbtt_GetFontOffsetForIndex((u08*)VAIData.Data, 0));
                    
                    s32 Width, Height, XOffset, YOffset;
                    u08 *MonoBitmap = stbtt_GetCodepointBitmap(&Font, 0, stbtt_ScaleForPixelHeight(&Font, 72.0f),
                                                               Data.Assets[DataIndex].Codepoint, &Width, &Height,
                                                               &XOffset, &YOffset);
                    
                    Bitmap.Width = Width;
                    Bitmap.Height = Height;
                    
                    vptr BitmapData = malloc(Width * Height * BITMAP_BPP);
                    u08 *Src = MonoBitmap;
                    u08 *Dest = BitmapData;
                    for(s32 Y = 0;
                        Y < Height;
                        ++Y)
                    {
                        for(s32 X = 0;
                            X < Width;
                            ++X)
                        {
                            u08 Alpha = *Src++;
                            *Dest++ = ((Alpha << 24) |
                                       (Alpha << 16) |
                                       (Alpha <<  8) |
                                       (Alpha <<  0));
                        }
                    }
                    
                    fwrite(BitmapData, Bitmap.Width * Bitmap.Height * BITMAP_BPP, 1, VAA);
                    free(BitmapData);
                    
                    stbtt_FreeBitmap(MonoBitmap, 0);
                    free(VAIData.Data);
                }
                
                fwrite(&Bitmap, sizeof(vai_bitmap), 1, VAI);
            } break;
        }
    }
    
    fclose(VAI);
    fclose(VAA);
}

void
main()
{
    gen_data Data = {0};
    
    BeginType(&Data, AssetType_Glyph);
    BeginGlyphAsset(&Data, "C:/Windows/Fonts/cour.ttf", 'A');
    AddTag_U32(&Data, AssetTag_Codepoint, 'A');
    EndAsset(&Data);
    EndType(&Data);
    
    WriteFiles(Data);
}

/*
<File Header>
  Magic Number
  Version
  Type Count

<Type Table> (x Types Count)
  ID
  Asset Headers Count
  Asset Headers Offset

<Asset Table> (x Asset Count)
  Tags Count
  Tags Offset
  Data Offset

<Tag Table> (x Tags Count)
    ID (Last 2 bits denote preferred data type)
      0 = Unsigned Int
      1 = Signed Int
      2 = Float
      3 = Undefined
    32-bit Value

<Data> (x Asset Count)
  Data Header
  Data Body


--PROCEDURE--
Loop through type table
Lookup asset type, jump to asset headers
Jump to and match tags
Jump to data, get data
*/

/*
.vai (Voxarc Information) files are information for the texture atlas
.vaa (Voxarc Atlas) files are where the data is stored
*/