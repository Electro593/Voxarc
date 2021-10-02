/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_ASSET_H_
#define GAME_ASSET_H_

typedef enum ttf_platform_id {
    TTFPlatformID_Unicode   = 0,
    TTFPlatformID_Macintosh = 1,
    TTFPlatformID_ISO       = 2,
    TTFPlatformID_Windows   = 3,
} ttf_platform_id;

typedef enum ttf_encoding_id {
    TTFEncodingID_Unicode_10          =  0,
    TTFEncodingID_Unicode_11          =  1,
    TTFEncodingID_Unicode_10646       =  2,
    TTFEncodingID_Unicode_20          =  3,
    TTFEncodingID_Unicode_20Full      =  4,
    TTFEncodingID_Unicode_Variation   =  5,
    TTFEncodingID_Unicode_Full        =  6,
    
    TTFEncodingID_ISO_7BitASCII       =  0,
    TTFEncodingID_ISO_10646           =  1,
    TTFEncodingID_ISO_8859_1          =  2,
    
    TTFEncodingID_Windows_Symbol      =  0,
    TTFEncodingID_Windows_UnicodeBMP  =  1,
    TTFEncodingID_Windows_ShiftJIS    =  2,
    TTFEncodingID_Windows_PRC         =  3,
    TTFEncodingID_Windows_Big5        =  4,
    TTFEncodingID_Windows_Wansung     =  5,
    TTFEncodingID_Windows_Johab       =  6,
    TTFEncodingID_Windows_UnicodeFull = 10,
} ttf_encoding_id;

typedef enum ttf_glyf_glyph_simple_flag_bit {
    TTFSimpleGlyphFlagBit_OnCurve       = 1 << 0,
    TTFSimpleGlyphFlagBit_ShortVectorX  = 1 << 1,
    TTFSimpleGlyphFlagBit_ShortVectorY  = 1 << 2,
    TTFSimpleGlyphFlagBit_Repeat        = 1 << 3,
    TTFSimpleGlyphFlagBit_SameX         = 1 << 4,
    TTFSimpleGlyphFlagBit_SameY         = 1 << 5,
    TTFSimpleGlyphFlagBit_SimpleOverlap = 1 << 6,
} ttf_glyf_glyph_simple_flag_bit;

typedef enum ttf_glyf_glyph_compound_flag_bit {
    TTFCompoundGlyphFlagBit_ArgSize                 = 1 <<  0,
    TTFCompoundGlyphFlagBit_ArgsAreOffsets          = 1 <<  1,
    TTFCompoundGlyphFlagBit_RoundOffsets            = 1 <<  2,
    TTFCompoundGlyphFlagBit_HasScale                = 1 <<  3,
    TTFCompoundGlyphFlagBit_MoreComponents          = 1 <<  5,
    TTFCompoundGlyphFlagBit_DiffScales              = 1 <<  6,
    TTFCompoundGlyphFlagBit_2X2Transform            = 1 <<  7,
    TTFCompoundGlyphFlagBit_HasInstructions         = 1 <<  8,
    TTFCompoundGlyphFlagBit_UseTheseMetrics         = 1 <<  9,
    TTFCompoundGlyphFlagBit_OverlapCompound         = 1 << 10,
    TTFCompoundGlyphFlagBit_ScaledComponentOffset   = 1 << 11,
    TTFCompoundGlyphFlagBit_UnscaledComponentOffset = 1 << 12,
} ttf_glyf_glyph_compound_flag_bit;

typedef enum ttf_kern_table_header_coverage_mask {
    TTFKernTableHeaderCoverageMask_Horizontal    = 0x0001,
    TTFKernTableHeaderCoverageMask_Minimum       = 0x0002,
    TTFKernTableHeaderCoverageMask_Perpendicular = 0x0004,
    TTFKernTableHeaderCoverageMask_Override      = 0x0008,
    TTFKernTableHeaderCoverageMask_Reserved      = 0x00F0,
    TTFKernTableHeaderCoverageMask_Format        = 0xFF00,
} ttf_kern_table_header_coverage_mask;

typedef struct ttf_fixed {
    u16 Major;
    u16 Minor;
} ttf_fixed;

typedef struct ttf                 ttf;
typedef struct ttf_offset_table    ttf_offset_table;
typedef struct ttf_dir_table_entry ttf_dir_table_entry;
typedef struct ttf_cmap            ttf_cmap;
typedef struct ttf_glyf            ttf_glyf;
typedef struct ttf_head            ttf_head;
typedef struct ttf_hhea            ttf_hhea;
typedef struct ttf_hmtx            ttf_hmtx;
typedef struct ttf_kern            ttf_kern;
typedef struct ttf_loca            ttf_loca;
typedef struct ttf_maxp            ttf_maxp;

#pragma pack(push,1)
struct ttf {
    struct ttf_offset_table {
        ttf_fixed SFNTVersion;
        u16 NumTables;
        u16 SearchRange;
        u16 EntrySelector;
        u16 RangeShift;
    } OffsetTable;
    
    struct ttf_dir_table_entry {
        chr Tag[4];
        u32 Checksum;
        u32 Offset;
        u32 Length;
    } *DirTableEntries;
    
    struct ttf_cmap {
        u16 VersionNumber;
        u16 EncodingTableCount;
        struct ttf_cmap_subtable_header {
            u16 PlatformID;
            u16 EncodingID;
            u32 SubtableOffset;
        } *Tables;
    } *cmap;
    
    struct ttf_glyf {
        s16 NumberOfContours;
        s16 XMin;
        s16 YMin;
        s16 XMax;
        s16 YMax;
        union ttf_glyf_glyph {
            struct ttf_glyf_glyph_simple {
                u16 *EndPoints;
                u16 InstructionLength;
                u08 *Instructions;
                union ttf_glyf_glyph_simple_flag {
                    flag08 Flag;
                    u08 RepeatedValue;
                } *Flags;
                u08 *XCoords;
                u08 *YCoords;
            } S;
            struct ttf_glyf_glyph_compound {
                u16 Flags;
                u16 GlyphIndex;
                union {
                    s16 SS;
                    u16 US;
                    s32 SL;
                    u32 UL;
                } Arg[2];
                vptr TransformOption;
            } C;
        } *Glyphs;
    } *glyf;
    
    struct ttf_head {
        ttf_fixed Version;
        ttf_fixed FontRevision;
        u32 ChecksumAdjustment;
        u32 MagicNumber;
        flag16 Flags;
        u16 UnitsPerEm;
        u64 Created;
        u64 Modified;
        s16 XMin;
        s16 YMin;
        s16 XMax;
        s16 YMax;
        u16 MacStyle;
        u16 LowestRecPPEM;
        s16 FontDirectionHint;
        s16 locaFormat;
        s16 GlyphDataFormat;
    } *head;
    
    struct ttf_hhea {
        ttf_fixed Version;
        s16 Ascent;
        s16 Descent;
        s16 LineGap;
        u16 AdvanceWidthMax;
        s16 MinLeftSideBearing;
        s16 MinRightSideBearing;
        s16 XMaxExtent;
        s16 CaretSlopeRise;
        s16 CaretSlopeRun;
        s16 CaretOffset;
        s16 Reserved[4];
        s16 MetricDataFormat;
        u16 LongHMetricCount;
    } *hhea;
    
    struct ttf_hmtx {
        struct ttf_hmtx_metric {
            u16 AdvanceWidth;
            s16 LeftSideBearing;
        } *HMetrics;
        s16 *LeftSideBearings;
    } *hmtx;
    
    struct ttf_kern {
        u16 Version;
        u16 TableCount;
        struct ttf_kern_table {
            struct ttf_kern_table_header {
                u16 Version;
                u16 Length;
                u16 Coverage;
            } Header;
            union {
                struct ttf_kern_table_f0 {
                    u16 PairCount;
                    u16 SearchRange;
                    u16 EntrySelector;
                    u16 RangeShift;
                    struct ttf_kern_table_f0_pair {
                        u16 Left;
                        u16 Right;
                        s16 Value;
                    } *Pairs;
                } F0;
                struct ttf_kern_table_f1 {
                    mem NotImplemented;
                } F1;
                struct ttf_kern_table_f2 {
                    mem NotImplemented;
                } F2;
                struct ttf_kern_table_f3 {
                    mem NotImplemented;
                } F3;
            };
        } *Tables;
    } *kern;
    
    struct ttf_loca {
        mem Offsets;
    } *loca;
    
    struct ttf_maxp {
        ttf_fixed TableVersionNumber;
        u16 NumGlyphs;
        u16 MaxPoints;
        u16 MaxContours;
        u16 MaxCompositePoints;
        u16 MaxCompositeContours;
        u16 MaxZones;
        u16 MaxTwilightPoints;
        u16 MaxStorage;
        u16 MaxFunctionDefs;
        u16 MaxInstructionDefs;
        u16 MaxStackElements;
        u16 MaxInstructionsSize;
        u16 MaxComponentElements;
        u16 MaxComponentDepth;
    } *maxp;
};
#pragma pack(pop)

typedef struct asset_ttf_data
{
    u32 GlyphCount;
    
    u32 HEADOffset;
    u32 CMAPOffset;
    u32 GLYFOffset;
    u32 MAXPOffset;
    
    u32 CMAPSubtableOffset;
    
    str FileText;    
} asset_ttf_data;

typedef struct asset_ttf_edge
{
    v2u16 Start;
    v2u16 End;
    v2u16 Control;
} asset_ttf_edge;

typedef struct asset_ttf_shape
{
    u32 ContourCount;
    u32 *Contours; // index into LogicalEdges
    u32 *LogicalEdges; // index into Edges
    asset_ttf_edge *Edges;
} asset_ttf_shape;

#define GAME__ASSET__EXPORTS \

#define GAME__ASSET__FUNCS \
    FUNC(ttf*, Font_ReadTTF, str FileText) \

#endif