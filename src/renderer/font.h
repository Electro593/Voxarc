/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TTF_MAKE_TAG(_0,_1,_2,_3) (((_0)<<24) | ((_1)<<16) | ((_2)<<8) | (_3))
// #define TTF_MAKE_TAG(_0,_1,_2,_3) (((_3)<<24) | ((_2)<<16) | ((_1)<<8) | (_0))

#define _SWAPENDIAN16(Data) ((((Data)&0xFF)<<8) | (((Data)>>8)&0xFF))

#define SWAPENDIAN16(Data) (Data = _SWAPENDIAN16(Data))
#define SWAPENDIAN32(Data) (Data = ((Data&0xFF)<<24) | ((Data&0xFF00)<<8) | ((Data>>8)&0xFF00) | ((Data>>24)&0xFF))
#define SWAPENDIAN64(Data) (Data = ((Data&0xFF)<<56) | ((Data&0xFF00)<<40)| ((Data&0xFF0000)<<24) | ((Data&0xFF000000)<<8) | ((Data>>8)&0xFF000000) | ((Data>>24)&0xFF0000) | ((Data>>40)&0xFF00) | ((Data>>56)&0xFF))

typedef s16 ttf_shortfrac;
typedef u32 ttf_fixed;
typedef s16 ttf_fword;
typedef u16 ttf_ufword;
typedef s16 ttf_f2dot14;
typedef s64 ttf_datetime;

#pragma pack(push, 1)
#define TTF_TAG_true TTF_MAKE_TAG('t','r','u','e')
typedef struct ttf_font_dir {
    struct {
        u32 ScalerType;
        u16 TableCount;
        u16 SearchRange;
        u16 EntrySelector;
        u16 RangeShift;
    } Offset;
    
    struct {
        u32 Tag;
        u32 Checksum;
        u32 Offset;
        u32 Length;
    } Tables[];
} ttf_font_dir;

#define TTF_CMAP_PLATFORM_UNICODE 0
#define TTF_CMAP_PLATFORM_MICROSOFT 3
#define TTF_CMAP_MICROSOFT_UNICODEBMP 1
#define TTF_TAG_cmap TTF_MAKE_TAG('c','m','a','p')
typedef struct ttf_cmap {
    u16 Version;
    u16 TableCount;
    struct {
        u16 PlatformID;
        u16 PlatformSpecificID;
        u32 Offset;
    } Records[];
} ttf_cmap;
typedef struct ttf_cmap_subtable_4 {
    u16 SegCountX2;
    u16 SearchRange;
    u16 EntrySelector;
    u16 RangeShift;
    u16 *EndCodes;
    u16 _Reserved;
    u16 *StartCodes;
    s16 *Deltas;
    u16 *RangeOffsets;
    u16 *GlyphIdArray;
} ttf_cmap_subtable_4;
typedef struct ttf_cmap_subtable {
    struct {
        u16 Format;
        u16 Length;
        u16 Language;
    } *Header;
    union {
        ttf_cmap_subtable_4 Type4;
    };
} ttf_cmap_subtable;

typedef enum ttf_glyf_flags {
    TTF_VERTEX_ON_CURVE = 0x01,
    TTF_VERTEX_SHORT_X  = 0x02,
    TTF_VERTEX_SHORT_Y  = 0x04,
    TTF_FLAG_REPEATED   = 0x08,
    TTF_VERTEX_SAME_X   = 0x10,
    TTF_VERTEX_POS_X    = 0x10,
    TTF_VERTEX_SAME_Y   = 0x20,
    TTF_VERTEX_POS_Y    = 0x20,
} ttf_glyf_flags;
#define TTF_TAG_glyf TTF_MAKE_TAG('g','l','y','f')
typedef struct ttf_glyph {
    s16 ContourCount;
    s16 XMin;
    s16 YMin;
    s16 XMax;
    s16 YMax;
    u08 Data[];
} ttf_glyph;

typedef enum ttf_loca_format {
    TTF_LOCA_SHORT_OFFSETS = 0,
    TTF_LOCA_LONG_OFFSETS  = 1,
} ttf_loca_format;
#define TTF_TAG_head TTF_MAKE_TAG('h','e','a','d')
typedef struct ttf_head {
    ttf_fixed Version;
    ttf_fixed Revision;
    u32 ChecksumAdjustment;
    u32 MagicNumber;
    u16 Flags;
    u16 UnitsPerEm;
    ttf_datetime Created;
    ttf_datetime Modified;
    ttf_fword XMin;
    ttf_fword YMin;
    ttf_fword XMax;
    ttf_fword YMax;
    u16 MaxStyle;
    u16 LowestRecPPEM;
    s16 FontDirectionHint;
    s16 IndexToLocFormat;
    s16 GlyphDataFormat;
} ttf_head;

#define TTF_TAG_hhea TTF_MAKE_TAG('h','h','e','a')
typedef struct ttf_hhea {
    ttf_fixed Version;
    ttf_fword Ascent;
    ttf_fword Descent;
    ttf_fword LineGap;
    ttf_ufword MaxAdvanceX;
    ttf_fword MinLeftBearing;
    ttf_fword MinRightBearing;
    ttf_fword MaxExtent;
    s16 CaretSlopeRise;
    s16 CaretSlopeRun;
    ttf_fword CaretOffset;
    u08 _Reserved[8];
    s16 MetricDataFormat;
    u16 HMetricCount;
} ttf_hhea;

#define TTF_TAG_hmtx TTF_MAKE_TAG('h','m','t','x')
typedef struct ttf_hmtx {
    struct ttf_hmetric {
        u16 AdvanceX;
        s16 LeftBearing;
    } *HMetrics;
    ttf_fword *ExtraBearings;
} ttf_hmtx;

#define TTF_TAG_loca TTF_MAKE_TAG('l','o','c','a')
typedef union ttf_loca {
    u16 Shorts[];
    u32 Longs[];
} ttf_loca;

#define TTF_TAG_maxp TTF_MAKE_TAG('m','a','x','p')
typedef struct ttf_maxp {
    ttf_fixed Version;
    u16 GlyphCount;
    u16 MaxPoints;
    u16 MaxContours;
    u16 MaxComponentPoints;
    u16 MaxComponentContours;
    u16 MaxZones;
    u16 MaxTwilightPoints;
    u16 MaxStorage;
    u16 MaxFunctionDefs;
    u16 MaxInstructionDefs;
    u16 MaxStackElements;
    u16 MaxSizeOfInstructions;
    u16 MaxComponentElements;
    u16 MaxComponentDepth;
} ttf_maxp;
#pragma pack(pop)

typedef struct font {
    ttf_font_dir *FontDir;
    ttf_cmap *cmap;
    u08 *glyfs;
    ttf_head *head;
    ttf_hhea *hhea;
    ttf_hmtx hmtx;
    ttf_loca *loca;
    ttf_maxp *maxp;
    
    ttf_cmap_subtable Encoding;
    
    u08 *FileBase;
} font;

typedef struct font_glyph {
    r32 Advance;
    v2r32 Bearing;
    v2r32 Size;
    
    msdf_shape Shape;
} font_glyph;