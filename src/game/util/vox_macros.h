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

#if !defined(GAME_UTIL_VOX_MACROS_H_)


//NOTE: Max args == 10
//CREDIT: https://gustedt.gitlabpages.inria.fr/p99/p99-html/index.html



#define _MAC_TOKLIST_       ,  ,  ,  ,  ,  ,  ,  ,  ,
#define _MAC_TOKLIST_0     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define _MAC_TOKLIST_1     1, 1, 1, 1, 1, 1, 1, 1, 1, 1
#define _MAC_TOKLIST_NUMI  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
#define _MAC_TOKLIST_NUMD  9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define _MAC_ARG(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define MAC_HAS_COMMA(...) _MAC_ARG(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)


//NOTE: Count args
#define MAC_NARG(...) _MAC_NARG__(MAC_IS_EMPTY(__VA_ARGS__), _MAC_NARG(__VA_ARGS__))
#define _MAC_NARG(...) _MAC_NARG_(__VA_ARGS__, _MAC_TOKLIST_NUMD)
#define _MAC_NARG_(...) _MAC_ARG(__VA_ARGS__)
#define _MAC_NARG__(IsEmpty, Num) _MAC_NARG___(MAC_CONCAT_2(_MAC_NARG_ISEMPTY_, IsEmpty), Num)
#define _MAC_NARG___(IsEmptyFunc, Num) IsEmptyFunc(Num)
#define _MAC_NARG_ISEMPTY_0(Num) Num
#define _MAC_NARG_ISEMPTY_1(Num) 0


//NOTE: Concatenation
#define  MAC_CONCAT(...) _MAC_CONCAT(MAC_CONCAT_, MAC_NARG(__VA_ARGS__))(__VA_ARGS__)
#define _MAC_CONCAT(...) _MAC_CONCAT_2(__VA_ARGS__)
#define  MAC_CONCAT_0()
#define  MAC_CONCAT_1(_0) _0
#define  MAC_CONCAT_2(_0, _1) _MAC_CONCAT_2(_0, _1)
#define  MAC_CONCAT_3(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_2(__VA_ARGS__))
#define  MAC_CONCAT_4(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_3(__VA_ARGS__))
#define  MAC_CONCAT_5(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_4(__VA_ARGS__))
#define  MAC_CONCAT_6(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_5(__VA_ARGS__))
#define  MAC_CONCAT_7(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_6(__VA_ARGS__))
#define  MAC_CONCAT_8(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_7(__VA_ARGS__))
#define  MAC_CONCAT_9(_0, ...) MAC_CONCAT_2(_0, MAC_CONCAT_8(__VA_ARGS__))

#define _MAC_CONCAT_2(_0, _1) _0 ## _1


#define _MAC_DEC_0 neg1
#define _MAC_DEC_1 0
#define _MAC_DEC_2 1
#define _MAC_DEC_3 2
#define _MAC_DEC_4 3
#define _MAC_DEC_5 4
#define _MAC_DEC_6 5
#define _MAC_DEC_7 6
#define _MAC_DEC_8 7
#define _MAC_DEC_9 8
#define  MAC_DEC(N) MAC_CONCAT_2(_MAC_DEC_, N)

// #define _MAC_INC_0 1
// #define _MAC_INC_1 2
// #define _MAC_INC_2 3
// #define _MAC_INC_3 4
// #define _MAC_INC_4 5
// #define _MAC_INC_5 6
// #define _MAC_INC_6 7
// #define _MAC_INC_7 8
// #define _MAC_INC_8 9
// #define  MAC_INC(N) MAC_CONCAT_2(_MAC_INC_, N)




//NOTE: Argument list segmentation
#define _MAC_ARGS_S_0(...)
#define _MAC_ARGS_S_1(_0, ...) _0
#define _MAC_ARGS_S_2(_0, ...) _0, _MAC_ARGS_S_1(__VA_ARGS__)
#define _MAC_ARGS_S_3(_0, ...) _0, _MAC_ARGS_S_2(__VA_ARGS__)
#define _MAC_ARGS_S_4(_0, ...) _0, _MAC_ARGS_S_3(__VA_ARGS__)
#define _MAC_ARGS_S_5(_0, ...) _0, _MAC_ARGS_S_4(__VA_ARGS__)
#define _MAC_ARGS_S_6(_0, ...) _0, _MAC_ARGS_S_5(__VA_ARGS__)
#define _MAC_ARGS_S_7(_0, ...) _0, _MAC_ARGS_S_6(__VA_ARGS__)
#define _MAC_ARGS_S_8(_0, ...) _0, _MAC_ARGS_S_7(__VA_ARGS__)
#define _MAC_ARGS_S_9(_0, ...) _0, _MAC_ARGS_S_8(__VA_ARGS__)
#define  MAC_ARGS_S_N(N, ...) MAC_CONCAT_2(_MAC_ARGS_S_, N)(__VA_ARGS__) // Start to N
#define  MAC_ARGS_S_EM1(...) MAC_ARGS_S_N(MAC_DEC(_MAC_NARG(__VA_ARGS__)), __VA_ARGS__) // Start to End minus 1

#define  MAC_ARGS_N_NPL(N, L, ...) _MAC_ARGS_N_NPL(N, MAC_ARGS_N_E(N, __VA_ARGS__)) // N to N plus Length
#define _MAC_ARGS_N_NPL(L, ...) MAC_ARGS_S_N(L, __VA_ARGS__)

#define _MAC_ARGS_0_E(...)     __VA_ARGS__
#define _MAC_ARGS_1_E(_0, ...) __VA_ARGS__
#define _MAC_ARGS_2_E(_0, ...) _MAC_ARGS_1_E(__VA_ARGS__)
#define _MAC_ARGS_3_E(_0, ...) _MAC_ARGS_2_E(__VA_ARGS__)
#define _MAC_ARGS_4_E(_0, ...) _MAC_ARGS_3_E(__VA_ARGS__)
#define _MAC_ARGS_5_E(_0, ...) _MAC_ARGS_4_E(__VA_ARGS__)
#define _MAC_ARGS_6_E(_0, ...) _MAC_ARGS_5_E(__VA_ARGS__)
#define _MAC_ARGS_7_E(_0, ...) _MAC_ARGS_6_E(__VA_ARGS__)
#define _MAC_ARGS_8_E(_0, ...) _MAC_ARGS_7_E(__VA_ARGS__)
#define _MAC_ARGS_9_E(_0, ...) _MAC_ARGS_8_E(__VA_ARGS__)
#define  MAC_ARGS_N_E(N, ...) MAC_CONCAT_3(_MAC_ARGS_, N, _E)(__VA_ARGS__) // N to End
#define  MAC_ARGS_EMN_E(N, ...) MAC_CONCAT(_MAC_ARGS_, MAC_SUB(MAC_NARG(__VA_ARGS__), N), _E)(__VA_ARGS__) // N from End, to End

#define  MAC_ARGS_A_B(A, B, ...) MAC_ARGS_N_E(A, MAC_ARGS_S_N(B, __VA_ARGS__))

#define  MAC_ARG_E(...) MAC_ARG_N(MAC_DEC(_MAC_NARG(__VA_ARGS__)), __VA_ARGS__)
#define  MAC_ARG_N(N, ...) _MAC_ARG_N(MAC_ARGS_N_E(N, __VA_ARGS__))
#define _MAC_ARG_N(...) _MAC_ARG__N(__VA_ARGS__,)
#define _MAC_ARG__N(_0, ...) _0


// #define _MAC_REPEAT_0(...)
// #define _MAC_REPEAT_1(...) __VA_ARGS__
// #define _MAC_REPEAT_2(...) _MAC_REPEAT_1(__VA_ARGS__)__VA_ARGS__
// #define _MAC_REPEAT_3(...) _MAC_REPEAT_2(__VA_ARGS__)__VA_ARGS__
// #define _MAC_REPEAT_4(...) _MAC_REPEAT_3(__VA_ARGS__)__VA_ARGS__
// #define _MAC_REPEAT_5(...) _MAC_REPEAT_4(__VA_ARGS__)__VA_ARGS__
// #define MAC_REPEAT(...) MAC_CONCAT(_MAC_REPEAT_0_, MAC_IS_LT(MAC_NARG(__VA_ARGS__), 2)))(__VA_ARGS__)



#define  MAC_REMOVE_PARENTHESIS(...) \
    MAC_IF_ELSE(MAC_HAS_NO_PARENTHESIS(__VA_ARGS__)) \
        (__VA_ARGS__) \
        (_MAC_REMOVE_PARENTHESIS(__VA_ARGS__))

#define _MAC_REMOVE_PARENTHESIS(_0) _MAC_REMOVE_PARENTHESIS_ _0
#define _MAC_REMOVE_PARENTHESIS_(...) __VA_ARGS__



#define _MAC_ROBUST(...) __VA_ARGS__ // Not sure what this does


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Logic
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if 1 // _MAC_IS_*_EQ_* and _MAC_IS_EQ_*
#define _MAC_IS__EQ_(...) ,
#define _MAC_IS_0_EQ_0(...) ,
#define _MAC_IS_1_EQ_1(...) ,
#define _MAC_IS_2_EQ_2(...) ,
#define _MAC_IS_3_EQ_3(...) ,
#define _MAC_IS_4_EQ_4(...) ,
#define _MAC_IS_5_EQ_5(...) ,
#define _MAC_IS_6_EQ_6(...) ,
#define _MAC_IS_7_EQ_7(...) ,
#define _MAC_IS_8_EQ_8(...) ,
#define _MAC_IS_9_EQ_9(...) ,

//TODO: Remove these? (replace with MAC_IS_EQ)
#define _MAC_IS_EQ_0(...) MAC_IS_EQ(0, __VA_ARGS__)
#define _MAC_IS_EQ_1(...) MAC_IS_EQ(1, __VA_ARGS__)
#define _MAC_IS_EQ_2(...) MAC_IS_EQ(2, __VA_ARGS__)
#define _MAC_IS_EQ_3(...) MAC_IS_EQ(3, __VA_ARGS__)
#define _MAC_IS_EQ_4(...) MAC_IS_EQ(4, __VA_ARGS__)
#define _MAC_IS_EQ_5(...) MAC_IS_EQ(5, __VA_ARGS__)
#define _MAC_IS_EQ_6(...) MAC_IS_EQ(6, __VA_ARGS__)
#define _MAC_IS_EQ_7(...) MAC_IS_EQ(7, __VA_ARGS__)
#define _MAC_IS_EQ_8(...) MAC_IS_EQ(8, __VA_ARGS__)
#define _MAC_IS_EQ_9(...) MAC_IS_EQ(9, __VA_ARGS__)
#endif

#define MAC_IS_NOT_EMPTY(...) MAC_NOT(MAC_IS_EMPTY(__VA_ARGS__))
#define MAC_IS_EMPTY(...) _MAC_IS_EMPTY(MAC_HAS_COMMA(             __VA_ARGS__),               /* Is there a comma? */       \
                                        MAC_HAS_COMMA(_MAC_IS__EQ_ __VA_ARGS__),               /* Are there parenthesis? */  \
                                        MAC_HAS_COMMA(             __VA_ARGS__ (,,,,,,,,,,,)), /* Is it a function? */       \
                                        MAC_HAS_COMMA(_MAC_IS__EQ_ __VA_ARGS__ (,,,,,,,,,,,))) /* Is there anything else? */
#define _MAC_IS_EMPTY(_0, _1, _2, _3) MAC_HAS_COMMA(MAC_CONCAT_5(_MAC_IS_EMPTY_, _0, _1, _2, _3))
#define _MAC_IS_EMPTY_0000 _0000
#define _MAC_IS_EMPTY_0001 ,
#define _MAC_IS_EMPTY_0010 _0010
#define _MAC_IS_EMPTY_0011 _0011
#define _MAC_IS_EMPTY_0100 _0100
#define _MAC_IS_EMPTY_0101 _0101
#define _MAC_IS_EMPTY_0110 _0110
#define _MAC_IS_EMPTY_0111 _0111
#define _MAC_IS_EMPTY_1000 _1000
#define _MAC_IS_EMPTY_1001 _1001
#define _MAC_IS_EMPTY_1010 _1010
#define _MAC_IS_EMPTY_1011 _1011
#define _MAC_IS_EMPTY_1100 _1100
#define _MAC_IS_EMPTY_1101 _1101
#define _MAC_IS_EMPTY_1110 _1110
#define _MAC_IS_EMPTY_1111 _1111

#define MAC_HAS_PARENTHESIS(...) ,
#define MAC_HAS_NO_PARENTHESIS(...) MAC_IS_EQ(_MAC_NARG(__VA_ARGS__), _MAC_NARG(MAC_HAS_PARENTHESIS __VA_ARGS__))

#define MAC_IS_EQ(Token, ...) _MAC_IS_EQ(MAC_CONCAT(_MAC_IS_, Token, _EQ_), __VA_ARGS__)
#define _MAC_IS_EQ(Macro, ...) _MAC_IS_EQ_(Macro, __VA_ARGS__)
#define _MAC_IS_EQ_(Macro, ...) MAC_HAS_COMMA(MAC_CONCAT(_MAC_IS_EQ__, MAC_NARG(Macro ## __VA_ARGS__ (~) Macro ## __VA_ARGS__))(~))
#define _MAC_IS_EQ__0(...) ~
#define _MAC_IS_EQ__1(...) ~
#define _MAC_IS_EQ__2(...) ,
// #define _MAC_IS_EQ__3(...) ~
// #define _MAC_IS_EQ__4(...) ~

#define  MAC_IS_LT(_0, _1) _MAC_IS_LT(_MAC_IS_EQ_0(_1), MAC_ARG_N(_0, MAC_ARGS_S_N(_1, _MAC_TOKLIST_1), _MAC_TOKLIST_0))
#define _MAC_IS_LT(_0, _1) MAC_CONCAT(_MAC_IS_LT__, _0)(_1)
#define _MAC_IS_LT__0(_0) _0
#define _MAC_IS_LT__1(_0) FALSE

#define MAC_IS_GT(_0, _1) MAC_IS_LT(_1, _0)
#define MAC_IS_LE(_0, _1) MAC_IS_GE(_1, _0)

#define  MAC_IS_GE(_0, _1) _MAC_IS_GE(MAC_IS_EQ(0, _1), MAC_ARG_N(_0, MAC_ARGS_S_N(_1, _MAC_TOKLIST_0), _MAC_TOKLIST_1))
#define _MAC_IS_GE(_1Is0, _0IsGE_1) MAC_CONCAT(_MAC_IS_GE__, _1Is0)(_0IsGE_1)
#define _MAC_IS_GE__0(_0) _0 // (A <= B)?
#define _MAC_IS_GE__1(_0)  1 // 0 <= N

#define MAC_NOT(_0) _MAC_NOT(MAC_IS_EQ(_0, 0))
#define _MAC_NOT(_0Is0) MAC_CONCAT(_MAC_NOT__, _0Is0)
#define _MAC_NOT__0 0
#define _MAC_NOT__1 1

#define MAC_ADD(_0, _1) _MAC_ADD(_0, _1, MAC_IS_EQ(0, _0), MAC_IS_EQ(0, _1), MAC_NARG(MAC_ARGS_S_N(_0, _MAC_TOKLIST_0), \
                                                                                      MAC_ARGS_S_N(_1, _MAC_TOKLIST_0)))
#define _MAC_ADD(_0, _1, _0Is0, _1Is0, Sum) MAC_CONCAT(_MAC_ADD__, _0Is0, _1Is0)(_0, _1, Sum)
#define _MAC_ADD__00(_0, _1, Sum)  Sum // A + B = Sum
#define _MAC_ADD__01(_0, _1, Sum) _0   // A + 0 = A
#define _MAC_ADD__10(_0, _1, Sum) _1   // 0 + B = B
#define _MAC_ADD__11(_0, _1, Sum)  0   // 0 + 0 = 0

//TODO: Test with _MAC_TOKLIST__
#define  MAC_SUB(_0, _1) _MAC_SUB(_0, _1, MAC_IS_EQ(_0, _1), MAC_IS_EQ(0, _0), MAC_IS_EQ(0, _1))
#define _MAC_SUB(_0, _1, _0Is_1, _0Is0, _1Is0) MAC_CONCAT(_MAC_SUB__, _0Is_1, _0Is0, _1Is0)(_0, _1)
#define _MAC_SUB__000(_0, _1) _MAC_SUB_(_0, _1, MAC_IS_LT(_0, _1)) // A - B =  Diff
#define _MAC_SUB__001(_0, _1) _0                                   // N - 0 =  N
#define _MAC_SUB__010(_0, _1) MAC_CONCAT(neg, _1)                 // 0 - N = -N
#define _MAC_SUB__011(_0, _1) MAC_ERROR__SUB_INVALID_OPTION
#define _MAC_SUB__100(_0, _1) 0                                    // N - N =  0
#define _MAC_SUB__101(_0, _1) MAC_ERROR__SUB_INVALID_OPTION
#define _MAC_SUB__110(_0, _1) MAC_ERROR__SUB_INVALID_OPTION
#define _MAC_SUB__111(_0, _1) MAC_ERROR__SUB_INVALID_OPTION
#define _MAC_SUB_(_0, _1, _0LT_1) MAC_CONCAT(_MAC_SUB__, _0LT_1)(_0, _1)
#define _MAC_SUB__0(_0, _1) _MAC_SUB__(_0, _1)                     // A > B; Diff =   A - B
#define _MAC_SUB__1(_0, _1) MAC_CONCAT(neg, _MAC_SUB__(_1, _0))    // A < B; Diff = -(B - A)
#define _MAC_SUB__(_0, _1) MAC_NARG(MAC_ARGS_N_E(_1, MAC_ARGS_S_N(_0, _MAC_TOKLIST_0)))


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Conditionals
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define MAC_CLAUSE_IGNORE(...) // Removes a clause
#define MAC_CLAUSE_IDENTITY(...) __VA_ARGS__
#define _MAC_CLAUSE_1(...) __VA_ARGS__ MAC_CLAUSE_IGNORE
#define _MAC_CLAUSE_2(...) MAC_CLAUSE_IDENTITY // The first clause is eaten by _MAC_CLAUSE_2, so the second is kept

#define _MAC_IF_CLAUSE(Expression) _MAC_IF_CLAUSE_(Expression, _MAC_CLAUSE_1, _MAC_CLAUSE_2, ~)
#define _MAC_IF_CLAUSE_(A, B, C, ...) C

#define MAC_IF_EQ(A, B) _MAC_IF_CLAUSE(MAC_CONCAT(_MAC_IS_, A, _EQ_, B)())

#define MAC_IF_LT(A, B) MAC_IF_EQ(MAC_IS_LT(A, B), TRUE)
#define MAC_IF_GT(A, B) MAC_IF_EQ(MAC_IS_GT(A, B), TRUE)
#define MAC_IF_LE(A, B) MAC_IF_EQ(MAC_IS_LE(A, B), TRUE)
#define MAC_IF_GE(A, B) MAC_IF_EQ(MAC_IS_GE(A, B), TRUE)

#define MAC_IF_ELSE(...) MAC_IF_EQ(0, MAC_IS_EQ(0, __VA_ARGS__))

#define MAC_IF_EMPTY(...)     MAC_IF_EQ(MAC_IS_EMPTY(__VA_ARGS__), 1)
#define MAC_IF_NOT_EMPTY(...) MAC_IF_EQ(MAC_IS_EMPTY(__VA_ARGS__), 0)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Looping
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define __MAC_FOR_OP_SEPARATE(Constant, Iteration, Prev, Curr) Prev; Curr
#define __MAC_FOR_OP_SEQUENCE(Constant, Iteration, Prev, Curr) Prev, Curr
#define __MAC_FOR_OP_CONCAT(Constant, Iteration, Prev, Curr) MAC_CONCAT(Prev, Curr)
#define __MAC_FOR_OP_SERIES(Constant, Iteration, Prev, Curr) Prev Curr
#define __MAC_FOR_OP_FUNC_PIC(Constant, Iteration, Prev, Curr) Constant(Prev, Iteration, Curr)

#define __MAC_FOR_FUNC_ENUM_ENTRY(Constant, Iteration, Data) MAC_CONCAT(Constant, _, Data) = Iteration
#define __MAC_FOR_FUNC_ASSIGN_DI(Constant, Iteration, Data) Data = Iteration
#define __MAC_FOR_FUNC_CONCAT_CD(Constant, Iteration, Data) MAC_CONCAT(Constant, Data)
#define __MAC_FOR_FUNC_SEQUENCE(Constant, Iteration, Data) MAC_REMOVE_PARENTHESIS(Constant), Data
#define __MAC_FOR_FUNC_MAKENAME(Constant, Iteration, Data) MAC_CONCAT_3(Constant, _, Iteration)
#define __MAC_FOR_FUNC_CONSTANT(Constant, Iteration, Data) Constant
#define __MAC_FOR_FUNC_DECLVAR(Constant, Iteration, Data) Constant Data
#define __MAC_FOR_FUNC_TYPEPTR(Constant, Iteration, Data) Data*
#define __MAC_FOR_FUNC_ACCESS(Constant, Iteration, Data) MAC_IF_EMPTY(Constant) ([Data]) ((Constant)[Data])
#define __MAC_FOR_FUNC_GROUP2(Constant, Iteration, Data) (Data, Data)
#define __MAC_FOR_FUNC_DATA(Constant, Iteration, Data) Data
#define __MAC_FOR_FUNC_FUNC_ID(Constant, Iteration, Data) Constant(Iteration, Data)

#define  __MAC_FOR(Constant, Iterations, Operation, Function, ...) MAC_CONCAT_2(___MAC_FOR_, Iterations)(Constant, Operation, Function, __VA_ARGS__)
#define ___MAC_FOR_0(Constant, Operation, Function, ...)
#define ___MAC_FOR_1(Constant, Operation, Function, ...)  Function(Constant, 0, _MAC_ARGS_S_1(__VA_ARGS__,))
#define ___MAC_FOR_2(Constant, Operation, Function, ...) Operation(Constant, 1, ___MAC_FOR_1(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 1, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_3(Constant, Operation, Function, ...) Operation(Constant, 2, ___MAC_FOR_2(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 2, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_4(Constant, Operation, Function, ...) Operation(Constant, 3, ___MAC_FOR_3(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 3, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_5(Constant, Operation, Function, ...) Operation(Constant, 4, ___MAC_FOR_4(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 4, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_6(Constant, Operation, Function, ...) Operation(Constant, 5, ___MAC_FOR_4(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 5, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_7(Constant, Operation, Function, ...) Operation(Constant, 6, ___MAC_FOR_4(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 6, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_8(Constant, Operation, Function, ...) Operation(Constant, 7, ___MAC_FOR_4(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 7, MAC_ARG_E(__VA_ARGS__)))
#define ___MAC_FOR_9(Constant, Operation, Function, ...) Operation(Constant, 8, ___MAC_FOR_4(Constant, Operation, Function, MAC_ARGS_S_EM1(__VA_ARGS__)), Function(Constant, 8, MAC_ARG_E(__VA_ARGS__)))


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define _MAC_GENERIC_OP_TYPE(T, ...) T
#define _MAC_GENERIC_OP_EXPRESSION(T, ...) __VA_ARGS__

#define _MAC_GENERIC_EXPRESSION(LeftOp, RightOp, Expression, Iteration, Pair) LeftOp Pair: RightOp Pair
#define _MAC_GENERIC_FUNC_EXPRESSION(Expression, Iteration, Pair) _MAC_GENERIC_EXPRESSION(_MAC_GENERIC_OP_TYPE, _MAC_GENERIC_OP_EXPRESSION, Expression, Iteration, Pair)

#define _MAC_GENERIC_(N, Function, Expression, Default, ...)                   \
_Generic((Expression),                                                         \
         MAC_IF_EMPTY(Default)()(default: (Default),)                          \
         __MAC_FOR((Expression), N, __MAC_FOR_OP_SEQUENCE, Function, __VA_ARGS__))

#define _MAC_GENERIC__(Function, Expression, Default, ...) \
_MAC_GENERIC_(MAC_NARG(__VA_ARGS__), _MAC_ROBUST(Function), _MAC_ROBUST(Expression), _MAC_ROBUST(Default), __VA_ARGS__)

#define _MAC_GENERIC(N, ...) MAC_IF_LT(N, 3)()(_MAC_GENERIC__(__VA_ARGS__))
#define  MAC_GENERIC(...) _MAC_GENERIC(MAC_NARG(__VA_ARGS__), _MAC_GENERIC_FUNC_EXPRESSION, __VA_ARGS__)

#define MAC_TYPE_OF(Var) MAC_GENERIC((Var), , __MAC_FOR(, MAC_NARG(TYPE_LIST), __MAC_FOR_OP_SEQUENCE, __MAC_FOR_FUNC_TYPETOK, TYPE_LIST))


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Definitions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: Types


#define MAC_GENERIC_EXPRESSION(Function, Data, Type) Type: Function(Type, MAC_REMOVE_PARENTHESIS(Data))

#define __MAC_GENERIC(Condition, Default, Function, Data, ...) \
    _Generic(Condition, \
         MAC_IF_EMPTY(Default)()(default: (Default),) \
         MAC_EVAL(MAC_WHILE(MAC__FE0_DE1_E2, (MAC_IS_LT, 0, MAC_ADD(MAC_NARG(__VA_ARGS__), 1)), \
                            MAC__RDE1_FE0_DE1, (MAC_INC, 0), \
                            MAC__RDE3_FE0_E1_E2_DDE3, (MAC_GENERIC_EXPRESSION, Function, Data, 0), \
                            MAC__DE0DE, 1, \
                            (1, __VA_ARGS__))))


//SUBSECTION: Arithmetic


#define MAC_INC(N, ...) MAC_ADD(N, 1)


//SUBSECTION: Loops


#define MAC_WHILE(Cond, CondExt,                                  \
                  Iter, IterExt,                                  \
                  Func, FuncExt,                                  \
                  Out, OutExt,                                    \
                  Data)                                           \
    MAC_IF_ELSE(MAC_IF_NOT_EMPTY(Cond)                            \
                (                                                 \
                    MAC_DEFER(Cond)                               \
                    (                                             \
                        MAC_IF_NOT_EMPTY(CondExt)                 \
                        (                                         \
                            MAC_REMOVE_PARENTHESIS(CondExt),      \
                        )()                                       \
                        MAC_REMOVE_PARENTHESIS(Data)              \
                    )                                             \
                )                                                 \
                ( TRUE ))                                         \
    (                                                             \
        MAC_DEFER_3(_MAC_DEFERRED__WHILE)()                       \
        (                                                         \
            Cond, CondExt,                                        \
            Iter, IterExt,                                        \
            Func, FuncExt,                                        \
            Out,  OutExt,                                         \
            (MAC_DEFER_2(MAC_IF_NOT_EMPTY)(Iter)                  \
             (                                                    \
                 MAC_DEFER_2(Iter)                                \
                 (                                                \
                     MAC_IF_NOT_EMPTY(IterExt)                    \
                     (                                            \
                         MAC_REMOVE_PARENTHESIS(IterExt),         \
                     )()                                          \
                     MAC_DEFER_2(MAC_IF_NOT_EMPTY)(Func)          \
                     (                                            \
                         MAC_DEFER_2(Func)                        \
                         (                                        \
                             MAC_IF_NOT_EMPTY(FuncExt)            \
                             (                                    \
                                 MAC_REMOVE_PARENTHESIS(FuncExt), \
                             )()                                  \
                             MAC_REMOVE_PARENTHESIS(Data)         \
                         )                                        \
                     )                                            \
                     (                                            \
                         MAC_REMOVE_PARENTHESIS(Data)             \
                     )                                            \
                 )                                                \
             )                                                    \
             (                                                    \
                 MAC_DEFER_2(MAC_IF_NOT_EMPTY)(Func)              \
                 (                                                \
                     MAC_DEFER_2(Func)                            \
                     (                                            \
                         MAC_IF_NOT_EMPTY(FuncExt)                \
                         (                                        \
                             MAC_REMOVE_PARENTHESIS(FuncExt),     \
                         )()                                      \
                         MAC_REMOVE_PARENTHESIS(Data)             \
                     )                                            \
                 )                                                \
                 (                                                \
                     MAC_REMOVE_PARENTHESIS(Data)                 \
                 )                                                \
            ))                                                    \
        )                                                         \
    )                                                             \
    (                                                             \
        MAC_IF_NOT_EMPTY(Out)                                     \
        (                                                         \
            MAC_DEFER(Out)                                        \
            (                                                     \
                MAC_IF_NOT_EMPTY(OutExt)                          \
                (                                                 \
                    MAC_REMOVE_PARENTHESIS(OutExt),               \
                )()                                               \
                MAC_REMOVE_PARENTHESIS(Data)                      \
            )                                                     \
        )                                                         \
        (                                                         \
            MAC_REMOVE_PARENTHESIS(Data)                          \
        )                                                         \
    )
#define _MAC_DEFERRED__WHILE() MAC_WHILE





#define MAC__RDE3_FE0_E1_E2_DE3(FE0, E1, E2, DE3, ...) /* Data[0..Ext[3]] Ext[0](Ext[1], Ext[2], Data[Ext[3]]) Data[Ext[3]+1..Len(Data)] */ \
    MAC_IF_GT(DE3, 0) (MAC_ARGS_S_N(DE3, __VA_ARGS__),)()                                                                                   \
    FE0(E1, E2, MAC_ARG_N(DE3, __VA_ARGS__))                                                                                                \
    MAC_IF_LT(MAC_INC(DE3), MAC_NARG(__VA_ARGS__)) (, MAC_ARGS_N_E(MAC_INC(DE3), __VA_ARGS__))()

#define MAC__RDE2_FE0_E1_DE2(FE0, E1, DE2, ...) /* Data[0..Ext[1]] Ext[0](Data[Ext[1]]) Data[Ext[1]+1..Len(Data)] */ \
    MAC_IF_GT(DE2, 0) (MAC_ARGS_S_N(DE2, __VA_ARGS__),)()                                                     \
    FE0(E1, MAC_ARG_N(DE2, __VA_ARGS__))                                                                          \
    MAC_IF_LT(MAC_INC(DE2), MAC_NARG(__VA_ARGS__)) (, MAC_ARGS_N_E(MAC_INC(DE2), __VA_ARGS__))()

// Replace Data[Ext[1]] with FE0(Data[Ext[1]])
#define MAC__RDE1_FE0_DE1(FE0, DE1, ...) /* Data[0..Ext[1]] Ext[0](Data[Ext[1]]) Data[Ext[1]+1..Len(Data)] */ \
    MAC_IF_GT(DE1, 0) (MAC_ARGS_S_N(DE1, __VA_ARGS__),)()                                                     \
    FE0(MAC_ARG_N(DE1, __VA_ARGS__))                                                                          \
    MAC_IF_LT(MAC_INC(DE1), MAC_NARG(__VA_ARGS__)) (, MAC_ARGS_N_E(MAC_INC(DE1), __VA_ARGS__))()

#define MAC__FE0_DE1_E2(FE0, DE1, E2, ...) /* Ext[0](Data[Ext[1]], Ext[2]) */ \
    FE0(MAC_ARG_N(DE1, __VA_ARGS__), E2)
#define MAC__FE0_E1_DE2(FE0, E1, DE2, ...) /* Ext[0](Data[1], Ext[Ext[2]]) */ \
    FE0(E1, MAC_ARG_N(DE2, __VA_ARGS__))
#define MAC__FE0_DE1_DE2(FE0, DE1, DE2, ...) /* Ext[0](Data[Ext[1]], Data[Ext[2]]) */ \
    FE0(MAC_ARG_N(DE1, __VA_ARGS__), MAC_ARG_N(DE2, __VA_ARGS__))

#define MAC__FE0_DE1(FE0, DE1, ...) /* Ext[0](Data[Ext[1]]) */ \
    FE0(MAC_ARG_N(DE1, __VA_ARGS__))


#define MAC__FE0_E1_E2_FE3_DE4_E5(FE0, E1, E2, FE3, DE4, E5, ...) /* Ext[0](Data[Ext[1]], Ext[2]) */ \
    FE0(E1, E2, FE3(MAC_ARG_N(DE4, __VA_ARGS__), E5))

#define MAC__DE0DE(DE0, ...) /* Data[Ext[0]..Len(Data)] */ \
    MAC_ARGS_N_E(DE0, __VA_ARGS__)
// #define MAC__DSDE0(DE0, ...) /* Data[Data[0]..Ext[0]] */ \
//     MAC_ARGS_S_N(DE0, __VA_ARGS__)

#define MAC_WHILE_ITER__ROTATE_LEFT__MERGE(...) \
    MAC_ARGS_N_E(1, __VA_ARGS__) MAC_ARG_N(0, __VA_ARGS__)

#define MAC_WHILE_FUNC__ACCESS_ARRAY(Array, DataIndex, DataOffset, ...) \
    MAC__RDE2_FE0_E1_DE2(MAC_ACCESS_ARRAY, Array, MAC_ADD(MAC_ARG_N(DataIndex, __VA_ARGS__), DataOffset), __VA_ARGS__)
#define MAC_WHILE_FUNC__DECL_VAR(Type, DataIndex, DataOffset, ...) \
    MAC__RDE2_FE0_E1_DE2(MAC_DECL_VAR, Type, MAC_ADD(MAC_ARG_N(DataIndex, __VA_ARGS__), DataOffset), __VA_ARGS__)
#define MAC__RDE3_FE0_E1_E2_DDE3(FE0, E1, E2, DE3, ...) \
    MAC__RDE3_FE0_E1_E2_DE3(FE0, E1, E2, MAC_ARG_N(DE3, __VA_ARGS__), __VA_ARGS__)


#define MAC_ACCESSORS(Array, ...)                                                         \
    MAC_EVAL(MAC_WHILE(MAC__FE0_DE1_E2,         (MAC_IS_LT, 0, MAC_NARG(__VA_ARGS__)), \
                       MAC__RDE1_FE0_DE1,       (MAC_INC, 0),                    \
                       MAC_WHILE_FUNC__ACCESS_ARRAY, (Array, 0, 1), \
                       MAC__DE0DE,               1,                                    \
                       (0, __VA_ARGS__)))

#define MAC_GEN_TYPE__Alias 1
#define MAC_GEN_TYPE__Vector 2

#define MAC_ACCESS_ARRAY(Array, N) Array[N]
#define MAC_DECL_VAR(Type, Name) Type Name;

#define MAC_GEN_TYPE(PackedType) MAC_DEFER(_MAC_GEN_TYPE)(MAC_REMOVE_PARENTHESIS(PackedType))
#define _MAC_GEN_TYPE(Type, ...) _MAC_GEN_TYPE_(MAC_CONCAT(MAC_GEN_TYPE__, Type), __VA_ARGS__)
#define _MAC_GEN_TYPE_(Type, ...) \
    MAC_DEFER(MAC_IF_EQ)(Type, MAC_GEN_TYPE__Alias) \
    ( \
        typedef MAC_GEN_TYPE__ALIAS(MAC_ARG_N(0, __VA_ARGS__), MAC_ARG_N(1, __VA_ARGS__)) \
    )( \
    MAC_DEFER(MAC_IF_EQ)(Type, MAC_GEN_TYPE__Vector) \
    ( \
        typedef MAC_GEN_TYPE__VECTOR(MAC_ARG_N(0, __VA_ARGS__), MAC_ARG_N(1, __VA_ARGS__)) \
    ) \
 /* else */ \
    ( \
        MAC_ERROR__GEN_TYPE__INVALID_TYPE \
    ))

#define MAC_GEN_TYPE__ALIAS(Original, Alias) \
    Original Alias;
#define MAC_GEN_STRUCT__UNIFORM(Name, ComponentType, ...) \
    struct Name \
    { \
        MAC_CONCAT(MAC_GEN_STRUCT__UNIFORM_, MAC_NARG(__VA_ARGS__))(Name, ComponentType, __VA_ARGS__) \
    } Name;
#define MAC_GEN_STRUCT__UNIFORM_1(Name, ComponentType, _0) \
        ComponentType _0;
#define MAC_GEN_STRUCT__UNIFORM_2(Name, ComponentType, _0, _1) \
        MAC_GEN_STRUCT__UNIFORM_1(Name, ComponentType, _0) \
        ComponentType _1;
#define MAC_GEN_STRUCT__UNIFORM_3(Name, ComponentType, _0, _1, _2) \
        MAC_GEN_STRUCT__UNIFORM_2(Name, ComponentType, _0, _1) \
        ComponentType _2;
#define MAC_GEN_STRUCT__UNIFORM_4(Name, ComponentType, _0, _1, _2, _3) \
        MAC_GEN_STRUCT__UNIFORM_3(Name, ComponentType, _0, _1, _2) \
        ComponentType _3;
#define MAC_GEN_TYPE__VECTOR(Count, ComponentType) \
    union MAC_CONCAT(v, Count, ComponentType) \
    { \
        MAC_DEFER(MAC_IF_LE)(Count, 4) \
        ( \
            MAC_GEN_STRUCT__UNIFORM(, ComponentType, MAC_ARGS_S_N(Count, X, Y, Z, W)) \
            MAC_DEFER(MAC_IF_EQ)(Count, 3) \
            ( \
                MAC_GEN_STRUCT__UNIFORM(, MAC_CONCAT(v2, ComponentType), XY, _UnusedZ_) \
            )() \
            MAC_DEFER(MAC_IF_EQ)(Count, 4) \
            ( \
                MAC_GEN_STRUCT__UNIFORM(, MAC_CONCAT(v2, ComponentType), XY, ZW) \
            )() \
        )() \
        ComponentType E[Count]; \
    } MAC_CONCAT(v, Count, ComponentType);


//SUBSECTION: Recursion


//CREDIT: https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms

#define MAC_NOP()
#define MAC_DEFER(_0)   _0 MAC_NOP()
#define MAC_DEFER_2(_0) _0 MAC_DEFER(MAC_NOP)()
#define MAC_DEFER_3(_0) _0 MAC_DEFER_2(MAC_NOP)()
#define MAC_DEFER_4(_0) _0 MAC_DEFER_3(MAC_NOP)()
#define MAC_DEFER_5(_0) _0 MAC_DEFER_4(MAC_NOP)()
#define MAC_DEFER_6(_0) _0 MAC_DEFER_5(MAC_NOP)()
#define MAC_DEFER_7(_0) _0 MAC_DEFER_6(MAC_NOP)()
#define MAC_DEFER_8(_0) _0 MAC_DEFER_7(MAC_NOP)()
#define MAC_DEFER_9(_0) _0 MAC_DEFER_8(MAC_NOP)()
#define MAC_DEFER_10(_0) _0 MAC_DEFER_9(MAC_NOP)()
#define MAC_DEFER_11(_0) _0 MAC_DEFER_10(MAC_NOP)()
#define MAC_DEFER_12(_0) _0 MAC_DEFER_11(MAC_NOP)()

// 3^N + 2 evaluations, where N is the max tier
#define MAC_EVAL(...) MAC_EVAL3(__VA_ARGS__)
// #define MAC_EVAL6(...) MAC_EVAL5(MAC_EVAL5(MAC_EVAL5(__VA_ARGS__)))
// #define MAC_EVAL5(...) MAC_EVAL4(MAC_EVAL4(MAC_EVAL4(__VA_ARGS__)))
// #define MAC_EVAL4(...) MAC_EVAL3(MAC_EVAL3(MAC_EVAL3(__VA_ARGS__)))
#define MAC_EVAL3(...) MAC_EVAL2(MAC_EVAL2(MAC_EVAL2(__VA_ARGS__)))
#define MAC_EVAL2(...) MAC_EVAL1(MAC_EVAL1(MAC_EVAL1(__VA_ARGS__)))
#define MAC_EVAL1(...) MAC_EVAL0(MAC_EVAL0(MAC_EVAL0(__VA_ARGS__)))
#define MAC_EVAL0(...) __VA_ARGS__



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Expressions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define TYPE_LIST v4u08, v4u16, v4r32, v4u32
#define SWIZZLE_WZYX(E) E[3], E[2], E[1], E[0]
#define SWIZZLE_ZYXW(E) E[2], E[1], E[0], E[3]

#define _SWIZZLE(Type, ...) ((Type){__VA_ARGS__})
#define SWIZZLE(Vector, Format) \
    _Pragma("warning(push,1)") \
    _Pragma("warning(disable:4244)") \
    __MAC_GENERIC(Vector,, _SWIZZLE, (Format(Vector.E)), TYPE_LIST) \
    _Pragma("warning(pop)")







#if 0
#define __MAC_EVAL3(...) __VA_ARGS__
// #define __MAC_EVAL(...)  __MAC_EVAL1(__MAC_EVAL1(__MAC_EVAL1(__VA_ARGS__)))
// #define __MAC_EVAL1(...) __MAC_EVAL2(__MAC_EVAL2(__MAC_EVAL2(__VA_ARGS__)))
// #define __MAC_EVAL2(...) __MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__VA_ARGS__)))
#define __MAC_PRINT(I, ...) Out_##I
#define __MAC_EAT(...)
#define __MAC_EMPTY()
#define __MAC_DEFER(_0) _0 __MAC_EMPTY()
#define __MAC_EXPAND(...) __VA_ARGS__
#define __MAC_OBSTRUCT(...) __VA_ARGS__ __MAC_DEFER(__MAC_EMPTY)()
#define __MAC_WHEN(Condition) MAC_IF_ELSE(Condition)(__MAC_EXPAND)(__MAC_EAT)

#define __MAC_FOR(N, Macro, ...)             \
    __MAC_WHEN(N)                            \
    (                                        \
        __MAC_OBSTRUCT(__MAC_FOR_INDIRECT)() \
        (                                    \
            MAC_DEC(N), Macro, __VA_ARGS__   \
        )                                    \
        __MAC_OBSTRUCT(Macro)                \
        (                                    \
            MAC_DEC(N), __VA_ARGS__          \
        )                                    \
    )
#define __MAC_FOR_INDIRECT() __MAC_FOR

/*
1. __MAC_FOR(5, __MAC_PRINT, A))
   __MAC_WHEN(5) (__MAC_FOR_INDIRECT __MAC_DEFER(__MAC_EMPTY)()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_DEFER(__MAC_EMPTY)() (4, A))
   __MAC_WHEN(5) (__MAC_FOR_INDIRECT __MAC_EMPTY __MAC_EMPTY()()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY __MAC_EMPTY()() (4, A))
   __MAC_WHEN(5) (__MAC_FOR_INDIRECT __MAC_EMPTY()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (4, A))
   MAC_IF_ELSE(5)(__MAC_EXPAND)(__MAC_EAT)(__MAC_FOR_INDIRECT __MAC_EMPTY()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (4, A))
   __MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (4, A))

2. __MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (4, A))
   __MAC_FOR_INDIRECT() (4, __MAC_PRINT, A) __MAC_PRINT(4, A)
   __MAC_FOR(4, __MAC_PRINT, A) Out_4
   __MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (3, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (3, A)) Out_4

3. __MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (3, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (3, A)) Out_4
   __MAC_EXPAND(__MAC_FOR_INDIRECT() (3, __MAC_PRINT, A) __MAC_PRINT(3, A)) Out_4

4. __MAC_EXPAND(__MAC_FOR_INDIRECT() (3, __MAC_PRINT, A) __MAC_PRINT(3, A)) Out_4
   __MAC_EXPAND(__MAC_FOR(3, __MAC_PRINT, A) Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (2, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (2, A)) Out_3) Out_4

5. __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (2, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (2, A)) Out_3) Out_4
   __MAC_EXPAND(__MAC_FOR_INDIRECT() (2, __MAC_PRINT, A) __MAC_PRINT(2, A) Out_3) Out_4
   __MAC_EXPAND(__MAC_FOR(2, __MAC_PRINT, A) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (1, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (1, A)) Out_2 Out_3) Out_4

6. __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (1, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (1, A)) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT() (1, __MAC_PRINT, A) __MAC_PRINT(1, A)) Out_2 Out_3) Out_4

7. __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT() (1, __MAC_PRINT, A) __MAC_PRINT(1, A)) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR(1, __MAC_PRINT, A) Out_1) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (0, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (0, A)) Out_1) Out_2 Out_3) Out_4

8. __MAC_EXPAND(__MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (0, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (0, A)) Out_1) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT() (0, __MAC_PRINT, A) __MAC_PRINT(0, A) Out_1) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR(0, __MAC_PRINT, A) Out_0 Out_1) Out_2 Out_3) Out_4
   __MAC_EXPAND(__MAC_EXPAND(Out_0 Out_1) Out_2 Out_3) Out_4




1. __MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (4, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (4, A))
2. __MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (3, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (3, A)) Out_4
3. __MAC_EXPAND(__MAC_FOR_INDIRECT() (3, __MAC_PRINT, A) __MAC_PRINT(3, A)) Out_4
4. __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (2, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (2, A)) Out_3) Out_4
5. __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (1, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (1, A)) Out_2 Out_3) Out_4
6. __MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT() (1, __MAC_PRINT, A) __MAC_PRINT(1, A)) Out_2 Out_3) Out_4
7. __MAC_EXPAND(__MAC_EXPAND(__MAC_EXPAND(__MAC_FOR_INDIRECT __MAC_EMPTY()() (0, __MAC_PRINT, A) __MAC_PRINT __MAC_EMPTY() (0, A)) Out_1) Out_2 Out_3) Out_4
8. __MAC_EXPAND(__MAC_EXPAND(Out_0 Out_1) Out_2 Out_3) Out_4

*/


__MAC_FOR(5, __MAC_PRINT, A)
__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A))
__MAC_EVAL3(__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A)))
__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A))))
__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A)))))
__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A))))))
__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A)))))))
__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_EVAL3(__MAC_FOR(5, __MAC_PRINT, A))))))))
#endif


#define GAME_UTIL_VOX_MACROS_H_
#endif