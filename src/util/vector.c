/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define DEFINE_VECTOR_INIT(Count, Type) \
    internal v##Count##Type \
    V##Count##Type( \
        MAC_FOR(Type, Count, MAC_FOR_OP_SEQ_REV, MAC_FOR_FUNC_DECLVAR, MAC_FOR_ARGS_VEC)) \
    { \
        v##Count##Type Result; \
        MAC_FOR(, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_V_ASSIGN, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_ADD(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Add(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        v##Count##Type Result; \
        MAC_FOR(+, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VV_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_ADDS(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_AddS(v##Count##Type V, \
                          Type S) \
    { \
        v##Count##Type Result; \
        MAC_FOR(+, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VS_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_SUB(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Sub(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        v##Count##Type Result; \
        MAC_FOR(-, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VV_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_SUBS(Count, Type)                                              \
    internal v##Count##Type                                                          \
    V##Count##Type##_SubS(v##Count##Type V,                                          \
                          Type S)                                                    \
    {                                                                                \
        v##Count##Type Result;                                                       \
        MAC_FOR(-, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VS_OP, MAC_FOR_ARGS_VEC); \
        return Result;                                                               \
    }

#define DEFINE_VECTOR_MUL(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Mul(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        v##Count##Type Result; \
        MAC_FOR(*, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VV_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_MULS(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_MulS(v##Count##Type V, \
                          Type S) \
    { \
        v##Count##Type Result; \
        MAC_FOR(*, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VS_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_DIV(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Div(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        v##Count##Type Result; \
        MAC_FOR(/, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VV_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_DIVS(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_DivS(v##Count##Type V, \
                          Type S) \
    { \
        v##Count##Type Result; \
        MAC_FOR(/, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VS_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_VOLUME(Type) \
    internal Type \
    V3##Type##_Volume(v3##Type V) \
    { \
        return V.X*V.Y*V.Z; \
    }

#define DEFINE_VECTOR_DOT(Count, Type) \
    internal Type \
    V##Count##Type##_Dot(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        Type Result; \
        Result = MAC_FOR(+, Count, MAC_FOR_OP_NAME_REV, MAC_FOR_FUNC_VEC_DOT, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_CROSS_3(Type) \
    internal v3##Type \
    V3##Type##_Cross(v3##Type A, \
                     v3##Type B) \
    { \
        v3##Type Result; \
        Result.X = (A.Y*B.Z)-(A.Z*B.Y); \
        Result.Y = (A.Z*B.X)-(A.X*B.Z); \
        Result.Z = (A.X*B.Y)-(A.Y*B.X); \
        return Result; \
    }

#define DEFINE_VECTOR_CROSS_2(Type) \
    internal Type \
    V2##Type##_Cross(v2##Type A, \
                     v2##Type B) \
    { \
        Type Result; \
        Result = (A.X*B.Y)-(A.Y*B.X); \
        return Result; \
    }

#define DEFINE_VECTOR_CROSS(Count, Type) \
    DEFINE_VECTOR_CROSS_##Count(Type)

#define DEFINE_VECTOR_CAST(Count, FromType, ToType) \
    internal v##Count##ToType \
    V##Count##FromType##_ToV##Count##ToType(v##Count##FromType V) \
    { \
        v##Count##ToType Result; \
        MAC_FOR((ToType), Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_V_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_NOCAST(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_ToV##Count##Type(v##Count##Type V) \
    { \
        return V; \
    }

#define DEFINE_VECTOR_CLAMP(Count, Type, TypeName) \
    internal v##Count##Type \
    V##Count##Type##_Clamp(v##Count##Type V, Type S, Type E) \
    { \
        v##Count##Type Result = V; \
        MAC_FOR(TypeName, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_CLAMP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_LERP(Count, Type, TypeName)                                             \
    internal v##Count##Type                                                                   \
    V##Count##Type##_Lerp(v##Count##Type A,                                                   \
                          v##Count##Type B,                                                   \
                          r32 T)                                                              \
    {                                                                                         \
        return (v##Count##Type){                                                              \
            MAC_FOR(TypeName, Count, MAC_FOR_OP_SEQ_REV, MAC_FOR_FUNC_LERP, MAC_FOR_ARGS_VEC) \
        };                                                                                    \
    }

#define DEFINE_VECTOR_EQUAL(Count, Type)                                                  \
    internal b08                                                               \
    V##Count##Type##_IsEqual(v##Count##Type A,                                            \
                             v##Count##Type B)                                            \
    {                                                                                     \
        if(MAC_FOR(&&, Count, MAC_FOR_OP_NAME_REV, MAC_FOR_FUNC_VV_EQ, MAC_FOR_ARGS_VEC)) \
            return TRUE;                                                                  \
        return FALSE;                                                                     \
    }

#define DEFINE_VECTOR_LEN(Count, Type)                            \
    internal r32                                                  \
    V##Count##Type##_Len(v##Count##Type V)                        \
    {                                                             \
        v##Count##r32 Vr32 = V##Count##Type##_ToV##Count##r32(V); \
        return R32_sqrt(V##Count##r32_Dot(Vr32, Vr32));           \
    }

#define DEFINE_VECTOR_NORM(Count, Type)                         \
    internal v##Count##r32                                      \
    V##Count##Type##_Norm(v##Count##Type V)                     \
    {                                                           \
        r32 L = V##Count##Type##_Len(V);                        \
        v##Count##r32 CV = V##Count##Type##_ToV##Count##r32(V); \
        return V##Count##r32_DivS(CV, L);                       \
    }

DEFINE_VECTOR_ADD(2, r32)
DEFINE_VECTOR_ADD(2, u32)
DEFINE_VECTOR_ADD(2, s16)
DEFINE_VECTOR_ADD(2, s32)
DEFINE_VECTOR_ADD(3, r32)
DEFINE_VECTOR_ADD(3, s32)

DEFINE_VECTOR_ADDS(3, r32)
DEFINE_VECTOR_ADDS(4, r32)

DEFINE_VECTOR_SUB(2, r32)
DEFINE_VECTOR_SUB(3, r32)
DEFINE_VECTOR_SUB(2, s16)
DEFINE_VECTOR_SUB(2, s32)
DEFINE_VECTOR_SUB(2, u32)

DEFINE_VECTOR_SUBS(3, r32)
DEFINE_VECTOR_SUBS(3, s32)

DEFINE_VECTOR_MUL(2, r32)
DEFINE_VECTOR_MUL(3, r32)

DEFINE_VECTOR_MULS(2, r32)
DEFINE_VECTOR_MULS(3, r32)
DEFINE_VECTOR_MULS(4, r32)
DEFINE_VECTOR_MULS(2, s16)

DEFINE_VECTOR_DIV(2, r32)
DEFINE_VECTOR_DIV(3, r32)
DEFINE_VECTOR_DIV(3, s32)

DEFINE_VECTOR_DIVS(2, r32)
DEFINE_VECTOR_DIVS(3, r32)
DEFINE_VECTOR_DIVS(4, r32)
DEFINE_VECTOR_DIVS(3, u32)

DEFINE_VECTOR_DOT(2, r32)
DEFINE_VECTOR_DOT(2, s16)
DEFINE_VECTOR_DOT(3, r32)
DEFINE_VECTOR_DOT(4, r32)

DEFINE_VECTOR_VOLUME(u32);

DEFINE_VECTOR_CROSS(2, r32)
DEFINE_VECTOR_CROSS(2, s16)

DEFINE_VECTOR_CAST(3, r32, s32)
DEFINE_VECTOR_CAST(3, r32, u32)
DEFINE_VECTOR_CAST(2, s16, r32)
DEFINE_VECTOR_CAST(3, s32, r32)
DEFINE_VECTOR_CAST(3, u32, r32)
DEFINE_VECTOR_CAST(3, u32, s32)

DEFINE_VECTOR_NOCAST(2, r32)
DEFINE_VECTOR_NOCAST(3, r32)

DEFINE_VECTOR_CLAMP(3, r32, R32)
DEFINE_VECTOR_CLAMP(4, r32, R32)
DEFINE_VECTOR_CLAMP(3, s32, S32)

DEFINE_VECTOR_LERP(4, u08, U08)

DEFINE_VECTOR_EQUAL(3, r32)
DEFINE_VECTOR_EQUAL(3, s32)

DEFINE_VECTOR_LEN(2, r32)
DEFINE_VECTOR_LEN(2, s16)
DEFINE_VECTOR_LEN(3, r32)

DEFINE_VECTOR_NORM(2, r32)
DEFINE_VECTOR_NORM(2, s16)
DEFINE_VECTOR_NORM(3, r32)

#undef DEFINE_VECTOR_INIT
#undef DEFINE_VECTOR_ADD
#undef DEFINE_VECTOR_ADDS
#undef DEFINE_VECTOR_SUB
#undef DEFINE_VECTOR_SUBS
#undef DEFINE_VECTOR_MUL
#undef DEFINE_VECTOR_MULS
#undef DEFINE_VECTOR_DIV
#undef DEFINE_VECTOR_DIVS
#undef DEFINE_VECTOR_DOT
#undef DEFINE_VECTOR_CROSS
#undef DEFINE_VECTOR_CAST
#undef DEFINE_VECTOR_NOCAST
#undef DEFINE_VECTOR_LERP
#undef DEFINE_VECTOR_LEN
#undef DEFINE_VECTOR_NORM

internal m4x4r32
M4x4r32_Translation(r32 X,
                    r32 Y,
                    r32 Z)
{
    m4x4r32 Result = M4x4r32_I;
    Result.V[0].E[3] = X;
    Result.V[1].E[3] = Y;
    Result.V[2].E[3] = Z;
    return Result;
}

internal m4x4r32
M4x4r32_Scaling(r32 X,
                r32 Y,
                r32 Z)
{
    m4x4r32 Result = M4x4r32_I;
    Result.V[0].E[0] = X;
    Result.V[1].E[1] = Y;
    Result.V[2].E[2] = Z;
    return Result;
}

internal m4x4r32
M4x4r32_Rotation(r32 Theta,
                 v3r32 Axis)
{
    r32 X,Y,Z, C,S;
    
    v3r32 A = V3r32_Norm(Axis);
    X = A.X; Y = A.Y; Z = A.Z;
    C = R32_cos(Theta);
    S = R32_sin(Theta);
    
    m4x4r32 Result;
    Result.V[0] = (v4r32){X*X*(1-C)+C,   Y*X*(1-C)-Z*S, Z*X*(1-C)+Y*S, 0};
    Result.V[1] = (v4r32){X*Y*(1-C)+Z*S, Y*Y*(1-C)+C,   Z*Y*(1-C)-X*S, 0};
    Result.V[2] = (v4r32){X*Z*(1-C)-Y*S, Y*Z*(1-C)+X*S, Z*Z*(1-C)+C,   0};
    Result.V[3] = (v4r32){0,             0,             0,             1};
    return Result;
}

internal m4x4r32
M4x4r32_Transpose(m4x4r32 M)
{
    m4x4r32 Result;
    Result.V[0] = (v4r32){M.V[0].E[0], M.V[1].E[0], M.V[2].E[0], M.V[3].E[0]};
    Result.V[1] = (v4r32){M.V[0].E[1], M.V[1].E[1], M.V[2].E[1], M.V[3].E[1]};
    Result.V[2] = (v4r32){M.V[0].E[2], M.V[1].E[2], M.V[2].E[2], M.V[3].E[2]};
    Result.V[3] = (v4r32){M.V[0].E[3], M.V[1].E[3], M.V[2].E[3], M.V[3].E[3]};
    return Result;
}

internal v4r32
M4x4r32_MulMV(m4x4r32 M,
              v4r32 V)
{
    v4r32 Result;
    Result.X = V4r32_Dot(M.V[0], V);
    Result.Y = V4r32_Dot(M.V[1], V);
    Result.Z = V4r32_Dot(M.V[2], V);
    Result.W = V4r32_Dot(M.V[3], V);
    return Result;
}

internal m4x4r32
M4x4r32_Mul(m4x4r32 A,
            m4x4r32 B)
{
    B = M4x4r32_Transpose(B);
    
    m4x4r32 Result;
    Result.V[0].E[0] = V4r32_Dot(A.V[0], B.V[0]);
    Result.V[0].E[1] = V4r32_Dot(A.V[0], B.V[1]);
    Result.V[0].E[2] = V4r32_Dot(A.V[0], B.V[2]);
    Result.V[0].E[3] = V4r32_Dot(A.V[0], B.V[3]);
    Result.V[1].E[0] = V4r32_Dot(A.V[1], B.V[0]);
    Result.V[1].E[1] = V4r32_Dot(A.V[1], B.V[1]);
    Result.V[1].E[2] = V4r32_Dot(A.V[1], B.V[2]);
    Result.V[1].E[3] = V4r32_Dot(A.V[1], B.V[3]);
    Result.V[2].E[0] = V4r32_Dot(A.V[2], B.V[0]);
    Result.V[2].E[1] = V4r32_Dot(A.V[2], B.V[1]);
    Result.V[2].E[2] = V4r32_Dot(A.V[2], B.V[2]);
    Result.V[2].E[3] = V4r32_Dot(A.V[2], B.V[3]);
    Result.V[3].E[0] = V4r32_Dot(A.V[3], B.V[0]);
    Result.V[3].E[1] = V4r32_Dot(A.V[3], B.V[1]);
    Result.V[3].E[2] = V4r32_Dot(A.V[3], B.V[2]);
    Result.V[3].E[3] = V4r32_Dot(A.V[3], B.V[3]);
    return Result;
}







internal b08
RayPlaneIntersection(v3r32 PlanePoint, v3r32 PlaneNormal,
                     v3r32 RayPoint, v3r32 RayDir,
                     r32 *T)
{
    v3r32 P0 = PlanePoint;
    v3r32 N = PlaneNormal;
    
    v3r32 L0 = RayPoint;
    v3r32 L = RayDir;
    
    *T = V3r32_Dot(V3r32_Sub(P0, L0), N) / V3r32_Dot(L, N);
    
    return TRUE;
}

internal b08
RayRectIntersectionA(v3r32 RectStart, v3r32 RectEnd, v3r32 RectNormal,
                     v3r32 RayPoint, v3r32 RayDir,
                     r32 *T, v3r32 *Intersection)
{
    v3r32 P0 = RectStart;
    v3r32 N = RectNormal;
    
    v3r32 L0 = RayPoint;
    v3r32 L = RayDir;
    
    r32 LDotN = V3r32_Dot(L, N);
    if(LDotN == 0) return FALSE;
    
    r32 t = V3r32_Dot(V3r32_Sub(P0, L0), N) / LDotN;
    if(T) *T = t;
    
    v3r32 I = V3r32_Add(L0, V3r32_MulS(L, t));
    if(Intersection) *Intersection = I;
    
    r32 Epsilon = 0.000001;
    if(R32_Within(I.X, RectStart.X, RectEnd.X, Epsilon) &&
       R32_Within(I.Y, RectStart.Y, RectEnd.Y, Epsilon) &&
       R32_Within(I.Z, RectStart.Z, RectEnd.Z, Epsilon))
        return TRUE;
    
    return FALSE;
}
