/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_STR_H_
#define UTIL_STR_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: C08

typedef u08  chr;
// typedef hmem str;

typedef struct str_footer
{
    u32 Length;
    u32 CharSize;
} str_footer;


//SUBSECTION: C16

typedef u16 wchr;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#ifdef _DEBUG
    #define UTIL__STR_DEBUG__EXPORTS \
        FUNC(heap_data_DEBUG*, _Str_GetHeapDebugData_DEBUG, void) \
        FUNC(void,             _Str_HeapMark_DEBUG, void) \
        FUNC(void,             _Str_HeapCheck_DEBUG, void) \
        FUNC(void,             _Str_HeapPrint_DEBUG, void) \
    
    #define UTIL__STR_DEBUG__FUNCS \
    
    #define Str_GetHeapDebugData_DEBUG() _Str_GetHeapDebugData_DEBUG()
    #define Str_HeapMark_DEBUG() _Str_HeapMark_DEBUG()
    #define Str_HeapCheck_DEBUG() _Str_HeapCheck_DEBUG()
    #define Str_HeapPrint_DEBUG() _Str_HeapPrint_DEBUG()
#else
    #define UTIL__STR_DEBUG__EXPORTS
    #define UTIL__STR_DEBUG__FUNCS
    
    #define Str_GetHeapDebugData_DEBUG()
    #define Str_HeapMark_DEBUG()
    #define Str_HeapCheck_DEBUG()
    #define Str_HeapPrint_DEBUG()
#endif


#define UTIL__STR__EXPORTS \
    UTIL__STR_DEBUG__EXPORTS \
    \
    FUNC(b08,         Chr_IsWhitespace, chr C) \
    FUNC(b08,         Chr_IsPrintableASCII, chr C) \
    FUNC(u32,         ChrArr_CountPrintableASCII, chr *Arr) \
    FUNC(void,        Str_SetHeap, heap *Heap) \
    FUNC(str,         Str_Create, str *Dest, chr *Chars, u32 Length) \
    FUNC(void,        Str_Free, str Str) \
    FUNC(u32,         Str_Len, str Str) \
    FUNC(u32,         Str_FindChar, str Str, chr Char, s32 Index) \
    FUNC(str,         Str_Sub, str *Dest, str Src, u32 StartIndex, u32 EndIndex) \
    FUNC(str,         Str_Cat, str *StrA, str StrB) \
    FUNC(str,         Str_CatC, str *Str, chr *ChrArr) \
    FUNC(str,         Str_Ins, str *Str, str Insertion, u32 Index) \
    FUNC(str,         Str_Dup, str *Dest, str Str) \
    FUNC(s32,         Str_Cmp, str A, str B) \
    FUNC(b08,         Str_To_Bool, str Str) \
    FUNC(r32,         Str_To_R32, str Str) \
    FUNC(s32,         Str_To_S32, str Str) \
    FUNC(u32,         Str_To_U32, str Str) \

#define UTIL__STR__FUNCS \
    UTIL__STR_DEBUG__FUNCS \
    \
    FUNC(chr,         Chr_ToLower, chr C) \
    FUNC(chr,         Chr_ToUpper, chr C) \
    FUNC(b08,         Chr_IsBase2, chr C) \
    FUNC(b08,         Chr_IsBase8, chr C) \
    FUNC(b08,         Chr_IsBase10, chr C) \
    FUNC(b08,         Chr_IsBase16, chr C) \
    FUNC(u32,         ChrArr_Len, chr *Arr) \
    FUNC(heap*,       Str_GetHeap, void) \
    FUNC(void,        Str_SetHeap, heap *Heap) \
    FUNC(str,         Str_Resize, str *Str, u32 Length) \
    FUNC(str_footer*, Str_GetFooter, str Str) \
    FUNC(r64,         Str_To_R64, str Str) \
    // FUNC(wstr, Str_To_WStr, wstr Dest, str Src) \


// #define Str_CreateEmpty() Str_Create(NULL, NULL, 0)



#if 0
//SUBSECTION: Types

typedef wchr* wstr;

//SUBSECTION: Functions

#define UTIL_FUNCS_STR_C16 \
    FUNC(u32,  WStr_Len, wstr WStr) \
    FUNC(u32,  WStr_Size, wstr WStr) \
    FUNC(u32,  WStr_FindChar, wstr WStr, wchr WChar, s32 Index) \
    \
    FUNC(void, WStr_Cat, wstr WStr, wstr Addend) \
    FUNC(void, WStr_Cpy, wstr Dest, wstr Src) \
    FUNC(void, WStr_Sub, wstr Dest, wstr Src, u32 StartIndex, u32 EndIndex) \
    FUNC(void, WStr_Ins, wstr WStr, wstr Insertion, u32 Index) \
    FUNC(void, WStr_Replace, wstr WStr, wstr Replacement, u32 Index) \
    FUNC(void, WStr_Clr, wstr WStr) \
    \
    FUNC(void, WStr_To_Str, str Dest, wstr Src) \

#endif


#endif