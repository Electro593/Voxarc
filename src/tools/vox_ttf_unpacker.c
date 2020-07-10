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
UnpackFont(c08 *Path, c08 *Filename, u32 Codepoint, r32 Size)
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
        
        FILE *File = fopen(Filename, "wb");
        
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
    
    c08 Buffer[256];
    memset(Buffer, 0, 256);
    strcpy(Buffer, "assets/");
    
    for(u32 Index = 33;
        Index < 127;
        ++Index)
    {
        strcpy(Buffer + 7, Names[Index]);
        
        UnpackFont("C:/Windows/Fonts/cour.ttf", Buffer, Index, 36.0f);
        printf("Unpacked %c into %s\n", (c08)Index, Buffer);
    }
    
    // for(c08 Codepoint = 'a';
    //     Codepoint <= 'z';
    //     ++Codepoint)
    // {
    //     UnpackFont("C:/Windows/Fonts/cour.ttf", Codepoint, 'L', 36.0f);
    // }
    
    // for(c08 Codepoint = 'A';
    //     Codepoint <= 'Z';
    //     ++Codepoint)
    // {
    //     UnpackFont("C:/Windows/Fonts/cour.ttf", Codepoint, 'U', 36.0f);
    // }
    
    // for(c08 Codepoint = '0';
    //     Codepoint <= '9';
    //     ++Codepoint)
    // {
    //     UnpackFont("C:/Windows/Fonts/cour.ttf", Codepoint, 'I', 36.0f);
    // }
}