/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct ui_font {
    assetpack_asset *CharCache[95];
    
    assetpack Assetpack;
} ui_font;

typedef struct ui_style {
    v2u32 Size;
    v2u32 StringOffset;
    u32 ZIndex;
    r32 FontSize;
    r32 TabSize;
    ui_font *Font;
} ui_style;

typedef struct ui_string {
    ui_style Style;
    
    string String;
    HEAP(u32) Lines;
    
    u32 PrintableCount;
    u32 LineCount;
    
    v2r32 Size;
} ui_string;