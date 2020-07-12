#include "util/vox_defines.h"

#include "game/vox_world.c"
#include "game/vox_entity.c"

#include "math/vox_m3x3r32.h"
#include "math/vox_m4x4r32.h"
#include "math/vox_math.h"
#include "math/vox_random.h"
#include "math/vox_v2r32.h"
#include "math/vox_v2s32.h"
#include "math/vox_v2u32.h"
#include "math/vox_v3r32.h"
#include "math/vox_v3s32.h"
#include "math/vox_v3u32.h"
#include "math/vox_v4r32.h"
#include "math/vox_v4u32.h"

#include "util/vox_assets.c"
#include "util/vox_crt.c"
#include "util/vox_debug.c"
#include "util/vox_file_io.c"
#include "util/vox_memory.h"
#include "util/vox_render.c"
#include "util/vox_string.h"

#include "voxarc.h"

local_func void
InitGame(game_state *State)
{
    State->ChunksHandlePool = CreateHandlePool(Megabytes(512));
    State->DebugHandlePool = CreateHandlePool(Megabytes(1));
    State->AssetsHandlePool = CreateHandlePool(Megabytes(512));
    State->UtilHandlePool = CreateHandlePool(Megabytes(1));
    State->TagSearchHandlePool = CreateHandlePool(AssetTag_Count * sizeof(voxai_tag));
    
    memory_handle *ChunksMemoryHandle = State->ChunksHandlePool->Handles;
    memory_handle *UtilMemoryHandle = State->UtilHandlePool->Handles;
    
    State->Config = ReadConfig(UtilMemoryHandle, "config.cfg");
    State->AssetMemory.Info = LoadVoxai(State->AssetsHandlePool, ASSET_INFO_FILENAME);
    //TODO: Make tags importable
    
    //NOTE: Init OpenGL
    u32 VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    //NOTE: Init RenderData3D
    // State->RenderData3D.ProgramID = LoadShaders(UtilMemoryHandle, "shaders/shader3d.vert", "shaders/shader3d.frag");
    
    // State->RenderData3D.MVPMatrixID = glGetUniformLocation(State->RenderData3D.ProgramID, "MVPMatrix");
    // State->RenderData3D.ViewMatrixID = glGetUniformLocation(State->RenderData3D.ProgramID, "ViewMatrix");
    // State->RenderData3D.ModelMatrixID = glGetUniformLocation(State->RenderData3D.ProgramID, "ModelMatrix");
    // State->RenderData3D.LightPositionID = glGetUniformLocation(State->RenderData3D.ProgramID, "LightPosition_worldspace");
    // State->RenderData3D.LightPowerID = glGetUniformLocation(State->RenderData3D.ProgramID, "LightPower");
    // State->RenderData3D.LightColorID = glGetUniformLocation(State->RenderData3D.ProgramID, "LightColor");
    // State->RenderData3D.ChannelMaskID = glGetUniformLocation(State->RenderData3D.ProgramID, "ChannelMask");
    // State->RenderData3D.ChannelRangeID = glGetUniformLocation(State->RenderData3D.ProgramID, "ChannelRange");
    // State->RenderData3D.ChannelRangeShiftID = glGetUniformLocation(State->RenderData3D.ProgramID, "ChannelRangeShift");
    // State->RenderData3D.ChannelShiftID = glGetUniformLocation(State->RenderData3D.ProgramID, "ChannelShift");
    // State->RenderData3D.ViewMatrix = M4x4r32_0();
    // State->RenderData3D.ProjectionMatrix = M4x4r32_0();
    
    // glGenBuffers(1, &State->RenderData3D.VertexBufferID);
    // glBindBuffer(GL_ARRAY_BUFFER, State->RenderData3D.VertexBufferID);
    
    // glGenBuffers(1, &State->RenderData3D.ColorBufferID);
    // glBindBuffer(GL_ARRAY_BUFFER, State->RenderData3D.ColorBufferID);
    
    // glGenBuffers(1, &State->RenderData3D.ElementBufferID);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, State->RenderData3D.ElementBufferID);
    
    // glUseProgram(State->RenderData3D.ProgramID);
    
    //NOTE: Init RenderData2D
    State->RenderData2D.ProgramID = LoadShaders(UtilMemoryHandle, "shaders/shader2d.vert", "shaders/shader2d.frag");
    
    State->RenderData2D.ViewportMatrixID = glGetUniformLocation(State->RenderData2D.ProgramID, "ViewportMatrix");
    State->RenderData2D.Texture0ID = glGetUniformLocation(State->RenderData2D.ProgramID, "Texture0");
    State->RenderData2D.ViewportMatrix = M4x4r32_0();
    
    glGenBuffers(1, &State->RenderData2D.VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, State->RenderData2D.VertexBufferID);
    
    glGenBuffers(1, &State->RenderData2D.TextureBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, State->RenderData2D.TextureBufferID);
    
    //NOTE: Init AssetMemory
    glGenTextures(1, &State->AssetMemory.Atlas0ID);
    glBindTexture(GL_TEXTURE_2D, State->AssetMemory.Atlas0ID);
    voxai_header *VoxaiHeader = GetVoxaiHeader(State->AssetMemory.Info);
    // TODO: Multiple atlases
    State->AssetMemory.Atlas0 = LoadVoxaa(State->AssetsHandlePool, ASSET_ATLAS_FILENAME);
    //NOTE: Maybe GL_ARGB?
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 VoxaiHeader->AtlasDim, VoxaiHeader->AtlasDim, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, State->AssetMemory.Atlas0->Base);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glUseProgram(State->RenderData2D.ProgramID);
    glUniform1i(State->RenderData2D.Texture0ID, 0);
    
    // NOTE: Channel Masks
    // u32 ChannelRange = 1;
    // for(u32 ChannelRangeIndex = 0;
    //     ChannelRangeIndex < State->Config.VisionBitsPerChannel;
    //     ++ChannelRangeIndex)
    // {
    //     ChannelRange *= 2;
    // }
    // ChannelRange -= 1;
    
    // v4u32 ChannelShift = V4u32_1_1_1_1(3U * State->Config.VisionBitsPerChannel,
    //                                    2U * State->Config.VisionBitsPerChannel,
    //                                    1U * State->Config.VisionBitsPerChannel,
    //                                    0U * State->Config.VisionBitsPerChannel);
    
    // v4u32 ChannelRangeShift = V4u32_1_1_1_1(ChannelRange << ChannelShift.X,
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
    
    //NOTE: Init Player
    // State->Player.Pos = V3r32_1_1_1((r32)(WORLD_SIZE_X * CHUNK_SIZE_X) / 2.0f,
    //                                 (r32)(WORLD_SIZE_Y * CHUNK_SIZE_Y) / 2.0f,
    //                                 (r32)(WORLD_SIZE_Z * CHUNK_SIZE_Z) / 2.0f),
    // State->Player.Yaw = -PI / 2;
    // State->Player.Pitch = 0.0f;
    // State->Player.Speed = 6.0f;
    
    //NOTE: Init World
    // State->World = CreateWorld(ChunksMemoryHandle, State->Config);
    
    //NOTE: Init Debug
    InitDebugData(&State->DebugData, State->DebugHandlePool);
    CreateDebugString("The quick brown fox jumped over the lazy dog.", State->TagSearchHandlePool, &State->AssetMemory,
                      &State->RenderData2D, &State->DebugData, 100, 100);
}

local_func void
UpdateGame(game_input *Input, game_state *State, platform_flags *Flags)
{
    if(Flags->DimensionsChanged)
    {
        // State->RenderData3D.ProjectionMatrix = M4x4r32_Perspective(Radians((r32)State->Config.FOV),
        //                                                          (r32)State->WindowDimensions.X /
        //                                                          (r32)State->WindowDimensions.Y,
        //                                                          0.1f, 100.0f);
        State->RenderData2D.ViewportMatrix = M4x4r32_Orthographic(0.0f, (r32)State->WindowDimensions.X,
                                                                  0.0f, (r32)State->WindowDimensions.Y);
        glUseProgram(State->RenderData2D.ProgramID);
        glUniformMatrix4fv(State->RenderData2D.ViewportMatrixID, 1, FALSE, &State->RenderData2D.ViewportMatrix.E[0]);
        
        Flags->DimensionsChanged = FALSE;
    }
    
    // if(Flags->CursorIsCaptured)
    // {
    //     if(Input->Keys[VK_ESCAPE])
    //     {
    //         Flags->CursorIsCaptured = FALSE;
    //     }
    //     else
    //     {
    //         v2s32 DeltaMouse = V2s32_Subtract(State->PrevCursorPos, Input->CursorPos);
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
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // glUseProgram(State->RenderData3D.ProgramID);
    
    // v3r32 LightPos = V3r32_1(30.0f);
    // glUniform3f(State->RenderData3D.LightPositionID, LightPos.X, LightPos.Y, LightPos.Z);
    // glUniform3f(State->RenderData3D.LightColorID, 1, 1, 1);
    // glUniform1ui(State->RenderData3D.LightPowerID, 2000);
    
    // RenderWorld(State->ChunksHandlePool->Handles, &State->World, State->Config,
    //             &State->Random, State->RenderData);
    
    //TODO: "F3" Screen
    glUseProgram(State->RenderData2D.ProgramID);
    Render2DMesh(&State->RenderData2D, &State->DebugData.MeshData);
}