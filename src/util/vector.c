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

#define DEFINE_VECTOR_SUB(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Sub(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        v##Count##Type Result; \
        MAC_FOR(-, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VV_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_MUL_VV(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Mul_VV(v##Count##Type A, \
                            v##Count##Type B) \
    { \
        v##Count##Type Result; \
        MAC_FOR(*, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VV_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_MUL_VS(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Mul_VS(v##Count##Type V, \
                            Type S) \
    { \
        v##Count##Type Result; \
        MAC_FOR(*, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_VS_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_DIV(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Div(v##Count##Type V, \
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

#define DEFINE_VECTOR_LEN(Count, Type) \
    internal r32 \
    V##Count##Type##_Len(v##Count##Type V) \
    { \
        v##Count##r32 Vr32 = V##Count##Type##_ToV##Count##r32(V); \
        r32 Result = V##Count##r32_Dot(Vr32, Vr32); \
        Result = R32_Sqrt(Result); \
        return Result; \
    }

#define DEFINE_VECTOR_NORM(Count, Type) \
    internal v##Count##r32 \
    V##Count##Type##_Norm(v##Count##Type V) \
    { \
        r32 Len = V##Count##Type##_Len(V); \
        v##Count##r32 Result = V##Count##Type##_ToV##Count##r32(V); \
        Result = V##Count##r32_Div(Result, Len); \
        return Result; \
    }

DEFINE_VECTOR_ADD(2, u32)

DEFINE_VECTOR_SUB(2, s16)
DEFINE_VECTOR_SUB(2, u32)

DEFINE_VECTOR_DIV(2, r32)

DEFINE_VECTOR_DOT(2, r32)

DEFINE_VECTOR_CROSS(2, r32)

DEFINE_VECTOR_CAST(2, s16, r32)

DEFINE_VECTOR_LEN(2, s16)

DEFINE_VECTOR_NORM(2, s16)

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