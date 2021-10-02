#include "platform/platform.c"

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

// #define FONT_NAME "cour"
#define FONT_NAME "arial"
// #define FONT_NAME "Speeday-axVZa"

typedef struct ttf_shape
{
    u32 ContourCount;
    u32 EdgeCount;
    u32 SegmentCount;
    
    u32 *Contours;
    u32 *Edges;
    stbtt_vertex *Segments;
    
    v3u08 *Colors;
} ttf_shape;

void
main(void)
{
    platform_exports PlatformCallbacks = {0};
    Win32_InitFunctions(&PlatformCallbacks);
    
    game_exports GameExports = {0};
    game_state GameState = {0};
    win32_dll GameDll = {0};
    Win32_LoadGameDll(&GameDll, &PlatformCallbacks, &GameExports, &GameState);
    
    u64 HeapSize = MEBIBYTES(2);
    u64 StackSize = MEBIBYTES(1);
    mem Base = Win32_AllocateMemory(HeapSize + StackSize);
    heap *Heap = Heap_Create(Base, (u32)HeapSize);
    Stack_Init(Base + HeapSize, StackSize);
    Str_SetHeap(Heap);
    Str_HeapMark_DEBUG();
    
    str Names[127] = {0};
    Names[' '] = Str_Create(NULL, "", 0);
    for(u32 Index = 33;
        Index < 127;
        ++Index)
    {
        Names[Index] = U32_To_Str(NULL, Index);
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
    
    // str FontPath = Str_Create(NULL, "C:/Windows/Fonts/" FONT_NAME ".ttf", 0);
    str FontPath = Str_Create(NULL, "D:/Andrew/Downloads/" FONT_NAME ".ttf", 0);
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
    JSON_CreateObjectStart(&WriteData, U32_To_Str(&TempStr, AssetGroupID_Font_Cour)); Str_Free(TempStr);
    
    JSON_CreateObjectStart(&WriteData, Str_Create(&TempStr, "Data", 0)); Str_Free(TempStr);
    JSON_CreateNum(&WriteData, Str_Create(&TempStr, "Ascent", 0), (num){TYPE_R32, .R32=Ascent}); Str_Free(TempStr);
    JSON_CreateNum(&WriteData, Str_Create(&TempStr, "Descent", 0), (num){TYPE_R32, .R32=Descent}); Str_Free(TempStr);
    JSON_CreateNum(&WriteData, Str_Create(&TempStr, "LineGap", 0), (num){TYPE_R32, .R32=LineGap}); Str_Free(TempStr);
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
    
    u32 Glyphs[127];
    for(u32 Codepoint = 32;
        Codepoint < 127;
        ++Codepoint)
    {
        Glyphs[Codepoint] = stbtt_FindGlyphIndex(&Font, Codepoint);
    }
    
    for(u32 Codepoint = 32;
        Codepoint < 127;
        ++Codepoint)
    {
        u32 GlyphIndex = Glyphs[Codepoint];
        
        v4s32 BitmapBox;
        stbtt_GetGlyphBitmapBox(&Font, GlyphIndex, Scale, Scale, &BitmapBox.X, &BitmapBox.Y, &BitmapBox.Z, &BitmapBox.W);
        r32 BearingY = -(r32)BitmapBox.W;
        
        s32 SAdvance, SBearingX;
        stbtt_GetGlyphHMetrics(&Font, GlyphIndex, &SAdvance, &SBearingX);
        r32 Advance = (r32)SAdvance * Scale;
        r32 BearingX = (r32)SBearingX * Scale;
        
        //TODO: That weird Valve font rendering technique
        //      - Multi-Channel Signed Distance Fields
        
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
        JSON_CreateNum(&WriteData, Str_Create(&TempStr, "BearingX", 0),  (num){TYPE_R32, .R32=BearingX});  Str_Free(TempStr);
        JSON_CreateNum(&WriteData, Str_Create(&TempStr, "BearingY", 0),  (num){TYPE_R32, .R32=BearingY});  Str_Free(TempStr);
        
        JSON_CreateArrayStart(&WriteData, Str_Create(&TempStr, "Advance", 0)); Str_Free(TempStr);
        for(u32 Codepoint1 = 32;
            Codepoint1 < 127;
            ++Codepoint1)
        {
            s32 SKern = stbtt_GetGlyphKernAdvance(&Font, GlyphIndex, Glyphs[Codepoint1]);
            r32 Kern = (r32)SKern * Scale;
            JSON_CreateNum(&WriteData, NULL, (num){TYPE_R32, .R32=Advance + Kern});
        }
        JSON_CreateArrayEnd(&WriteData);
        
        JSON_CreateObjectEnd(&WriteData);
        
        if(!stbtt_IsGlyphEmpty(&Font, GlyphIndex))
        {
            Stack_Push();
            ttf_shape Shape = {0};
            r32 CrossThresh = 0.17365f; // ~ sin(pi/18)
            Shape.SegmentCount = stbtt_GetGlyphShape(&Font, GlyphIndex, &Shape.Segments);
            Shape.Edges = Stack_Allocate(Shape.SegmentCount * sizeof(u32) + 1);
            Shape.Contours = Stack_Allocate(Shape.SegmentCount * sizeof(u32) + 1);
            v2s16 PrevDirection={0}, Direction={0}, NextDirection={0};
            b08 ContourStart = FALSE;
            ASSERT(Codepoint != 'R');
            //TODO: Handle teardrop case
            for(u32 SegmentIndex = 0;
                SegmentIndex < Shape.SegmentCount;
                ++SegmentIndex)
            {
                ContourStart = FALSE;
                stbtt_vertex Segment = Shape.Segments[SegmentIndex];
                // ASSERT(!(Codepoint == 'R' && SegmentIndex == 10));
                switch(Segment.type)
                {
                    case STBTT_vmove:
                    {
                        Shape.Contours[Shape.ContourCount] = Shape.EdgeCount;
                        ++Shape.ContourCount;
                        NextDirection = *(v2s16*)&Shape.Segments[SegmentIndex];
                        ContourStart = TRUE;
                    } break;
                    case STBTT_vline:
                    {
                        ASSERT(SegmentIndex != 0);
                        Direction = V2s16_Sub(*(v2s16*)&Shape.Segments[SegmentIndex], *(v2s16*)&Shape.Segments[SegmentIndex-1]);
                        NextDirection = Direction;
                    } break;
                    case STBTT_vcurve:
                    {
                        ASSERT(SegmentIndex != 0);
                        Direction = V2s16_Sub(*(v2s16*)&Shape.Segments[SegmentIndex].cx, *(v2s16*)&Shape.Segments[SegmentIndex-1]);
                        NextDirection = V2s16_Sub(*(v2s16*)&Shape.Segments[SegmentIndex], *(v2s16*)&Shape.Segments[SegmentIndex].cx);
                    } break;
                }
                
                if(!ContourStart &&
                   (R32_Abs(V2s16_Cross(PrevDirection, Direction)) > CrossThresh ||
                    V2s16_Dot(PrevDirection, Direction) <= 0))
                {
                    Shape.Edges[Shape.EdgeCount] = SegmentIndex - 1;
                    ++Shape.EdgeCount;
                }
                
                PrevDirection = NextDirection;
            }
            // Makes getting the segments per edge/contour easier
            Shape.Contours[Shape.ContourCount] = Shape.EdgeCount;
            Shape.Edges[Shape.EdgeCount] = Shape.SegmentCount;
            
            // for(u32 SegmentIndex = 0;
            //     SegmentIndex < Shape.SegmentCount;
            //     ++SegmentIndex)
            // {
            //     Shape.Segments[SegmentIndex].x *= Scale;
            //     Shape.Segments[SegmentIndex].y *= Scale;
            //     Shape.Segments[SegmentIndex].cx *= Scale;
            //     Shape.Segments[SegmentIndex].cy *= Scale;
            // }
            
            Shape.Colors = Stack_Allocate(Shape.EdgeCount * sizeof(v3u08));
            v3u08 Color_011 = V3u08_3x1(  0, 255, 255);
            v3u08 Color_101 = V3u08_3x1(255,   0, 255);
            v3u08 Color_110 = V3u08_3x1(255, 255,   0);
            v3u08 Color_111 = V3u08_3x1(255, 255, 255);
            for(u32 ContourIndex = 0;
                ContourIndex < Shape.ContourCount;
                ++ContourIndex)
            {
                v3u08 Curr;
                if(Shape.Contours[ContourIndex+1] - Shape.Contours[ContourIndex] == 1)
                    Curr = Color_111;
                else
                    Curr = Color_101;
                
                for(u32 EdgeIndex = Shape.Edges[ContourIndex];
                    EdgeIndex < Shape.Edges[ContourIndex+1];
                    ++EdgeIndex)
                {
                    Shape.Colors[EdgeIndex] = Curr;
                    if(V3u08_Cmp(Curr, Color_110) == 0)
                        Curr = Color_011;
                    else
                        Curr = Color_110;
                }
            }
            
            v2s32 Size = V2s32_Sub(BitmapBox.ZW, BitmapBox.XY);
            u32 FileSize = sizeof(bitmap_header) + (Size.X * Size.Y * 3);
            hmem Bitmap = Heap_Allocate(Heap, FileSize);
            v3u08 *BitmapData = (v3u08*)(Bitmap + sizeof(bitmap_header));
            
            for(u32 PY = Size.Y - 1;
                PY >= 0;
                --PY)
            {
                for(u32 PX = 0;
                    PX < Size.X;
                    ++PX)
                {
                    v3r32 D = V3r32_1x1(R32_MAX);
                    v3u32 E = {-1};
                    v2s16 P = V2s16_2x1((s16)(((r32)PX+.5f) / Scale), (s16)(((r32)PY+.5f) / Scale));
                    
                    for(u32 EdgeIndex = 0;
                        EdgeIndex < Shape.EdgeCount;
                        ++EdgeIndex)
                    {
                        r32 CurrDist = R32_MAX;
                        v2s16 P0 = {0};
                        for(u32 SegmentIndex = Shape.Segments[Shape.Edges[EdgeIndex]];
                            SegmentIndex < Shape.Segments[Shape.Edges[EdgeIndex+1]];
                            ++SegmentIndex)
                        {
                            stbtt_vertex Segment = Shape.Segments[SegmentIndex];
                            switch(Segment.type)
                            {
                                case STBTT_vmove:
                                {
                                    P0 = *(v2s16*)&Segment.x;
                                } break;
                                case STBTT_vline:
                                {
                                    v2s16 P1 = *(v2s16*)&Shape.Segments[SegmentIndex].x;
                                    
                                    s32 Numer = V2s16_Dot(V2s16_Sub(P, P0), V2s16_Sub(P1, P0));
                                    s32 Denom = V2s16_Dot(V2s16_Sub(P1, P0), V2s16_Sub(P1, P0));
                                    r32 T = (r32)Numer / (r32)Denom;
                                    if(T < 0.0f) T = 0.0f;
                                    else if(T > 1.0f) T = 1.0f;
                                    v2s16 PT = V2s16_Lerp(P0, P1, T);
                                    r32 NumerR = R32_Abs(V2s16_Cross(V2s16_Sub(P1, P0), V2s16_Sub(P0, PT)));
                                    r32 DenomR = V2s16_Len(V2s16_To_V2r32(V2s16_Sub(P1, P0)));
                                    r32 Dist = NumerR / DenomR;
                                    if(Dist < CurrDist) CurrDist = Dist;
                                    
                                    P0 = P1;
                                } break;
                                case STBTT_vcurve:
                                {
                                    v2s16 P1 = *(v2s16*)&Shape.Segments[SegmentIndex].cx;
                                    v2s16 P2 = *(v2s16*)&Shape.Segments[SegmentIndex].x;
                                    v2s16 V0 = V2s16_Sub(P0, P);
                                    v2s16 V1 = V2s16_Sub(P1, P0);
                                    v2s16 V2 = V2s16_Sub(V2s16_Sub(P2, P1), V1);
                                    r32 A = V2s16_Dot(V2, V2);
                                    r32 B = 3*V2s16_Dot(V1, V2);
                                    r32 C = 2*V2s16_Dot(V1, V1) + V2s16_Dot(V0, V2);
                                    r32 D = V2s16_Dot(V0, V1);
                                    r32 BN = B/A;
                                    r32 CN = C/A;
                                    r32 DN = D/A;
                                    if(R32_Cmp(A, 0) == EQUAL) // Bx^2 + Cx + D = 0
                                    { // Quadratic
                                        if(R32_Cmp(B, 0) == EQUAL)    // Cx + D = 0
                                        { // Linear
                                            if(R32_Cmp(C, 0) == EQUAL)     // D = 0
                                            { // Constant
                                                if(R32_Cmp(D, 0) == EQUAL) // 0 = 0
                                                { // All real numbers
                                                    
                                                }
                                            }
                                        }
                                    }
                                    if(R32_Abs(BN) < 1e12 && R32_Abs(CN) < 1e12 && R32_Abs(DN) < 1e12)
                                    { // Cubic
                                        ;
                                    }
                                    
                                    
                                    
                                    
                                    P0 = P2;
                                } break;
                            }
                        }
                        
                        if(Shape.Colors[EdgeIndex].X != 0 && Cmp(CurrDist, D.X) == LESS)
                        {
                            D.X = CurrDist;
                            E.X = EdgeIndex;
                        }
                        if(Shape.Colors[EdgeIndex].Y != 0 && Cmp(CurrDist, D.Y) == LESS)
                        {
                            D.Y = CurrDist;
                            E.Y = EdgeIndex;
                        }
                        if(Shape.Colors[EdgeIndex].Z != 0 && Cmp(CurrDist, D.Z) == LESS)
                        {
                            D.Z = CurrDist;
                            E.Z = EdgeIndex;
                        }
                    }
                    
                    D.X = EdgePseudoSignedDistance(P, E.X);
                    D.Y = EdgePseudoSignedDistance(P, E.Y);
                    D.Z = EdgePseudoSignedDistance(P, E.Z);
                    
                    v3u08 Color;
                    r32 DistanceMax = ;
                    r32 Range = 2 * DistanceMax;
                    Color.X = (u08)(((D.X / Range) + 0.5f) * 255.0f);
                    Color.Y = (u08)(((D.Y / Range) + 0.5f) * 255.0f);
                    Color.Z = (u08)(((D.Z / Range) + 0.5f) * 255.0f);
                    
                    BitmapData[INDEX_2D(PX, PY, Size.X)] = ;
                }
            }
            
            // v2s32 Size;
            // mem MonoBitmap = stbtt_GetGlyphBitmap(&Font, Scale, Scale, GlyphIndex, &Size.X, &Size.Y, NULL, NULL);
            
            // mem Src = MonoBitmap;
            // v4u08 *DestRow = (v4u08*)(Bitmap + FileSize) - Size.X;
            // for(s32 Row = 0;
            //     Row < Size.Y;
            //     ++Row)
            // {
            //     v4u08 *Dest = DestRow;
                
            //     for(s32 Col = 0;
            //         Col < Size.X;
            //         ++Col)
            //     {
            //         *Dest = V4u08_4x1(*Src, *Src, *Src, 255);
            //         ++Src, ++Dest;
            //     }
                
            //     DestRow -= Size.X;
            // }
            
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
            // BitmapHeader->BitsPerPixel = 32;
            BitmapHeader->BitsPerPixel = 24;
            
            vptr FileHandle;
            Platform_OpenFile(&FileHandle, FileName, FileMode_Write);
            Platform_WriteFile(Bitmap, &FileHandle, FileSize, 0);
            Platform_CloseFile(&FileHandle);
            Heap_Free(Bitmap);
            
            // stbtt_FreeBitmap(MonoBitmap, Font.userdata);
            stbtt_FreeShape(&Font, Shape.Segments);
            Stack_Pop();
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