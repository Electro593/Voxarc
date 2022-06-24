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
#include <game/msdf.c>
#include <renderer/font.c>
#include <game/file.c>
#include <renderer/opengl/mesh.c>
#include <game/ui.c>
#include <renderer/opengl/render.c>

// TODO handle errors, e.g. make a memory representation of a default
//      file if loading it fails

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
    
    // File_CreateAssetpack("assets\\0.pack", RendererHeap, 60.0f);
    assetpack Assetpack = File_LoadAssetpack("assets\\0.pack", RendererHeap);
    Renderer->Assetpack = Assetpack;
    
    Renderer_Init(Renderer, RendererHeap, Platform->WindowSize);
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
    if(Platform->Updates & WINDOW_RESIZED) {
        Renderer_Resize(Platform->WindowSize, &Renderer->PerspectiveMatrix);
    }
    
    r32 Step = 0.01;
    v3r32 PosDelta = {0};
    if(Platform->Keys[ScanCode_ArrowLeft] != KEY_RELEASED)
        PosDelta.X -= Step;
    if(Platform->Keys[ScanCode_ArrowRight] != KEY_RELEASED)
        PosDelta.X += Step;
    if(Platform->Keys[ScanCode_ArrowDown] != KEY_RELEASED)
        PosDelta.Z += Step;
    if(Platform->Keys[ScanCode_ArrowUp] != KEY_RELEASED)
        PosDelta.Z -= Step;
    if(Platform->Keys[ScanCode_Space] != KEY_RELEASED)
        PosDelta.Y += Step;
    if(Platform->Keys[ScanCode_ShiftLeft]  != KEY_RELEASED ||
       Platform->Keys[ScanCode_ShiftRight] != KEY_RELEASED)
        PosDelta.Y -= Step;
    V3r32_Norm(PosDelta);
    Renderer->Pos = V3r32_Add(Renderer->Pos, PosDelta);
    
    Step = 0.01;
    v3r32 DirDelta = (v3r32){0};
    if(Platform->Keys[ScanCode_X] != KEY_RELEASED)
        DirDelta.Y += Step;
    if(Platform->Keys[ScanCode_C] != KEY_RELEASED)
        DirDelta.Y -= Step;
    Renderer->Dir = V3r32_Add(Renderer->Dir, DirDelta);
    
    if((Platform->Updates & WINDOW_RESIZED) ||
       !V3r32_IsEqual(PosDelta, (v3r32){0}) ||
       !V3r32_IsEqual(DirDelta, (v3r32){0}))
    {
        r32 y = Renderer->Dir.Y;
        
        Renderer->ViewMatrix = (m4x4r32){
             R32_cos(y), 0,      R32_sin(y), -Renderer->Pos.X,
             0,          1,      0,          -Renderer->Pos.Y,
            -R32_sin(y), 0,      R32_cos(y),  Renderer->Pos.Z,
             0,          0,      0,           1
        };
        
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        OpenGL_UniformMatrix4fv(Renderer->UI.Mesh.VPMatrix, 1, FALSE, VPMatrix);
        
        Platform->Updates &= ~WINDOW_RESIZED;
    }
    
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