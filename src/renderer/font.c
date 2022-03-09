/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// #define STBTT_pow(x,y) x
// #define STBTT_fmod(x,y) x
// #define STBTT_cos(x) x
// #define STBTT_acos(x) x
// #define STBTT_assert(x) Assert(x)
// #define STBTT_strlen(x) Mem_BytesUntil(x, 0)
// #define STBTT_ifloor(x) (s32)R32_Floor(x)
// #define STBTT_iceil(x) (s32)R32_Ceil(x)
// #define STBTT_sqrt(x) R32_sqrt(x)
// #define STBTT_fabs(x) R32_Abs(x)
// #define STBTT_malloc(x,u) Stack_Allocate(x)
// #define STBTT_free(x,u)
// #define STBTT_memcpy Mem_Cpy
// #define STBTT_memset Mem_Set
// #define size_t u64
// #define STB_TRUETYPE_IMPLEMENTATION
// #include <libraries/stb_truetype.h>
// #undef STBTT_pow
// #undef STBTT_fmod
// #undef STBTT_cos
// #undef STBTT_acos
// #undef STBTT_fabs
// #undef STBTT_malloc
// #undef STBTT_free
// #undef STBTT_assert
// #undef STBTT_strlen
// #undef STBTT_memcpy
// #undef STBTT_memset
// #undef STB_TRUETYPE_IMPLEMENTATION

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

internal u32
Font_GetGlyphOffset(font Font,
                    u32 GlyphIndex)
{
    if(Font.head->IndexToLocFormat == TTF_LOCA_SHORT_OFFSETS)
        return Font.loca->Shorts[GlyphIndex] * 2;
    else
        return Font.loca->Longs[GlyphIndex];
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
    for(u32 I = 0; I < Font->maxp->GlyphCount; I++) {
        u32 GlyphOffset = Font_GetGlyphOffset(*Font, I);
        u32 NextOffset = Font_GetGlyphOffset(*Font, I+1);
        if(NextOffset == GlyphOffset) continue;
        
        ttf_glyph *Glyph = (ttf_glyph*)(Font->glyfs + GlyphOffset);
        SWAPENDIAN16(Glyph->ContourCount);
        SWAPENDIAN16(Glyph->XMin);
        SWAPENDIAN16(Glyph->YMin);
        SWAPENDIAN16(Glyph->XMax);
        SWAPENDIAN16(Glyph->YMax);
        
        u08 *Data = Glyph->Data;
        if(Glyph->ContourCount > 0) {
            for(u16 C = 0; C < Glyph->ContourCount; C++) {
                SWAPENDIAN16(*(u16*)Data);
                Data += 2;
            }
            
            SWAPENDIAN16(*(u16*)Data);
            Data += 2;
        } else if(Glyph->ContourCount < 0) {
            Assert(I < ' '-29 || I > '~'-29);
        }
    }
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

#define SIN_ALPHA 0.08715574274765817355806427083747
internal void
Font_FindSegments(msdf_shape *Shape,
                  u08 *Data)
{
    u16 *EndPointsOfContours = (u16*)Data;
    Data += Shape->ContourCount * sizeof(u16);
    u16 InstructionLength = *(u16*)Data;
    Data += 2;
    Data += InstructionLength;
    
    u32 VertexCount = EndPointsOfContours[Shape->ContourCount-1]+1;
    Shape->SegmentCount = VertexCount;
    u08 *Flags = Stack_Allocate(Shape->SegmentCount);
    
    u08 Flag;
    u32 RepeatCount = 0;
    for(u32 F = 0; F < Shape->SegmentCount; F++) {
        if(RepeatCount == 0) {
            Flag = *Data++;
            if(Flag & TTF_FLAG_REPEATED) RepeatCount = *Data++;
        } else RepeatCount--;
        Flags[F] = Flag;
        if(!(Flag & TTF_VERTEX_ON_CURVE)) Shape->SegmentCount--;
    }
    
    Shape->Segments = Stack_Allocate(Shape->SegmentCount * sizeof(msdf_segment));
    Mem_Set(Shape->Segments, 0, Shape->SegmentCount*sizeof(msdf_segment));
    
    v2s32 Pos = {0};
    for(u32 V = 0; V < Shape->SegmentCount; V++) {
        if(Flags[V] & TTF_VERTEX_SHORT_X) {
            s16 Delta = *Data++;
            Pos.X += (Flags[V] & TTF_VERTEX_POS_X) ? Delta : -Delta;
        } else if(!(Flags[V] & TTF_VERTEX_SAME_X)) {
            Pos.X += (s16)_SWAPENDIAN16(*(u16*)Data);
            Data += 2;
        }
        if(Flags[V] & TTF_VERTEX_ON_CURVE)
            Shape->Segments[V].P1.X = Pos.X;
        else {
            V--;
            Shape->Segments[V].C1.X = Pos.X;
            Shape->Segments[V].CPCount++;
        }
    }
    for(u32 V = 0; V < Shape->SegmentCount; V++) {
        if(Flags[V] & TTF_VERTEX_SHORT_Y) {
            s16 Delta = *Data++;
            Pos.Y += (Flags[V] & TTF_VERTEX_POS_Y) ? Delta : -Delta;
        } else if(!(Flags[V] & TTF_VERTEX_SAME_Y)) {
            Pos.Y += (s16)_SWAPENDIAN16(*(u16*)Data);
            Data += 2;
        }
        if(Flags[V] & TTF_VERTEX_ON_CURVE)
            Shape->Segments[V].P1.Y = Pos.Y;
        else {
            V--;
            Shape->Segments[V].C1.Y = Pos.Y;
            Shape->Segments[V].CPCount++;
        }
    }
    
    u32 C=0, E=0, V=0;
    Shape->Contours = Stack_Allocate(Shape->ContourCount*sizeof(msdf_contour));
    Shape->Edges = Stack_Allocate((Shape->ContourCount+Shape->SegmentCount)*sizeof(msdf_edge));
    Shape->EdgeCount = 0;
    for(; C < Shape->ContourCount; C++) {
        u32 StartV=V, StartE=E, StartEV=V;
        Shape->Contours[C].Edges = Shape->Edges+E;
        Shape->Edges[E].Segments = Shape->Segments+V;
        Shape->Edges[E].Color = 0b101;
        
        for(; V < EndPointsOfContours[C]; V++)
            Shape->Segments[V].P2 = Shape->Segments[V+1].P1;
        Shape->Segments[V].P2 = Shape->Segments[StartV].P1;
        
        r32 Cross, Dot;
        v2r32 FirstDir, PrevDir, CurrDir, NextDir;
        for(V = StartV; V <= EndPointsOfContours[C]; V++) {
            if(Shape->Segments[V].CPCount == 0) {
                CurrDir = V2r32_Sub(Shape->Segments[V].P2, Shape->Segments[V].P1);
                NextDir = CurrDir;
            } else {
                CurrDir = V2r32_Sub(Shape->Segments[V].C1, Shape->Segments[V].P1);
                NextDir = V2r32_Sub(Shape->Segments[V].P2, Shape->Segments[V].C1);
            }
            
            if(V != StartV) {
                PrevDir = V2r32_Norm(PrevDir);
                CurrDir = V2r32_Norm(CurrDir);
                Cross = V2r32_Cross(PrevDir, CurrDir);
                Dot = V2r32_Dot(PrevDir, CurrDir);
                
                if(Dot < 0 || R32_Abs(Cross) >= SIN_ALPHA) {
                    Shape->Edges[E].SegmentCount = V - StartEV;
                    StartEV = V;
                    E++;
                    Shape->Edges[E].Segments = Shape->Segments+V;
                    
                    if(Shape->Edges[E-1].Color == 0b110)
                        Shape->Edges[E].Color = 0b011;
                    else
                        Shape->Edges[E].Color = 0b110;
                }
            } else FirstDir = CurrDir;
            
            PrevDir = NextDir;
        }
        
        Shape->Edges[E].SegmentCount = V - StartEV;
        E++;
        Shape->Contours[C].EdgeCount = E - StartE;
        
        if(Shape->Contours[C].EdgeCount == 1) {
            CurrDir = V2r32_Norm(CurrDir);
            FirstDir = V2r32_Norm(FirstDir);
            Cross = V2r32_Cross(CurrDir, FirstDir);
            Dot = V2r32_Dot(CurrDir, PrevDir);
            if(Dot < 0 || R32_Abs(Cross) >= SIN_ALPHA) {
                u32 OldSegCount = Shape->Edges[E].SegmentCount;
                u32 NewSegCount = Shape->Edges[E].SegmentCount / 2;
                Shape->Edges[E].SegmentCount = NewSegCount;
                Shape->Edges[E+1].SegmentCount = OldSegCount - NewSegCount;
                Shape->Edges[E+1].Segments = Shape->Segments+StartEV+NewSegCount;
                Shape->Edges[E+1].Color = 0b110;
                E++;
            } else Shape->Contours[C].Edges[0].Color = 0b111;
        }
    }
}
#undef SIN_ALPHA

internal font_glyph
Font_GetGlyph(font Font,
              u32 Codepoint,
              r32 Scale/*,
              stbtt_fontinfo FontInfo*/)
{
    font_glyph Glyph = {0};
    u32 GlyphIndex = Font_GetGlyphIndex(Font, Codepoint);
    
    if(GlyphIndex < Font.hhea->HMetricCount) {
        Glyph.Advance = Font.hmtx.HMetrics[GlyphIndex].AdvanceX;
        Glyph.Bearing.X = Font.hmtx.HMetrics[GlyphIndex].LeftBearing;
    } else {
        Glyph.Advance = Font.hmtx.HMetrics[Font.hhea->HMetricCount-1].AdvanceX;
        Glyph.Bearing.X = Font.hmtx.ExtraBearings[GlyphIndex - Font.hhea->HMetricCount];
    }
    Glyph.Advance = (s32)((r32)Glyph.Advance * Scale);
    Glyph.Bearing.X = (s32)((r32)Glyph.Bearing.X * Scale);
    
    u32 Offset = Font_GetGlyphOffset(Font, GlyphIndex);
    u32 NextOffset = Font_GetGlyphOffset(Font, GlyphIndex+1);
    ttf_glyph *GlyphData = (ttf_glyph*)(Font.glyfs+Offset);
    
    // Glyph.VertexCount = stbtt_GetGlyphShape(&FontInfo, GlyphIndex, &Glyph.Vertices);
    // Glyph.Shape = MSDF_MakeShape(Glyph.Vertices, Glyph.VertexCount);
    
    if(NextOffset == Offset) {
        Glyph.Bearing.Y = 0;
        Glyph.Size = (v2u32){0};
    } else {
        Glyph.Shape.Bounds.X = (s32)R32_Floor( GlyphData->XMin * Scale);
        Glyph.Shape.Bounds.Y = (s32)R32_Floor(-GlyphData->YMax * Scale);
        Glyph.Shape.Bounds.Z = (s32)R32_Ceil ( GlyphData->XMax * Scale);
        Glyph.Shape.Bounds.W = (s32)R32_Ceil (-GlyphData->YMin * Scale);
        Glyph.Bearing.Y = -Glyph.Shape.Bounds.W;
        Glyph.Size = (v2u32){Glyph.Shape.Bounds.Z - Glyph.Shape.Bounds.X,
                             Glyph.Shape.Bounds.W - Glyph.Shape.Bounds.Y};
        
        msdf_shape *Shape = &Glyph.Shape;
        Shape->ContourCount = GlyphData->ContourCount;
        Assert(Codepoint != '$');
        Font_FindSegments(Shape, GlyphData->Data);
        // Shape.Bounds = MSDF_FindEdges(Shape, Vertices, VertexCount);
        // MSDF_AssignColors(*Shape);
        
        // Glyph.Shape = MSDF_MakeShape(Vertices, VertexCount);
    }
    
    return Glyph;
}