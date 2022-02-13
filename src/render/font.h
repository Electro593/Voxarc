/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct font_character {
    v2s32 Pos;
    v2u32 Size;
    
    u32 Codepoint;
    s32 AdvanceX;
    s32 BearingX;
    
    u64 BitmapOffset;
} font_character;

typedef struct font {
    v2u32 BitmapSize;
    r32 FontSize;
    s32 Ascent;
    s32 Descent;
    s32 LineGap;
    s32 AdvanceY;
    
    font_character Characters[127-32];
    
    u08 Bitmap[];
} font;

#pragma pack(push, 1)
typedef struct bitmap_header {
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