/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define HEAP(Type) heap_handle *

typedef vptr heap;
typedef struct heap_handle {
    vptr Data;
    u64 Index    : 16;
    u64 Offset   : 46;
    u64 Free     :  1;
    u64 Anchored :  1;
    u32 Size;
    u16 PrevFree;
    u16 NextFree;
    u16 PrevUsed;
    u16 NextUsed;
    u16 PrevBlock;
    u16 NextBlock;
} heap_handle;

typedef struct stack {
    u64 Size;
    u08 *Cursor;
    vptr *FirstMarker;
} stack;





internal vptr Mem_Set(vptr Dest, s32 Data, u64 Size);
internal vptr Mem_Cpy(vptr Dest, vptr Src, u64 Size);
internal u64 Mem_BytesUntil(u08 *Data, u08 Byte);

internal heap *Heap_Init(vptr MemBase, u64 Size);
internal void Heap_Defragment(heap *Heap);
internal heap_handle *Heap_Allocate(heap *Heap, u64 Size);
internal vptr Heap_AllocateA(heap *Heap, u64 Size);
internal void Heap_Free(heap_handle *Handle);

internal stack *Stack_Init(vptr Mem, u64 Size);
internal void Stack_Push(void); 
internal vptr Stack_Allocate(u64 Size);
internal void Stack_Pop(void);