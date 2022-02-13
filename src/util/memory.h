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

typedef struct heap_handle {
    u08 *Data;
    u64 Size;
    u64 Offset;
    u16 Index;
    u16 Next;
    u16 Prev;
    u16 PrevBlock;
    u16 NextBlock;
    b08 Free;
} heap_handle;

typedef struct heap {
    u64 TotalSize;
    
    u16 HandleCount;
    u16 FreeList;
    u16 UsedList;
    u16 BlockList;
    
    u08 *DataCursor; // Grows leftward
} heap;

typedef struct stack {
    u64 Size;
    vptr *FirstMarker;
    u08 *Cursor;
} stack;