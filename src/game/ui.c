/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal assetpack_asset *
GetGlyph(ui_font *Font, c08 Codepoint)
{
    u32 I = Codepoint - ' ';
    assetpack_asset *Asset = Font->CharCache[I];
    
    if(!Asset) {
        assetpack_tag *Tag = Assetpack_FindExactTag(Font->Assetpack, TAG_CODEPOINT, (vptr)(u64)Codepoint);
        if(Tag && Tag->AssetCount)
            Asset = Font->CharCache[I] = Tag->Assets[0];
    }
    
    return Asset;
}

internal ui_string
MakeUIString(heap *Heap, string String, ui_style Style)
{
    ui_string UIStr;
    UIStr.Size = (v2r32){0};
    UIStr.Style = Style;
    UIStr.String = String;
    UIStr.LineCount = 1;
    UIStr.PrintableCount = 0;
    
    UIStr.Lines = Heap_Allocate(Heap, 4*sizeof(u32));
    ((u32*)UIStr.Lines->Data)[0] = 0;
    
    if(!String.Length) return UIStr;
    
    ui_font *Font = Style.Font;
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Font->Assetpack, TAG_FONT_NAME);
    Assert(Tag && Tag->AssetCount);
    assetpack_font *FontData = &Tag->Assets[0]->Font;
    
    v2r32 P = {0, Style.FontSize * (FontData->Ascent-FontData->Descent)};
    r32 MaxX = 0;
    
    r32 AdvanceY = Style.FontSize * (FontData->Ascent-FontData->Descent+FontData->LineGap);
    
    u32 LastWordBreakI = 0;
    u32 LastWordBreakX = 0;
    b08 BrokeOnCurrentLine = FALSE;
    
    u32 I = 0;
    for(; I < String.Length; I++) {
        c08 C = String.Text[I];
        
        r32 AdvanceX = 0;
        
        if('!' <= C && C <= '~') {
            assetpack_asset *Asset = GetGlyph(Style.Font, C);
            Assert(Asset);
            
            AdvanceX = Style.FontSize * Asset->Glyph.AdvanceX;
            UIStr.PrintableCount++;
        } else if(C == ' ') {
            assetpack_asset *Asset = GetGlyph(Style.Font, ' ');
            Assert(Asset);
            
            AdvanceX = Style.FontSize * Asset->Glyph.AdvanceX;
        } else if(C == '\t') {
            u32 NumTabStops = P.X / Style.TabSize + 1;
            r32 NextTabStop = Style.TabSize * NumTabStops;
            AdvanceX = NextTabStop - P.X;
        } else if(C == '\n') {
            if(P.Y + AdvanceY > Style.Size.Y)
                break;
            
            P.Y += AdvanceY;
            P.X = 0;
            
            if((UIStr.LineCount+1) * sizeof(u32) > UIStr.Lines->Size)
                Heap_Resize(UIStr.Lines, UIStr.Lines->Size + 4*sizeof(u32));
            ((u32*)UIStr.Lines->Data)[UIStr.LineCount] = I+1;
            UIStr.LineCount++;
            
            BrokeOnCurrentLine = FALSE;
            
            continue;
        } else if(C == '\r') {
            continue;
        } else if(C == '\0') {
            break;
        } else Assert(FALSE, "Unsupported character!");
        
        if(P.X + AdvanceX > Style.Size.X) {
            if(P.Y + AdvanceY > Style.Size.Y)
                break;
            
            if(BrokeOnCurrentLine) {
                if((UIStr.LineCount+1) * sizeof(u32) > UIStr.Lines->Size)
                    Heap_Resize(UIStr.Lines, UIStr.Lines->Size + 4*sizeof(u32));
                ((u32*)UIStr.Lines->Data)[UIStr.LineCount] = LastWordBreakI;
                UIStr.LineCount++;
                
                P.Y += AdvanceY;
                P.X = P.X + AdvanceX - LastWordBreakX;
                
                BrokeOnCurrentLine = FALSE;
            } else {
                if((UIStr.LineCount+1) * sizeof(u32) > UIStr.Lines->Size)
                    Heap_Resize(UIStr.Lines, UIStr.Lines->Size + 4*sizeof(u32));
                ((u32*)UIStr.Lines->Data)[UIStr.LineCount] = I+1;
                UIStr.LineCount++;
                
                P.Y += AdvanceY;
                P.X = AdvanceX;
            }
        } else {
            P.X += AdvanceX;
        }
        
        if(C == ' ' || C == '-' || C == '\t') {
            LastWordBreakI = I+1;
            LastWordBreakX = P.X;
            BrokeOnCurrentLine = TRUE;
        }
        
        if(MaxX < P.X) MaxX = P.X;
    }
    
    if((UIStr.LineCount+1) * sizeof(u32) > UIStr.Lines->Size)
        Heap_Resize(UIStr.Lines, UIStr.Lines->Size + 4*sizeof(u32));
    ((u32*)UIStr.Lines->Data)[UIStr.LineCount] = I;
    
    UIStr.Size.X = MaxX;
    UIStr.Size.Y = P.Y;
    
    return UIStr;
}

internal mesh_object
MakeUIStringObject(heap *Heap, ui_string UIStr, v2u32 Pos, v2u32 ViewSize)
{
    mesh_object Object;
    
    Object.Vertices = Heap_Allocate(Heap, 4*UIStr.PrintableCount*sizeof(glyph_vertex));
    Object.Indices  = Heap_Allocate(Heap, 6*UIStr.PrintableCount*sizeof(u32));
    
    Object.RotationMatrix    = M4x4r32_I;
    
    if(!UIStr.PrintableCount) {
        Object.TranslationMatrix = M4x4r32_I;
        Object.ScalingMatrix     = M4x4r32_I;
        return Object;
    }
    
    ui_style Style = UIStr.Style;
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Style.Font->Assetpack, TAG_FONT_NAME);
    Assert(Tag && Tag->AssetCount);
    assetpack_font *FontData = &Tag->Assets[0]->Font;
    
    glyph_vertex *Vertex = Object.Vertices->Data;
    u32          *Index  = Object.Indices->Data;
    
    u32 *Lines = UIStr.Lines->Data;
    
    v2r32 P = {0, UIStr.Size.Y - Style.FontSize*FontData->Ascent};
    r32 AdvanceY = Style.FontSize * (FontData->Ascent-FontData->Descent+FontData->LineGap);
    
    u32 PI = 0;
    
    for(u32 L = 0; L < UIStr.LineCount; L++) {
        P.X = 0;
        
        for(u32 I = Lines[L]; I < Lines[L+1]; I++) {
            c08 C = UIStr.String.Text[I];
            
            if('!' <= C && C <= '~') {
                assetpack_asset *Asset = GetGlyph(Style.Font, C);
                
                u32 Bytes = (u64)Asset - (u64)Style.Font->Assetpack.Assets;
                
                v2r32 Q = V2r32_Add(P, V2r32_MulS(Asset->Glyph.Bearing, Style.FontSize));
                v2r32 S = V2r32_MulS(Asset->Glyph.SizeR, Style.FontSize);
                
                s16 PXL = ((Q.X    )*2/UIStr.Size.X - 1) * 0x7FFF;
                s16 PXR = ((Q.X+S.X)*2/UIStr.Size.X - 1) * 0x7FFF;
                s16 PYB = ((Q.Y    )*2/UIStr.Size.Y - 1) * 0x7FFF;
                s16 PYT = ((Q.Y+S.Y)*2/UIStr.Size.Y - 1) * 0x7FFF;
                
                *Vertex++ = (glyph_vertex){{PXL, PYB}, (Style.ZIndex<<24) | Bytes | 0b00};
                *Vertex++ = (glyph_vertex){{PXR, PYB}, (Style.ZIndex<<24) | Bytes | 0b01};
                *Vertex++ = (glyph_vertex){{PXR, PYT}, (Style.ZIndex<<24) | Bytes | 0b11};
                *Vertex++ = (glyph_vertex){{PXL, PYT}, (Style.ZIndex<<24) | Bytes | 0b10};
                
                *Index++ = 4*PI + 0;
                *Index++ = 4*PI + 1;
                *Index++ = 4*PI + 2;
                *Index++ = 4*PI + 0;
                *Index++ = 4*PI + 2;
                *Index++ = 4*PI + 3;
                
                P.X += Style.FontSize * Asset->Glyph.AdvanceX;
                
                PI++;
            } else if(C == ' ') {
                assetpack_asset *Asset = GetGlyph(Style.Font, ' ');
                
                P.X += Style.FontSize * Asset->Glyph.AdvanceX;
            } else if(C == '\t') {
                u32 NumTabStops = P.X / Style.TabSize + 1;
                r32 NextTabStop = Style.TabSize * NumTabStops;
                P.X = NextTabStop;
            }
        }
        
        P.Y -= AdvanceY;
    }
    
    P.Y -= -FontData->Descent * Style.FontSize;
    
    Object.TranslationMatrix = M4x4r32_Translation(
        -1 + (Style.StringOffset.X + Pos.X + UIStr.Size.X)/ViewSize.X,
         1 - (Style.StringOffset.Y + Pos.Y + UIStr.Size.Y)/ViewSize.Y,
         0);
    Object.ScalingMatrix = M4x4r32_Scaling(
        UIStr.Size.X/ViewSize.X,
        UIStr.Size.Y/ViewSize.Y,
        1);
    
    return Object;
}