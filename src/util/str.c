/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: C08 (chr)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal chr
Chr_ToLower(chr C)
{
    if(C >= 'A' && C <= 'Z')
    {
        return 'a' + C - 'A';
    }
    return C;
}

internal chr
Chr_ToUpper(chr C)
{
    if(C >= 'a' && C <= 'z')
    {
        return C - 'a' + 'A';
    }
    return C;
}

internal b08
Chr_IsWhitespace(chr C)
{
    if(C == ' ' ||
       C == '\n' ||
       C == '\t' ||
       C == '\v' ||
       C == '\f' ||
       C == '\r')
    {
        return TRUE;
    }
    return FALSE;
}

internal b08
Chr_IsPrintableASCII(chr C)
{
    if(C > 32 && C < 127)
    {
        return TRUE;
    }
    return FALSE;
}

internal b08
Chr_IsBase2(chr C)
{
    if(C - '0' < 2 && C - '0' >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

internal b08
Chr_IsBase8(chr C)
{
    if(C - '0' < 8 && C - '0' >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

internal b08
Chr_IsBase10(chr C)
{
    if(C - '0' < 10 && C - '0' >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

internal b08
Chr_IsBase16(chr C)
{
    if((C - '0' < 10 && C - '0' >= 0) ||
       (Chr_ToUpper(C) - 'A' < 6 && Chr_ToUpper(C) - 'A' >= 0))
    {
        return TRUE;
    }
    return FALSE;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: C08[] (chr*)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//TODO: Vectorize
//NOTE: Does not include null terminator
internal u32
ChrArr_Len(chr *Arr)
{
    u32 Length = 0;
    
    while(*Arr++ != '\0') ++Length;
    
    return Length;
}

internal u32
ChrArr_CountPrintableASCII(chr *Arr)
{
    u32 Count = 0;
    
    while(*Arr)
    {
        if(Chr_IsPrintableASCII(*Arr++))
        {
            ++Count;
        }
    }
    
    return Count;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: String (str)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


internal heap *
Str_GetHeap(void)
{
    return UtilState->StrHeap;
}
internal void
Str_SetHeap(heap *Heap)
{
    UtilState->StrHeap = Heap;
}

#if _MODE_DEBUG
internal heap_data_DEBUG *
_Str_GetHeapDebugData_DEBUG(void)
{
    return &UtilState->StrHeapDebugData;
}

internal void
_Str_HeapMark_DEBUG(void)
{
    Heap_Mark_DEBUG(UtilState->StrHeap, &UtilState->StrHeapDebugData);
}

internal void
_Str_HeapCheck_DEBUG(void)
{
    Heap_Check_DEBUG(UtilState->StrHeap, &UtilState->StrHeapDebugData);
}

internal void
_Str_HeapPrint_DEBUG(void)
{
    Heap_Print_DEBUG(UtilState->StrHeap, &UtilState->StrHeapDebugData);
}
#endif

//NOTE: Length doesn't include the null terminator
internal str
Str_Create(str *Dest,
           chr *Chars,
           u32 Length)
{
    if(Chars && !Length)
    {
        Length = ChrArr_Len(Chars);
    }
    
    str Str = Heap_Allocate(UtilState->StrHeap, Length + 1 + sizeof(str_footer));
    str_footer *Footer = (str_footer*)(Str + Heap_GetSize(Str)- sizeof(str_footer));
    
    if(Chars)
    {
        Mem_Cpy(Str, Chars, Length);
    }
    Str[Length] = '\0';
    
    Footer->Length = Length;
    Footer->CharSize = sizeof(chr);
    
    if(Dest)
    {
        *Dest = Str;
    }
    
    return Str;
}

//NOTE: Length does not include null terminator
internal str
Str_Resize(str *Str,
           u32 Length)
{
    str_footer OldFooter = *Str_GetFooter(*Str);
    
    Heap_Resize(Str, Length + 1 + sizeof(str_footer));
    str_footer *NewFooter = (str_footer*)(*Str + Heap_GetSize(*Str) - sizeof(str_footer));
    *NewFooter = OldFooter;
    
    return *Str;
}

internal void
Str_Free(str Str)
{
    Heap_Free(Str);
}

internal str_footer *
Str_GetFooter(str Str)
{
    return (str_footer*)(Str + Heap_GetHeader(Str)->Size - sizeof(str_footer));
}

internal u32
Str_GetSize(str Str)
{
    return Heap_GetSize(Str) - sizeof(str_footer);
}

//NOTE: Does not include the null terminator
internal u32
Str_Len(str Str)
{
    return Str_GetFooter(Str)->Length;
}

//TODO: See what the crt does for all of these
//TODO: Option for case sensitivity, like Str_CmpCS
//TODO: Vectorize somehow
internal s32
Str_Cmp(str A,
        str B)
{
    u32 ALen = Str_Len(A);
    u32 BLen = Str_Len(B);
    
    if(ALen != BLen)
    {
        return (ALen > BLen) ? 1 : -1;
    }
    
    while(ALen > 0 && Chr_ToLower(*A) == Chr_ToLower(*B))
    {
        ++A;
        ++B;
        --ALen;
    }
    
    return (ALen == 0) ? 0 : ((s32)*A - (s32)*B);
}

//NOTE: Index =  0 is the first match from left to right
//      Index = -1 is the first match from right to left
internal u32
Str_FindChar(str Str,
             chr Char,
             s32 Index)
{
    u32 Result;
    
    s32 Count = 0;
    s32 Sign = S32_Sign(Index);
    if(Sign > 0)
    {
        Count = -1;
    }
    
    chr *C = Str;
    if(Sign < 0)
    {
        C += Str_Len(Str) - 1; // To go from a count to an index
    }
    
    while(*C)
    {
        if(*C == Char)
        {
            Count += Sign;
            
            if(Count == Index)
            {
                break;
            }
        }
        
        C += Sign;
    }
    
    Result = (u32)((u64)C - (u64)Str);
    
    return Result;
}

internal str
Str_Dup(str *Dest,
         str Src)
{
    str New = Str_Create(NULL, Src, Str_Len(Src));
    
    if(Dest)
    {
        *Dest = New;
    }
    
    return New;
}

internal str
Str_Sub(str *Dest,
        str Src,
        u32 StartIndex,
        u32 EndIndex)
{
    u32 SrcLen = Str_Len(Src);
    u32 SubLen = EndIndex - StartIndex;
    
    ASSERT(StartIndex <= EndIndex);
    ASSERT(EndIndex <= SrcLen);
    
    str New = NULL;
    if(SubLen > 0)
    {
        Str_Create(&New, Src + StartIndex, SubLen);
    }
    else
    {
        Str_Create(&New, NULL, 0);
    }
    
    if(Dest)
    {
        *Dest = New;
    }
    
    return New;
}

internal str
Str_Cat(str *StrA,
        str StrB)
{
    u32 ALen = Str_Len(*StrA);
    u32 BLen = Str_Len(StrB);
    
    if(Str_GetSize(*StrA) < ALen + BLen + 1)
    {
        Str_Resize(StrA, ALen + BLen);
    }
    Str_GetFooter(*StrA)->Length += BLen;
    
    Mem_Cpy(*StrA + ALen, StrB, BLen + 1);
    
    return *StrA;
}

internal str
Str_CatC(str *Str,
         chr *ChrArr)
{
    str TempStr = Str_Create(NULL, ChrArr, 0);
    Str_Cat(Str, TempStr);
    Str_Free(TempStr);
    
    return *Str;
}

internal str
Str_Ins(str *Str,
        str Insertion,
        u32 Index)
{
    u32 StrLen = Str_Len(*Str);
    u32 InsLen = Str_Len(Insertion);
    
    if(InsLen == 0)
    {
        return *Str;
    }
    ASSERT(Index < StrLen);
    
    if(Str_GetSize(*Str) < StrLen + InsLen + 1)
    {
        Str_Resize(Str, StrLen + InsLen);
    }
    Str_GetFooter(*Str)->Length += InsLen;
    
    Mem_Cpy(*Str + Index + InsLen, *Str + Index, StrLen - Index + 1);
    Mem_Cpy(*Str + Index, Insertion, InsLen);
    
    return *Str;
}

//SUBSECTION: Casts

internal b08
Str_To_Bool(str Str)
{
    u32 StrLen = Str_Len(Str);
    
    if(Mem_Cmp(Str, "true", StrLen) == 0)
    {
        return TRUE;
    }
    
    ASSERT(Mem_Cmp(Str, "false", StrLen) == 0);
    
    return FALSE;
}

internal s32
Str_To_S32(str Str)
{
    s32 Value = 0;
    
    chr *C = (chr*)Str;
    b08 Negative = FALSE;
    if(*C == '-')
    {
        ++C;
        Negative = TRUE;
    }
    if(*C == '+')
    {
        ++C;
    }
    
    while(Chr_IsBase10(*C))
    {
        Value = ((Value * 10) + *C++) - '0';
    }
    
    if(Negative)
    {
        Value = -Value;
    }
    
    return Value;
}

//TODO: Try to optimize these
internal u32
Str_To_U32(str Str)
{
    chr *C = (chr*)Str;
    
    //TODO: Handle incorrect types, like a negative sign
    //      Also, do XXXXh and XXXXb
    
    u32 Value = 0;
    if(*C == '0')
    {
        C++;
        if(*C == 'x')
        {
            C++;
            while(Chr_IsBase16(*C))
            {
                Value *= 16;
                if(Chr_IsBase10(*C))
                {
                    Value += *C++ - '0';
                }
                else
                {
                    Value += 10 + (Chr_ToUpper(*C++) - 'A');
                }
            }
            
            return Value;
        }
        else if(*C == 'b')
        {
            C++;
            while(Chr_IsBase2(*C))
            {
                Value = (Value * 2) + (*C++ - '0');
            }
            
            return Value;
        }
        else
        {
            while(Chr_IsBase8(*C))
            {
                Value = (Value * 8) + (*C++ - '0');
            }
            
            return Value;
        }
    }
    
    while(Chr_IsBase10(*C))
    {
        Value = (Value * 10) + (*C++ - '0');
    }
    
    return Value;
}

internal r32
Str_To_R32(str Str)
{
    chr *C = (chr*)Str;
    
    b08 Negative = FALSE;
    if(*C == '-')
    {
        Negative = TRUE;
        C++;
    }
    else if(*C == '+')
    {
        C++;
    }
    
    r32 Value = 0;
    while(Chr_IsBase10(*C))
    {
        Value = (Value * 10.0f) + (*C++ - '0');
    }
    
    if(*C == '.')
    {
        ++C;
        r32 Fraction = 0.0f;
        r32 Increment = 0.1f;
        while(Chr_IsBase10(*C))
        {
            Fraction += Increment * (*C++ - '0');
            Increment *= 0.1f;
        }
        Value += Fraction;
    }
    
    if(Negative)
    {
        Value = -Value;
    }
    
    if(Chr_ToLower(*C++) != 'e')
    {
        return Value;
    }
    
    Negative = FALSE;
    if(*C == '-')
    {
        Negative = TRUE;
        C++;
    }
    else if(*C == '+')
    {
        C++;
    }
    
    r32 Exponent = 0.0;
    while(Chr_IsBase10(*C))
    {
        Exponent = (Exponent * 10.0f) + (*C++ - '0');
    }
    
    r32 Increment = Negative ? 0.1f : 10.0f;
    while(Exponent-- > 0)
    {
        Value *= Increment;
    }
    
    return Value;
}

internal r64
Str_To_R64(str Str)
{
    chr *C = (chr*)Str;
    
    b08 Negative = FALSE;
    if(*C == '-')
    {
        Negative = TRUE;
        C++;
    }
    else if(*C == '+')
    {
        C++;
    }
    
    r64 Value = 0;
    while(Chr_IsBase10(*C))
    {
        Value = (Value * 10.0) + (*C++ - '0');
    }
    
    if(*C == '.')
    {
        ++C;
        r64 Fraction = 0.0;
        r64 Increment = 0.1;
        while(Chr_IsBase10(*C))
        {
            Fraction += Increment * (*C++ - '0');
            Increment *= 0.1;
        }
        Value += Fraction;
    }
    
    if(Negative)
    {
        Value = -Value;
    }
    
    if(Chr_ToLower(*C++) != 'e')
    {
        return Value;
    }
    
    Negative = FALSE;
    if(*C == '-')
    {
        Negative = TRUE;
        C++;
    }
    else if(*C == '+')
    {
        C++;
    }
    
    r64 Exponent = 0.0;
    while(Chr_IsBase10(*C))
    {
        Exponent = (Exponent * 10.0) + (*C++ - '0');
    }
    
    r64 Increment = Negative ? 0.1 : 10.0;
    while(Exponent-- > 0)
    {
        Value *= Increment;
    }
    return Value;
}

//TODO: Vectorize
// internal wstr
// Str_To_WStr(str Src)
// {
//     u32 SrcSize = Str_Len(Src) + 1;
//     u32 DestSize = SrcSize * sizeof(wchr1);
    
//     wstr Dest = WStr_Create(NULL, SrcSize - 1);
    
//     ASSERT((u64)Src > (u64)Dest + DestSize ||
//            (u64)Src + SrcSize < (u64)Dest);
//     // s32 Delta = 1;
//     // u32 StartOffset = 0;
//     // if((u64)Src < (u64)Dest &&
//     //    (u64)Src + DestSize >= (u64)Dest)
//     // {
//     //     Delta = -1;
//     //     StartOffset = SrcSize - 1;
//     // }
    
//     // Src += StartOffset;
//     // Dest += StartOffset;
//     while(SrcSize > 0)
//     {
//         // *Dest = *Src;
//         // Src += Delta;
//         // Dest += Delta;
//         *Dest++ = *Src++;
        
//         --SrcSize;
//     }
    
//     return Dest;
// }


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: C16 (wstr)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#if 0
//NOTE: Includes the null terminator
internal u32
WStr_Len(wstr WStr)
{
    u32 Result;
    
    u16 *WC = WStr;
    while(*WC++);
    
    Result = (u32)((u64)WC - (u64)WStr) >> 1; // Since the chars are two bytes
    
    return Result;
}

internal u32
WStr_Size(wstr WStr)
{
    u32 Result;
    
    u32 WStrLen = WStr_Len(WStr);
    Result = WStrLen * sizeof(wchr);
    
    return Result;
}

//NOTE: Index =  0 is the first match from left to right
//      Index = -1 is the first match from right to left
internal u32
WStr_FindChar(wstr WStr,
              wchr WChar,
              s32 Index)
{
    u32 Result;
    
    s32 Count = 0;
    s32 Sign = S32_Sign(Index);
    if(Sign > 0)
    {
        Count = -1;
    }
    
    wchr *WC = WStr;
    if(Sign < 0)
    {
        WC += WStr_Len(WStr) - 2; // One to go from a count to an index, another to pass the null terminator
    }
    
    while(*WC)
    {
        if(*WC == WChar)
        {
            Count += Sign;
            
            if(Count == Index)
            {
                break;
            }
        }
        
        WC += Sign;
    }
    
    Result = (u32)((u64)WC - (u64)WStr) >> 1;
    
    return Result;
}

//NOTE: Size of WStr >= (WStr_Len(WStr) + WStr_Len(Addend) - 1) * 2
internal void
WStr_Cat(wstr WStr,
         wstr Addend)
{
    u32 StrLen = WStr_Len(WStr);
    u32 AddLen = WStr_Len(Addend);
    
    Mem_Cpy(WStr + StrLen - 1, Addend, AddLen * sizeof(wchr));
}

//NOTE: Size of Dest >= WStr_Len(Src)
internal void
WStr_Cpy(wstr Dest,
         wstr Src)
{
    u32 SrcLen = WStr_Len(Src);
    
    Mem_Cpy(Dest, Src, SrcLen * sizeof(wchr));
}

//NOTE: Size of Dest >= (EndIndex - StartIndex + 1) * 2
internal void
WStr_Sub(wstr Dest,
         wstr Src,
         u32 StartIndex,
         u32 EndIndex)
{
    u32 SrcLen = WStr_Len(Src);
    u32 SubLen = EndIndex - StartIndex;
    
    ASSERT(StartIndex < EndIndex);
    ASSERT(EndIndex < SrcLen);
    
    Mem_Cpy(Dest, Src + StartIndex, SubLen * sizeof(wchr));
    Dest[SubLen] = L'\0';
}

//NOTE: Size of WStr >= (WStr_Len(WStr) + WStr_Len(Insertion) - 1) * 2, does not copy null terminator
internal void
WStr_Ins(wstr WStr,
         wstr Insertion,
         u32 Index)
{
    Stack_Push();
    
    u32 WStrLen = WStr_Len(WStr);
    u32 InsLen = WStr_Len(Insertion);
    u32 RemLen = WStrLen - Index;
    wstr TempStr = Stack_Allocate(RemLen);
    
    ASSERT(Index < WStrLen);
    
    Mem_Cpy(TempStr, WStr + Index, RemLen * sizeof(wchr));
    Mem_Cpy(WStr + Index, Insertion, (InsLen - 1) * sizeof(wchr));
    Mem_Cpy(WStr + Index + InsLen - 1, TempStr, RemLen * sizeof(wchr));
    
    Stack_Pop();
}

//NOTE: Index + WStr_Len(Replacement) <= WStr_Len(WStr)
internal void
WStr_Replace(wstr WStr,
             wstr Replacement,
             u32 Index)
{
    u32 WStrLen = WStr_Len(WStr);
    u32 RepLen = WStr_Len(Replacement);
    
    ASSERT(Index + RepLen <= WStrLen);
    
    Mem_Cpy(WStr + Index, Replacement, (RepLen - 1) * sizeof(wchr));
}

internal void
WStr_Clr(wstr WStr)
{
    WStr[0] = L'\0';
}

//SUBSECTION: Casts

//NOTE: sizeof(Dest) * 2 >= Str_Len(Src)
internal void
WStr_To_Str(str Dest,
           wstr Src)
{
    while(*Src)
    {
        *Dest++ = *Src++;
    }
    *Dest = 0;
}
#endif