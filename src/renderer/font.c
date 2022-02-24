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

internal u32
Font_CalculateChecksum(u32 *Table,
                       u32 Size)
{
    u32 Sum = 0;
    Size = (Size+3)/4;
    while(Size--) {
        u32 Value = *Table++;
        Sum += SWAPENDIAN32(Value);
    }
    return Sum;
}

internal vptr
Font_FindTable(font *Font,
               u32 Tag)
{
    ttf_font_dir *FontDir = Font->FontDir;
    u08 *FileBase = Font->FileBase;
    u32 Index;
    u32 Start = 0;
    u32 End = FontDir->Offset.TableCount;
    while(Start != End) {
        Index = Start+(End-Start)/2;
        if(FontDir->Tables[Index].Tag == Tag) break;
        if(FontDir->Tables[Index].Tag < Tag) Start = Index+1;
        else End = Index;
    }
    if(Start == End) return NULL;
    vptr Table = FileBase+FontDir->Tables[Index].Offset;
    u32 TableSize = FontDir->Tables[Index].Length;
    u32 Checksum = FontDir->Tables[Index].Checksum;
    if(Tag == TTF_TAG_head) {
        u32 Adjustment = ((ttf_head*)Table)->ChecksumAdjustment;
        Checksum += SWAPENDIAN32(Adjustment);
    }
    // if(Font_CalculateChecksum(Table, TableSize) != Checksum) return NULL;
    return Table;
}

internal void
Font_LoadFontDir(font *Font)
{
    ttf_font_dir *FontDir = Font->FontDir = (ttf_font_dir*)Font->FileBase;
    SWAPENDIAN32(FontDir->Offset.ScalerType);
    SWAPENDIAN16(FontDir->Offset.TableCount);
    SWAPENDIAN16(FontDir->Offset.SearchRange);
    SWAPENDIAN16(FontDir->Offset.EntrySelector);
    SWAPENDIAN16(FontDir->Offset.RangeShift);
    Assert(FontDir->Offset.ScalerType == 0x00010000 || FontDir->Offset.ScalerType == TTF_TAG_true);
    for(u32 T = 0; T < FontDir->Offset.TableCount; T++) {
        SWAPENDIAN32(FontDir->Tables[T].Tag);
        SWAPENDIAN32(FontDir->Tables[T].Checksum);
        SWAPENDIAN32(FontDir->Tables[T].Offset);
        SWAPENDIAN32(FontDir->Tables[T].Length);
    }
}

internal void
Font_LoadcmapTable(font *Font)
{
    ttf_cmap_subtable *Encoding = &Font->Encoding;
    ttf_cmap *cmap = Font->cmap = Font_FindTable(Font, TTF_TAG_cmap);
    Assert(cmap);
    SWAPENDIAN16(cmap->Version);
    SWAPENDIAN16(cmap->TableCount);
    for(u32 I = 0; I < cmap->TableCount; I++) {
        SWAPENDIAN16(cmap->Records[I].PlatformID);
        SWAPENDIAN16(cmap->Records[I].PlatformSpecificID);
        SWAPENDIAN32(cmap->Records[I].Offset);
        switch(cmap->Records[I].PlatformID) {
            case TTF_CMAP_PLATFORM_MICROSOFT: {
                switch(cmap->Records[I].PlatformSpecificID) {
                    case TTF_CMAP_MICROSOFT_UNICODEBMP: {
                        Encoding->Header = (vptr)((u08*)cmap + cmap->Records[I].Offset);
                        SWAPENDIAN16(Encoding->Header->Format);
                        SWAPENDIAN16(Encoding->Header->Length);
                        SWAPENDIAN16(Encoding->Header->Language);
                    } break;
                }
            } break;
        }
    }
    Assert(Encoding->Header);
    switch(Encoding->Header->Format) {
        case 4: {
            u08 *Data = (u08*)(Encoding->Header+1);
            Encoding->Type4.SegCountX2    = SWAPENDIAN16(*(u16*)(Data + OFFSET_OF(ttf_cmap_subtable_4, SegCountX2)));
            Encoding->Type4.SearchRange   = SWAPENDIAN16(*(u16*)(Data + OFFSET_OF(ttf_cmap_subtable_4, SearchRange)));
            Encoding->Type4.EntrySelector = SWAPENDIAN16(*(u16*)(Data + OFFSET_OF(ttf_cmap_subtable_4, EntrySelector)));
            Encoding->Type4.RangeShift    = SWAPENDIAN16(*(u16*)(Data + OFFSET_OF(ttf_cmap_subtable_4, RangeShift)));
            Data += OFFSET_OF(ttf_cmap_subtable_4, EndCodes);
            Encoding->Type4.EndCodes     = (u16*)(Data + 0*Encoding->Type4.SegCountX2);
            Encoding->Type4.StartCodes   = (u16*)(Data + 1*Encoding->Type4.SegCountX2);
            Encoding->Type4.Deltas       = (s16*)(Data + 2*Encoding->Type4.SegCountX2);
            Encoding->Type4.RangeOffsets = (u16*)(Data + 3*Encoding->Type4.SegCountX2);
            Encoding->Type4.GlyphIdArray = (u16*)(Data + 4*Encoding->Type4.SegCountX2);
            for(u32 I = 0; I < (u32)Encoding->Type4.SegCountX2/2; I++) {
                SWAPENDIAN16(Encoding->Type4.EndCodes[I]);
                SWAPENDIAN16(Encoding->Type4.StartCodes[I]);
                SWAPENDIAN16(Encoding->Type4.Deltas[I]);
                SWAPENDIAN16(Encoding->Type4.RangeOffsets[I]);
            }
            Data += 4*Encoding->Type4.SegCountX2;
            u32 RemainingSize = Encoding->Header->Length - (u32)((u64)Data-(u64)Encoding->Header);
            for(u32 I = 0; I < RemainingSize/2; I++)
                SWAPENDIAN16(Encoding->Type4.GlyphIdArray[I]);
        }
    }
}

internal void
Font_LoadglyfTable(font *Font)
{
    Font->glyfs = Font_FindTable(Font, TTF_TAG_glyf);
    Assert(Font->glyfs);
    // for(u32 I = 0; I < Font->maxp->GlyphCount; I++) {
    //     u32 GlyphOffset;
    //     if(Font->head->IndexToLocFormat == TTF_LOCA_SHORT_OFFSETS)
    //         GlyphOffset = Font->loca->Shorts[I] * 2;
    //     else
    //         GlyphOffset = Font->loca->Longs[I];
    //     ttf_glyf_entry *Glyph = (ttf_glyf_entry*)(Font->glyfs + GlyphOffset);
        
    //     SWAPENDIAN16(Glyph->ContourCount);
    //     SWAPENDIAN16(Glyph->XMin);
    //     SWAPENDIAN16(Glyph->YMin);
    //     SWAPENDIAN16(Glyph->XMax);
    //     SWAPENDIAN16(Glyph->YMax);
        
    //     if(Glyph->ContourCount >= 0) {
    //         u16 *EndPtsOfContours = (u16*)Glyph->Data;
    //         for(u32 J = 0; J < (u32)Glyph->ContourCount; J++) {
    //             SWAPENDIAN16(*EndPtsOfContours);
    //             EndPtsOfContours++;
    //         }
    //         s32 MaxFlags = *(EndPtsOfContours-1);
    //         u16 *InstructionLength = EndPtsOfContours;
    //         SWAPENDIAN16(*InstructionLength);
    //         u08 *Flags = (u08*)(InstructionLength+1) + *InstructionLength;
    //         s32 FlagCount=0;
    //         u32 XCoordsSize=0, YCoordsSize=0;
    //         for(; FlagCount < MaxFlags; FlagCount++) {
    //             u32 RepeatModifier = 1;
    //             if(Flags[FlagCount] & TTF_GLYF_FLAG_REPEATED) {
    //                 RepeatModifier += Flags[FlagCount+1];
    //                 MaxFlags -= Flags[FlagCount+1] - 1;
    //             }
    //             if(Flags[FlagCount] & TTF_GLYF_FLAG_SHORT_X)
    //                 XCoordsSize += sizeof(u08)*RepeatModifier;
    //             else if(!(Flags[FlagCount] & TTF_GLYF_FLAG_SAME_X))
    //                 XCoordsSize += sizeof(s16)*RepeatModifier;
    //             if(Flags[FlagCount] & TTF_GLYF_FLAG_SHORT_Y)
    //                 YCoordsSize += sizeof(u08)*RepeatModifier;
    //             else if(!(Flags[FlagCount] & TTF_GLYF_FLAG_SAME_Y))
    //                 YCoordsSize += sizeof(s16)*RepeatModifier;
    //             if(Flags[FlagCount] & TTF_GLYF_FLAG_REPEATED)
    //                 FlagCount++;
    //         }
    //         u08 *XCoords = Flags + FlagCount;
    //         u08 *YCoords = XCoords + XCoordsSize;
    //         for(u32 J = 0; J < (u32)FlagCount; J++) {
    //             u32 RepeatModifier = 1;
    //             if(Flags[J] & TTF_GLYF_FLAG_REPEATED)
    //                 RepeatModifier += Flags[J+1];
    //             if(Flags[J] & TTF_GLYF_FLAG_SHORT_X)
    //                 XCoords += sizeof(u08)*RepeatModifier;
    //             else if(!(Flags[J] & TTF_GLYF_FLAG_SAME_X)) {
    //                 for(u32 K = 0; K < RepeatModifier; K++) {
    //                     SWAPENDIAN16(*(u16*)XCoords);
    //                     XCoords += 2;
    //                 }
    //             }
    //             // if(Flags[J] & TTF_GLYF_FLAG_SHORT_Y)
    //             //     YCoords += sizeof(u08)*RepeatModifier;
    //             // else if(!(Flags[J] & TTF_GLYF_FLAG_SAME_Y)) {
    //             //     for(u32 K = 0; K < RepeatModifier; K++) {
    //             // //         SWAPENDIAN16(*(u16*)YCoords);
    //             //         (u16*)YCoords++;
    //             //     }
    //             // }
    //             if(Flags[J] & TTF_GLYF_FLAG_REPEATED) J++;
    //         }
    //     } else {
            
    //     }
    // }
}

internal void
Font_LoadheadTable(font *Font)
{
    ttf_head *head = Font->head = Font_FindTable(Font, TTF_TAG_head);
    Assert(head);
    SWAPENDIAN32(head->Version);
    SWAPENDIAN32(head->Revision);
    SWAPENDIAN32(head->ChecksumAdjustment);
    SWAPENDIAN32(head->MagicNumber);
    SWAPENDIAN16(head->Flags);
    SWAPENDIAN16(head->UnitsPerEm);
    SWAPENDIAN64(head->Created);
    SWAPENDIAN64(head->Modified);
    SWAPENDIAN16(head->XMin);
    SWAPENDIAN16(head->YMin);
    SWAPENDIAN16(head->XMax);
    SWAPENDIAN16(head->YMax);
    SWAPENDIAN16(head->MaxStyle);
    SWAPENDIAN16(head->LowestRecPPEM);
    SWAPENDIAN16(head->FontDirectionHint);
    SWAPENDIAN16(head->IndexToLocFormat);
    SWAPENDIAN16(head->GlyphDataFormat);
}

internal void
Font_LoadhheaTable(font *Font)
{
    ttf_hhea *hhea = Font->hhea = Font_FindTable(Font, TTF_TAG_hhea);
    Assert(hhea);
    SWAPENDIAN32(hhea->Version);
    SWAPENDIAN16(hhea->Ascent);
    SWAPENDIAN16(hhea->Descent);
    SWAPENDIAN16(hhea->LineGap);
    SWAPENDIAN16(hhea->MaxAdvanceX);
    SWAPENDIAN16(hhea->MinLeftBearing);
    SWAPENDIAN16(hhea->MinRightBearing);
    SWAPENDIAN16(hhea->MaxExtent);
    SWAPENDIAN16(hhea->CaretSlopeRise);
    SWAPENDIAN16(hhea->CaretSlopeRun);
    SWAPENDIAN16(hhea->CaretOffset);
    SWAPENDIAN16(hhea->MetricDataFormat);
    SWAPENDIAN16(hhea->HMetricCount);
}

internal void
Font_LoadhmtxTable(font *Font)
{
    vptr hmtx = Font_FindTable(Font, TTF_TAG_hmtx);
    Assert(hmtx);
    Font->hmtx.HMetrics = hmtx;
    for(u32 I = 0; I < Font->hhea->HMetricCount; I++) {
        SWAPENDIAN16(Font->hmtx.HMetrics[I].AdvanceX);
        SWAPENDIAN16(Font->hmtx.HMetrics[I].LeftBearing);
    }
    Font->hmtx.ExtraBearings = (ttf_fword*)((u08*)hmtx + Font->hhea->HMetricCount*sizeof(struct ttf_hmetric));
    for(u32 I = 0; I < (u32)Font->maxp->GlyphCount-Font->hhea->HMetricCount; I++)
        SWAPENDIAN16(Font->hmtx.ExtraBearings[I]);
}

internal void
Font_LoadlocaTable(font *Font)
{
    Font->loca = Font_FindTable(Font, TTF_TAG_loca);
    Assert(Font->loca);
    if(Font->head->IndexToLocFormat == TTF_LOCA_SHORT_OFFSETS)
        for(u32 I = 0; I <= Font->maxp->GlyphCount; I++)
            SWAPENDIAN16(Font->loca->Shorts[I]);
    else
        for(u32 I = 0; I <= Font->maxp->GlyphCount; I++)
            SWAPENDIAN32(Font->loca->Longs[I]);
}

internal void
Font_LoadmaxpTable(font *Font)
{
    ttf_maxp *maxp = Font->maxp = Font_FindTable(Font, TTF_TAG_maxp);
    Assert(maxp);
    SWAPENDIAN32(maxp->Version);
    SWAPENDIAN16(maxp->GlyphCount);
    SWAPENDIAN16(maxp->MaxPoints);
    SWAPENDIAN16(maxp->MaxContours);
    SWAPENDIAN16(maxp->MaxComponentPoints);
    SWAPENDIAN16(maxp->MaxComponentContours);
    SWAPENDIAN16(maxp->MaxZones);
    SWAPENDIAN16(maxp->MaxTwilightPoints);
    SWAPENDIAN16(maxp->MaxStorage);
    SWAPENDIAN16(maxp->MaxFunctionDefs);
    SWAPENDIAN16(maxp->MaxInstructionDefs);
    SWAPENDIAN16(maxp->MaxStackElements);
    SWAPENDIAN16(maxp->MaxSizeOfInstructions);
    SWAPENDIAN16(maxp->MaxComponentElements);
    SWAPENDIAN16(maxp->MaxComponentDepth);
}

internal font
Font_Init(u08 *FileData)
{
    Assert(FileData);
    font Font = {0};
    Font.FileBase = FileData;
    
    Font_LoadFontDir(&Font);
    Font_LoadcmapTable(&Font);
    Font_LoadheadTable(&Font);
    Font_LoadhheaTable(&Font);
    Font_LoadmaxpTable(&Font);
    Font_LoadhmtxTable(&Font);
    Font_LoadlocaTable(&Font);
    Font_LoadglyfTable(&Font);
    
    return Font;
}

internal u32
Font_GetGlyphIndex(font Font,
                   u32 Codepoint)
{
    u32 GlyphIndex = 0;
    switch(Font.Encoding.Header->Format) {
        case 4: {
            ttf_cmap_subtable_4 *Table = &Font.Encoding.Type4;
            u32 Index;
            u32 Start = 0;
            u32 End = Table->SegCountX2/2;
            while(Start != End) {
                Index = Start+(End-Start)/2;
                if(Table->EndCodes[Index] < Codepoint) Start = Index+1;
                else if(Table->StartCodes[Index] > Codepoint) End = Index;
                else break;
            }
            if(Start != End) {
                u32 Offset = Table->RangeOffsets[Index] / 2;
                if(Offset) {
                    u32 CodepointOffset = Codepoint - Table->StartCodes[Index];
                    GlyphIndex = *(Table->RangeOffsets+Index+Offset+CodepointOffset);
                    if(GlyphIndex) GlyphIndex += Table->Deltas[Index];
                } else {
                    GlyphIndex = Codepoint + Table->Deltas[Index];
                }
            }
            GlyphIndex %= U16_MAX+1;
        } break;
    }
    return GlyphIndex;
}

// internal font_glyph
// Font_CreateGlyph(ttf_glyf_entry *GlyphEntry)
// {
//     font_glyph Glyph = {0};
    
//     if(GlyphEntry->ContourCount > 0) {
//         Glyph.ContourCount = GlyphEntry->ContourCount;
//         SWAPENDIAN16(Glyph.ContourCount);
//         u08 *Data = GlyphEntry->Data;
//         u16 *EndPointsOfContours = (u16*)Data;
//         u32 TotalSegmentCount = EndPointsOfContours[Glyph.ContourCount-1];
//         SWAPENDIAN16(TotalSegmentCount);
//         Data += Glyph.ContourCount*sizeof(u16);
//         u16 InstructionLength = *(u16*)Data;
//         SWAPENDIAN16(InstructionLength);
//         Data += sizeof(u16) + InstructionLength;
        
//         u32 FlagCount = TotalSegmentCount;
//         u08 *Flags = Stack_Allocate(FlagCount);
//         u32 RepeatCount = 0;
//         u08 CurrFlags = 0;
//         for(u32 I = 0; I < FlagCount; I++) {
//             if(!RepeatCount) {
//                 CurrFlags = *Data++;
//                 if(CurrFlags & TTF_GLYF_FLAG_REPEATED)
//                     RepeatCount = *Data++;
//             } else
//                 RepeatCount--;
//             if(!(Flags[I] & TTF_GLYF_FLAG_ON_CURVE))
//                 TotalSegmentCount--;
//             Flags[I] = CurrFlags;
//         }
        
//         s16 CurrX = 0;
//         u32 SI = 0;
//         TotalSegmentCount++;
//         Glyph.Contours = Stack_Allocate((Glyph.ContourCount+1)*sizeof(font_edge*));
//         font_segment *Segments = Stack_Allocate(TotalSegmentCount*sizeof(font_segment));
//         Mem_Set(Segments, 0, TotalSegmentCount*sizeof(font_segment));
//         for(u32 I = 0; I < FlagCount; I++) {
//             if(Flags[I] & TTF_GLYF_FLAG_SHORT_X) {
//                 s16 DX = *Data++;
//                 CurrX += (Flags[I] & TTF_GLYF_FLAG_POS_X) ? DX : -DX;
//             }
//             else if(!(Flags[I] & TTF_GLYF_FLAG_SAME_X)) {
//                 CurrX += Data[1]*256 + Data[0];
//                 Data += 2;
//             }
//             if(Flags[I] & TTF_GLYF_FLAG_ON_CURVE) {
//                 Segments[SI].P2.X = CurrX;
//                 Segments[SI+1].P1.X = CurrX;
//                 SI++;
//             } else {
//                 Segments[SI].C.X = CurrX;
//                 Segments[SI].HasControl = TRUE;
//             }
//         }
        
//         SI = 0;
//         u32 C = 0;
//         u16 LastPointInContour = EndPointsOfContours[C];
//         SWAPENDIAN16(LastPointInContour);
//         s16 CurrY = 0;
//         for(u32 I = 0; I < FlagCount; I++) {
//             if(Flags[I] & TTF_GLYF_FLAG_SHORT_Y) {
//                 s16 DY = *Data++;
//                 CurrY += (Flags[I] & TTF_GLYF_FLAG_POS_Y) ? DY : -DY;
//             }
//             else if(!(Flags[I] & TTF_GLYF_FLAG_SAME_Y)) {
//                 CurrY += Data[1]*256 + Data[0];
//                 Data += 2;
//             }
//             if(I == LastPointInContour) {
//                 LastPointInContour = EndPointsOfContours[C+1];
//                 SWAPENDIAN16(LastPointInContour);
//                 Glyph.Contours[C] = (vptr)(Segments+SI);
//                 C++;
//             }
//             if(Segments[SI].HasControl)
//                 Segments[SI].C.Y = CurrY;
//             else {
//                 Segments[SI].P2.Y = CurrY;
//                 Segments[SI+1].P1.Y = CurrY;
//                 SI++;
//             }
//         }
        
//         C = 0;
//         u32 S = 0;
//         Glyph.Edges = Stack_Allocate(TotalSegmentCount*sizeof(font_edge));
//         Glyph.Edges[0].SegmentCount = 1;
//         Glyph.Edges[0].Segments = Segments;
//         Glyph.EdgeCount = 1;
//         TotalSegmentCount = S;
//         for(S = 0; S < TotalSegmentCount-1; S++) {
//             v2r32 D1, D2;
//             v2s16 Diff;
//             if(Segments[S].HasControl)
//                 Diff = V2s16_Sub(Segments[S].P2, Segments[S].C);
//             else
//                 Diff = V2s16_Sub(Segments[S].P2, Segments[S].P1);
//             D1 = V2s16_Norm(Diff);
//             if(Segments[S+1].HasControl)
//                 Diff = V2s16_Sub(Segments[S+1].C, Segments[S+1].P1);
//             else
//                 Diff = V2s16_Sub(Segments[S+1].P2, Segments[S+1].P1);
//             D2 = V2s16_Norm(Diff);
            
//             #define SIN_ALPHA 0.08715574274765817355806427083747
//             r32 Cross = R32_Abs(V2r32_Cross(D1, D2));
//             r32 Dot = V2r32_Dot(D1, D2);
//             if(Dot < 0 || Cross >= SIN_ALPHA) {
//                 Glyph.Edges[Glyph.EdgeCount].SegmentCount = 0;
//                 Glyph.Edges[Glyph.EdgeCount].Segments = Segments+S+1;
//                 Glyph.EdgeCount++;
//             }
//             Glyph.Edges[Glyph.EdgeCount-1].SegmentCount++;
            
//             if(Segments+S == (font_segment*)Glyph.Contours+C) {
//                 Glyph.Contours[C] = Glyph.Edges+Glyph.EdgeCount-1;
//                 C++;
//             }
//         }
//     } else {
//         Assert(FALSE, "Unsupported");
//     }
    
//     return Glyph;
// }

internal void
Font_CreateMTSDF(u08 *FileData)
{
    Stack_Push();
    font Font = Font_Init(FileData);
    
    u32 GlyphIndex = Font_GetGlyphIndex(Font, 'R');
    
    u32 GlyphOffset;
    if(Font.head->IndexToLocFormat == TTF_LOCA_SHORT_OFFSETS)
        GlyphOffset = Font.loca->Shorts[GlyphIndex] * 2;
    else
        GlyphOffset = Font.loca->Longs[GlyphIndex];
    ttf_glyf_entry *GlyfEntry = (ttf_glyf_entry*)(Font.glyfs + GlyphOffset);
    
    // font_glyph Glyph = Font_CreateGlyph(GlyfEntry);
    
    
    
    Stack_Pop();
}