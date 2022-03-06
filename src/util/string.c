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
internal string CString(c08 *Chars) { return CLString(Chars, Mem_BytesUntil(Chars, 0)+1); }
internal string EString(u64 Length) { return CLString(Stack_Allocate(Length), Length); }

// internal void
// GrowString(string String)
// {
//     Assert(String.Resizable);
    
//     String.Capacity *= 2;
//     HeapResize(&String.Text, String.Capacity);
// }

internal void
String_Free(string String)
{
    if(!String.Resizable) return;
    
    // Heap_Free(String.Text);
    String.Length = 0;
    String.Capacity = 0;
    String.Resizable = FALSE;
    String.Text = NULL;
}


internal string
U64_ToString(u64 N,
             u08 Radix)
{
    Assert(2 <= Radix && Radix <= 64);
    
    persist c08 Chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";
    
    u32 StrSize = 64+10+1;
    u32 Index = StrSize;
    
    c08 *LineStr = Stack_Allocate(StrSize);
    LineStr[--Index] = 0;
    
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