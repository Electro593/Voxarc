#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "util/vox_string.h"
#include "tools/vox_asset_packer.h"

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
BeginAsset(gen_data *Data, c08 *Path)
{
    CopyMemory(Data->Assets[Data->TotalAssets].Path, Path, MAX_PATH_SIZE);
    Data->Assets[Data->TotalAssets].StartTag = Data->TotalTags;
    Data->Assets[Data->TotalAssets].Type = AssetType_Bitmap;
}

local_func void
AddTag_S32(gen_data *Data, asset_tag Tag, s32 Value)
{
    Data->Tags[Data->TotalTags].ID = Tag;
    Data->Tags[Data->TotalTags].Format = 0;
    Data->Tags[Data->TotalTags].Value_S32 = Value;
    ++Data->TotalTags;
}

local_func void
AddTag_R32(gen_data *Data, asset_tag Tag, r32 Value)
{
    Data->Tags[Data->TotalTags].ID = Tag;
    Data->Tags[Data->TotalTags].Format = 1;
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

local_func packer_node *
FindNode(packer_node *Node, u32 Width, u32 Height)
{
    if(Node->Used)
    {
        packer_node *Result = FindNode(Node->Right, Width, Height);
        if(!Result)
        {
            Result = FindNode(Node->Down, Width, Height);
        }
        return Result;
    }
    else if(Width <= Node->Width && Height <= Node->Height)
    {
        return Node;
    }
    else
    {
        return NULL;
    }
}

local_func void
WriteFiles(gen_data Data)
{
    voxai_header Header = {0};
    Header.MagicNumber = VOXAI_MAGIC_NUMBER;
    Header.Version = VOXAI_VERSION;
    Header.AtlasDim = MAX_ATLAS_DIM;
    Header.TypeCount = Data.TotalTypes;
    
    u32 HeaderSize = sizeof(voxai_header);
    u32 TypeTableSize = Data.TotalTypes * sizeof(voxai_type);
    u32 AssetTableSize = Data.TotalAssets * sizeof(voxai_asset);
    u32 TagTableSize = Data.TotalTags * sizeof(voxai_tag);
    
    FILE *VOXAI = fopen(ASSET_INFO_FILENAME, "wb");
    if(!VOXAI)
    {
        return;
    }
    
    fwrite(&Header, sizeof(voxai_header), 1, VOXAI);
    
    u32 AssetOffset = 0;
    for(u32 TypeIndex = 0;
        TypeIndex < Data.TotalTypes;
        ++TypeIndex)
    {
        voxai_type Type = {0};
        Type.ID = Data.Types[TypeIndex].Type;
        Type.AssetCount = Data.Types[TypeIndex].AssetCount;
        Type.AssetsOffset = HeaderSize + TypeTableSize + AssetOffset;
        AssetOffset += Type.AssetCount * sizeof(voxai_asset);
        fwrite(&Type, sizeof(voxai_type), 1, VOXAI);
    }
    
    u32 TagsOffset = 0;
    u32 DataOffset = 0;
    for(u32 AssetIndex = 0;
        AssetIndex < Data.TotalAssets;
        ++AssetIndex)
    {
        voxai_asset Asset = {0};
        Asset.TagsCount = Data.Assets[AssetIndex].TagCount;
        Asset.TagsOffset = HeaderSize + TypeTableSize + AssetTableSize + TagsOffset;
        Asset.DataOffset = HeaderSize + TypeTableSize + AssetTableSize + TagTableSize + DataOffset;
        TagsOffset += Asset.TagsCount * sizeof(voxai_tag);
        switch(Data.Assets[AssetIndex].Type)
        {
            case AssetType_Bitmap:
            {
                file_data FileData = ReadEntireFile(Data.Assets[AssetIndex].Path);
                
                Data.Assets[AssetIndex].Bitmap.Width = *(u32*)((size)FileData.Data + 18);
                Data.Assets[AssetIndex].Bitmap.Height = *(u32*)((size)FileData.Data + 22);
                
                free(FileData.Data);
                
                DataOffset += sizeof(voxai_bitmap);
            } break;
        }
        
        fwrite(&Asset, sizeof(voxai_asset), 1, VOXAI);
    }
    
    for(u32 TagIndex = 0;
        TagIndex < Data.TotalTags;
        ++TagIndex)
    {
        fwrite(&Data.Tags[TagIndex], sizeof(voxai_tag), 1, VOXAI);
    }
    
    FILE *VOXAA = fopen(ASSET_ATLAS_FILENAME, "wb");
    if(!VOXAA)
    {
        return;
    }
    
    packer Packer = {0};
    Packer.Nodes = calloc(MAX_ASSETS * 4, sizeof(packer_node));
    Packer.Nodes[0].Width = MAX_ATLAS_DIM;
    Packer.Nodes[0].Height = MAX_ATLAS_DIM;
    ++Packer.NodeCount;
    
#if _VOX_AP_BITMAP
    bitmap_header BMPHeader = {0};
    BMPHeader.Signature[0] = 'B';
    BMPHeader.Signature[1] = 'M';
    BMPHeader.DataOffset = sizeof(bitmap_header);
    BMPHeader.Size = 40;
    BMPHeader.Planes = 1;
    BMPHeader.BitsPerPixel = 32;
    BMPHeader.ColorsUsed = 256;
    BMPHeader.FileSize = sizeof(bitmap_header) + MAX_ATLAS_BYTES;
    BMPHeader.Width = MAX_ATLAS_DIM;
    BMPHeader.Height = MAX_ATLAS_DIM;
    fwrite(&BMPHeader, sizeof(bitmap_header), 1, VOXAA);
#endif
    
    u08 *Atlas = calloc(MAX_ATLAS_BYTES, 1);
    
    b08 BeenPacked[MAX_ASSETS];
    memset(BeenPacked, 0, MAX_ASSETS);
    for(u32 SortIndex0 = 0;
        SortIndex0 < Data.TotalAssets;
        ++SortIndex0)
    {
        gen_asset *Largest = 0;
        u32 Area = 0;
        
        u32 ChosenIndex = 0;
        for(u32 SortIndex1 = 0;
            SortIndex1 < Data.TotalAssets;
            ++SortIndex1)
        {
            gen_asset *CurrAsset = &Data.Assets[SortIndex1];
            if(CurrAsset->Type == AssetType_Bitmap && !BeenPacked[SortIndex1])
            {
                u32 CurrArea = CurrAsset->Bitmap.Width * CurrAsset->Bitmap.Height;
                if(CurrArea > Area)
                {
                    Area = CurrArea;
                    Largest = CurrAsset;
                    ChosenIndex = SortIndex1;
                }
            }
        }
        BeenPacked[ChosenIndex] = TRUE;
        
        packer_node *Node = FindNode(&Packer.Nodes[0], Largest->Bitmap.Width, Largest->Bitmap.Height);
        if(Node)
        {
            Node->Used = TRUE;
            Node->Down = &Packer.Nodes[Packer.NodeCount++];
            Node->Right = &Packer.Nodes[Packer.NodeCount++];
            Node->Down->OffsetX = Node->OffsetX;
            Node->Down->OffsetY = Node->OffsetY + Largest->Bitmap.Height;
            Node->Down->Width = Node->Width;
            Node->Down->Height = Node->Height - Largest->Bitmap.Height;
            Node->Right->OffsetX = Node->OffsetX + Largest->Bitmap.Width;
            Node->Right->OffsetY = Node->OffsetY;
            Node->Right->Width = Node->Width - Largest->Bitmap.Width;
            Node->Right->Height = Largest->Bitmap.Height;
            Largest->Bitmap.OffsetX = Node->OffsetX;
            Largest->Bitmap.OffsetY = Node->OffsetY;
            
            file_data FileData = ReadEntireFile(Largest->Path);
            
            u32 *Src = (u32*)((u08*)FileData.Data + sizeof(bitmap_header));
            for(u32 Y = Node->OffsetY;
                Y < Node->OffsetY + Largest->Bitmap.Height;
                ++Y)
            {
                for(u32 X = Node->OffsetX;
                    X < Node->OffsetX + Largest->Bitmap.Width;
                    ++X)
                {
                    u32 *Dest = (u32*)Atlas + (Y * MAX_ATLAS_DIM) + X;
                    *Dest = *Src++;
                }
            }
            printf("Wrote asset %d of %d\n", SortIndex0 + 1, Data.TotalAssets);
            
            free(FileData.Data);
        }
    }
    fwrite(Atlas, MAX_ATLAS_BYTES, 1, VOXAA);
    free(Atlas);
    free(Packer.Nodes);
    
    for(u32 DataIndex = 0;
        DataIndex < Data.TotalAssets;
        ++DataIndex)
    {
        switch(Data.Assets[DataIndex].Type)
        {
            case AssetType_Bitmap:
            {
                fwrite(&Data.Assets[DataIndex].Bitmap, sizeof(voxai_bitmap), 1, VOXAI);
            } break;
        }
    }
    
    fclose(VOXAA);
    fclose(VOXAI);
}

void
main()
{
    gen_data Data = {0};
    memset(&Data, 0, sizeof(gen_data));
    Data.Tags = calloc(MAX_ASSETS * MAX_TAGS, sizeof(voxai_tag));
    Data.Assets = calloc(MAX_ASSETS, sizeof(gen_asset));
    
    c08 *Names[128];
    Names[33] = "exclamation.bmp";
    Names[34] = "quotation.bmp";
    Names[35] = "number.bmp";
    Names[36] = "dollar.bmp";
    Names[37] = "percent.bmp";
    Names[38] = "ampersand.bmp";
    Names[39] = "apostrophe.bmp";
    Names[40] = "left_parenthesis.bmp";
    Names[41] = "right_parenthesis.bmp";
    Names[42] = "asterisk.bmp";
    Names[43] = "plus.bmp";
    Names[44] = "comma.bmp";
    Names[45] = "hyphen.bmp";
    Names[46] = "period.bmp";
    Names[47] = "slash.bmp";
    Names[48] = "0.bmp";
    Names[49] = "1.bmp";
    Names[50] = "2.bmp";
    Names[51] = "3.bmp";
    Names[52] = "4.bmp";
    Names[53] = "5.bmp";
    Names[54] = "6.bmp";
    Names[55] = "7.bmp";
    Names[56] = "8.bmp";
    Names[57] = "9.bmp";
    Names[58] = "colon.bmp";
    Names[59] = "semicolon.bmp";
    Names[60] = "less_than.bmp";
    Names[61] = "equal.bmp";
    Names[62] = "greater_than.bmp";
    Names[63] = "question.bmp";
    Names[64] = "at.bmp";
    Names[65] = "UA.bmp";
    Names[66] = "UB.bmp";
    Names[67] = "UC.bmp";
    Names[68] = "UD.bmp";
    Names[69] = "UE.bmp";
    Names[70] = "UF.bmp";
    Names[71] = "UG.bmp";
    Names[72] = "UH.bmp";
    Names[73] = "UI.bmp";
    Names[74] = "UJ.bmp";
    Names[75] = "UK.bmp";
    Names[76] = "UL.bmp";
    Names[77] = "UM.bmp";
    Names[78] = "UN.bmp";
    Names[79] = "UO.bmp";
    Names[80] = "UP.bmp";
    Names[81] = "UQ.bmp";
    Names[82] = "UR.bmp";
    Names[83] = "US.bmp";
    Names[84] = "UT.bmp";
    Names[85] = "UU.bmp";
    Names[86] = "UV.bmp";
    Names[87] = "UW.bmp";
    Names[88] = "UX.bmp";
    Names[89] = "UY.bmp";
    Names[90] = "UZ.bmp";
    Names[91] = "left_sqare.bmp";
    Names[92] = "backslash.bmp";
    Names[93] = "right_square.bmp";
    Names[94] = "caret.bmp";
    Names[95] = "underscore.bmp";
    Names[96] = "grave.bmp";
    Names[97] = "La.bmp";
    Names[98] = "Lb.bmp";
    Names[99] = "Lc.bmp";
    Names[100] = "Ld.bmp";
    Names[101] = "Le.bmp";
    Names[102] = "Lf.bmp";
    Names[103] = "Lg.bmp";
    Names[104] = "Lh.bmp";
    Names[105] = "Li.bmp";
    Names[106] = "Lj.bmp";
    Names[107] = "Lk.bmp";
    Names[108] = "Ll.bmp";
    Names[109] = "Lm.bmp";
    Names[110] = "Ln.bmp";
    Names[111] = "Lo.bmp";
    Names[112] = "Lp.bmp";
    Names[113] = "Lq.bmp";
    Names[114] = "Lr.bmp";
    Names[115] = "Ls.bmp";
    Names[116] = "Lt.bmp";
    Names[117] = "Lu.bmp";
    Names[118] = "Lv.bmp";
    Names[119] = "Lw.bmp";
    Names[120] = "Lx.bmp";
    Names[121] = "Ly.bmp";
    Names[122] = "Lz.bmp";
    Names[123] = "left_curly.bmp";
    Names[124] = "vertical.bmp";
    Names[125] = "right_curly.bmp";
    Names[126] = "tilde.bmp";
    
    c08 Buffer[MAX_PATH_SIZE];
    memset(Buffer, 0, MAX_PATH_SIZE);
    strcpy(Buffer, "assets/");
    
    BeginType(&Data, AssetType_Bitmap);
    
    for(u32 Index = 33;
        Index < 127;
        ++Index)
    {
        strcpy(Buffer + 7, Names[Index]);
        
        BeginAsset(&Data, Buffer);
        AddTag_S32(&Data, AssetTag_Codepoint, Index);
        AddTag_S32(&Data, AssetTag_IsMonospace, 1);
        EndAsset(&Data);
    }
    
    EndType(&Data);
    
    // Buffer[7] = 'U';
    // for(u32 Codepoint = 'A';
    //     Codepoint <= 'Z';
    //     ++Codepoint)
    // {
    //     Buffer[8] = (c08)Codepoint;
    //     BeginAsset(&Data, Buffer);
    //     AddTag_S32(&Data, AssetTag_Codepoint, Codepoint);
    //     AddTag_S32(&Data, AssetTag_IsMonospace, 1);
    //     EndAsset(&Data);
    // }
    
    // Buffer[7] = 'L';
    // for(u32 Codepoint = 'a';
    //     Codepoint <= 'z';
    //     ++Codepoint)
    // {
    //     Buffer[8] = (c08)Codepoint;
    //     BeginAsset(&Data, Buffer);
    //     AddTag_S32(&Data, AssetTag_Codepoint, Codepoint);
    //     AddTag_S32(&Data, AssetTag_IsMonospace, 1);
    //     EndAsset(&Data);
    // }
    
    // Buffer[8] = '.';
    // Buffer[9] = 'b';
    // Buffer[10] = 'm';
    // Buffer[11] = 'p';
    // Buffer[12] = '\0';
    // for(u32 Codepoint = '0';
    //     Codepoint <= '9';
    //     ++Codepoint)
    // {
    //     Buffer[7] = (c08)Codepoint;
    //     BeginAsset(&Data, Buffer);
    //     AddTag_S32(&Data, AssetTag_Codepoint, Codepoint);
    //     AddTag_S32(&Data, AssetTag_IsMonospace, 1);
    //     EndAsset(&Data);
    // }
    
    
    WriteFiles(Data);
    
    free(Data.Assets);
    free(Data.Tags);
}

/*
<File Header>
  Magic Number
  Version
  Atlas Dim
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

<Data Table> (x Asset Count)
  Union:
    Bitmap
      Width
      Height
      Offset X
      Offset Y


--PROCEDURE--
Loop through type table
Lookup asset type, jump to asset headers
Jump to and match tags
Jump to data, get data
*/