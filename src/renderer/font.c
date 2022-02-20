/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define STBTT_pow(x,y) x
#define STBTT_fmod(x,y) x
#define STBTT_cos(x) x
#define STBTT_acos(x) x
#define STBTT_assert(x) Assert(x)
#define STBTT_strlen(x) Mem_BytesUntil(x, 0)
#define STBTT_ifloor(x) (s32)R32_Floor(x)
#define STBTT_iceil(x) (s32)R32_Ceil(x)
#define STBTT_sqrt(x) R32_Sqrt(x)
#define STBTT_fabs(x) R32_Abs(x)
#define STBTT_malloc(x,u) Stack_Allocate(x)
#define STBTT_free(x,u)
#define STBTT_memcpy Mem_Cpy
#define STBTT_memset Mem_Set
#define size_t u64
#define STB_TRUETYPE_IMPLEMENTATION
#include <libraries/stb_truetype.h>
#undef STBTT_ifloor
#undef STBTT_iceilSTBTT_sqrt
#undef STBTT_pow
#undef STBTT_fmod
#undef STBTT_cos
#undef STBTT_acos
#undef STBTT_fabs
#undef STBTT_malloc
#undef STBTT_free
#undef STBTT_assert
#undef STBTT_strlen
#undef STBTT_memcpy
#undef STBTT_memset
#undef STB_TRUETYPE_IMPLEMENTATION



internal void
MakeFont(u08 *TTFData,
         r32 FontSize,
         vptr *FontOut,
         u64 *FontSizeOut,
         vptr *BitmapOut,
         u64 *BitmapSizeOut)
{
    stbtt_fontinfo FontInfo;
    u32 FontOffset = stbtt_GetFontOffsetForIndex(TTFData, 0);
    stbtt_InitFont(&FontInfo, TTFData, FontOffset);
    
    r32 Scale = stbtt_ScaleForPixelHeight(&FontInfo, FontSize);
    
    s32 SX, SY, EX, EY;
    stbtt_GetFontBoundingBox(&FontInfo, &SX, &SY, &EX, &EY);
    u32 CellWidth = (u32)((r32)(EX - SX) * Scale);
    u32 CellHeight = (u32)((r32)(EY - SY) * Scale);
    
    u32 BitmapWidth = CellWidth * 8;
    u32 BitmapHeight = CellHeight * (((127-33)/8)+1);
    u64 BitmapSize = BitmapWidth * BitmapHeight;
    
    u64 BitmapFileOffset = OFFSET_OF(font, Bitmap);
    u64 FileSize = BitmapFileOffset + BitmapSize;
    *FontSizeOut = FileSize;
    *BitmapSizeOut = FileSize + sizeof(bitmap_header);
    u08 *FullFile = *BitmapOut = Stack_Allocate(*BitmapSizeOut);
    font *Font = *FontOut = FullFile + sizeof(bitmap_header);
    
    s32 Ascent, Descent, LineGap;
    stbtt_GetFontVMetrics(&FontInfo, &Ascent, &Descent, &LineGap);
    Ascent  = (s32)((r32)Ascent  * Scale);
    Descent = (s32)((r32)Descent * Scale);
    LineGap = (s32)((r32)LineGap * Scale);
    
    Font->BitmapSize = (v2u32){BitmapWidth, BitmapHeight};
    Font->FontSize = FontSize;
    Font->Ascent = Ascent;
    Font->Descent = Descent;
    Font->LineGap = LineGap;
    Font->AdvanceY = Ascent - Descent + LineGap;
    
    u32 Cell = 0;
    for(u32 Codepoint = 32; Codepoint < 127; ++Codepoint)
    {
        font_character *Character = Font->Characters + Codepoint - 32;
        Character->Codepoint = Codepoint;
        
        u32 GlyphIndex = stbtt_FindGlyphIndex(&FontInfo, Codepoint);
        if(GlyphIndex == 0) continue;
        
        s32 AdvanceX, BearingX;
        stbtt_GetGlyphHMetrics(&FontInfo, GlyphIndex, &AdvanceX, &BearingX);
        Character->AdvanceX = (s32)((r32)AdvanceX * Scale);
        Character->BearingX = (s32)((r32)BearingX * Scale);
        
        if(stbtt_IsGlyphEmpty(&FontInfo, GlyphIndex)) {
            Character->Pos = (v2s32){0};
            Character->Size = (v2u32){0};
            Character->BitmapOffset = 0;
        } else {
            stbtt_GetGlyphBitmapBox(&FontInfo, GlyphIndex, Scale, Scale, &SX, &SY, &EX, &EY);
            Character->Pos = (v2s32){SX, -EY};
            Character->Size = (v2u32){EX - SX, EY - SY};
            
            u64 BitmapOffset = INDEX_2D((Cell%8)*CellWidth, (Cell/8)*CellHeight, BitmapWidth);
            u08 *CharBitmap = Font->Bitmap + BitmapOffset;
            Character->BitmapOffset = BitmapFileOffset + BitmapOffset;
            stbtt_MakeGlyphBitmap(&FontInfo, CharBitmap, CellWidth, CellHeight, BitmapWidth, Scale, Scale, GlyphIndex);
            
            Cell++;
        }
    }
    
    bitmap_header *Header = (bitmap_header*)FullFile;
    Header->Signature[0] = 'B';
    Header->Signature[1] = 'M';
    Header->FileSize = (u32)*BitmapSizeOut;
    Header->Reserved = 0;
    Header->DataOffset = (u32)(sizeof(bitmap_header) + BitmapFileOffset);
    Header->Size = 40;
    Header->Width = BitmapWidth;
    Header->Height = -(s32)BitmapHeight;
    Header->Planes = 1;
    Header->BitsPerPixel = 8;
    Header->Compression = 0;
    Header->ImageSize = (u32)BitmapSize;
    Header->XPixelsPerM = 0;
    Header->YPixelsPerM = 0;
    Header->ColorsUsed = 0;
    Header->ImportantColors = 0;
}