/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal void
Game_Init(game_state *Game,
          renderer_state *Renderer)
{
    Renderer_Init(Renderer);
    
    u64 HeapSize = 64*1024*1024;
    heap *Heap = Platform_AllocateMemory(HeapSize);
    Heap_Init(Heap, HeapSize);
    
    heap_handle *HandleA = Heap_Allocate(Heap, 1024);
    heap_handle *HandleB = Heap_Allocate(Heap, 0);
    heap_handle *HandleC = Heap_Allocate(Heap, 1);
    heap_handle *HandleD = Heap_Allocate(Heap, 14);
    heap_handle *HandleE = Heap_Allocate(Heap, 64*1024);
    
    Heap_Free(HandleD);
    Heap_Free(HandleA);
    Heap_Free(HandleC);
    
    HandleC = Heap_Allocate(Heap, 23);
    
    Heap_Free(HandleE);
    Heap_Free(HandleB);
    
    HandleD = Heap_Allocate(Heap, 1);
    
    Heap_Free(HandleC);
    
    HandleA = Heap_Allocate(Heap, 128);
    
    Heap_Free(HandleD);
    Heap_Free(HandleA);
    
    Platform_FreeMemory(Heap);
}

internal void
Game_Update(game_state *Game,
            renderer_state *Renderer)
{
    r32 R = R32_Abs((r32)(s08)Game->DebugCounter)/128.f;
    r32 G = 0;
    r32 B = 0;
    
    OpenGL_ClearColor(R, G, B, 1);
    Game->DebugCounter++;
    
    OpenGL_Clear(GL_COLOR_BUFFER_BIT);
    
    Renderer_Draw(Renderer);
}