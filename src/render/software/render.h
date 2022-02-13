/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TERMINAL__INIT_CHAR_COUNT 256
#define TERMINAL__INIT_LINE_COUNT 32

typedef struct terminal {
    u32 LineCount;
    u32 MaxLines;
    u32 *Lines;
    
    string Text;
    
    font *Font;
} terminal;

typedef struct renderer {
    v2u32 Size;
    v4u08 *Buffer;
} renderer;