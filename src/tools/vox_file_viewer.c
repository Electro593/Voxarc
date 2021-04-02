#include "util/vox_defines.h"
#include "util/vox_assets.h"

#include <stdio.h>
#include <stdlib.h>

void
main(void)
{
    chr Buffer[260];
    chr TypeChar;
    
    printf("Path to file: ");
    scanf("%s", Buffer);
    printf("Type of file:\n0 = Asset Info\n1 = Asset Atlas\n");
    getc(stdin);
    scanf("%c", &TypeChar);
    FILE *File = fopen(Buffer, "rb");
    fseek(File, 0, SEEK_END);
    u32 Size = ftell(File);
    fseek(File, 0, SEEK_SET);
    str FileData = malloc(Size);
    fread(FileData, Size, 1, File);
    fclose(File);
    
    switch(TypeChar - '0')
    {
        case 0:
        {
            voxai_header Header = *(voxai_header*)FileData;
            printf("File Header:\n");
            printf("|   Magic Value-----: %s\n", (str)&Header.MagicNumber);
            printf("|   Version---------: %d\n", Header.Version);
            printf("|   Atlas Dimensions: %d\n", Header.AtlasDim);
            printf("|   Type Count------: %d\n", Header.TypeCount);
            FileData += sizeof(voxai_header);
            printf("Type Table:\n");
            u32 AssetCount = 0;
            for(u32 TypeIndex = 0;
                TypeIndex < Header.TypeCount;
                ++TypeIndex)
            {
                voxai_type Type = *(voxai_type*)FileData;
                AssetCount += Type.AssetCount;
                printf("|   Asset Type: %s\n", AssetTypeJSONIDs[Type.ID]);
                printf("|   |   Asset Count--: %d\n", Type.AssetCount);
                printf("|   |   Assets Offset: %llu\n", Type.AssetsOffset);
                FileData += sizeof(voxai_type);
            }
            printf("Asset Table:\n");
            u32 TagCount = 0;
            for(u32 AssetIndex = 0;
                AssetIndex < AssetCount;
                ++AssetIndex)
            {
                voxai_asset Asset = *(voxai_asset*)FileData;
                TagCount += Asset.TagsCount;
                printf("|   Asset:\n");
                printf("|   |   Tag Count--: %d\n", Asset.TagsCount);
                printf("|   |   Tags Offset: %llu\n", Asset.TagsOffset);
                FileData += sizeof(voxai_asset);
            }
            printf("Tag Table:\n");
            for(u32 TagIndex = 0;
                TagIndex < TagCount;
                ++TagIndex)
            {
                voxai_tag Tag = *(voxai_tag*)FileData;
                printf("|   Tag Name: %s\n", AssetTagJSONIDs[Tag.ID]);
                printf("|   |   Tag Value: %f\n", Tag.Value);
                FileData += sizeof(voxai_tag);
            }
        } break;
    }
    
    free(FileData);
    
    getc(stdin);
}