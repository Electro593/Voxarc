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
#include <game/world.c>

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
    Renderer->Assetpack = File_LoadAssetpack("assets\\0.pack", RendererHeap);
    
    Renderer_Init(Renderer, RendererHeap, Platform->WindowSize);
    
    Stack_Push();
    
    u32 *TextureBytes = Stack_Allocate(sizeof(u32) * BLOCK_Count);
    for(u32 I = 1; I < BLOCK_Count; I++) {
        assetpack_tag *Tag = Assetpack_FindExactTag(Renderer->Assetpack, TAG_BLOCK_TEXTURE, (vptr)(u64)I);
        assetpack_asset *Asset = Tag->Assets[0];
        TextureBytes[I] = (u64)((u08*)Asset - (u64)Renderer->Assetpack.Assets);
    }
    
    chunk Chunk = MakeChunk(RendererHeap, &Renderer->PTMesh, (v3s32){0,0,0}, TextureBytes);
    mesh_object *Objects[] = {&Chunk.Object};
    Mesh_AddObjects(&Renderer->PTMesh, 1, Objects);
    Mesh_FreeObject(Chunk.Object);
    
    Mesh_Update(&Renderer->PTMesh);
    
    Stack_Pop();
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
    if(Platform->Updates & WINDOW_RESIZED) {
        Renderer_Resize(Platform->WindowSize, &Renderer->PerspectiveMatrix);
    }
    if(Platform->Updates & CURSOR_DISABLED) {
        Game->PrevCursorPos = Platform->CursorPos;
        Platform->Updates &= ~CURSOR_DISABLED;
    }
    
    b08 Moved = FALSE;
    v3r32 DirDelta = (v3r32){0};
    
    if(Platform->CursorIsDisabled) {
        v2s32 CursorDelta = V2s32_Sub(Platform->CursorPos, Game->PrevCursorPos);
        Game->PrevCursorPos = Platform->CursorPos;
        
        DirDelta.X = CursorDelta.Y/120.0f;
        DirDelta.Y = CursorDelta.X/120.0f;
        Renderer->Dir = V3r32_Add(Renderer->Dir, DirDelta);
        if(Renderer->Dir.X >  R32_PI/2) Renderer->Dir.X =  R32_PI/2;
        if(Renderer->Dir.X < -R32_PI/2) Renderer->Dir.X = -R32_PI/2;
        
        v3r32 MoveDir = {0};
        r32 Step = 0.1;
        if(Platform->Keys[ScanCode_A] != RELEASED)
            MoveDir.X--;
        if(Platform->Keys[ScanCode_D] != RELEASED)
            MoveDir.X++;
        if(Platform->Keys[ScanCode_S] != RELEASED)
            MoveDir.Z--;
        if(Platform->Keys[ScanCode_W] != RELEASED)
            MoveDir.Z++;
        if(Platform->Keys[ScanCode_Space] != RELEASED)
            MoveDir.Y++;
        if(Platform->Keys[ScanCode_ShiftLeft]  != RELEASED ||
           Platform->Keys[ScanCode_ShiftRight] != RELEASED)
            MoveDir.Y--;
        
        r32 Len = V3r32_Len(MoveDir);
        if(Len != 0) {
            MoveDir = V3r32_DivS(MoveDir, Len);
            
            if(MoveDir.X != 0 || MoveDir.Z != 0) {
                r32 XZLen = R32_sqrt(MoveDir.X*MoveDir.X + MoveDir.Z*MoveDir.Z);
                r32 Theta = R32_arccos(MoveDir.Z/XZLen);
                Theta *= R32_Sign(MoveDir.X);
                Theta += Renderer->Dir.Y;
                
                Renderer->Pos.X += Step*R32_sin(Theta);
                Renderer->Pos.Z += Step*R32_cos(Theta);
            }
            
            Renderer->Pos.Y += Step*MoveDir.Y;
            
            Moved = TRUE;
        }
    }
    
    if((Platform->Updates & WINDOW_RESIZED) ||
       Moved ||
       !V3r32_IsEqual(DirDelta, (v3r32){0}))
    {
        r32 Pitch = Renderer->Dir.X;
        r32 Yaw   = Renderer->Dir.Y;
        r32 CosYaw = R32_cos(Yaw);
        r32 SinYaw = R32_sin(Yaw);
        r32 CosPitch = R32_cos(Pitch);
        r32 SinPitch = R32_sin(Pitch);
        
        v3r32 Right = { CosYaw,           0,               -SinYaw};
        v3r32 Up    = {-SinYaw*SinPitch,  CosPitch,        -CosYaw*SinPitch};
        v3r32 Front = { SinYaw*CosPitch,  SinPitch,         CosYaw*CosPitch};
        v3r32 Pos   = {-Renderer->Pos.X, -Renderer->Pos.Y, -Renderer->Pos.Z};
        
        Renderer->ViewMatrix = (m4x4r32){
            Right.X, Right.Y, Right.Z, V3r32_Dot(Right, Pos),
            Up.X,    Up.Y,    Up.Z,    V3r32_Dot(Up,    Pos),
            Front.X, Front.Y, Front.Z, V3r32_Dot(Front, Pos),
            0,       0,       0,       1
        };
        
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        
        OpenGL_UseProgram(Renderer->PCProgram);
        OpenGL_UniformMatrix4fv(Renderer->PCMesh.VPMatrix,  1, FALSE, VPMatrix);
        
        OpenGL_UseProgram(Renderer->PTProgram);
        OpenGL_UniformMatrix4fv(Renderer->PTMesh.VPMatrix, 1, FALSE, VPMatrix);
        
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