#include "platform/vox_platform.c"

#pragma warning(push,1)
#pragma warning(disable:4100)
    #define STB_TRUETYPE_IMPLEMENTATION
    #define STBTT_ifloor(X) (s32)R32_Floor((r32)X)
    #define STBTT_iceil(X)  (s32)R32_Ceil((r32)X)
    #define STBTT_sqrt(X)   R32_Sqrt(X)
    #define STBTT_malloc(Size, Heap) Heap_Allocate(Heap, Size)
    #define STBTT_free(Data, Heap) Heap_Free(Data)
    #define STBTT_assert(X) ASSERT(X)
    #define STBTT_strlen(X) (Str_Len(X) - 1)
    #define STBTT_memcpy(Dest, Src, Size) Mem_Cpy(Dest, Src, Size)
    #define STBTT_memset(Dest, Data, Size) Mem_Set(Dest, Data, Size)
    #include "tools/stb_truetype.h"
#pragma warning(pop)

#define FONT_NAME "cour"

void
main(void)
{
    platform_callbacks PlatformCallbacks = {0};
    Win32_InitFunctions(&PlatformCallbacks);
    
    game_exports GameExports = {0};
    game_state GameState = {0};
    win32_dll GameDll = {0};
    Win32_LoadGameDll(&GameDll, &PlatformCallbacks, &GameExports, &GameState);
    
    size HeapSize = MEBIBYTES(1);
    mem HeapBase = Win32_AllocateMemory(HeapSize);
    heap *Heap = Heap_Create(HeapBase, (u32)HeapSize);
    Str_SetHeap(Heap);
    
    str Names[127] = {0};
    Names[' '] = Str_Create(NULL, "", 0);
    for(u32 Index = 33;
        Index < 127;
        ++Index)
    {
        Names[Index] = U32_ToStr(NULL, Index);
    }
    #if 0
    str Names[128];
    Names[ 32] = "";
    Names[ 33] = "exclamation.bmp";
    Names[ 34] = "quotation.bmp";
    Names[ 35] = "number.bmp";
    Names[ 36] = "dollar.bmp";
    Names[ 37] = "percent.bmp";
    Names[ 38] = "ampersand.bmp";
    Names[ 39] = "apostrophe.bmp";
    Names[ 40] = "left_parenthesis.bmp";
    Names[ 41] = "right_parenthesis.bmp";
    Names[ 42] = "asterisk.bmp";
    Names[ 43] = "plus.bmp";
    Names[ 44] = "comma.bmp";
    Names[ 45] = "hyphen.bmp";
    Names[ 46] = "period.bmp";
    Names[ 47] = "slash.bmp";
    Names[ 48] = "0.bmp";
    Names[ 49] = "1.bmp";
    Names[ 50] = "2.bmp";
    Names[ 51] = "3.bmp";
    Names[ 52] = "4.bmp";
    Names[ 53] = "5.bmp";
    Names[ 54] = "6.bmp";
    Names[ 55] = "7.bmp";
    Names[ 56] = "8.bmp";
    Names[ 57] = "9.bmp";
    Names[ 58] = "colon.bmp";
    Names[ 59] = "semicolon.bmp";
    Names[ 60] = "less_than.bmp";
    Names[ 61] = "equal.bmp";
    Names[ 62] = "greater_than.bmp";
    Names[ 63] = "question.bmp";
    Names[ 64] = "at.bmp";
    Names[ 65] = "upper_a.bmp";
    Names[ 66] = "upper_b.bmp";
    Names[ 67] = "upper_c.bmp";
    Names[ 68] = "upper_d.bmp";
    Names[ 69] = "upper_e.bmp";
    Names[ 70] = "upper_f.bmp";
    Names[ 71] = "upper_g.bmp";
    Names[ 72] = "upper_h.bmp";
    Names[ 73] = "upper_i.bmp";
    Names[ 74] = "upper_j.bmp";
    Names[ 75] = "upper_k.bmp";
    Names[ 76] = "upper_l.bmp";
    Names[ 77] = "upper_m.bmp";
    Names[ 78] = "upper_n.bmp";
    Names[ 79] = "upper_o.bmp";
    Names[ 80] = "upper_p.bmp";
    Names[ 81] = "upper_q.bmp";
    Names[ 82] = "upper_r.bmp";
    Names[ 83] = "upper_s.bmp";
    Names[ 84] = "upper_t.bmp";
    Names[ 85] = "upper_u.bmp";
    Names[ 86] = "upper_v.bmp";
    Names[ 87] = "upper_w.bmp";
    Names[ 88] = "upper_x.bmp";
    Names[ 89] = "upper_y.bmp";
    Names[ 90] = "upper_z.bmp";
    Names[ 91] = "left_sqare.bmp";
    Names[ 92] = "backslash.bmp";
    Names[ 93] = "right_square.bmp";
    Names[ 94] = "caret.bmp";
    Names[ 95] = "underscore.bmp";
    Names[ 96] = "grave.bmp";
    Names[ 97] = "lower_a.bmp";
    Names[ 98] = "lower_b.bmp";
    Names[ 99] = "lower_c.bmp";
    Names[100] = "lower_d.bmp";
    Names[101] = "lower_e.bmp";
    Names[102] = "lower_f.bmp";
    Names[103] = "lower_g.bmp";
    Names[104] = "lower_h.bmp";
    Names[105] = "lower_i.bmp";
    Names[106] = "lower_j.bmp";
    Names[107] = "lower_k.bmp";
    Names[108] = "lower_l.bmp";
    Names[109] = "lower_m.bmp";
    Names[110] = "lower_n.bmp";
    Names[111] = "lower_o.bmp";
    Names[112] = "lower_p.bmp";
    Names[113] = "lower_q.bmp";
    Names[114] = "lower_r.bmp";
    Names[115] = "lower_s.bmp";
    Names[116] = "lower_t.bmp";
    Names[117] = "lower_u.bmp";
    Names[118] = "lower_v.bmp";
    Names[119] = "lower_w.bmp";
    Names[120] = "lower_x.bmp";
    Names[121] = "lower_y.bmp";
    Names[122] = "lower_z.bmp";
    Names[123] = "left_curly.bmp";
    Names[124] = "vertical.bmp";
    Names[125] = "right_curly.bmp";
    Names[126] = "tilde.bmp";
    #endif
    
    str TempStr;
    
    str FontPath = Str_Create(NULL, "C:/Windows/Fonts/" FONT_NAME ".ttf", 0);
    str FontText = File_Read(FontPath, 0, 0);
    Str_Free(FontPath);
    
    stbtt_fontinfo Font;
    stbtt_InitFont(&Font, FontText, stbtt_GetFontOffsetForIndex(FontText, 0));
    Font.userdata = Heap;
    
    r32 Scale = stbtt_ScaleForPixelHeight(&Font, ASSET_FONT_DEFAULT_HEIGHT);
    s32 SAscent, SDescent, SLineGap;
    stbtt_GetFontVMetrics(&Font, &SAscent, &SDescent, &SLineGap);
    r32 Ascent = (r32)SAscent * Scale;
    r32 Descent = (r32)SDescent * Scale;
    r32 LineGap = (r32)SLineGap * Scale;
    
    json_write_data WriteData = {0};
    JSON_CreateInit(Heap, &WriteData);
    JSON_CreateObjectStart(&WriteData, NULL);
    JSON_CreateObjectStart(&WriteData, U32_ToStr(&TempStr, AssetGroupID_Font_Cour)); Str_Free(TempStr);
    
    JSON_CreateObjectStart(&WriteData, Str_Create(&TempStr, ASSET_PACK_JSON_KEY_FONT_DATA, 0)); Str_Free(TempStr);
    JSON_CreateNum(&WriteData, Str_Create(&TempStr, ASSET_PACK_JSON_KEY_FONT_DATA_ASCENT, 0), (num){TYPE_R32, .R32=Ascent}); Str_Free(TempStr);
    JSON_CreateNum(&WriteData, Str_Create(&TempStr, ASSET_PACK_JSON_KEY_FONT_DATA_DESCENT, 0), (num){TYPE_R32, .R32=Descent}); Str_Free(TempStr);
    JSON_CreateNum(&WriteData, Str_Create(&TempStr, ASSET_PACK_JSON_KEY_FONT_DATA_LINEGAP, 0), (num){TYPE_R32, .R32=LineGap}); Str_Free(TempStr);
    JSON_CreateObjectEnd(&WriteData);
    
    #if 0 // Not worth the trouble
    //NOTE: Merge with previous data
    str DataText = File_Read(Heap, ASSET_DATA_FILENAME);
    if(DataText && DataText[0] != '\0')
    { //TODO: What if there's already an object with the same name
        json_token *Tokens = JSON_Parse(Heap, DataText);
        ASSERT(Tokens[0].Type == JSONToken_Object && // Root
               Tokens[1].Type == JSONToken_String && // Entry name
               Tokens[2].Type == JSONToken_Object);  // Entry object
        
        // Get everything except the root
        u32 TokenCount = Heap_GetSize(Tokens) / sizeof(json_token);
        JSON_CreateFromTokens(&WriteData, DataText, Tokens + 1, TokenCount - 1);
    }
    Str_Free(DataText);
    #endif
    
    str FileNameBuffer = Str_Create(NULL, FILE_GROUP__ASSETS, 0);
    Str_CatC(&FileNameBuffer, FONT_NAME);
    Str_CatC(&FileNameBuffer, "/");
    u32 FileNameInsertIndex = Str_Len(FileNameBuffer);
    Str_CatC(&FileNameBuffer, ".bmp");
    
    for(u32 Codepoint = 32;
        Codepoint < 127;
        ++Codepoint)
    {
        u32 GlyphIndex = stbtt_FindGlyphIndex(&Font, Codepoint);
        
        v4s32 BitmapBox;
        stbtt_GetGlyphBitmapBox(&Font, GlyphIndex, Scale, Scale, &BitmapBox.X, &BitmapBox.Y, &BitmapBox.Z, &BitmapBox.W);
        r32 BearingY = -(r32)BitmapBox.W;
        
        s32 SAdvance, SBearingX;
        stbtt_GetGlyphHMetrics(&Font, GlyphIndex, &SAdvance, &SBearingX);
        r32 Advance = (r32)SAdvance * Scale;
        r32 BearingX = (r32)SBearingX * Scale;
        
        //TODO: Kerning table (rewatch how Casey does it)
        //TODO: That weird Valve font rendering technique
        
        str FileName;
        if(Names[Codepoint][0] == '\0')
        {
            Str_Create(&FileName, NULL, 0);
        }
        else
        {
            Str_Create(&FileName, FileNameBuffer, Str_Len(FileNameBuffer));
            Str_Ins(&FileName, Names[Codepoint], FileNameInsertIndex);
        }
        
        JSON_CreateObjectStart(&WriteData, FileName);
        
        JSON_CreateNum(&WriteData, Str_Create(&TempStr, "Codepoint", 0), (num){TYPE_U32, .U32=Codepoint}); Str_Free(TempStr);
        JSON_CreateNum(&WriteData, Str_Create(&TempStr, "Advance", 0),   (num){TYPE_R32, .R32=Advance});   Str_Free(TempStr);
        JSON_CreateNum(&WriteData, Str_Create(&TempStr, "BearingX", 0),  (num){TYPE_R32, .R32=BearingX});  Str_Free(TempStr);
        JSON_CreateNum(&WriteData, Str_Create(&TempStr, "BearingY", 0),  (num){TYPE_R32, .R32=BearingY});  Str_Free(TempStr);
        
        JSON_CreateObjectEnd(&WriteData);
        
        if(!stbtt_IsGlyphEmpty(&Font, GlyphIndex))
        {
            v2s32 Size;
            mem MonoBitmap = stbtt_GetGlyphBitmap(&Font, Scale, Scale, GlyphIndex, &Size.X, &Size.Y, NULL, NULL);
            
            u32 FileSize = sizeof(bitmap_header) + (Size.X * Size.Y * BYTES_PER_PIXEL);
            hmem Bitmap = Heap_Allocate(Heap, FileSize);
            mem BitmapData = Bitmap + sizeof(bitmap_header);
            
            mem Src = MonoBitmap;
            v4u08 *DestRow = (v4u08*)(Bitmap + FileSize) - Size.X;
            for(s32 Row = 0;
                Row < Size.Y;
                ++Row)
            {
                v4u08 *Dest = DestRow;
                
                for(s32 Col = 0;
                    Col < Size.X;
                    ++Col)
                {
                    *Dest = V4u08_4x1(*Src, *Src, *Src, 255);
                    ++Src, ++Dest;
                }
                
                DestRow -= Size.X;
            }
            
            bitmap_header *BitmapHeader = (bitmap_header*)Bitmap;
            Mem_Zero(BitmapHeader, sizeof(bitmap_header));
            BitmapHeader->Signature[0] = 'B';
            BitmapHeader->Signature[1] = 'M';
            BitmapHeader->FileSize = FileSize;
            BitmapHeader->DataOffset = sizeof(bitmap_header);
            BitmapHeader->Size = 40;
            BitmapHeader->Width = Size.X;
            BitmapHeader->Height = Size.Y;
            BitmapHeader->Planes = 1;
            BitmapHeader->BitsPerPixel = 32;
            
            vptr FileHandle;
            Platform_OpenFile(&FileHandle, FileName, FileMode_Write);
            Platform_WriteFile(Bitmap, &FileHandle, FileSize, 0);
            Platform_CloseFile(&FileHandle);
            Heap_Free(Bitmap);
            
            stbtt_FreeBitmap(MonoBitmap, Font.userdata);
        }
        
        Str_Free(FileName);
    }
    
    JSON_CreateObjectEnd(&WriteData);
    JSON_CreateObjectEnd(&WriteData);
    
    JSON_Print(&WriteData, Str_Create(&TempStr, ASSET_DATA_FILENAME, 0)); Str_Free(TempStr);
    JSON_CreateEnd(&WriteData);
    
    for(u32 StrIndex = 0;
        StrIndex < ARRAY_COUNT(Names);
        ++StrIndex)
    {
        if(Names[StrIndex])
        {
            Str_Free(Names[StrIndex]);
        }
    }
    Str_Free(FileNameBuffer);
    Str_Free(FontText);
    
    Str_HeapCheck_DEBUG();
    ExitProcess(0);
}