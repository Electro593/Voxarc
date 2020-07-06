#include "util/vox_defines.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "tools/stb_truetype.h"

#include <stdio.h>

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
UnpackFont(c08 *Path, c08 Codepoint, r32 Size)
{
    file_data FileData = ReadEntireFile(Path);
    if(FileData.Size > 0)
    {
        stbtt_fontinfo Font;
        stbtt_InitFont(&Font, (u08*)FileData.Data, stbtt_GetFontOffsetForIndex((u08*)FileData.Data, 0));
        
        bitmap_header Header = {0};
        Header.Signature[0] = 'B';
        Header.Signature[1] = 'M';
        Header.DataOffset = sizeof(bitmap_header);
        Header.Size = 40;
        Header.Planes = 1;
        Header.BitsPerPixel = 32;
        Header.ColorsUsed = 256;
        
        c08 Buffer[256];
        memset(Buffer, 0, 256);
        Buffer[0] = '.';
        Buffer[1] = '.';
        Buffer[2] = '/';
        Buffer[3] = 'a';
        Buffer[4] = 's';
        Buffer[5] = 's';
        Buffer[6] = 'e';
        Buffer[7] = 't';
        Buffer[8] = 's';
        Buffer[9] = '/';
        Buffer[10] = Codepoint;
        Buffer[11] = '.';
        Buffer[12] = 'b';
        Buffer[13] = 'm';
        Buffer[14] = 'p';
        Buffer[15] = '\0';
        FILE *File = fopen(Buffer, "wb");
        
        s32 Width, Height, XOffset, YOffset;
        u08 *MonoBitmap = stbtt_GetCodepointBitmap(&Font, 0, stbtt_ScaleForPixelHeight(&Font, Size),
                                                   Codepoint, &Width, &Height, &XOffset, &YOffset);
        
        Header.FileSize = sizeof(bitmap_header) + (Width * Height * 4);
        Header.Width = Width;
        Header.Height = Height;
        
        vptr BitmapData = malloc(Width * Height * 4);
        u08 *Src = MonoBitmap;
        u08 *DestRow = (u08*)BitmapData + (Height - 1) * Width * 4;
        for(s32 Y = 0;
            Y < Height;
            ++Y)
        {
            u32 *Dest = (u32*)DestRow;
            for(s32 X = 0;
                X < Width;
                ++X)
            {
                u08 Gray = *Src++;
                *Dest++ = ((0xFF << 24) |
                           (Gray << 16) |
                           (Gray <<  8) |
                           (Gray <<  0));
            }
            
            DestRow -= Width * 4;
        }
            
        fwrite(&Header, sizeof(bitmap_header), 1, File);
        fwrite(BitmapData, Width * Height * 4, 1, File);
        free(BitmapData);
        
        stbtt_FreeBitmap(MonoBitmap, 0);
        free(FileData.Data);
        
        fclose(File);
    }
}

void
main()
{
    for(c08 Codepoint = 'a';
        Codepoint <= 'z';
        ++Codepoint)
    {
        UnpackFont("C:/Windows/Fonts/cour.ttf", Codepoint, 36.0f);
    }
    
    for(c08 Codepoint = 'A';
        Codepoint <= 'Z';
        ++Codepoint)
    {
        UnpackFont("C:/Windows/Fonts/cour.ttf", Codepoint, 36.0f);
    }
    
    for(c08 Codepoint = '0';
        Codepoint <= '9';
        ++Codepoint)
    {
        UnpackFont("C:/Windows/Fonts/cour.ttf", Codepoint, 36.0f);
    }
}