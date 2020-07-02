#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "util/vox_string.h"
#include "math/vox_random.h"
#include "math/vox_math.h"
#include "math/vox_v2r32.h"
#include "math/vox_v2s32.h"
#include "math/vox_v3r32.h"
#include "math/vox_v4u32.h"
#include "math/vox_m4x4r32.h"

#include "util/vox_crt.c"
#include "util/vox_file_io.c"

#include "vox_world.c"
#include "vox_entity.c"

#include "voxarc.h"

local_func void
InitGame(game_state *State)
{
    size ChunksMemorySize = Megabytes(512);
    size UtilMemorySize = Megabytes(1);
    vptr MemoryBlock = ReserveMemory(ChunksMemorySize + UtilMemorySize);
    
    State->ChunksHandlePool = CreateHandlePool(MemoryBlock, ChunksMemorySize);
    State->UtilHandlePool = CreateHandlePool((u08*)MemoryBlock + ChunksMemorySize, UtilMemorySize);
    memory_handle *UtilMemoryHandle = State->UtilHandlePool->Handles;
    memory_handle *ChunksMemoryHandle = State->ChunksHandlePool->Handles;
    
    State->Config = ReadConfig(UtilMemoryHandle, "config.cfg");
    State->Program3DID = LoadShaders(UtilMemoryHandle, "shaders/shader3d.vert", "shaders/shader3d.frag");
    glUseProgram(State->Program3DID);
    
    State->RenderData.MVPMatrixID = glGetUniformLocation(State->Program3DID, "MVPMatrix");
    State->RenderData.ViewMatrixID = glGetUniformLocation(State->Program3DID, "ViewMatrix");
    State->RenderData.ModelMatrixID = glGetUniformLocation(State->Program3DID, "ModelMatrix");
    State->RenderData.ViewMatrix = M4x4r32_0();
    State->RenderData.ProjectionMatrix = M4x4r32_0();
    
    State->RenderData.LightPositionID = glGetUniformLocation(State->Program3DID, "LightPosition_worldspace");
    State->RenderData.LightPowerID = glGetUniformLocation(State->Program3DID, "LightPower");
    State->RenderData.LightColorID = glGetUniformLocation(State->Program3DID, "LightColor");
    State->RenderData.ChannelMaskID = glGetUniformLocation(State->Program3DID, "ChannelMask");
    State->RenderData.ChannelRangeID = glGetUniformLocation(State->Program3DID, "ChannelRange");
    State->RenderData.ChannelRangeShiftID = glGetUniformLocation(State->Program3DID, "ChannelRangeShift");
    State->RenderData.ChannelShiftID = glGetUniformLocation(State->Program3DID, "ChannelShift");
    
    State->Player.Pos = V3r32_1_1_1((r32)(WORLD_SIZE_X * CHUNK_SIZE_X) / 2.0f,
                                    (r32)(WORLD_SIZE_Y * CHUNK_SIZE_Y) / 2.0f,
                                    (r32)(WORLD_SIZE_Z * CHUNK_SIZE_Z) / 2.0f),
    State->Player.Yaw = -PI / 2;
    State->Player.Pitch = 0.0f;
    State->Player.Speed = 6.0f;
    
    u32 VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    State->World = CreateWorld(ChunksMemoryHandle, State->Config, State->RenderData);
    
    u32 ChannelRange = 1;
    for(u32 ChannelRangeIndex = 0;
        ChannelRangeIndex < State->Config.VisionBitsPerChannel;
        ++ChannelRangeIndex)
    {
        ChannelRange *= 2;
    }
    ChannelRange -= 1;
    
    v4u ChannelShift = V4u32_1_1_1_1(3U * State->Config.VisionBitsPerChannel,
                                     2U * State->Config.VisionBitsPerChannel,
                                     1U * State->Config.VisionBitsPerChannel,
                                     0U * State->Config.VisionBitsPerChannel);
    
    v4u ChannelRangeShift = V4u32_1_1_1_1(ChannelRange << ChannelShift.X,
                                          ChannelRange << ChannelShift.Y,
                                          ChannelRange << ChannelShift.Z,
                                          ChannelRange << ChannelShift.W);
    
    u32 ChannelMask = ((State->Config.VisionChannelBinary & 8U) ? ChannelRangeShift.X : 0U) |
                      ((State->Config.VisionChannelBinary & 4U) ? ChannelRangeShift.Y : 0U) |
                      ((State->Config.VisionChannelBinary & 2U) ? ChannelRangeShift.Z : 0U) |
                      ((State->Config.VisionChannelBinary & 1U) ? ChannelRangeShift.W : 0U);
    
    glUniform1ui(State->RenderData.ChannelRangeID, ChannelRange);
    glUniform4ui(State->RenderData.ChannelShiftID, ChannelShift.X,
                                                   ChannelShift.Y,
                                                   ChannelShift.Z,
                                                   ChannelShift.W);
    glUniform4ui(State->RenderData.ChannelRangeShiftID, ChannelRangeShift.X,
                                                        ChannelRangeShift.Y,
                                                        ChannelRangeShift.Z,
                                                        ChannelRangeShift.W);
    glUniform1ui(State->RenderData.ChannelMaskID, ChannelMask);
}

local_func void
UpdateGame(game_input *Input, game_state *State, platform_flags *Flags)
{
    if(Flags->AspectRatioChanged)
    {
        State->RenderData.ProjectionMatrix = M4x4r32_Perspective(Radians((r32)State->Config.FOV),
                                                                 State->WindowAspectRatio, 0.1f, 100.0f);
        Flags->AspectRatioChanged = FALSE;
    }
    
    if(Flags->CursorIsCaptured)
    {
        if(Input->Keys[VK_ESCAPE])
        {
            Flags->CursorIsCaptured = FALSE;
        }
        else
        {
            v2s32 DeltaMouse = V2s32_Subtract(State->PrevCursorPos, Input->CursorPos);
            UpdatePlayer(&State->Player, &State->RenderData.ViewMatrix, DeltaMouse,
                         State->Config.Sensitivity, Input, &State->Config);
            State->PrevCursorPos = Input->CursorPos;
        }
    }
    else if(Input->Keys[VK_LBUTTON])
    {
        Flags->CursorIsCaptured = TRUE;
        State->PrevCursorPos = Input->CursorPos;
    }
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram(State->Program3DID);
    
    glUniformMatrix4fv(State->RenderData.ViewMatrixID, 1, FALSE, &State->RenderData.ViewMatrix.M[0][0]);
    
    v3r LightPos = V3r32_1(30.0f);
    glUniform3f(State->RenderData.LightPositionID, LightPos.X, LightPos.Y, LightPos.Z);
    glUniform3f(State->RenderData.LightColorID, 1, 1, 1);
    glUniform1ui(State->RenderData.LightPowerID, 2000);
    
    // RenderWorld(State->ChunksHandlePool->Handles, &State->World, State->Config,
    //             &State->Random, State->RenderData);
    
    //TODO: "F3" Screen
}