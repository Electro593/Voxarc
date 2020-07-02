#ifndef UTIL_VOX_STRING_H_

#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "math/vox_v3u32.h"

inline b08
String_EqualC(memory_handle *A, c08 *B)
{
    b08 Result = TRUE;
    u32 SizeA = PointerCount(A->Base);
    u32 SizeB = PointerCount(B);
    
    if(SizeA == SizeB)
    {
        u08 *ByteA = (u08*)A->Base;
        u08 *ByteB = (u08*)B;
        while(SizeA--)
        {
            if(*ByteA++ != *ByteB++)
            {
                return FALSE;
            }
        }
        return TRUE;
    }
    
    return FALSE;
}

inline b08
String_Equal(memory_handle *A, memory_handle *B)
{
    u32 SizeA = PointerCount(A->Base);
    u32 SizeB = PointerCount(B->Base);
    
    if(SizeA == SizeB)
    {
        u08 *ByteA = (u08*)A->Base;
        u08 *ByteB = (u08*)B->Base;
        while(SizeA--)
        {
            if(*ByteA++ != *ByteB++)
            {
                return FALSE;
            }
        }
        return TRUE;
    }
    
    return FALSE;
}

inline c08
ToLower(c08 C)
{
    if(C >= 'A' && C <= 'Z')
    {
        return 'a' + C - 'A';
    }
    return C;
}

inline b08
IsSpace(c08 C)
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

inline b08
IsDigit(c08 C)
{
    if(C - '0' < 10 && C - '0' >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
IsBinary(c08 C)
{
    if(C - '0' < 2 && C - '0' >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

inline b08
IsHex(c08 C)
{
    if((C - '0' < 10 && C - '0' >= 0) ||
       (ToLower(C) - 'a' < 6 && ToLower(C) - 'a' >= 0))
    {
        return TRUE;
    }
    return FALSE;
}

local_func u32
String_ToU32(vptr Base)
{
    c08 *Char = (c08*)Base;
    while(IsSpace(*Char))
    {
        ++Char;
    }
    u32 Value = 0;
    if(*Char == '0')
    {
        Char++;
        if(*Char == 'x')
        {
            Char++;
            while(IsHex(*Char))
            {
                Value *= 16;
                if(IsDigit(*Char))
                {
                    Value += *Char++ - '0';
                }
                else
                {
                    Value += ToLower(*Char++) - 'a';
                }
            }
            return Value;
        }
        else if(*Char == 'b')
        {
            Char++;
            while(IsBinary(*Char))
            {
                Value = ((Value * 2) + *Char++) - '0';
            }
            return Value;
        }
    }
    while(IsDigit(*Char))
    {
        Value = ((Value * 10) + *Char++) - '0';
    }
    return Value;
}

local_func r32
String_ToR32(vptr Base)
{
    c08 *Char = (c08*)Base;
    while(IsSpace(*Char))
    {
        ++Char;
    }
    b08 Negative = FALSE;
    if(*Char == '-')
    {
        Negative = TRUE;
        Char++;
    }
    else if(*Char == '+')
    {
        Char++;
    }
    r32 Value = 0;
    while(IsDigit(*Char))
    {
        Value = ((Value * 10.0f) + *Char++) - '0';
    }
    if(*Char == '.')
    {
        ++Char;
        r32 Fraction = 0.0f;
        r32 Increment = 0.1f;
        while(IsDigit(*Char))
        {
            Fraction += Increment * (*Char++ - '0');
            Increment *= 0.1f;
        }
        Value += Fraction;
    }
    if(Negative)
    {
        Value = -Value;
    }
    if(ToLower(*Char++) != 'e')
    {
        return Value;
    }
    Negative = FALSE;
    if(*Char == '-')
    {
        Negative = TRUE;
        Char++;
    }
    else if(*Char == '+')
    {
        Char++;
    }
    r32 Increment = Negative ? 0.1f : 10.0f;
    r32 Exponent = 0.0;
    while(IsDigit(*Char))
    {
        Exponent = ((Exponent * 10.0f) + *Char++) - '0';
    }
    while(Exponent-- > 0)
    {
        Value *= Increment;
    }
    return Value;
}

inline v3u
String_ToV3u32(vptr Base)
{
    return V3u32_1(String_ToU32(Base));
}

inline memory_handle *
String_Contatenate(memory_handle *A, memory_handle *B)
{
    c08 *CharA = (c08*)A->Base;
    while(*CharA != '\0')
    {
        CharA++;
    }
    size SizeA = (CharA - (c08*)A->Base);
    c08 *CharB = (c08*)B->Base;
    while(*CharB != '\0')
    {
        CharB++;
    }
    size SizeB = (CharB - (c08*)B->Base);
    memory_handle *Result = AllocateMemory(GetHandlePool(A), SizeA + SizeB);
    c08 *Char = (c08*)Result->Base;
    CharA = (c08*)A->Base;
    while(SizeA--)
    {
        *Char++ = *CharA++;
    }
    CharB = (c08*)B->Base;
    while(SizeB--)
    {
        *Char++ = *CharB++;
    }
    FreeMemory(A);
    FreeMemory(B);
    return Result;
}

inline memory_handle *
String_ContatenateCS(char *A, memory_handle *B)
{
    c08 *CharA = A;
    while(*CharA != '\0')
    {
        CharA++;
    }
    size SizeA = (CharA - (c08*)A);
    c08 *CharB = (c08*)B->Base;
    while(*CharB != '\0')
    {
        CharB++;
    }
    size SizeB = (CharB - (c08*)B->Base);
    memory_handle *Result = AllocateMemory(GetHandlePool(B), SizeA + SizeB);
    c08 *Char = (c08*)Result->Base;
    CharA = (c08*)A;
    while(SizeA--)
    {
        *Char++ = *CharA++;
    }
    CharB = (c08*)B->Base;
    while(SizeB--)
    {
        *Char++ = *CharB++;
    }
    FreeMemory(B);
    return Result;
}

#define UTIL_VOX_STRING_H_
#endif