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