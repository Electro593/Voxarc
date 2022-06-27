/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct ui_style {
    v2u32 Size;
    u32 ZIndex;
    r32 FontSize;
    v3u08 Color;
    v3u08 BackgroundColor;
} ui_style;

typedef struct ui_font {
    assetpack_asset *CharCache[95];
    
    assetpack Assetpack;
} ui_font;

typedef struct ui_string {
    ui_font *Font;
    
    string String;
    HEAP(u32) Lines;
    
    u32 PrintableCount;
    u32 LineCount;
    
    v2r32 Size;
} ui_string;