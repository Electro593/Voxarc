/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define HEAP_NULL_HANDLE 0xFFFF
#define HEAP(Type) heap_handle *

// TODO null handle at 0?

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
    u08 *DataCursor;
    
    u16 HandleCount;
    u16 FirstFree;
    u16 LastFree;
    u16 FirstUsed;
    u16 LastUsed;
    u16 FirstBlock;
    u16 LastBlock;
} heap;

typedef struct stack {
    u64 Size;
    vptr *FirstMarker;
    u08 *Cursor;
} stack;