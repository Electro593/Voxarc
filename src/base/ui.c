/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef INCLUDE_HEADER

typedef struct ui_font {
   assetpack_asset *CharCache[95];
   
   assetpack Assetpack;
} ui_font;

typedef struct ui_style {
   b08 Enabled;
   b08 Visible;
   v2u32 Size;
   v4u32 Padding;
   u32 ZIndex;
   r32 FontSize;
   r32 TabSize;
   v4u08 BackgroundColor;
   v4u08 BorderColor;
   v4u32 BorderSize;
   ui_font *Font;
} ui_style;

#define DEFAULT_STYLE                         \
   (ui_style){                                \
      TRUE,                                   \
      TRUE,                                   \
      { 0, 0 },                               \
      { (u32)-1, (u32)-1, (u32)-1, (u32)-1 }, \
      (u32)-1,                                \
      R32_NINF,                               \
      R32_NINF,                               \
      { (u08)-1, (u08)-1, (u08)-1, 0 },       \
      { (u08)-1, (u08)-1, (u08)-1, 0 },       \
      { (u32)-1, (u32)-1, (u32)-1, (u32)-1 }, \
      NULL                                    \
   }

typedef struct ui_string {
   ui_style Style;
   
   string String;
   HEAP(u32) Lines;
   
   u32 PrintableCount;
   u32 LineCount;
   
   v2r32 Size;
   r32 OffsetX;
} ui_string;

typedef struct ui_component {
   struct ui_component *Parent;
   
   ui_style Style;
   string String;
   
   mesh_object RenderedString;
   mesh_object RenderedComponent;
   
   b08 DirtyString;
   b08 DirtyComponent;
} ui_component;

#define DEFAULT_COMPONENT \
   (ui_component){        \
      NULL,               \
      DEFAULT_STYLE,      \
      EString(),          \
      {0},                \
      {0},                \
      TRUE,               \
      TRUE                \
   }

#define UI_FUNCS \
   EXPORT(ui_string, MakeUIString, heap *Heap, string String, ui_style Style) \
   EXPORT(mesh_object, MakeUIStringObject, heap *Heap, ui_string UIStr, v2u32 Pos, v2u32 ViewSize) \
   EXPORT(void, FreeUIString, ui_string String) \
   EXPORT(ui_style, GetInheritedStyle, ui_component *Component) \

#endif



#ifdef INCLUDE_SOURCE

internal assetpack_asset *
GetGlyph(
   ui_font *Font,
   c08 Codepoint)
{
   u32 I = Codepoint - ' ';
   assetpack_asset *Asset = Font->CharCache[I];
   
   if(!Asset) {
      Asset = FindFirstAssetFromExactTag(Font->Assetpack, TAG_CODEPOINT, &Codepoint);
      Font->CharCache[I] = Asset;
      // assetpack_tag *Tag = Assetpack_FindExactTag(Font->Assetpack, TAG_CODEPOINT, (vptr)(u64)Codepoint);
      // if(Tag && Tag->AssetCount)
      //     Asset = Font->CharCache[I] = Tag->Assets[0];
   }
   
   return Asset;
}

internal ui_style
GetInheritedStyle(
   ui_component *Component)
{
   if(!Component) return DEFAULT_STYLE;
   
   ui_style Style = Component->Style;
   while(Component = Component->Parent) {
      ui_style CStyle = Component->Style;
      
      if(Style.Enabled                     == DEFAULT_STYLE.Enabled)          Style.Enabled         = CStyle.Enabled;
      if(Style.Size.X                      == DEFAULT_STYLE.Size.X)           Style.Size.X          = CStyle.Size.X;
      if(Style.Size.Y                      == DEFAULT_STYLE.Size.Y)           Style.Size.Y          = CStyle.Size.Y;
      if(Style.Padding.X                   == DEFAULT_STYLE.Padding.X)        Style.Padding.X       = CStyle.Padding.X;
      if(Style.Padding.Y                   == DEFAULT_STYLE.Padding.Y)        Style.Padding.Y       = CStyle.Padding.Y;
      if(Style.Padding.Z                   == DEFAULT_STYLE.Padding.Z)        Style.Padding.Z       = CStyle.Padding.Z;
      if(Style.Padding.W                   == DEFAULT_STYLE.Padding.W)        Style.Padding.W       = CStyle.Padding.W;
      if(Style.ZIndex                      == DEFAULT_STYLE.ZIndex)           Style.ZIndex          = CStyle.ZIndex;
      if(Style.FontSize                    == DEFAULT_STYLE.FontSize)         Style.FontSize        = CStyle.FontSize;
      if(Style.TabSize                     == DEFAULT_STYLE.TabSize)          Style.TabSize         = CStyle.TabSize;
      if(V4u08_IsEqual(Style.BackgroundColor, DEFAULT_STYLE.BackgroundColor)) Style.BackgroundColor = CStyle.BackgroundColor;
      if(V4u08_IsEqual(Style.BorderColor,     DEFAULT_STYLE.BorderColor))     Style.BorderColor     = CStyle.BorderColor;
      if(Style.BorderSize.X                == DEFAULT_STYLE.BorderSize.X)     Style.BorderSize.X    = CStyle.BorderSize.X;
      if(Style.BorderSize.Y                == DEFAULT_STYLE.BorderSize.Y)     Style.BorderSize.Y    = CStyle.BorderSize.Y;
      if(Style.BorderSize.Z                == DEFAULT_STYLE.BorderSize.Z)     Style.BorderSize.Z    = CStyle.BorderSize.Z;
      if(Style.BorderSize.W                == DEFAULT_STYLE.BorderSize.W)     Style.BorderSize.W    = CStyle.BorderSize.W;
      if(Style.Font                        == DEFAULT_STYLE.Font)             Style.Font            = CStyle.Font;
   }
   
   return Style;
}

internal ui_string
MakeUIString(
   heap *Heap,
   string String,
   ui_style Style)
{
   ui_string UIStr;
   UIStr.Size = (v2r32){0};
   UIStr.OffsetX = R32_INF;
   UIStr.Style = Style;
   UIStr.String = String;
   UIStr.LineCount = 1;
   UIStr.PrintableCount = 0;
   
   UIStr.Lines = Heap_Allocate(Heap, 4*sizeof(u32));
   ((u32*)UIStr.Lines->Data)[0] = 0;
   
   if(!String.Length) return UIStr;
   
   ui_font *Font = Style.Font;
   
   assetpack_font FontData = FindFirstAssetFromExactTag(Font->Assetpack, TAG_FONT_DESC, &(u32){0})->Font;
   
   v2r32 P = {0, Style.FontSize * (FontData.Ascent-FontData.Descent)};
   
   r32 AdvanceY = Style.FontSize * (FontData.Ascent-FontData.Descent+FontData.LineGap);
   
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
         r32 BearingX = Style.FontSize * Asset->Glyph.Bearing.X;
         if(!P.X && BearingX < UIStr.OffsetX) UIStr.OffsetX = BearingX;
         
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
         if(UIStr.Size.X < P.X) UIStr.Size.X = P.X;
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
      
      if(UIStr.Size.X < P.X) UIStr.Size.X = P.X;
   }
   
   if((UIStr.LineCount+1) * sizeof(u32) > UIStr.Lines->Size)
      Heap_Resize(UIStr.Lines, UIStr.Lines->Size + 4*sizeof(u32));
   ((u32*)UIStr.Lines->Data)[UIStr.LineCount] = I;
   
   if(UIStr.OffsetX == R32_INF) UIStr.OffsetX = 0;
   if(UIStr.Size.X < P.X) UIStr.Size.X = P.X;
   UIStr.Size.X -= UIStr.OffsetX;
   UIStr.Size.Y = P.Y;
   
   return UIStr;
}

internal mesh_object
MakeUIStringObject(
   heap *Heap,
   ui_string UIStr,
   v2u32 Pos,
   v2u32 ViewSize)
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
   
   //TODO: Cache the font data
   assetpack_font FontData = FindFirstAssetFromExactTag(Style.Font->Assetpack, TAG_FONT_DESC, &(u32){0})->Font;
   
   glyph_vertex *Vertex = Object.Vertices->Data;
   u32          *Index  = Object.Indices->Data;
   
   u32 *Lines = UIStr.Lines->Data;
   
   v2r32 P = {0, UIStr.Size.Y - Style.FontSize*FontData.Ascent};
   r32 AdvanceY = Style.FontSize * (FontData.Ascent-FontData.Descent+FontData.LineGap);
   
   u32 PI = 0;
   
   for(u32 L = 0; L < UIStr.LineCount; L++) {
      P.X = 0;
      
      for(u32 I = Lines[L]; I < Lines[L+1]; I++) {
         c08 C = UIStr.String.Text[I];
         
         if('!' <= C && C <= '~') {
            assetpack_asset *Asset = GetGlyph(Style.Font, C);
            
            u32 Bytes = (u64)Asset - (u64)Style.Font->Assetpack.Assets;
            
            v2r32 Q = V2r32_Add(P, V2r32_MulS(Asset->Glyph.Bearing, Style.FontSize));
            Q.X -= UIStr.OffsetX;
            v2r32 S = V2r32_MulS(Asset->Glyph.SizeR, Style.FontSize);
            if(!P.X) S.X += UIStr.OffsetX;
            
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
   
   P.Y -= -FontData.Descent * Style.FontSize;
   
   Object.TranslationMatrix = M4x4r32_Translation(
     -1 + (Style.Padding.X + Pos.X + UIStr.OffsetX + UIStr.Size.X) / ViewSize.X,
      1 - (Style.Padding.Y + Pos.Y + UIStr.Size.Y) / ViewSize.Y,
      0
   );
   
   Object.ScalingMatrix = M4x4r32_Scaling(
      UIStr.Size.X/ViewSize.X,
      UIStr.Size.Y/ViewSize.Y,
      1
   );
   
   return Object;
}

internal void
FreeUIString(
   ui_string String)
{
   Heap_Free(String.Lines);
}

#endif