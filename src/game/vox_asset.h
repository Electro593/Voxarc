/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
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

#if !defined(GAME_VOX_ASSET_H_)
#define      GAME_VOX_ASSET_H_

typedef enum ttf_platform_id
{
    TTFPlatformID_Unicode   = 0,
    TTFPlatformID_Macintosh = 1,
    TTFPlatformID_ISO       = 2,
    TTFPlatformID_Windows   = 3,
} ttf_platform_id;

typedef enum ttf_encoding_id
{
    TTFEncodingID_Windows_Symbol      =  0,
    TTFEncodingID_Windows_UnicodeBMP  =  1,
    TTFEncodingID_Windows_ShiftJIS    =  2,
    TTFEncodingID_Windows_PRC         =  3,
    TTFEncodingID_Windows_Big5        =  4,
    TTFEncodingID_Windows_Wansung     =  5,
    TTFEncodingID_Windows_Johab       =  6,
    TTFEncodingID_Windows_UnicodeFull = 10,
} ttf_encoding_id;

// typedef enum ttf_simple_glyph_flag_bit
// {
//     SimpleGlyphFlagBit_OnCurve       = 1 << 0,
//     SimpleGlyphFlagBit_ShortVectorX  = 1 << 1,
//     SimpleGlyphFlagBit_ShortVectorY  = 1 << 2,
//     SimpleGlyphFlagBit_Repeat        = 1 << 3,
//     SimpleGlyphFlagBit_SameX         = 1 << 4,
//     SimpleGlyphFlagBit_SameY         = 1 << 5,
//     SimpleGlyphFlagBit_SimpleOverlap = 1 << 6,
// } ttf_simple_glyph_flag_bit;

#pragma pack(push,1)
typedef struct ttf_fixed
{
    u16 Major;
    u16 Minor;
} ttf_fixed;

typedef struct ttf_offset_table
{
    ttf_fixed SFNTVersion;
    u16 NumTables;
    u16 SearchRange;
    u16 EntrySelector;
    u16 RangeShift;
} ttf_offset_table;

// typedef struct ttf_simple_glyph_flag
// {
//     flag08 Bits;
//     u08 RepeatValue;
// } ttf_simple_glyph_flag;

// typedef struct ttf_simple_glyph
// {
//     u16 *ContourEndPoints;
//     u16 InstructionLength;
//     mem Instructions;
//     ttf_simple_glyph_flag *Flags;
// } ttf_simple_glyph;

// typedef struct ttf_glyf
// {
//     s16 NumContours;
//     s16 XMin;
//     s16 YMin;
//     s16 XMax;
//     s16 YMax;
//     ttf_simple_glyph Value;
// } ttf_glyf;

typedef struct ttf_cmap_subtable_header
{
    u16 PlatformID;
    u16 EncodingID;
    u32 SubtableOffset;
} ttf_cmap_subtable_header;

typedef struct ttf_cmap
{
    u16 VersionNumber;
    u16 EncodingTableCount;
    ttf_cmap_subtable_header *Tables;
} ttf_cmap;

typedef struct ttf_maxp
{
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
} ttf_maxp;

typedef struct ttf_head
{
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
    s16 LocFormatIndex;
    s16 GlyphDataFormat;
} ttf_head;

typedef struct ttf_dir_table_entry
{
    chr Tag[4];
    u32 Checksum;
    u32 Offset;
    u32 Length;
} ttf_dir_table_entry;

typedef struct ttf_header
{
    ttf_offset_table OffsetTable;
    ttf_dir_table_entry *DirTableEntries;
} ttf_header;
#pragma pack(pop)

typedef struct asset_ttf_data
{
    u32 GlyphCount;
    
    u32 HEADOffset;
    u32 CMAPOffset;
    u32 GLYFOffset;
    u32 MAXPOffset;
    
    u32 CMAPSubtableOffset;
    s16 LocFormatIndex;
} asset_ttf_data;

#endif