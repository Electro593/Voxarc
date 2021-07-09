/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "vox_shared.h"

#include "platform/vox_module.h"
#include "game/vox_module.h"

global_var global_state *__Global;

#include "game/vox_file.c"
// #include "game/vox_asset.c"
#include "game/vox_mesh.c"
#include "game/vox_ui.c"
// #include "game/vox_world.c"
#include "game/util/vox_mem.c"
#include "game/util/vox_str.c"
#include "game/util/vox_json.c"
#include "game/util/math/vox_rand.c"
#include "game/util/math/vox_bit.c"
#include "game/util/math/vox_s32.c"
#include "game/util/math/vox_u32.c"
#include "game/util/math/vox_u64.c"
#include "game/util/math/vox_r32.c"
#include "game/util/math/vox_v2.c"
#include "game/util/math/vox_v3.c"
#include "game/util/math/vox_v4.c"
#include "game/util/math/vox_m3x3.c"
#include "game/util/math/vox_m4x4.c"

//TODO: Make a logger
internal void
Game_Init(platform_state *PlatformState,
          game_state *GameState,
          game_input *Input)
{
    UNUSED(Input);
    
    u32 StackSize = MEBIBYTES(2);
    u32 GeneralHeapSize = MEBIBYTES(4);
    u32 MeshHeapSize = MEBIBYTES(2);
    u32 StrHeapSize = MEBIBYTES(2);
    ASSERT(StackSize + GeneralHeapSize + MeshHeapSize + StrHeapSize <= PlatformState->AllocationSize);
    mem MemoryBase = PlatformState->AllocationBase;
    
    Stack_Init(MemoryBase, StackSize);
    MemoryBase += StackSize;
    
    heap *GeneralHeap = GameState->GeneralHeap = Heap_Create(MemoryBase, GeneralHeapSize);
    MemoryBase += GeneralHeapSize;
    
    heap *MeshHeap = GameState->MeshHeap = Heap_Create(MemoryBase, MeshHeapSize);
    MemoryBase += MeshHeapSize;
    
    Str_SetHeap(Heap_Create(MemoryBase, StrHeapSize));
    MemoryBase += StrHeapSize;
    
    Stack_Push();
    
    File_CreateAssetPack(GeneralHeap);
    
    // File_ReadConfig(&GameState->Config, Str_Create(&TempStr, "config.cfg", 0)); Str_Free(TempStr);
    
    //TODO: Make tags importable?
    GameState->AssetPack = File_ReadAssetPack(GeneralHeap);
    
    #if defined(_VOX_DEBUG)
        GL_Enable(GL_DEBUG_OUTPUT);
        GL_DebugMessageCallback(GL_DebugCallback, NULL);
        u32 ID = 131185;
        GL_DebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, &ID, FALSE);
    #endif
    
    mesh *UIMesh = &GameState->UIMesh;
    UIMesh->ProgramID = File_ReadShaders("shaders/general_pt.vert", "shaders/general_pt.frag");
    GL_UseProgram(UIMesh->ProgramID);
    
    Mesh_Init(MeshHeap, UIMesh, MESH_HAS_TEXTURES);
    
    GL_GenTextures(1, &GameState->UIMesh.TextureID);
    GL_BindTexture(GL_TEXTURE_2D_ARRAY, GameState->UIMesh.TextureID);
    GL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    GL_TexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, GameState->AssetPack->AtlasSize.X,
                  GameState->AssetPack->AtlasSize.Y, GameState->AssetPack->AtlasCount,
                  0, GL_RGBA, GL_UNSIGNED_BYTE, GameState->AssetPack->Atlases);
    GL_Uniform1ui(GameState->UIMesh.AtlasCountID, GameState->AssetPack->AtlasCount);
    GL_Uniform1i(GameState->UIMesh.SamplerID, 0);
    
    u32 ObjectCount = 2;
    mesh_object *Objects = Stack_Allocate(ObjectCount * sizeof(mesh_object));
    str UIStr = Str_Create(NULL, "abcdefghijklmnopqrstuvwxyz  ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0);
    // str UIStr = Str_Create(NULL, "LY  Y", 0);
    str UIStr2 = Str_Create(NULL, "0123456789`~-_=+[{]}\\|;:\'\",<.>/?!@#$%^&*()", 0);
    UI_CreateStringObject(MeshHeap, &GameState->UIMesh, Objects, GameState->AssetPack,
                          UIStr, V3r32_3x1(-1.0f, 1.0f, 0.0f), 72.0f);
    UI_CreateStringObject(MeshHeap, &GameState->UIMesh, Objects+1, GameState->AssetPack,
                          UIStr2, V3r32_3x1(-1.0f, 0.0f, 0.0f), 72.0f);
    // UI_CreateStringObject(MeshHeap, &GameState->UIMesh, Objects+2, GameState->AssetPack,
    //                       UIStr3, V3r32_3x1(-1.0f, 0.0f, 0.0f), 72.0f);
    Str_Free(UIStr);
    Str_Free(UIStr2);
    // Str_Free(UIStr3);
    
    //TODO: Mesh indexing function
    
    Mesh_AddObjects(UIMesh, ObjectCount, Objects);
    Mesh_Finalize(UIMesh);
    
    v3u16 TC = *(v3u16*)((u32*)UIMesh->Vertices+1); UNUSED(TC);
    
    GL_Enable(GL_CULL_FACE);
    GL_Enable(GL_DEPTH_TEST);
    GL_Enable(GL_SCISSOR_TEST);
    
    GL_Enable(GL_BLEND);
    GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GL_ClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    //TODO: Look into multi-draw indirect rendering, bindless textures, persistent mapping
    //      Also, Multi-draw SBTA (Sparse Bindless Texture Array), fallback on Multi-draw texture array
    //      Or just bindless textures (remember std140)
    
    // u32 ChannelRange = 1;
    // for(u32 ChannelRangeIndex = 0;
    //     ChannelRangeIndex < State->Config.VisionBitsPerChannel;
    //     ++ChannelRangeIndex)
    // {
    //     ChannelRange *= 2;
    // }
    // ChannelRange -= 1;
    
    // v4u32 ChannelShift = V4u32_4x1(3U * State->Config.VisionBitsPerChannel,
    //                                    2U * State->Config.VisionBitsPerChannel,
    //                                    1U * State->Config.VisionBitsPerChannel,
    //                                    0U * State->Config.VisionBitsPerChannel);
    
    // v4u32 ChannelRangeShift = V4u32_4x1(ChannelRange << ChannelShift.X,
    //                                         ChannelRange << ChannelShift.Y,
    //                                         ChannelRange << ChannelShift.Z,
    //                                         ChannelRange << ChannelShift.W);
    
    // u32 ChannelMask = ((State->Config.VisionChannelBinary & 8U) ? ChannelRangeShift.X : 0U) |
    //                   ((State->Config.VisionChannelBinary & 4U) ? ChannelRangeShift.Y : 0U) |
    //                   ((State->Config.VisionChannelBinary & 2U) ? ChannelRangeShift.Z : 0U) |
    //                   ((State->Config.VisionChannelBinary & 1U) ? ChannelRangeShift.W : 0U);
    
    // glUniform1ui(State->RenderData3D.ChannelRangeID, ChannelRange);
    // glUniform4ui(State->RenderData3D.ChannelShiftID, ChannelShift.X,
    //                                                ChannelShift.Y,
    //                                                ChannelShift.Z,
    //                                                ChannelShift.W);
    // glUniform4ui(State->RenderData3D.ChannelRangeShiftID, ChannelRangeShift.X,
    //                                                     ChannelRangeShift.Y,
    //                                                     ChannelRangeShift.Z,
    //                                                     ChannelRangeShift.W);
    // glUniform1ui(State->RenderData3D.ChannelMaskID, ChannelMask);
    
    // State->Player.Pos = V3r32_3x1((r32)(WORLD_SIZE_X * CHUNK_SIZE_X) / 2.0f,
    //                                 (r32)(WORLD_SIZE_Y * CHUNK_SIZE_Y) / 2.0f,
    //                                 (r32)(WORLD_SIZE_Z * CHUNK_SIZE_Z) / 2.0f),
    // State->Player.Yaw = -PI / 2;
    // State->Player.Pitch = 0.0f;
    // State->Player.Speed = 6.0f;
    
    GameState->TestCounter = 0;
    
    Stack_Pop();
}

internal void
Game_Update(platform_state *PlatformState,
            game_state *GameState,
            game_input *Input)
{
    UNUSED(Input);
    
    b08 UpdateProjectionMatrices = FALSE;
    if(FLAG_TEST(PlatformState->UpdateFlags, UPDATE_SIZE))
    {
        v2r32 Size = V2u32_ToV2r32(PlatformState->Size);
        
        r32 Aspect = (r32)VIRTUAL_WINDOW_SIZE_Y / (r32)VIRTUAL_WINDOW_SIZE_X;
        r32 Width = Size.X;
        r32 Height = Width * Aspect;
        
        if(Height > Size.Y)
        {
            Aspect = (r32)VIRTUAL_WINDOW_SIZE_X / (r32)VIRTUAL_WINDOW_SIZE_Y;
            Height = Size.Y;
            Width = Height * Aspect;
        }
        
        r32 Left = (Size.X - Width) / 2.0f;
        r32 Bottom = (Size.Y - Height) / 2.0f;
        
        GL_Viewport((s32)Left, (s32)Bottom, (s32)Width, (s32)Height);
        GL_Scissor((s32)Left, (s32)Bottom, (s32)Width, (s32)Height);
        GameState->OrthographicMatrix = M4x4r32_1(1.0f);
        
        UpdateProjectionMatrices = TRUE;
        FLAG_CLEAR(PlatformState->UpdateFlags, UPDATE_SIZE);
    }
    
    // if(Flags->CursorIsCaptured)
    // {
    //     if(Input->Keys[VK_ESCAPE])
    //     {
    //         Flags->CursorIsCaptured = FALSE;
    //     }
    //     else
    //     {
    //         v2s32 DeltaMouse = V2s32_Sub(State->PrevCursorPos, Input->CursorPos);
    //         UpdatePlayer(&State->Player, &State->RenderData3D.ViewMatrix, DeltaMouse,
    //                      State->Config.Sensitivity, Input, &State->Config);
    //         State->PrevCursorPos = Input->CursorPos;
            
    //         glUniformMatrix4fv(State->RenderData3D.ViewMatrixID, 1, FALSE, &State->RenderData3D.ViewMatrix.E[0]);
    //     }
    // }
    // else if(Input->Keys[VK_LBUTTON])
    // {
    //     Flags->CursorIsCaptured = TRUE;
    //     State->PrevCursorPos = Input->CursorPos;
    // }
    
    GL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    Mesh_Render(&GameState->UIMesh, GameState->OrthographicMatrix, UpdateProjectionMatrices);
    
    // v3r32 LightPos = V3r32_1(30.0f);
    // glUniform3f(State->RenderData3D.LightPositionID, LightPos.X, LightPos.Y, LightPos.Z);
    // glUniform3f(State->RenderData3D.LightColorID, 1, 1, 1);
    // glUniform1ui(State->RenderData3D.LightPowerID, 2000);
}

internal void
Game_Cleanup(platform_state *PlatformState,
             game_state *GameState,
             game_input *Input)
{
    Mesh_Free(&GameState->UIMesh);
    Heap_Free(GameState->AssetPack);
    #if defined(_VOX_DEBUG)
        Heap_Free(GameState->GlobalState.StrHeapDebugData.List);
    #endif
    
    ASSERT(GameState->GeneralHeap->BlockCount == 1);
    ASSERT(GameState->MeshHeap->BlockCount == 1);
    ASSERT(GameState->GlobalState.StrHeap->BlockCount == 1);
    
    
    #if defined(_VOX_DEBUG)
        if(GameState->GlobalState.StrHeapDebugData.DebugHeap)
        {
            Platform_FreeMemory(GameState->GlobalState.StrHeapDebugData.DebugHeap);
        }
    #endif
    
    UNUSED(PlatformState, Input);
}

external void
Game_Unload(game_exports *Functions,
            game_state *State)
{
    UNUSED(State);
    
    Mem_Zero(Functions, sizeof(game_exports));
}

external void
Game_Load(platform_callbacks *PlatformCallbacks,
          game_exports *Functions,
          game_state *State)
{
    if(State)
    {
        __Global = &State->GlobalState;
    }
    
    if(PlatformCallbacks)
    {
        #define PROC(ReturnType, Name, ...) \
            Platform_##Name = PlatformCallbacks->Platform_##Name;
        PLATFORM_CALLBACKS
        #undef PROC
        
        #if defined(_VOX_DEBUG)
            #define PROC(ReturnType, Name, ...) \
                Platform_##Name = PlatformCallbacks->Platform_##Name;
            PLATFORM_CALLBACKS_DEBUG
            #undef PROC
        #endif
        
        #define PROC(ReturnType, Name, ...) \
            Name = PlatformCallbacks->Name;
        PLATFORM_FUNCS
        #undef PROC
        
        #define PROC(ReturnType, Name, ...) \
            GL_##Name = PlatformCallbacks->GL_##Name;
        GL_PROCS
        GL_PLATFORM_PROCS
        #undef PROC
    }
    
    if(Functions)
    {
        #define PROC(ReturnType, Name, ...) \
            Functions->Name = Name;
        GAME__EXPORTS
        GAME__MODULE__EXPORTS
        #undef PROC
    }
}