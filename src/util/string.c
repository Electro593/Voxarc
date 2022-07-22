/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal string
CLString(c08 *Chars,
         u64 Length)
{
    string String;
    String.Length = Length;
    String.Capacity = Length;
    String.Resizable = FALSE;
    String.Text = Chars;
    return String;
}
internal string CString(c08 *Chars) { return CLString(Chars, Mem_BytesUntil(Chars, 0)); }
internal string EString(u64 Length) { return CLString(Stack_Allocate(Length), Length); }

/*
TODO:
 - ' Flag
 - n$ Specifier
 - %u, %b/%B, %x/%X, %o
 - %f, %e/%E, %g/%G
 - %c, %p, %n
*/
internal string
FString(c08 *Format, ...)
{
    va_list Args;
    VA_Start(Args, Format);
    
    c08 *Out = Stack_GetCursor();
    string Result;
    Result.Resizable = FALSE;
    Result.Text = Out;
    
    c08 *C = Format;
    while(*C) {
        if(*C == '%') {
            C++;
            if(*C == '%') {
                *Out++ = *C++;
            } else {
                b08 AlignLeft = FALSE;
                b08 PrefixPlus = FALSE;
                b08 PrefixSpace = FALSE;
                b08 PrefixHex = FALSE;
                b08 PadZero = FALSE;
                b08 CustomPrecision = FALSE;
                u32 LengthLen = 0;
                c08 *LengthChars;
                u32 IntWidth = 4;
                u32 FloatWidth = 8;
                s32 MinChars = 0;
                s32 Precision = 0;
                
                while(TRUE) {
                    if(*C == '-')
                        AlignLeft = TRUE;
                    else if(*C == '+')
                        PrefixPlus = TRUE;
                    else if(*C == ' ')
                        PrefixSpace = TRUE;
                    else if(*C == '#')
                        PrefixHex = TRUE;
                    else if(*C == '0')
                        PadZero = TRUE;
                    else break;
                    C++;
                }
                
                if(*C == '*') {
                    MinChars = VA_Next(Args, s64);
                    if(MinChars < 0) {
                        AlignLeft = !AlignLeft;
                        MinChars = -MinChars;
                    }
                    C++;
                } else {
                    while(*C >= '0' && *C <= '9') {
                        MinChars = MinChars*10 + *C-'0';
                        C++;
                    }
                }
                
                if(*C == '.') {
                    C++;
                    CustomPrecision = TRUE;
                    
                    if(*C == '*') {
                        Precision = VA_Next(Args, s64);
                        if(Precision < 0) Precision = 0;
                        C++;
                    } else {
                        while(*C >= '0' && *C <= '9') {
                            Precision = Precision*10 + *C-'0';
                            C++;
                        }
                    }
                }
                
                if((*C == 'h' && *(C+1) == 'h') || (*C == 'l' && *(C+1) == 'l')) {
                    LengthChars = C;
                    LengthLen += 2;
                    C += 2;
                } else if(*C == 'h' || *C == 'l' || *C == 'j' || *C == 'z' || *C == 't' || *C == 'L' || *C == 'q') {
                    LengthChars = C;
                    LengthLen++;
                    C++;
                }
                
                switch(*C) {
                    case 'd':
                    case 'i': {
                        s64 Temp = VA_Next(Args, s64);
                        
                        if(LengthLen == 2 && LengthChars[0] == 'h' && LengthChars[1] == 'h')
                            Temp = (s08)Temp;
                        else if(LengthLen == 1 && LengthChars[0] == 'h')
                            Temp = (s16)Temp;
                        if(!LengthLen || (LengthLen == 1 && LengthChars[0] == 'l'))
                            Temp = (s32)Temp;
                        
                        if(!CustomPrecision) Precision = 1;
                        
                        s64 Value = Temp;
                        b08 Negative = (Value < 0);
                        b08 HasPrefix = Negative || PrefixSpace || PrefixPlus;
                        c08 Prefix;
                        if(Negative) Prefix = '-';
                        else if(PrefixPlus) Prefix = '+';
                        else if(PrefixSpace) Prefix = ' ';
                        
                        s32 Len = 0;
                        while(Value > 0) {
                            Value /= 10;
                            Len++;
                        }
                        Len = MAX(Len, Precision);
                        if(PadZero) Len = MAX(MinChars-HasPrefix, Len);
                        s32 Padding = MinChars - (Len + HasPrefix);
                        
                        Value = (s64)Temp * (1-2*Negative);
                        
                        if(!AlignLeft) {
                            while(Padding-- > 0)
                                *Out++ = ' ';
                        }
                        
                        if(HasPrefix)
                            *Out++ = Prefix;
                        
                        for(s32 I = Len-1; I >= 0; I--) {
                            Out[I] = (Value%10) + '0';
                            Value /= 10;
                        }
                        Out += Len;
                        
                        while(Padding-- > 0)
                            *Out++ = ' ';
                    } break;
                    
                    // case 'u': {
                        
                    // } break;
                    
                    // case 'o': {
                        
                    // } break;
                    
                    // case 'x': {
                        
                    // } break;
                    
                    // case 'X': {
                        
                    // } break;
                    
                    // case 'f': {
                        
                    // } break;
                    
                    // case 'F': {
                        
                    // } break;
                    
                    // case 'e': {
                        
                    // } break;
                    
                    // case 'E': {
                        
                    // } break;
                    
                    // case 'g': {
                        
                    // } break;
                    
                    // case 'G': {
                        
                    // } break;
                    
                    // case 'a': {
                        
                    // } break;
                    
                    // case 'A': {
                        
                    // } break;
                    
                    // case 'c': {
                        
                    // } break;
                    
                    // case 's': {
                        
                    // } break;
                    
                    case 'p': {
                        
                    } break;
                    
                    case 'n': {
                        if(LengthLen == 2 && LengthChars[0] == 'h' && LengthChars[1] == 'h') {
                            s08 *Value = VA_Next(Args, s08*);
                            *Value = (u64)Out - (u64)Result.Text;
                        } else if(LengthLen == 1 && LengthChars[0] == 'h') {
                            s16 *Value = VA_Next(Args, s16*);
                            *Value = (u64)Out - (u64)Result.Text;
                        } else if(LengthLen == 0 || (LengthLen == 1 && LengthChars[0] == 'l')) {
                            s32 *Value = VA_Next(Args, s32*);
                            *Value = (u64)Out - (u64)Result.Text;
                        } else {
                            s64 *Value = VA_Next(Args, s64*);
                            *Value = (u64)Out - (u64)Result.Text;
                        }
                    } break;
                    
                    default: {
                        Assert(FALSE, "Invalid format!");
                    }
                }
                C++;
            }
        } else {
            *Out++ = *C++;
        }
    }
    
    Result.Length = (u64)Out - (u64)Result.Text;
    Result.Capacity = Result.Length;
    
    Stack_SetCursor(Out);
    
    VA_End(Args);
    
    return Result;
}

internal string
String_Cat(string A, string B)
{
    string Result = EString(A.Length + B.Length);
    
    Mem_Cpy(Result.Text,          A.Text, A.Length);
    Mem_Cpy(Result.Text+A.Length, B.Text, B.Length);
    
    return Result;
}

internal string
U64_ToString(u64 N,
             u08 Radix)
{
    Assert(2 <= Radix && Radix <= 64);
    
    persist c08 Chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";
    
    u32 StrSize = 64+10;
    u32 Index = StrSize;
    
    c08 *LineStr = Stack_Allocate(StrSize);
    
    if(Radix != 2 && Radix != 10 && Radix != 16)
    {
        LineStr[--Index] = ')';
        LineStr[--Index] = (Radix%10) + '0';
        if(Radix >= 10)
            LineStr[--Index] = (Radix/10) + '0';
        LineStr[--Index] = ' ';
        LineStr[--Index] = 'e';
        LineStr[--Index] = 's';
        LineStr[--Index] = 'a';
        LineStr[--Index] = 'b';
        LineStr[--Index] = '(';
        LineStr[--Index] = ' ';
    }
    
    do {
        LineStr[--Index] = Chars[N % Radix];
        N /= Radix;
    } while(N > 0);
    
    if(Radix == 2) {
        LineStr[--Index] = 'b';
        LineStr[--Index] = '0';
    } else if(Radix == 16) {
        LineStr[--Index] = 'x';
        LineStr[--Index] = '0';
    }
    
    return CLString(LineStr + Index, StrSize - Index);
}

internal string U32_ToString(u32 N, u08 Radix) { return U64_ToString((u64)N, Radix); }
internal string U16_ToString(u16 N, u08 Radix) { return U64_ToString((u64)N, Radix); }
internal string U08_ToString(u08 N, u08 Radix) { return U64_ToString((u64)N, Radix); }

internal string
S64_ToString(s64 N)
{
    if(N >= 0)
        return U64_ToString(N, 10);
    
    string Str;
    
    if(N == S64_MIN) {
        Str = U64_ToString(-(N+1), 10);
        Str.Text[Str.Length-1]++;
    } else Str = U64_ToString(-N, 10);
    
    // Guaranteed because U64_ToString over-allocates, to account for bases.
    Str.Length++;
    Str.Capacity++;
    Str.Text--;
    Str.Text[0] = '-';
    
    return Str;
}

internal string S32_ToString(s32 N) { return S64_ToString((s64)N); }
internal string S16_ToString(s16 N) { return S64_ToString((s64)N); }
internal string S08_ToString(s08 N) { return S64_ToString((s64)N); }

internal string
R32_ToString(r32 N, u32 DigitsAfterDecimal)
{
    string Whole = S32_ToString((s32)N);
    
    r32 Frac = N - (s32)N;
    if(Frac == 0) return Whole;
    
    if(Frac < 0) {
        if((s32)N == 0) {
            Whole.Length++;
            Whole.Text--;
            Whole.Text[0] = '-';
        }
        
        Frac *= -1;
    }
    
    string Str = EString(Whole.Length + 1 + DigitsAfterDecimal);
    Mem_Cpy(Str.Text, Whole.Text, Whole.Length);
    Str.Text[Whole.Length] = '.';
    
    for(u32 I = 0; I < DigitsAfterDecimal; I++) {
        Frac *= 10;
        Str.Text[Whole.Length+1+I] = '0' + (s32)Frac;
        Frac -= (s32)Frac;
    }
    
    //NOTE: This truncates, not rounds, the last decimal
    
    return Str;
}