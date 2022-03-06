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
    // v2r32 C2 = Segment.C2;
    switch(Segment.CPCount) {
        case 0: {
            return V2r32_Sub(P2, P1);
        }
        case 1: {
            v2r32 C1mP1t2 = V2r32_MulS(V2r32_Sub(C1, P1), 2);
            v2r32 P2m2C1pP1 = V2r32_Add(V2r32_Add(P2, V2r32_MulS(C1, -2)), P1);
            return V2r32_Add(V2r32_MulS(P2m2C1pP1, 2*t), C1mP1t2);
        }
        // case 2: {
            
        // }
    }
    return (v2r32){0};
}

internal v2r32
MSDF_SegmentPoint(msdf_segment Segment, r32 t)
{
    v2r32 P1 = Segment.P1;
    v2r32 P2 = Segment.P2;
    v2r32 C1 = Segment.C1;
    // v2r32 C2 = Segment.C2;
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
        // case 2: {
            
        // }
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
        // C2 = Segment->C2 = V2s16_ToV2r32(*(v2s16*)&Vertex.cx1);
        
        switch(Vertex.type) {
            case STBTT_vmove: {
                Shape->EdgeCount++;
                Edge++;
                Edge->Segments = Segment;
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
            // case STBTT_vcubic: {
            //     Segment->CPCount = 2;
            //     Dir = V2s16_Sub(C1, P1);
            //     NextDir = V2s16_Sub(P2, C2);
            // } break;
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
                Contour->EdgeCount++;
            }
        }
        
        PrevDir = NextDir;
        Edge->SegmentCount++;
        Segment++;
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
    if(A.Orthogonality < B.Orthogonality) return LESS;
    if(A.Orthogonality > B.Orthogonality) return GREATER;
    return EQUAL;
}

internal u32
MSDF_SegmentDistances(v2r32 P,
                      msdf_segment Segment,
                      r32 *ts)
{
    v2r32 P1 = Segment.P1;
    v2r32 P2 = Segment.P2;
    v2r32 C1 = Segment.C1;
    // v2r32 C2 = Segment.C2;
    switch(Segment.CPCount) {
        case 0: {
            v2r32 PmP1 = V2r32_Sub(P, P1);
            v2r32 P2mP1 = V2r32_Sub(P2, P1);
            r32 Numer = V2r32_Dot(PmP1, P2mP1);
            r32 Denom = V2r32_Dot(P2mP1, P2mP1);
            ts[0] = Numer / Denom;
            return 1;
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
        // case 2: {
        //     v2r32 p = V2s16_Sub(P, P1);
        //     v2r32 p1 = V2s16_Sub(C1, P1);
        //     v2r32 p2 = V2s16_Add(V2s16_Sub(C2, V2s16_MulS(C1, 2)), P1);
        //     v2r32 p3 = V2s16_Add(V2s16_Sub(P2, V2s16_MulS(C2, 3)), V2s16_Sub(V2s16_MulS(C1, 3), P1));
        //     r32 A = V2r32_Dot(p3, p3);
        //     r32 B = 5*V2r32_Dot(p3, p2);
        //     r32 C = 4*V2r32_Dot(p3, p1) + 6*V2r32_Dot(p2, p2);
        //     r32 D = 9*V2r32_Dot(p2, p1) - V2r32_Dot(p2, p);
        //     r32 E = 3*V2r32_Dot(p1, p1) - 2*V2r32_Dot(p2, p);
        //     r32 F = -V2r32_Dot(p1, p);
        //     return R32_SolveQuintic(A, B, C, D, E, F, ts);
        // } // if re-enabled, remember to change ts[n] declaration
    }
    Assert(FALSE, "CPCount greater than 1");
    return 0;
}

internal msdf_dist
MSDF_EdgeSignedDistance(v2r32 P,
                        msdf_edge Edge)
{
    msdf_dist MinDist = {R32_MAX, 0};
    for(u32 S = 0; S < Edge.SegmentCount; S++) {
        msdf_segment Segment = Edge.Segments[S];
        r32 ts[5] = {0, 1};
        u32 Count = MSDF_SegmentDistances(P, Segment, ts+2);
        for(u32 D = 0; D < Count+2; D++) {
            r32 t = ts[D];
            if(D >= 2 && (t < 0 || t > 1)) continue;
            v2r32 Deriv = MSDF_SegmentDir(Segment, t);
            v2r32 Point = MSDF_SegmentPoint(Segment, t);
            v2r32 DistVec = V2r32_Sub(P, Point);
            r32 Dist = V2r32_Len(DistVec);
            v2r32 Dir = V2r32_Norm(Deriv);
            r32 Ortho = R32_Abs(V2r32_Cross(Dir, V2r32_DivS(DistVec, Dist)));
            r32 Sign = V2r32_Cross(Deriv, V2r32_Sub(Point, P));
            Dist *= R32_Sign(Sign);
            msdf_dist NewDist = {Dist, Ortho};
            if(MSDF_Cmp(NewDist, MinDist) == LESS) MinDist = NewDist;
        }
    }
    return MinDist;
}

internal r32
MSDF_EdgeSignedPseudoDistance(v2r32 P,
                              msdf_edge Edge)
{
    r32 MinDist = R32_MAX;
    for(u32 S = 0; S < Edge.SegmentCount; S++) {
        msdf_segment Segment = Edge.Segments[S];
        r32 ts[5] = {0, 1};
        u32 Count = MSDF_SegmentDistances(P, Segment, ts+2);
        for(u32 D = 0; D < Count+2; D++) {
            r32 t = ts[D];
            v2r32 Deriv = MSDF_SegmentDir(Segment, t);
            v2r32 Point = MSDF_SegmentPoint(Segment, t);
            r32 Dist = V2r32_Len(V2r32_Sub(P, Point));
            r32 Sign = V2r32_Cross(Deriv, V2r32_Sub(Point, P));
            Dist *= R32_Sign(Sign);
            if(R32_Abs(Dist) < R32_Abs(MinDist)) MinDist = Dist;
        }
    }
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

#define ARTIFACT_EPSILON 0.01
#define IS_BENIGN 0
#define IS_CANDIDATE 1
#define IS_ARTIFACT 2
#define IS_ERROR 1
#define IS_PROTECTED 2

internal b08
MSDF_Evaluate()
{
    
}

internal u08
MSDF_RangeTest(r32 T1, r32 T2, r32 T,
               u08 M1, u08 M2, u08 M,
               r32 Delta, u08 Protected)
{
    r32 D1, D2;
    
    if((M1 > 128 && M2 > 128 && M <= 128) || (M1 < 128 && M2 < 128 && M >= 128) || (!Protected && U08_Median(M1, M2, M) != M)) {
        D1 = (T-T1)*Delta;
        D2 = (T2-T)*Delta;
        
        if(!(M1-D1 <= M && M <= M1+D1 && M2-D2 <= M && M <= M2+D2))
            return IS_CANDIDATE | IS_ARTIFACT;
        return IS_CANDIDATE;
    }
    return IS_BENIGN;
}

internal b08
_MSDF_HasLinearArtifact(r32 Delta,
                        b08 Protected,
                        s08 M1, s08 M2,
                        v4u08 A, v4u08 B,
                        s08 dA, s08 dB)
{
    v4u08 I;
    r32 T;
    u08 M;
    
    T = (r32)dA/(dA-dB);
    if (T > 0 && T < 1) {
        I = V4u08_Lerp(A, B, T);
        M = U08_Median(I.X, I.Y, I.Z);
        return MSDF_RangeTest(0, 1, T, M1, M2, M, Delta, Protected) & IS_ARTIFACT;
    }
    return IS_BENIGN;
}

internal b08
MSDF_HasLinearArtifact(r32 Delta,
                       b08 Protected,
                       u08 Median,
                       v4u08 A,
                       v4u08 B)
{
    s08 M1, M2;
    
    M1 = (s08)Median - 128;
    M2 = (s08)U08_Median(B.X, B.Y, B.Z) - 128;
    
    if(R32_Abs(M1) < R32_Abs(M2)) return FALSE;
    return _MSDF_HasLinearArtifact(Delta, Protected, M1, M2, A, B, (s08)A.Y-A.X, (s08)B.Y-B.X) ||
           _MSDF_HasLinearArtifact(Delta, Protected, M1, M2, A, B, (s08)A.Z-A.Y, (s08)B.Z-B.Y) ||
           _MSDF_HasLinearArtifact(Delta, Protected, M1, M2, A, B, (s08)A.X-A.Z, (s08)B.X-B.Z);
}

internal b08
_MSDF_HasDiagonalArtifact(r32 Delta,
                         b08 Protected,
                         r32 M1,
                         r32 M4,
                         v3r32 A,
                         v3r32 L,
                         v3r32 Q,
                         r32 dA,
                         r32 dBC,
                         r32 dD,
                         r32 TX0,
                         r32 TX1)
{
    r32 T[2];
    u32 Count = R32_SolveQuadratic(T, dD-dBC+dA, dBC-dA-dA, dA);
    for(u32 S = 0; S < Count; S++) {
        if(T[S] > ARTIFACT_EPSILON && t[i] < 1-ARTIFACT_T_EPSILON) {
            // Interpolate median xm at t.
            float xm = interpolatedMedian(a, l, q, t[i]);
            // Determine if xm deviates too much from medians of a, d.
            int rangeFlags = artifactClassifier.rangeTest(0, 1, t[i], am, dm, xm);
            // Additionally, check xm against the interpolated medians at the local extremes tEx0, tEx1.
            double tEnd[2];
            float em[2];
            // tEx0
            if (tEx0 > 0 && tEx0 < 1) {
                tEnd[0] = 0, tEnd[1] = 1;
                em[0] = am, em[1] = dm;
                tEnd[tEx0 > t[i]] = tEx0;
                em[tEx0 > t[i]] = interpolatedMedian(a, l, q, tEx0);
                rangeFlags |= artifactClassifier.rangeTest(tEnd[0], tEnd[1], t[i], am, dm, xm);
            }
            if(TX1 > 0 && TX1 < 1) {
                tEnd[0] = 0, tEnd[1] = 1;
                em[0] = am, em[1] = dm;
                tEnd[tEx1 > t[i]] = tEx1;
                em[tEx1 > t[i]] = interpolatedMedian(a, l, q, tEx1);
                rangeFlags |= artifactClassifier.rangeTest(tEnd[0], tEnd[1], t[i], am, dm, xm);
            }
            if(MSDF_Evaluate(T[i], M, RangeFlags))
                return TRUE;
        }
    }
    return FALSE;
}

internal b08
MSDF_HasDiagonalArtifact(r32 Delta,
                         b08 Protected,
                         r32 M1,
                         v3r32 A,
                         v3r32 B,
                         v3r32 C,
                         v3r32 D)
{
    v3r32 ABC, L, Q, TX;
    r32 M4;
    
    M4 = R32_Median(D.X, D.Y, D.Z);
    
    if(R32_Abs(M1-0.5f) >= R32_Abs(M4-0.5f)) {
        ABC = V3r32_Sub(V3r32_Sub(A, B), C);
        L = V3r32_Sub(V3r32_MulS(A, -1), ABC);
        Q = V3r32_Add(D, ABC);
        TX = V3r32_Div(V3r32_MulS(L, -0.5f), Q);
        
        return _MSDF_HasDiagonalArtifact(Delta, Protected, M1, M4, A, L, Q, A.Y-A.X, B.Y-B.X+C.Y-C.X, D.Y-D.X, TX.X, TX.Y) ||
               _MSDF_HasDiagonalArtifact(Delta, Protected, M1, M4, A, L, Q, A.Z-A.Y, B.Z-B.Y+C.Z-C.Y, D.Z-D.Y, TX.Y, TX.Z) ||
               _MSDF_HasDiagonalArtifact(Delta, Protected, M1, M4, A, L, Q, A.X-A.Z, B.X-B.Z+C.X-C.Z, D.X-D.Z, TX.Z, TX.X);
    }
    return FALSE;
}

internal void
MSDF_FindErrors(v4u08 *Bitmap,
                v2u32 BitmapSize,
                u08 *Buffer,
                v2u32 BufferSize,
                r32 Range,
                v2r32 Scale,
                v4s16 Bounds,
                r32 MinDeviationRatio)
{
    b08 PF, LE, RE, BE, TE, BLE, BRE, TLE, TRE, SE, DE;
    v4u08 C, L, R, B, T, BL, BR, TL, TR;
    r32 dh, dv, dd;
    u08 M, *F;
    u32 X, Y;
    
    dh = MinDeviationRatio * V2r32_Len((v2r32){1/Range/Scale.X-Bounds.X, -Bounds.Y});
    dv = MinDeviationRatio * V2r32_Len((v2r32){-Bounds.X, 1/Range/Scale.Y-Bounds.Y});
    dd = MinDeviationRatio * V2r32_Len((v2r32){1/Range/Scale.X-Bounds.X, 1/Range/Scale.Y-Bounds.Y});
    
    for(Y = 0; Y < BufferSize.Y; Y++) {
        for(X = 0; X < BufferSize.X; X++) {
            C = Bitmap[INDEX_2D(X, Y, BitmapSize.X)];
            M = U08_Median(C.X, C.Y, C.Z);
            F = Buffer+INDEX_2D(X, Y, BufferSize.X);
            PF = (*F & 0b10) != 0;
            
            L  = Bitmap[INDEX_2D(X-1, Y,   BitmapSize.X)];
            R  = Bitmap[INDEX_2D(X+1, Y,   BitmapSize.X)];
            B  = Bitmap[INDEX_2D(X,   Y-1, BitmapSize.X)];
            T  = Bitmap[INDEX_2D(X,   Y+1, BitmapSize.X)];
            BL = Bitmap[INDEX_2D(X-1, Y-1, BitmapSize.X)];
            BR = Bitmap[INDEX_2D(X+1, Y-1, BitmapSize.X)];
            TL = Bitmap[INDEX_2D(X-1, Y+1, BitmapSize.X)];
            TR = Bitmap[INDEX_2D(X+1, Y+1, BitmapSize.X)];
            
            LE = X > 0              && MSDF_HasLinearArtifact(dh, PF, M, C, L);
            RE = X < BufferSize.X-1 && MSDF_HasLinearArtifact(dh, PF, M, C, R);
            BE = Y > 0              && MSDF_HasLinearArtifact(dv, PF, M, C, B);
            TE = Y < BufferSize.Y-1 && MSDF_HasLinearArtifact(dv, PF, M, C, T);
            
            BLE = X > 0              && Y > 0              && MSDF_HasDiagonalArtifact(dd, PF, M, C, L, B, BL);
            BRE = X < BufferSize.X-1 && Y > 0              && MSDF_HasDiagonalArtifact(dd, PF, M, C, R, B, BR);
            TLE = X > 0              && Y < BufferSize.Y-1 && MSDF_HasDiagonalArtifact(dd, PF, M, C, L, T, TL);
            TRE = X < BufferSize.X-1 && Y < BufferSize.Y-1 && MSDF_HasDiagonalArtifact(dd, PF, M, C, R, T, TR);
            
            *F |= 0b01 * (LE||RE||BE||TE||BLE||BRE||TLE||TRE);
        }
    }
}

internal void
MSDF_ProtectAll(u08 *Buffer,
                v2u32 BufferSize)
{
    u32 B;
    
    for(B = 0; B < BufferSize.X*BufferSize.Y; B++)
        Buffer[B] |= 0b10;
}

internal void
MSDF_FixErrors(v4u08 *Bitmap,
               v2u32 BitmapSize,
               u08 *Buffer,
               v2u32 BufferSize)
{
    u32 X, Y;
    v4u08 *C;
    
    for(Y = 0; Y < BufferSize.Y; Y++) {
        for(X = 0; X < BufferSize.X; X++) {
            if(Buffer[INDEX_2D(X, Y, BufferSize.X)] & 0b01) {
                C = Bitmap+INDEX_2D(X, Y, BitmapSize.X);
                C->X = C->Y = C->Z = U08_Median(C->X, C->Y, C->Z);
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
    
    v4s16 Bounds = MSDF_FindEdges(&Shape, Vertices, VertexCount);
    v2r32 MaxBounds = {Bounds.Z - Bounds.X, Bounds.W - Bounds.Y};
    
    MSDF_AssignColors(Shape);
    
    /* TODO Possible things to add
       - Short edge merging
    */
    
    v2r32 Offset = {1, 1};
    v2r32 SlotSizeR = {SlotSize.X, SlotSize.Y};
    v2r32 Size = V2r32_Sub(SlotSizeR, V2r32_MulS(Offset, 2));
    v2r32 Scale = V2r32_Div(Size, MaxBounds);
    r32 Range = 4 / (Scale.X + Scale.Y);
    
    for(s16 Y = 0; Y < (s16)SlotSize.Y; Y++) {
        for(s16 X = 0; X < (s16)SlotSize.X; X++) {
            v2r32 P = {
                (X-Offset.X+0.5)/Scale.X+Bounds.X,
                (Y-Offset.Y+0.5)/Scale.Y+Bounds.Y
            };
            
            msdf_dist Dists[3] = {{R32_MAX,1}, {R32_MAX,1}, {R32_MAX,1}};
            msdf_edge Edges[3] = {Shape.Contours[0].Edges[0], Shape.Contours[0].Edges[0], Shape.Contours[0].Edges[0]};
            
            for(u32 C = 0; C < Shape.ContourCount; C++) {
                msdf_contour Contour = Shape.Contours[C];
                for(u32 E = 0; E < Contour.EdgeCount; E++) {
                    msdf_edge Edge = Contour.Edges[E];
                    msdf_dist Dist = MSDF_EdgeSignedDistance(P, Edge);
                    if((Edge.Color&0b100) && MSDF_Cmp(Dist, Dists[0]) == LESS) {
                        Dists[0] = Dist;
                        Edges[0] = Edge;
                    } if((Edge.Color&0b010) && MSDF_Cmp(Dist, Dists[1]) == LESS) {
                        Dists[1] = Dist;
                        Edges[1] = Edge;
                    } if((Edge.Color&0b001) && MSDF_Cmp(Dist, Dists[2]) == LESS) {
                        Dists[2] = Dist;
                        Edges[2] = Edge;
                    }
                }
            }
            
            v4r32 FinalDists;
            FinalDists.X = MSDF_EdgeSignedPseudoDistance(P, Edges[0]);
            FinalDists.Y = MSDF_EdgeSignedPseudoDistance(P, Edges[1]);
            FinalDists.Z = MSDF_EdgeSignedPseudoDistance(P, Edges[2]);
            
            msdf_dist TrueDist = Dists[0];
            if(MSDF_Cmp(Dists[1], TrueDist) == LESS) TrueDist = Dists[1];
            if(MSDF_Cmp(Dists[2], TrueDist) == LESS) TrueDist = Dists[2];
            FinalDists.W = TrueDist.Distance;
            
            v4u08 Color = MSDF_DistanceColor(FinalDists, Range);
            Bitmap[INDEX_2D(X, Y, BitmapSize.X)] = Color;
        }
    }
    
    u08 *Buffer = Stack_Allocate(SlotSize.X*SlotSize.Y);
    Mem_Set(Buffer, 0, SlotSize.X*SlotSize.Y);
    
    r32 MinDeviationRatio = 1.11111111111111111;
    r32 MinImproveRatio = 1.11111111111111111;
    
    MSDF_ProtectCorners();
    MSDF_ProtectEdges();
    MSDF_FindErrors(Bitmap, BitmapSize, Buffer, SlotSize, Range, Scale, Bounds, MinDeviationRatio);
    MSDF_ProtectAll(Buffer, SlotSize);
    MSDF_FindErrorsOverlappedOrSomething();
    MSDF_FixErrors(Bitmap, BitmapSize, Buffer, SlotSize);
}