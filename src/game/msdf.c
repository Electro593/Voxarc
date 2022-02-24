/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal msdf_shape
MSDF_MakeShape(stbtt_vertex *Vertices,
               u32 VertexCount)
{
    msdf_shape Shape = {0};
    
    for(u32 V = 0; V < VertexCount; ++V) {
        switch(Vertices[V].type) {
            case STBTT_vmove: {
                Shape.ContourCount++;
            } break;
            default: {
                Shape.SegmentCount++;
            } break;
        }
    }
    
    Shape.Contours = Stack_Allocate(Shape.ContourCount * sizeof(msdf_contour));
    Shape.Segments = Stack_Allocate(Shape.SegmentCount * sizeof(msdf_segment));
    Shape.Edges = Stack_Allocate(Shape.SegmentCount * sizeof(msdf_edge));
    
    msdf_contour *Contour = Shape.Contours-1;
    msdf_edge *Edge = Shape.Edges-1;
    msdf_segment *Segment = Shape.Segments;
    v2s16 P1, P2, C1, C2;
    v2s16 PrevDir = {0}, Dir = {0}, NextDir = {0};
    for(u32 V = 0; V < VertexCount; ++V) {
        stbtt_vertex Vertex = Vertices[VertexIndex];
        
        P1 = Segment->P1 = *(v2s16*)&Vertices[V-1].x;
        P2 = Segment->P2 = *(v2s16*)&Vertex.x;
        C1 = Segment->C1 = *(v2s16*)&Vertex.cx;
        C2 = Segment->C2 = *(v2s16*)&Vertex.cx1;
        
        switch(Vertex.type) {
            case STBTT_vmove: {
                Shape.EdgeCount++;
                Edge++;
                Edge->Segments = Segment+1;
                Contour++;
                Contour->EdgeCount = 1;
                Contour->Edges = Edge;
            } break;
            case STBTT_vline: {
                Segment->CPCount = 0;
                Dir = V2s16_Sub(P2, P1);
                NextDir = Dir;
            } break;
            case STBTT_vcurve: {
                Segment->CPCount = 1;
                Dir = V2s16_Sub(C1, P1);
                NextDir = V2s16_Sub(P2, C1);
            } break;
            case STBTT_vcubic: {
                Segment->CPCount = 2;
                Dir = V2s16_Sub(C1, P1);
                NextDir = V2s16_Sub(P2, C2);
            } break;
        }
        
        if(Vertex.type == STBTT_vmove) continue;
        
        if(Vertices[V-1].type != STBTT_vmove) {
            v2r32 D1, D2;
            D1 = V2s16_Norm(PrevDir);
            D2 = V2s16_Norm(Dir);
            
            r32 Cross = V2r32_Cross(D1, D2);
            r32 Dot = V2r32_Dot(D1, D2);
            
            #define SIN_ALPHA 0.08715574274765817355806427083747
            if(Dot < 0 || R32_Abs(Cross) >= SIN_ALPHA) {
                Shape.EdgeCount++;
                Edge++;
                Edge->Segments = Segment;
                Contour->EdgeCount++;
                Shape.EdgeCount++;
            }
        }
        
        PrevDir = NextDir;
        Edge->SegmentCount++;
        Segment++;
    }
    
    return Shape;
}