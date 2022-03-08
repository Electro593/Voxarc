/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal v2r32
MSDF_SegmentDir(msdf_segment Segment, r32 t)
{
    v2r32 P1 = Segment.P1;
    v2r32 P2 = Segment.P2;
    v2r32 C1 = Segment.C1;
    switch(Segment.CPCount) {
        case 0: {
            return V2r32_Norm(V2r32_Sub(P2, P1));
        }
        case 1: {
            v2r32 C1mP1t2 = V2r32_MulS(V2r32_Sub(C1, P1), 2);
            v2r32 P2m2C1pP1 = V2r32_Add(V2r32_Add(P2, V2r32_MulS(C1, -2)), P1);
            return V2r32_Norm(V2r32_Add(V2r32_MulS(P2m2C1pP1, 2*t), C1mP1t2));
        }
    }
    return (v2r32){0};
}

internal v2r32
MSDF_SegmentPoint(msdf_segment Segment, r32 t)
{
    v2r32 P1 = Segment.P1;
    v2r32 P2 = Segment.P2;
    v2r32 C1 = Segment.C1;
    switch(Segment.CPCount) {
        case 0: {
            return V2r32_Add(P1, V2r32_MulS(V2r32_Sub(P2, P1), t));
        }
        case 1: {
            v2r32 Part1 = P1;
            v2r32 Part2 = V2r32_MulS(V2r32_Sub(C1, P1), 2*t);
            v2r32 Part3 = V2r32_MulS(V2r32_Add(V2r32_Sub(P2, V2r32_MulS(C1, 2)), P1), t*t);
            return V2r32_Add(V2r32_Add(Part1, Part2), Part3);
        }
    }
    return (v2r32){0};
}

internal v4s16
MSDF_FindEdges(msdf_shape *Shape,
               stbtt_vertex *Vertices,
               u32 VertexCount)
{
    v4s16 Bounds = {S16_MAX, S16_MAX, S16_MIN, S16_MIN};
    msdf_contour *Contour = Shape->Contours-1;
    msdf_edge *Edge = Shape->Edges-1;
    msdf_segment *Segment = Shape->Segments;
    v2r32 P1, P2, C1, C2;
    v2r32 PrevDir = {0}, Dir = {0}, NextDir = {0};
    for(u32 V = 0; V < VertexCount; ++V) {
        stbtt_vertex Vertex = Vertices[V];
        
        if(V > 0) P1 = Segment->P1 = V2s16_ToV2r32(*(v2s16*)&Vertices[V-1].x);
        P2 = Segment->P2 = V2s16_ToV2r32(*(v2s16*)&Vertex.x);
        C1 = Segment->C1 = V2s16_ToV2r32(*(v2s16*)&Vertex.cx);
        
        switch(Vertex.type) {
            case STBTT_vmove: {
                Shape->EdgeCount++;
                Edge++;
                Edge->Segments = Segment;
                Edge->SegmentCount = 0;
                Contour++;
                Contour->EdgeCount = 1;
                Contour->Edges = Edge;
            } break;
            case STBTT_vline: {
                Segment->CPCount = 0;
                Dir = V2r32_Sub(P2, P1);
                NextDir = Dir;
            } break;
            case STBTT_vcurve: {
                Segment->CPCount = 1;
                Dir = V2r32_Sub(C1, P1);
                NextDir = V2r32_Sub(P2, C1);
            } break;
        }
        
        if(Vertex.type == STBTT_vmove) continue;
        if(Dir.X == 0 && Dir.Y == 0) {
            Shape->SegmentCount--;
            continue;
        }
        
        if(V % 2 == 1) {
            if(P1.X < Bounds.X) Bounds.X = P1.X;
            if(P1.Y < Bounds.Y) Bounds.Y = P1.Y;
            if(P1.X > Bounds.Z) Bounds.Z = P1.X;
            if(P1.Y > Bounds.W) Bounds.W = P1.Y;
            if(P2.X < Bounds.X) Bounds.X = P2.X;
            if(P2.Y < Bounds.Y) Bounds.Y = P2.Y;
            if(P2.X > Bounds.Z) Bounds.Z = P2.X;
            if(P2.Y > Bounds.W) Bounds.W = P2.Y;
            if(Segment->CPCount >= 1) {
                if(C1.X < Bounds.X) Bounds.X = C1.X;
                if(C1.Y < Bounds.Y) Bounds.Y = C1.Y;
                if(C1.X > Bounds.Z) Bounds.Z = C1.X;
                if(C1.Y > Bounds.W) Bounds.W = C1.Y;
            }
        }
        
        if(Vertices[V-1].type != STBTT_vmove) {
            v2r32 D1, D2;
            D1 = V2r32_Norm(PrevDir);
            D2 = V2r32_Norm(Dir);
            
            r32 Cross = V2r32_Cross(D1, D2);
            r32 Dot = V2r32_Dot(D1, D2);
            
            #define SIN_ALPHA 0.08715574274765817355806427083747
            if(Dot < 0 || R32_Abs(Cross) >= SIN_ALPHA) {
                Shape->EdgeCount++;
                Edge++;
                Edge->Segments = Segment;
                Edge->SegmentCount = 0;
                Contour->EdgeCount++;
            }
        }
        
        PrevDir = NextDir;
        Edge->SegmentCount++;
        Segment++;
    }
    
    u32 Offset = 0;
    u32 MoveSize = Shape->EdgeCount * sizeof(msdf_edge);
    Contour = Shape->Contours;
    for(u32 C = 0; C < Shape->ContourCount; C++) {
        MoveSize -= Contour->EdgeCount * sizeof(msdf_edge);
        Contour->Edges += Offset;
        if(Contour->EdgeCount == 1) {
            Edge = Contour->Edges;
            Mem_Cpy(Edge+2, Edge+1, MoveSize);
            msdf_edge *NewEdge = Edge+1;
            
            Assert(Edge->SegmentCount > 1);
            NewEdge->SegmentCount = Edge->SegmentCount/2;
            Edge->SegmentCount -= NewEdge->SegmentCount;
            NewEdge->Segments = Edge->Segments+Edge->SegmentCount;
            
            Contour->EdgeCount++;
            Shape->EdgeCount++;
            Offset++;
        }
        
        Contour++;
    }
    
    return Bounds;
}

internal void
MSDF_AssignColors(msdf_shape Shape)
{
    u08 Color;
    for(u32 C = 0; C < Shape.ContourCount; C++) {
        msdf_contour Contour = Shape.Contours[C];
        if(Contour.EdgeCount == 1)
            Color = 0b111;
        else
            Color = 0b101;
        for(u32 E = 0; E < Contour.EdgeCount; E++) {
            Contour.Edges[E].Color = Color;
            if(Color == 0b110)
                Color = 0b011;
            else
                Color = 0b110;
        }
    }
}

internal s08
MSDF_Cmp(msdf_dist A,
         msdf_dist B)
{
    r32 ADist = R32_Abs(A.Distance);
    r32 BDist = R32_Abs(B.Distance);
    if(ADist < BDist) return LESS;
    if(ADist > BDist) return GREATER;
    if(A.Orthogonality > B.Orthogonality) return LESS;
    if(A.Orthogonality < B.Orthogonality) return GREATER;
    return EQUAL;
}

internal u32
MSDF_LinearSegmentDistance(v2r32 P,
                           msdf_segment Segment,
                           r32 *ts)
{
    v2r32 PmP1 = V2r32_Sub(P, Segment.P1);
    v2r32 P2mP1 = V2r32_Sub(Segment.P2, Segment.P1);
    r32 Numer = V2r32_Dot(PmP1, P2mP1);
    r32 Denom = V2r32_Dot(P2mP1, P2mP1);
    ts[0] = Numer / Denom;
    return 1;
}

internal u32
MSDF_SegmentDistances(v2r32 P,
                      msdf_segment Segment,
                      r32 *ts)
{
    v2r32 P1 = Segment.P1;
    v2r32 P2 = Segment.P2;
    v2r32 C1 = Segment.C1;
    switch(Segment.CPCount) {
        case 0: {
            return MSDF_LinearSegmentDistance(P, Segment, ts);
        }
        case 1: {
            v2r32 p = V2r32_Sub(P, P1);
            v2r32 p1 = V2r32_Sub(C1, P1);
            v2r32 p2 = V2r32_Add(V2r32_Sub(P2, V2r32_MulS(C1, 2)), P1);
            r32 A = V2r32_Dot(p2, p2);
            r32 B = 3*V2r32_Dot(p1, p2);
            r32 C = 2*V2r32_Dot(p1, p1) - V2r32_Dot(p2, p);
            r32 D = -V2r32_Dot(p1, p);
            
            return R32_SolveCubic(A, B, C, D, ts);
        }
    }
    Assert(FALSE, "CPCount greater than 1");
    return 0;
}

internal msdf_dist
MSDF_EdgeSignedDistance(v2r32 P,
                        msdf_edge Edge,
                        msdf_segment *SegmentOut)
{
    msdf_dist MinDist = {R32_MAX, 0};
    for(u32 S = 0; S < Edge.SegmentCount; S++) {
        msdf_segment Segment = Edge.Segments[S];
        r32 ts[5] = {0, 1};
        u32 Count = MSDF_SegmentDistances(P, Segment, ts+2);
        for(u32 D = 0; D < Count+2; D++) {
            r32 t = ts[D];
            if(t < 0 || t > 1) continue;
            v2r32 Dir = MSDF_SegmentDir(Segment, t);
            v2r32 Point = MSDF_SegmentPoint(Segment, t);
            v2r32 DistVec = V2r32_Sub(P, Point);
            r32 Dist = V2r32_Len(DistVec);
            r32 Ortho = R32_Abs(V2r32_Cross(Dir, V2r32_DivS(DistVec, Dist)));
            r32 Sign = V2r32_Cross(Dir, V2r32_Sub(Point, P));
            Dist *= R32_Sign(Sign);
            msdf_dist NewDist = {Dist, Ortho};
            if(MSDF_Cmp(NewDist, MinDist) == LESS) {
                MinDist = NewDist;
                *SegmentOut = Segment;
            }
        }
    }
    return MinDist;
}

internal r32
_MSDF_SignedPseudoDistance(v2r32 P,
                           msdf_segment Segment,
                           r32 t)
{
    v2r32 Dir = MSDF_SegmentDir(Segment, t);
    v2r32 Point = MSDF_SegmentPoint(Segment, t);
    v2r32 EdgeToP = V2r32_Sub(P, Point);
    r32 Dist = V2r32_Cross(EdgeToP, Dir);
    return Dist;
}

internal r32
MSDF_SignedPseudoCubicDistances(v2r32 P,
                                msdf_segment Segment,
                                r32 *Dists)
{
    persist r32 LowEpsilon  = -0.00002;
    persist r32 HighEpsilon =  1.00002;
    
    u32 Count = 0;
    r32 Roots[3];
    
    u32 CubicCount = MSDF_SegmentDistances(P, Segment, Roots);
    for(u32 R = 0; R < CubicCount; R++) {
        if(LowEpsilon <= Roots[R] && Roots[R] <= HighEpsilon)
            Dists[Count++] = _MSDF_SignedPseudoDistance(P, Segment, Roots[R]);
    }
    
    msdf_segment LinearSegment = {0};
    LinearSegment.P1 = Segment.P1;
    LinearSegment.P2 = Segment.C1;
    MSDF_LinearSegmentDistance(P, LinearSegment, Roots);
    if(Roots[0] < LowEpsilon)
        Dists[Count++] = _MSDF_SignedPseudoDistance(P, LinearSegment, Roots[0]);
    
    LinearSegment.P1 = Segment.C1;
    LinearSegment.P2 = Segment.P2;
    MSDF_LinearSegmentDistance(P, LinearSegment, Roots);
    if(Roots[0] > HighEpsilon)
        Dists[Count++] = _MSDF_SignedPseudoDistance(P, LinearSegment, Roots[0]);
    
    if(Count == 0) {
        for(u32 R = 0; R < CubicCount; R++)
            Dists[Count++] = _MSDF_SignedPseudoDistance(P, Segment, Roots[R]);
    }
    
    return Count;
}

internal r32
MSDF_SignedPseudoDistance(v2r32 P,
                          msdf_segment Segment)
{
    r32 MinDist = R32_MAX;
    r32 Dists[5];
    u32 Count;
    
    if(Segment.CPCount == 0) {
        r32 t;
        Count = MSDF_LinearSegmentDistance(P, Segment, &t);
        Dists[0] = _MSDF_SignedPseudoDistance(P, Segment, t);
    } else {
        Count = MSDF_SignedPseudoCubicDistances(P, Segment, Dists);
    }
    
    for(u32 D = 0; D < Count; D++)
        if(R32_Abs(Dists[D]) < R32_Abs(MinDist)) MinDist = Dists[D];
    
    return MinDist;
}

internal v4u08
MSDF_DistanceColor(v4r32 Dists,
                   r32 Range)
{
    Dists = V4r32_DivS(Dists, Range);
    Dists = V4r32_AddS(Dists, 0.5);
    Dists = V4r32_Clamp(Dists, 0, 1);
    Dists = V4r32_MulS(Dists, 255);
    return (v4u08){(u08)Dists.X, (u08)Dists.Y, (u08)Dists.Z, (u08)Dists.W};
}

internal b08
MSDF_HasSideError(v4r32 A,
                  v4r32 B,
                  r32 Threshold)
{
    u32 Count = 0;
    for(u32 C = 0; C < 4; C++) {
        r32 CA = A.E[C];
        r32 CB = B.E[C];
        
        if(R32_Abs(CA) < Threshold && R32_Abs(CB) < Threshold) continue;
        if(R32_Sign(CA) != R32_Sign(CB)) Count++;
    }
    
    return (Count >= 2) ? TRUE : FALSE;
}

internal void
MSDF_FindErrors(u08 *ErrorMap,
               v4r32 *FloatMap,
               v2u32 Size,
               r32 Threshold)
{
    for(u32 Y = 0; Y < Size.Y; Y++) {
        for(u32 X = 0; X < Size.X; X++) {
            v4r32 P, L,B,R,T, LB,LT,RB,RT;
            b08 HasL,HasB,HasR,HasT, HasLB,HasLT,HasRB,HasRT;
            b08 LE,BE,RE,TE, LBE,LTE,RBE,RTE, E;
            
            // Assert(X != Size.X-1 || Y != 24);
            
            HasL = X > 0;
            HasB = Y > 0;
            HasR = X < Size.X-1;
            HasT = Y < Size.Y-1;
            HasLB = HasL && HasB;
            HasLT = HasL && HasT;
            HasRB = HasR && HasB;
            HasRT = HasR && HasT;
            
            P = FloatMap[INDEX_2D(X, Y, Size.X)];
            
            L  = HasL  ? FloatMap[INDEX_2D(X-1, Y,   Size.X)] : (v4r32){0};
            B  = HasB  ? FloatMap[INDEX_2D(X,   Y-1, Size.X)] : (v4r32){0};
            R  = HasR  ? FloatMap[INDEX_2D(X+1, Y,   Size.X)] : (v4r32){0};
            T  = HasT  ? FloatMap[INDEX_2D(X,   Y+1, Size.X)] : (v4r32){0};
            LB = HasLB ? FloatMap[INDEX_2D(X-1, Y-1, Size.X)] : (v4r32){0};
            LT = HasLT ? FloatMap[INDEX_2D(X-1, Y+1, Size.X)] : (v4r32){0};
            RB = HasRB ? FloatMap[INDEX_2D(X+1, Y-1, Size.X)] : (v4r32){0};
            RT = HasRT ? FloatMap[INDEX_2D(X+1, Y+1, Size.X)] : (v4r32){0};
            
            LE = HasL && MSDF_HasSideError(P, L, Threshold);
            BE = HasB && MSDF_HasSideError(P, B, Threshold);
            RE = HasR && MSDF_HasSideError(P, R, Threshold);
            TE = HasT && MSDF_HasSideError(P, T, Threshold);
            
            LBE = HasLB && MSDF_HasSideError(P, LB, Threshold);
            LTE = HasLT && MSDF_HasSideError(P, LT, Threshold);
            RBE = HasRB && MSDF_HasSideError(P, RB, Threshold);
            RTE = HasRT && MSDF_HasSideError(P, RT, Threshold);
            
            E = LE|BE|RE|TE | LBE|LTE|RBE|RTE;
            ErrorMap[INDEX_2D(X, Y, Size.X)] |= MSDF_PIXEL_HAS_ERROR*E;
        }
    }
}

internal void
MSDF_FixErrors(u08 *ErrorMap,
              v4r32 *FloatMap,
              v2u32 Size)
{
    for(u32 Y = 0; Y < Size.Y; Y++) {
        for(u32 X = 0; X < Size.X; X++) {
            u32 I = INDEX_2D(X, Y, Size.X);
            v4r32 F = FloatMap[I];
            if(ErrorMap[I] & MSDF_PIXEL_HAS_ERROR) {
                r32 M = R32_Median(F.X, F.Y, F.Z);
                FloatMap[I] = (v4r32){M, M, M, F.W};
            }
        }
    }
}

internal void
MSDF_MakeShape(stbtt_vertex *Vertices,
               r32 _Scale,
               v2u32 SlotSize,
               u32 VertexCount,
               v4u08 *Bitmap,
               v2u32 BitmapOffset,
               u32 BitmapIndex,
               v2u32 BitmapSize)
{
    Stack_Push();
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
    Shape.Edges = Stack_Allocate((Shape.SegmentCount+Shape.ContourCount) * sizeof(msdf_edge));
    
    v4s16 Bounds = MSDF_FindEdges(&Shape, Vertices, VertexCount);
    v2r32 MaxBounds = {Bounds.Z - Bounds.X, Bounds.W - Bounds.Y};
    
    MSDF_AssignColors(Shape);
    
    /* TODO Possible things to add
       - Short edge merging
       - Error correction
    */
    
    v2r32 Offset = {1, 1};
    v2r32 SlotSizeR = {SlotSize.X, SlotSize.Y};
    v2r32 Size = V2r32_Sub(SlotSizeR, V2r32_MulS(Offset, 2));
    v2r32 Scale = V2r32_Div(Size, MaxBounds);
    r32 Range = 4 / (Scale.X + Scale.Y);
    
    v4r32 *FloatMap = Stack_Allocate(SlotSize.X*SlotSize.Y*sizeof(v4r32));
    
    for(u32 Y = 0; Y < SlotSize.Y; Y++) {
        for(u32 X = 0; X < SlotSize.X; X++) {
            v2r32 P = {
                (X-Offset.X+0.5)/Scale.X+Bounds.X,
                (Y-Offset.Y+0.5)/Scale.Y+Bounds.Y
            };
            
            msdf_dist Dists[3] = {{R32_MAX,1}, {R32_MAX,1}, {R32_MAX,1}};
            msdf_edge Edges[3] = {Shape.Contours[0].Edges[0], Shape.Contours[0].Edges[0], Shape.Contours[0].Edges[0]};
            msdf_segment Segments[3] = {Edges[0].Segments[0], Edges[0].Segments[0], Edges[0].Segments[0]};
            
            for(u32 C = 0; C < Shape.ContourCount; C++) {
                msdf_contour Contour = Shape.Contours[C];
                for(u32 E = 0; E < Contour.EdgeCount; E++) {
                    msdf_edge Edge = Contour.Edges[E];
                    msdf_segment Segment;
                    msdf_dist Dist = MSDF_EdgeSignedDistance(P, Edge, &Segment);
                    if((Edge.Color&0b100) && MSDF_Cmp(Dist, Dists[0]) == LESS) {
                        Dists[0] = Dist;
                        Edges[0] = Edge;
                        Segments[0] = Segment;
                    } if((Edge.Color&0b010) && MSDF_Cmp(Dist, Dists[1]) == LESS) {
                        Dists[1] = Dist;
                        Edges[1] = Edge;
                        Segments[1] = Segment;
                    } if((Edge.Color&0b001) && MSDF_Cmp(Dist, Dists[2]) == LESS) {
                        Dists[2] = Dist;
                        Edges[2] = Edge;
                        Segments[2] = Segment;
                    }
                }
            }
            
            v4r32 FinalDists;
            // Assert(X != SlotSize.X-16 || Y != SlotSize.Y-12);
            FinalDists.X = MSDF_SignedPseudoDistance(P, Segments[0]);
            FinalDists.Y = MSDF_SignedPseudoDistance(P, Segments[1]);
            FinalDists.Z = MSDF_SignedPseudoDistance(P, Segments[2]);
            
            msdf_dist TrueDist = Dists[0];
            if(MSDF_Cmp(Dists[1], TrueDist) == LESS) TrueDist = Dists[1];
            if(MSDF_Cmp(Dists[2], TrueDist) == LESS) TrueDist = Dists[2];
            FinalDists.W = TrueDist.Distance;
            
            FloatMap[INDEX_2D(X, Y, (u32)SlotSize.X)] = FinalDists;
        }
    }
    
    u08 *ErrorMap = Stack_Allocate(SlotSize.X*SlotSize.Y);
    Mem_Set(ErrorMap, 0, SlotSize.X*SlotSize.Y);
    
    r32 ThresholdModifier = 0.72;
    r32 Threshold = Range * ThresholdModifier;
    MSDF_FindErrors(ErrorMap, FloatMap, SlotSize, Threshold);
    MSDF_FixErrors(ErrorMap, FloatMap, SlotSize);
    
    for(u32 Y = 0; Y < SlotSize.Y; Y++) {
        for(u32 X = 0; X < SlotSize.X; X++) {
            v4r32 Floats = FloatMap[INDEX_2D(X, Y, SlotSize.X)];
            v4u08 Color = MSDF_DistanceColor(Floats, Range);
            Bitmap[INDEX_2D(X, Y, BitmapSize.X)] = Color;
        }
    }
    
    Stack_Pop();
}