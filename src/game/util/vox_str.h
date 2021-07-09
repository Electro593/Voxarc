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

#if !defined(GAME_UTIL_VOX_STR_H_)
#define      GAME_UTIL_VOX_STR_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: C08

typedef u08  chr;
typedef hmem str;

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


#if defined(_VOX_DEBUG)
    #define GAME_UTIL__STR_DEBUG__EXPORTS \
        PROC(heap_data_DEBUG*, _Str_GetHeapDebugData_DEBUG, void) \
        PROC(void,             _Str_HeapMark_DEBUG, void) \
        PROC(void,             _Str_HeapCheck_DEBUG, void) \
        PROC(void,             _Str_HeapPrint_DEBUG, void) \
    
    #define GAME_UTIL__STR_DEBUG__FUNCS \
    
    #define Str_GetHeapDebugData_DEBUG() _Str_GetHeapDebugData_DEBUG()
    #define Str_HeapMark_DEBUG() _Str_HeapMark_DEBUG()
    #define Str_HeapCheck_DEBUG() _Str_HeapCheck_DEBUG()
    #define Str_HeapPrint_DEBUG() _Str_HeapPrint_DEBUG()
#else
    #define GAME_UTIL__STR_DEBUG__EXPORTS
    #define GAME_UTIL__STR_DEBUG__FUNCS
    
    #define Str_GetHeapDebugData_DEBUG()
    #define Str_HeapMark_DEBUG()
    #define Str_HeapCheck_DEBUG()
    #define Str_HeapPrint_DEBUG()
#endif


#define GAME_UTIL__STR__EXPORTS \
    GAME_UTIL__STR_DEBUG__EXPORTS \
    \
    PROC(void,        Str_SetHeap, heap *Heap) \
    PROC(str,         Str_Create, str *Dest, chr *Chars, u32 Length) \
    PROC(void,        Str_Free, str Str) \
    PROC(u32,         Str_Len, str Str) \
    PROC(u32,         Str_FindChar, str Str, chr Char, s32 Index) \
    PROC(str,         Str_Sub, str *Dest, str Src, u32 StartIndex, u32 EndIndex) \
    PROC(str,         Str_Cat, str *StrA, str StrB) \
    PROC(str,         Str_CatC, str *Str, chr *ChrArr) \
    PROC(str,         Str_Ins, str *Str, str Insertion, u32 Index) \

#define GAME_UTIL__STR__FUNCS \
    GAME_UTIL__STR_DEBUG__FUNCS \
    \
    PROC(chr,         Chr_ToLower, chr C) \
    PROC(chr,         Chr_ToUpper, chr C) \
    PROC(b08,         Chr_IsWhitespace, chr C) \
    PROC(b08,         Chr_IsPrintableASCII, chr C) \
    PROC(b08,         Chr_IsBase2, chr C) \
    PROC(b08,         Chr_IsBase8, chr C) \
    PROC(b08,         Chr_IsBase10, chr C) \
    PROC(b08,         Chr_IsBase16, chr C) \
    PROC(u32,         ChrArr_Len, chr *Arr) \
    PROC(u32,         ChrArr_CountPrintableASCII, chr *Arr) \
    PROC(heap*,       Str_GetHeap, void) \
    PROC(void,        Str_SetHeap, heap *Heap) \
    PROC(str,         Str_Resize, str *Str, u32 Length) \
    PROC(str_footer*, Str_GetFooter, str Str) \
    PROC(s32,         Str_Cmp, str A, str B) \
    PROC(str,         Str_Dup, str *Dest, str Str) \
    PROC(b08,         Str_ToBool, str Str) \
    PROC(s32,         Str_ToS32, str Str) \
    PROC(u32,         Str_ToU32, str Str) \
    PROC(r32,         Str_ToR32, str Str) \
    PROC(r64,         Str_ToR64, str Str) \
    // PROC(wstr, Str_ToWStr, wstr Dest, str Src) \


// #define Str_CreateEmpty() Str_Create(NULL, NULL, 0)



#if 0
//SUBSECTION: Types

typedef wchr* wstr;

//SUBSECTION: Functions

#define GAME_UTIL_FUNCS_STR_C16 \
    PROC(u32,  WStr_Len, wstr WStr) \
    PROC(u32,  WStr_Size, wstr WStr) \
    PROC(u32,  WStr_FindChar, wstr WStr, wchr WChar, s32 Index) \
    \
    PROC(void, WStr_Cat, wstr WStr, wstr Addend) \
    PROC(void, WStr_Cpy, wstr Dest, wstr Src) \
    PROC(void, WStr_Sub, wstr Dest, wstr Src, u32 StartIndex, u32 EndIndex) \
    PROC(void, WStr_Ins, wstr WStr, wstr Insertion, u32 Index) \
    PROC(void, WStr_Replace, wstr WStr, wstr Replacement, u32 Index) \
    PROC(void, WStr_Clr, wstr WStr) \
    \
    PROC(void, WStr_ToStr, str Dest, wstr Src) \

#endif


#endif