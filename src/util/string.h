/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct string {
    u64 Length;
    u64 Capacity;
    
    b08 Resizable;
    
    c08 *Text;
} string;

internal string U64_ToString(u64 N, u08 Radix);
internal string U32_ToString(u32 N, u08 Radix);
internal string U16_ToString(u16 N, u08 Radix);
internal string U08_ToString(u08 N, u08 Radix);