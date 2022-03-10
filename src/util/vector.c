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
DEFINE_VECTOR_ADD(3, r32)

DEFINE_VECTOR_ADDS(3, r32)
DEFINE_VECTOR_ADDS(4, r32)

DEFINE_VECTOR_SUB(2, r32)
DEFINE_VECTOR_SUB(2, s16)
DEFINE_VECTOR_SUB(2, u32)
DEFINE_VECTOR_SUB(3, r32)

DEFINE_VECTOR_MULS(2, r32)
DEFINE_VECTOR_MULS(3, r32)
DEFINE_VECTOR_MULS(4, r32)
DEFINE_VECTOR_MULS(2, s16)

DEFINE_VECTOR_DIV(2, r32)
DEFINE_VECTOR_DIV(3, r32)

DEFINE_VECTOR_DIVS(2, r32)
DEFINE_VECTOR_DIVS(3, r32)
DEFINE_VECTOR_DIVS(4, r32)

DEFINE_VECTOR_DOT(2, r32)
DEFINE_VECTOR_DOT(2, s16)

DEFINE_VECTOR_CROSS(2, r32)
DEFINE_VECTOR_CROSS(2, s16)

DEFINE_VECTOR_CAST(2, s16, r32)
DEFINE_VECTOR_CAST(3, r32, s32)

DEFINE_VECTOR_NOCAST(2, r32)

DEFINE_VECTOR_CLAMP(3, r32, R32)
DEFINE_VECTOR_CLAMP(4, r32, R32)

DEFINE_VECTOR_LERP(4, u08, U08)

DEFINE_VECTOR_LEN(2, r32)
DEFINE_VECTOR_LEN(2, s16)

DEFINE_VECTOR_NORM(2, s16)
DEFINE_VECTOR_NORM(2, r32)

#undef DEFINE_VECTOR_INIT
#undef DEFINE_VECTOR_ADD
#undef DEFINE_VECTOR_SUB
#undef DEFINE_VECTOR_MUL_VV
#undef DEFINE_VECTOR_MUL_VS
#undef DEFINE_VECTOR_DIV
#undef DEFINE_VECTOR_DOT
#undef DEFINE_VECTOR_CROSS
#undef DEFINE_VECTOR_CAST
#undef DEFINE_VECTOR_LEN
#undef DEFINE_VECTOR_NORM