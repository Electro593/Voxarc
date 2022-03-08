/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MSDF_PIXEL_HAS_ERROR 1

typedef struct msdf_dist {
    r32 Distance;
    r32 Orthogonality;
} msdf_dist;

typedef struct msdf_segment {
    v2r32 P1;
    v2r32 P2;
    v2r32 C1;
    u08 CPCount;
} msdf_segment;

typedef struct msdf_edge {
    msdf_segment *Segments;
    u32 SegmentCount;
    u08 Color;
} msdf_edge;

typedef struct msdf_contour {
    msdf_edge *Edges;
    u32 EdgeCount;
} msdf_contour;

typedef struct msdf_shape {
    msdf_contour *Contours;
    msdf_edge *Edges;
    msdf_segment *Segments;
    u32 ContourCount;
    u32 EdgeCount;
    u32 SegmentCount;
} msdf_shape;