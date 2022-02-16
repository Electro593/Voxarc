/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define HEAP(Type) heap_handle *

// TODO: Free handles point to free blocks
//       Can remove offset, so no more size jankness
//       It might effect Prev/NextBlock, too, so check that

typedef vptr heap;
typedef struct heap_handle {
    u08 *Data;
    u64 Offset : 39;
    u64 Free   :  1;
    u64 Index  : 16;
    u64 SizeU  :  8;
    u32 SizeL;
    u16 PrevFree;
    u16 NextFree;
    u16 PrevUsed;
    u16 NextUsed;
    u16 PrevBlock;
    u16 NextBlock;
} heap_handle;

typedef struct stack {
    u64 Size;
    vptr *FirstMarker;
    u08 *Cursor;
} stack;