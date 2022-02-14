/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define HEAP(Type) heap_handle *

// TODO no null node, instead, have the first real link be a loop
// TODO free handles refer to blocks, remove offset

typedef struct heap_handle {
    u08 *Data;
    u64 Size;
    u64 Offset;
    u16 Index;
    u16 Prev;
    u16 Next;
    u16 PrevBlock;
    u16 NextBlock;
    b08 Free;
} heap_handle;

typedef struct heap {
    u64 TotalSize;
} heap;

typedef struct stack {
    u64 Size;
    vptr *FirstMarker;
    u08 *Cursor;
} stack;