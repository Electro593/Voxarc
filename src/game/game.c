/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

global_state __Global;

#include <util/scalar.c>
#include <util/vector.c>
#include <util/memory.c>
#include <util/string.c>
#include <renderer/font.c>
#include <renderer/opengl/render.c>
#include <game/file.c>

internal void
Game_Init(platform_state *Platform,
          game_state *Game,
          renderer_state *Renderer)
{
    u64 StackSize = 32*1024*1024;
    u64 RendererHeapSize = 32*1024*1024;
    vptr MemBase = Platform_AllocateMemory(StackSize+RendererHeapSize);
    
    __Global.Stack = Stack_Init(MemBase, StackSize);
    (u08*)MemBase += StackSize;
    
    heap *RendererHeap = Heap_Init(MemBase, RendererHeapSize);
    (u08*)MemBase += RendererHeapSize;
    
    // File_LoadAssetpack("assets\\0.pack");
    File_CreateAssetpack("assets\\0.bmp", RendererHeap);
    
    Renderer_Init(Renderer, RendererHeap);
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
    if(Platform->Updates & WINDOW_RESIZED) {
        Renderer_Resize(Platform->WindowSize);
        Platform->Updates &= ~WINDOW_RESIZED;
    }
    
    u32 Scale = 2048;
    s32 Value = Game->DebugCounter%Scale - (Scale/2);
    
    r32 R = 0;
    r32 G = 0;
    r32 B = R32_Abs((r32)Value/(Scale/2));
    
    OpenGL_ClearColor(R, G, B, 1);
    Game->DebugCounter++;
    
    Renderer_Draw(Renderer);
}

external void API_EXPORT
Game_Load(platform_exports *PlatformFuncs,
          game_exports *GameFuncs,
          game_state *GameState,
          opengl_funcs *OpenGLFuncs)
{
    if(PlatformFuncs) {
        #define EXPORT(ReturnType, Namespace, Name, ...) \
            Namespace##_##Name = PlatformFuncs->Namespace##_##Name;
        #define X PLATFORM_FUNCS
        #include <x.h>
    }
    
    if(GameFuncs) {
        #define EXPORT(ReturnType, Namespace, Name, ...) \
            GameFuncs->Namespace##_##Name = Namespace##_##Name;
        #define X GAME_FUNCS
        #include <x.h>
    }
    
    if(GameState) {
        __Global = GameState->GlobalState;
    }
    
    Renderer_Load(OpenGLFuncs);
}

external void API_EXPORT
Game_Unload(game_state *GameState)
{
    if(GameState) {
        GameState->GlobalState = __Global;
    }
}