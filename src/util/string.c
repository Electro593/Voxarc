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
internal string CNString(c08 *Chars) { return CLString(Chars, Mem_BytesUntil(Chars, 0)+1); }
internal string LString(u64 Length) { return CLString(Stack_Allocate(Length), Length); }

internal void
SetVAArgsToIndex(va_list ArgsStart, va_list *Args, u32 Index)
{
    #ifdef _X64
        *Args = ArgsStart + 8*Index;
    #endif
}

/* TODO:
  - %f, %e/%E, %g/%G
  - %lc
*/
internal string
VString(string Format, va_list Args)
{
    va_list ArgsStart = Args;
    
    c08 *Out = Stack_GetCursor();
    string Result;
    Result.Resizable = FALSE;
    Result.Text = Out;
    
    c08 *C = Format.Text;
    while(C < Format.Text+Format.Length) {
        if(*C == '%') {
            C++;
            if(*C == '%') {
                *Out++ = *C++;
            } else {
                persist c08 *BaseChars = "0123456789abcdef0123456789ABCDEF";
                u32 BaseCharsUOffset = 16;
                b08 AlignLeft = FALSE;
                b08 PrefixPlus = FALSE;
                b08 PrefixSpace = FALSE;
                b08 HashFlag = FALSE;
                b08 PadZero = FALSE;
                b08 CustomPrecision = FALSE;
                u32 LengthLen = 0;
                c08 *LengthChars;
                u32 IntWidth = 4;
                u32 FloatWidth = 8;
                s32 MinChars = 0;
                s32 Precision = 0;
                b08 CustomConversionIndex = FALSE;
                u32 ConversionIndex = 0;
                b08 HasSeparatorChar = FALSE;
                
                if(*C >= '0' && *C <= '9') {
                    u32 Index = 0;
                    while(*C >= '0' && *C <= '9')
                        Index = Index*10 + *C++-'0';
                    
                    if(*C == '$') {
                        CustomConversionIndex = TRUE;
                        ConversionIndex = Index;
                        C++;
                    } else {
                        MinChars = Index;
                    }
                }
                
                while(TRUE) {
                    if(*C == '-')
                        AlignLeft = TRUE;
                    else if(*C == '+')
                        PrefixPlus = TRUE;
                    else if(*C == ' ')
                        PrefixSpace = TRUE;
                    else if(*C == '#')
                        HashFlag = TRUE;
                    else if(*C == '0')
                        PadZero = TRUE;
                    else if(*C == '\'')
                        HasSeparatorChar = TRUE;
                    else break;
                    C++;
                }
                
                if(*C == '*') {
                    C++;
                    if(*C >= '0' && *C <= '9') {
                        u32 Index = 0;
                        while(*C >= '0' && *C <= '9')
                            Index = Index*10 + *C++-'0';
                        Assert(*C == '$', "Invalid format!");
                        C++;
                        
                        SetVAArgsToIndex(ArgsStart, &Args, Index-1);
                    }
                    
                    MinChars = VA_Next(Args, s64);
                    if(MinChars < 0) {
                        AlignLeft = !AlignLeft;
                        MinChars = -MinChars;
                    }
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
                        C++;
                        if(*C >= '0' && *C <= '9') {
                            u32 Index = 0;
                            while(*C >= '0' && *C <= '9')
                                Index = Index*10 + *C++-'0';
                            Assert(*C == '$', "Invalid format!");
                            C++;
                            
                            SetVAArgsToIndex(ArgsStart, &Args, Index-1);
                        }
                        
                        Precision = VA_Next(Args, s64);
                        if(Precision < 0) Precision = 0;
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
                
                if(CustomConversionIndex)
                    SetVAArgsToIndex(ArgsStart, &Args, ConversionIndex-1);
                
                switch(*C) {
                    // Signed numbers
                    case 'd':
                    case 'i': {
                        s64 Temp = VA_Next(Args, s64);
                        
                        if(LengthLen == 2 && LengthChars[0] == 'h' && LengthChars[1] == 'h')
                            Temp = (s08)Temp;
                        else if(LengthLen == 1 && LengthChars[0] == 'h')
                            Temp = (s16)Temp;
                        else if(!((LengthLen == 2 && LengthChars[0] == 'l' && LengthChars[1] == 'l') || (LengthLen == 1 && (LengthChars[0] == 'L' || LengthChars[0] == 't' || LengthChars[0] == 'j' || LengthChars[0] == 'z' || LengthChars[0] == 'q'))))
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
                        s32 SeparatorCount = 0;
                        if(HasSeparatorChar)
                            SeparatorCount = (Len-1)/3;
                        s32 MinDigits = MinChars - HasPrefix - SeparatorCount;
                        if(PadZero) Len = MAX(MinDigits, Len);
                        s32 Padding = MinDigits - Len;
                        
                        Value = (s64)Temp * (1-2*Negative);
                        
                        if(!AlignLeft) {
                            while(Padding-- > 0)
                                *Out++ = ' ';
                        }
                        
                        if(HasPrefix)
                            *Out++ = Prefix;
                        
                        u32 IStart = Len + SeparatorCount - 1;
                        for(s32 I = IStart; I >= 0; I--) {
                            if(HasSeparatorChar && (IStart-I+1)%4 == 0) {
                                Out[I] = ',';
                            } else {
                                Out[I] = (Value%10) + '0';
                                Value /= 10;
                            }
                        }
                        Out += IStart + 1;
                        
                        while(Padding-- > 0)
                            *Out++ = ' ';
                    } break;
                    
                    // Unsigned numbers
                    case 'b':
                    case 'B':
                    case 'o':
                    case 'u':
                    case 'x':
                    case 'X': {
                        u64 Temp = VA_Next(Args, u64);
                        
                        if(LengthLen == 2 && LengthChars[0] == 'h' && LengthChars[1] == 'h')
                            Temp = (u08)Temp;
                        else if(LengthLen == 1 && LengthChars[0] == 'h')
                            Temp = (u16)Temp;
                        else if(!((LengthLen == 2 && LengthChars[0] == 'l' && LengthChars[1] == 'l') || (LengthLen == 1 && (LengthChars[0] == 'L' || LengthChars[0] == 't' || LengthChars[0] == 'j' || LengthChars[0] == 'z' || LengthChars[0] == 'q'))))
                            Temp = (u32)Temp;
                        
                        if(!CustomPrecision) Precision = 1;
                        
                        u32 Radix;
                        u32 SeparatorGap;
                        c08 SeparatorChar;
                        if(*C == 'b' || *C == 'B') {
                            Radix = 2;
                            SeparatorGap = 8;
                            SeparatorChar = '_';
                        } else if(*C == 'o') {
                            Radix = 8;
                            SeparatorGap = 4;
                            SeparatorChar = '_';
                        } else if(*C == 'u') {
                            Radix = 10;
                            SeparatorGap = 3;
                            SeparatorChar = ',';
                        } else if(*C == 'x' || *C == 'X') {
                            Radix = 16;
                            SeparatorGap = 4;
                            SeparatorChar = '_';
                        }
                        
                        u32 Offset = (*C == 'B' || *C == 'X') ? BaseCharsUOffset : 0;
                        
                        u64 Value = Temp;
                        b08 PrefixLen = HashFlag*(2*(Radix == 2) + (Radix == 8) + 2*(Radix == 16));
                        s32 Len = 0;
                        while(Value > 0) {
                            Value /= Radix;
                            Len++;
                        }
                        Len = MAX(Len, Precision);
                        s32 SeparatorCount = 0;
                        if(HasSeparatorChar)
                            SeparatorCount = (Len-1)/SeparatorGap;
                        s32 MinDigits = MinChars - PrefixLen - SeparatorCount;
                        if(PadZero) Len = MAX(MinDigits, Len);
                        s32 Padding = MinDigits - Len;
                        
                        Value = Temp;
                        
                        if(!AlignLeft) {
                            while(Padding-- > 0)
                                *Out++ = ' ';
                        }
                        
                        if(PrefixLen && Radix == 2) {
                            *Out++ = '0';
                            *Out++ = *C;
                        } else if(PrefixLen && Radix == 8)
                            *Out++ = '0';
                        else if(PrefixLen && Radix == 16) {
                            *Out++ = '0';
                            *Out++ = *C;
                        }
                        
                        u32 IStart = Len + SeparatorCount - 1;
                        for(s32 I = IStart; I >= 0; I--) {
                            if(HasSeparatorChar && (IStart-I+1)%(SeparatorGap+1) == 0) {
                                Out[I] = SeparatorChar;
                            } else {
                                Out[I] = BaseChars[Value%Radix + Offset];
                                Value /= Radix;
                            }
                        }
                        Out += IStart + 1;
                        
                        while(Padding-- > 0)
                            *Out++ = ' ';
                    } break;
                    
                    // Floating point numbers
                    
                    case 'e':
                    case 'E':
                    case 'f':
                    case 'F':
                    case 'g':
                    case 'G': {
                        
                    } break;
                    
                    case 'a':
                    case 'A': {
                        if(LengthLen == 1 && LengthChars[0] == 'L')
                            Assert(FALSE, "Long doubles not implemeneted!");
                        
                        b08 Caps = (*C == 'A');
                        u32 Offset = Caps * BaseCharsUOffset;
                        
                        r64 Value = VA_Next(Args, r64);
                        u64 Binary = FORCE_CAST(u64, Value);
                        
                        s32 Sign         = (Binary & R64_SIGN_MASK) >> R64_SIGN_SHIFT;
                        s32 Exponent     = (s32)((Binary & R64_EXPONENT_MASK) >> R64_EXPONENT_SHIFT) - R64_EXPONENT_BIAS;
                        u64 MantissaBits = Binary & R64_MANTISSA_MASK;
                        
                        u32 Whole;
                        if(Exponent == -R64_EXPONENT_BIAS) {
                            Whole = 0;
                        } else {
                            u64 NewExp = MIN(Exponent, 3);
                            Exponent -= NewExp;
                            u64 ModifiedExp = (NewExp + R64_EXPONENT_BIAS) << R64_EXPONENT_SHIFT;
                            u64 ModifiedBin = ModifiedExp | MantissaBits;
                            r64 NewFloat = FORCE_CAST(r64, ModifiedBin);
                            Whole = (u32)NewFloat;
                        }
                        
                        if(!CustomPrecision) Precision = 0;
                        
                        u32 FracLen = 0;
                        u32 Index;
                        b08 ValidIndex = Intrin_BitScanForward64(&Index, MantissaBits);
                        if(!ValidIndex) FracLen = 0;
                        else FracLen = Index/4 + 1;
                        FracLen = MAX(Precision, FracLen);
                        u32 FracMax = (R64_MANTISSA_BITS+3)/4;
                        
                        b08 Negative = Sign;
                        b08 HasPrefix = Negative || PrefixSpace || PrefixPlus;
                        c08 Prefix;
                        
                        b08 HasExpSign = (Exponent < 0) || PrefixPlus;
                        s32 ExpLen = 1;
                        s32 ExpCpy = Exponent;
                        while(ExpCpy /= 10) ExpLen++;
                        
                        b08 HasDecimal = (Precision != 0) || HashFlag;
                        u32 TotalLen = HasPrefix + 2 + 1 + HasDecimal + FracLen + 1 + HasExpSign + ExpLen;
                        s32 Padding = MinChars - TotalLen;
                        
                        if(!AlignLeft) {
                            while(Padding-- > 0)
                                *Out++ = ' ';
                        }
                        
                        if(HasPrefix) {
                            if(Negative)        *Out++ = '-';
                            else if(PrefixPlus) *Out++ = '+';
                            else                *Out++ = ' ';
                        }
                        
                        *Out++ = '0';
                        *Out++ = (Caps) ? 'X' : 'x';
                        
                        *Out++ = BaseChars[Whole + Offset];
                        
                        if(HasDecimal)
                            *Out++ = '.';
                        
                        while(FracLen) {
                            u32 Digit = (MantissaBits >> (4*(FracMax-FracLen))) & 0xF;
                            *Out++ = BaseChars[Digit + Offset];
                            FracLen--;
                        }
                        
                        *Out++ = (Caps) ? 'P' : 'p';
                        
                        if(HasExpSign)
                            *Out++ = (Exponent < 0) ? '-' : '+';
                        
                        do {
                            *Out++ = (Exponent%10) + '0';
                        } while(Exponent /= 10);
                        
                        while(Padding-- > 0)
                            *Out++ = ' ';
                    } break;
                    
                    // Other values
                    
                    case 'c': {
                        s32 Value = VA_Next(Args, s32);
                        
                        s32 Len;
                        if(Value) Len = 18;
                        else      Len = 6;
                        s32 Padding = MinChars - Len;
                        
                        if(!AlignLeft) {
                            while(Padding-- > 0)
                                *Out++ = ' ';
                        }
                        
                        if(LengthLen == 1 && LengthChars[0] == 'l')
                            Assert(FALSE, "Wide chars not implemented!");
                        else
                            *Out++ = (c08)Value;
                        
                        while(Padding-- > 0)
                            *Out++ = ' ';
                    } break;
                    
                    case 's': {
                        if(!CustomPrecision) Precision = S32_MAX;
                        
                        if(LengthLen == 1 && LengthChars[0] == 'l') {
                            Assert(FALSE, "Wide strings not implemented!");
                        } else {
                            c08 *In = VA_Next(Args, c08*);
                            c08 *Start = Out;
                            
                            if(!AlignLeft) {
                                s32 Len = Mem_BytesUntil(In, 0);
                                Len = MIN(Len, Precision);
                                s32 Padding = MinChars-Len;
                                while(Padding-- > 0)
                                    *Out++ = ' ';
                            }
                            
                            while(*In && Precision > 0) {
                                *Out++ = *In++;
                                Precision--;
                            }
                            
                            while(Out < Start + MinChars)
                                *Out++ = ' ';
                        }
                    } break;
                    
                    case 'p': {
                        u64 Value = (u64)VA_Next(Args, vptr);
                        
                        s32 Len;
                        if(Value) Len = 18;
                        else      Len = 6;
                        s32 Padding = MinChars - Len;
                        
                        if(!AlignLeft) {
                            while(Padding-- > 0)
                                *Out++ = ' ';
                        }
                        
                        if(Value) {
                            *Out++ = '0';
                            *Out++ = 'x';
                            for(s32 I = 15; I >= 0; I--) {
                                Out[I] = BaseChars[Value%16 + BaseCharsUOffset];
                                Value /= 16;
                            }
                            Out += 16;
                        } else {
                            Mem_Cpy(Out, "(null)", 6);
                            Out += 6;
                        }
                        
                        while(Padding-- > 0)
                            *Out++ = ' ';
                    } break;
                    
                    case 'n': {
                        if(LengthLen == 2 && LengthChars[0] == 'h' && LengthChars[1] == 'h') {
                            s08 *Value = VA_Next(Args, s08*);
                            *Value = (u64)Out - (u64)Result.Text;
                        } else if(LengthLen == 1 && LengthChars[0] == 'h') {
                            s16 *Value = VA_Next(Args, s16*);
                            *Value = (u64)Out - (u64)Result.Text;
                        } else if((LengthLen == 2 && LengthChars[0] == 'l' && LengthChars[1] == 'l') || (LengthLen == 1 && (LengthChars[0] == 'L' || LengthChars[0] == 't' || LengthChars[0] == 'j' || LengthChars[0] == 'z' || LengthChars[0] == 'q'))) {
                            s64 *Value = VA_Next(Args, s64*);
                            *Value = (u64)Out - (u64)Result.Text;
                        } else {
                            s32 *Value = VA_Next(Args, s32*);
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
    
    return Result;
}

internal string
FString(string Format, ...)
{
    va_list Args;
    VA_Start(Args, Format);
    
    string Result = VString(Format, Args);
    
    VA_End(Args);
    
    return Result;
}

internal string
CFString(c08 *Format, ...)
{
    va_list Args;
    VA_Start(Args, Format);
    
    string Result = VString(CString(Format), Args);
    
    VA_End(Args);
    
    return Result;
}

internal string
String_Cat(string A, string B)
{
    string Result = LString(A.Length + B.Length);
    
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
    
    string Str = LString(Whole.Length + 1 + DigitsAfterDecimal);
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