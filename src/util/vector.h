/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// CREDIT https://gitlab.inria.fr/gustedt/p99/-/blob/master/p99/p99_for.h

#define MAC_CAT_2(_0, _1) _0 ## _1
#define MAC_PASTE_2(_0, _1) MAC_CAT_2(_0, _1)

#define MAC_PRED(N) _MAC_PRED(N)
#define _MAC_PRED(N) _MAC_PRED_(MAC_PRED_ , N)
#define _MAC_PRED_(P, N) P ## N
#define MAC_PRED_1 0
#define MAC_PRED_2 1
#define MAC_PRED_3 2
#define MAC_PRED_4 3

#define _MAC_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _MAC_NARG_(...) _MAC_ARG(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,)
#define _MAC_NARG(...) _MAC_NARG_(__VA_ARGS__)

#define MAC_SKP(N, ...) MAC_PASTE_2(MAC_SKP_, N)(__VA_ARGS__)
#define MAC_SKP_0(...) __VA_ARGS__
#define MAC_SKP_1(_0, ...) __VA_ARGS__
#define MAC_SKP_2(_0, ...) MAC_SKP_1(__VA_ARGS__)
#define MAC_SKP_3(_0, ...) MAC_SKP_2(__VA_ARGS__)

#define MAC_CHS(N, ...) _MAC_CHS(MAC_SKP(N, __VA_ARGS__))
#define _MAC_CHS(...) _MAC_CHS_(__VA_ARGS__,)
#define _MAC_CHS_(X, ...) X

#define MAC_PRE_0(...)
#define MAC_PRE_1(_0, ...) _0
#define MAC_PRE_2(_0, ...) _0, MAC_PRE_1(__VA_ARGS__)
#define MAC_PRE_3(_0, ...) _0, MAC_PRE_2(__VA_ARGS__)

#define MAC_LAST(...) MAC_CHS(MAC_PRED(_MAC_NARG(__VA_ARGS__)), __VA_ARGS__,)
#define MAC_ALLBUTLAST(...) MAC_PASTE_2(MAC_PRE_, MAC_PRED(_MAC_NARG(__VA_ARGS__)))(__VA_ARGS__,)

#define MAC_FOR_OP_SEP_REV(NAME, ITER, PREV, CURR) CURR; PREV
#define MAC_FOR_OP_SEQ_REV(NAME, ITER, PREV, CURR) CURR, PREV
#define MAC_FOR_OP_NAME_REV(NAME, ITER, PREV, CURR) CURR NAME PREV

#define MAC_FOR_FUNC_DECLVAR(NAME, ARG, ITER) NAME ARG
#define MAC_FOR_FUNC_V_ASSIGN(NAME, ARG, ITER) Result.ARG = ARG
#define MAC_FOR_FUNC_V_OP(NAME, ARG, ITER) Result.ARG = NAME V.ARG
#define MAC_FOR_FUNC_VV_OP(NAME, ARG, ITER) Result.ARG = A.ARG NAME B.ARG
#define MAC_FOR_FUNC_VS_OP(NAME, ARG, ITER) Result.ARG = V.ARG NAME S
#define MAC_FOR_FUNC_VEC_DOT(NAME, ARG, ITER) (B.ARG*A.ARG)

#define MAC_FOR(NAME, N, OP, FUNC, ...) MAC_PASTE_2(MAC_FOR_, N)(NAME, OP, FUNC, __VA_ARGS__)
#define MAC_FOR_0(NAME, OP, FUNC, ...)
#define MAC_FOR_1(NAME, OP, FUNC, ...) FUNC(NAME, MAC_LAST(__VA_ARGS__), 0)
#define MAC_FOR_2(NAME, OP, FUNC, ...) OP(NAME, 1, MAC_FOR_1(NAME, OP, FUNC, MAC_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, MAC_LAST(__VA_ARGS__), 1))
#define MAC_FOR_3(NAME, OP, FUNC, ...) OP(NAME, 2, MAC_FOR_2(NAME, OP, FUNC, MAC_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, MAC_LAST(__VA_ARGS__), 2))
#define MAC_FOR_4(NAME, OP, FUNC, ...) OP(NAME, 3, MAC_FOR_3(NAME, OP, FUNC, MAC_ALLBUTLAST(__VA_ARGS__)), FUNC(NAME, MAC_LAST(__VA_ARGS__), 3))

#define MAC_FOR_ARGS_VEC W, Z, Y, X

#define DECLARE_VECTOR_TYPE(Count, Type) \
    typedef union v##Count##Type { \
        struct { \
            MAC_FOR(Type, Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_DECLVAR, MAC_FOR_ARGS_VEC); \
        }; \
        Type E[Count]; \
    } v##Count##Type;

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

#define DEFINE_VECTOR_DOT(Count, Type) \
    internal Type \
    V##Count##Type##_Dot(v##Count##Type A, \
                         v##Count##Type B) \
    { \
        Type Result; \
        Result = \
            MAC_FOR(+, Count, MAC_FOR_OP_NAME_REV, MAC_FOR_FUNC_VEC_DOT, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

#define DEFINE_VECTOR_CROSS(Count, Type) \
    internal v##Count##Type \
    V##Count##Type##_Cross(v##Count##Type A, \
                           v##Count##Type B) \
    { \
        v##Count##Type Result; \
        Result.X = (A.Y*B.Z)-(A.Z*B.Y); \
        Result.Y = (A.Z*B.X)-(A.X*B.Z); \
        Result.Z = (A.X*B.Y)-(A.Y*B.X); \
        return Result; \
    }

#define DEFINE_VECTOR_LEN(Count, Type) \
    internal r64 \
    V##Count##Type##_Len(v##Count##Type V) \
    { \
        r64 Result = V##Count##Type##_Dot(V, V); \
        asm("fsqrt" : "+t" (Result)); \
        return Result; \
    }

#define DEFINE_VECTOR_CAST(Count, FromType, ToType) \
    internal v##Count##ToType \
    V##Count##FromType##_ToV##Count##ToType(v##Count##FromType V) \
    { \
        v##Count##ToType Result; \
        MAC_FOR((ToType), Count, MAC_FOR_OP_SEP_REV, MAC_FOR_FUNC_V_OP, MAC_FOR_ARGS_VEC); \
        return Result; \
    }

DECLARE_VECTOR_TYPE(2, r32)
DECLARE_VECTOR_TYPE(2, s32)
DECLARE_VECTOR_TYPE(2, u32)
DECLARE_VECTOR_TYPE(3, r32)
DECLARE_VECTOR_TYPE(4, u08)


#undef DEFINE_VECTOR_MUL_VS
#undef DEFINE_VECTOR_EQUAL
#undef DEFINE_VECTOR_CAST
#undef DECLARE_VECTOR_TYPE