#ifndef UTIL_VOX_RENDER_H_

#include "util/vox_defines.h"
#include "math/vox_m4x4r32.h"

struct mesh_data_3d
{
    memory_handle *Vertices;
    memory_handle *Colors;
    memory_handle *Indices;
    
    m4x4r32 ModelMatrix;
};

struct render_data_3d
{
    u32 ProgramID;
    
    u32 VertexBufferID;
    u32 ColorBufferID;
    u32 ElementBufferID;
    
    s32 ModelMatrixID;
    s32 ViewMatrixID;
    s32 MVPMatrixID;
    s32 LightPositionID;
    s32 LightPowerID;
    s32 LightColorID;
    s32 ChannelMaskID;
    s32 ChannelRangeID;
    s32 ChannelRangeShiftID;
    s32 ChannelShiftID;
    
    m4x4r32 ViewMatrix;
    m4x4r32 ProjectionMatrix;
};

struct mesh_data_2d
{
    memory_handle *Vertices;
    memory_handle *Textures;
    u32 AtlasIndex;
};

struct render_data_2d
{
    u32 ProgramID;
    
    u32 VertexBufferID;
    u32 TextureBufferID;
    
    s32 ViewportMatrixID;
    s32 Texture0ID;
    
    m4x4r32 ViewportMatrix;
};

#define UTIL_VOX_RENDER_H_
#endif