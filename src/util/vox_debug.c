#include "util/vox_defines.h"
#include "util/vox_debug.h"
#include "util/vox_memory.h"

local_func void
InitDebugData(debug_data *DebugData,
              handle_pool *Pool)
{
    DebugData->Memory = Pool;
    DebugData->MeshData.AtlasIndex = 0;
    DebugData->MeshData.Vertices = AllocateMemory(Pool, 0);
    DebugData->MeshData.Textures = AllocateMemory(Pool, 0);
}

local_func void
CreateDebugString(c08 *String,
                  handle_pool *SearchPool,
                  asset_memory *AssetMemory,
                  render_data_2d *RenderData,
                  debug_data *DebugData,
                  u32 OffsetX,
                  u32 OffsetY)
{
#if 0
    r32 X = (r32)100;
    r32 Y = (r32)100;
    r32 W = (r32)600;
    r32 H = (r32)600;
    r32 U1 = (r32)0;
    r32 U2 = (r32)1;
    r32 V1 = (r32)0;
    r32 V2 = (r32)1;
    
    v2r32 Vertices[6] = {
        {X,     Y + H},
        {X,     Y    },
        {X + W, Y    },
        {X,     Y + H},
        {X + W, Y    },
        {X + W, Y + H}
    };
    v2r32 Textures[6] = {
        {U1, V2},
        {U1, V1},
        {U2, V1},
        {U1, V2},
        {U2, V1},
        {U2, V2}
    };
    
    DebugData->MeshData.Vertices = ResizeMemory(DebugData->MeshData.Vertices,
                                                DebugData->MeshData.Vertices->Size + sizeof(Vertices));
    CopyMemory((u08*)DebugData->MeshData.Vertices->Base + (DebugData->MeshData.Vertices->Size - sizeof(Vertices)),
               Vertices, sizeof(Vertices));
    
    DebugData->MeshData.Textures = ResizeMemory(DebugData->MeshData.Textures,
                                                DebugData->MeshData.Textures->Size + sizeof(Textures));
    CopyMemory((u08*)DebugData->MeshData.Textures->Base + (DebugData->MeshData.Textures->Size - sizeof(Textures)),
               Textures, sizeof(Textures));
#else
    u32 StrSize = PointerCount(String);
    for(u32 Index = 0;
        Index < StrSize;
        ++Index)
    {
        c08 Char = String[Index];
        
        memory_handle *Tags[2];
        
        Tags[0] = AllocateMemory(SearchPool, sizeof(voxai_tag));
        ((voxai_tag*)Tags[0]->Base)->ID = AssetTag_Codepoint;
        ((voxai_tag*)Tags[0]->Base)->Format = 0;
        ((voxai_tag*)Tags[0]->Base)->Value_S32 = (s32)Char;
        
        Tags[1] = AllocateMemory(SearchPool, sizeof(voxai_tag));
        ((voxai_tag*)Tags[1]->Base)->ID = AssetTag_IsMonospace;
        ((voxai_tag*)Tags[1]->Base)->Format = 0;
        ((voxai_tag*)Tags[1]->Base)->Value_S32 = (s32)1;
        
        voxai_bitmap *Data = GetVoxaiBitmap(AssetMemory->Info, TagSearch_Exact, 2, Tags);
        voxai_header *Header = GetVoxaiHeader(AssetMemory->Info);
        
        FreeMemory(Tags[0]);
        FreeMemory(Tags[1]);
        
        if(Data)
        {
            r32 X = (r32)OffsetX;
            r32 Y = (r32)OffsetY;
            r32 W = (r32)Data->Width;
            r32 H = (r32)Data->Height;
            r32 U1 = (r32)Data->OffsetX / (r32)Header->AtlasDim;
            r32 U2 = (r32)(Data->OffsetX + Data->Width) / (r32)Header->AtlasDim;
            r32 V1 = (r32)Data->OffsetY / (r32)Header->AtlasDim;
            r32 V2 = (r32)(Data->OffsetY + Data->Height) / (r32)Header->AtlasDim;
            
            v2r32 Vertices[6] = {
                {X,     Y + H},
                {X,     Y    },
                {X + W, Y    },
                {X,     Y + H},
                {X + W, Y    },
                {X + W, Y + H}
            };
            v2r32 Textures[6] = {
                {U1, V2},
                {U1, V1},
                {U2, V1},
                {U1, V2},
                {U2, V1},
                {U2, V2}
            };
            
            DebugData->MeshData.Vertices = ResizeMemory(DebugData->MeshData.Vertices,
                                                        DebugData->MeshData.Vertices->Size + sizeof(Vertices));
            CopyMemory((u08*)DebugData->MeshData.Vertices->Base + (DebugData->MeshData.Vertices->Size - sizeof(Vertices)),
                       Vertices, sizeof(Vertices));
            
            DebugData->MeshData.Textures = ResizeMemory(DebugData->MeshData.Textures,
                                                        DebugData->MeshData.Textures->Size + sizeof(Textures));
            CopyMemory((u08*)DebugData->MeshData.Textures->Base + (DebugData->MeshData.Textures->Size - sizeof(Textures)),
                       Textures, sizeof(Textures));
            
            OffsetX += Data->Width;
        }
    }
#endif
}

#if 0
    glUseProgram(RenderData->ProgramID);
    glUniformMatrix4fv(RenderData->ViewportMatrixID, 1, FALSE, &RenderData->ViewportMatrix.E[0]);
    glActiveTexture(GL_TEXTURE0);
    u32 StrSize = PointerCount(String);
    for(u32 Index = 0;
        Index < StrSize;
        ++Index)
    {
        c08 Char = String[Index];
        
        memory_handle *Tags[2];
        
        Tags[0] = AllocateMemory(SearchPool, sizeof(voxai_tag));
        ((voxai_tag*)Tags[0]->Base)->ID = AssetTag_Codepoint;
        ((voxai_tag*)Tags[0]->Base)->Format = 0;
        ((voxai_tag*)Tags[0]->Base)->Value_S32 = (s32)Char;
        
        Tags[1] = AllocateMemory(SearchPool, sizeof(voxai_tag));
        ((voxai_tag*)Tags[1]->Base)->ID = AssetTag_IsMonospace;
        ((voxai_tag*)Tags[1]->Base)->Format = 0;
        ((voxai_tag*)Tags[1]->Base)->Value_S32 = (s32)1;
        
        voxai_bitmap *Data = GetVoxaiBitmap(AssetMemory->Info, TagSearch_Exact, 2, Tags);
        voxai_header *Header = GetVoxaiHeader(AssetMemory->Info);
        
        FreeMemory(Tags[0]);
        FreeMemory(Tags[1]);
        
        r32 X = (r32)OffsetX;
        r32 Y = (r32)OffsetY;
        r32 W = (r32)Data->Width;
        r32 H = (r32)Data->Height;
        r32 U1 = (r32)Data->OffsetX / (r32)Header->AtlasDim;
        r32 U2 = (r32)(Data->OffsetX + Data->Width) / (r32)Header->AtlasDim;
        r32 V1 = (r32)Data->OffsetY / (r32)Header->AtlasDim;
        r32 V2 = (r32)(Data->OffsetY + Data->Height) / (r32)Header->AtlasDim;
        
        r32 Vertices[6][4] = {
            {X,     Y + H, U1, V2},
            {X,     Y,     U1, V1},
            {X + W, Y,     U2, V1},
            {X,     Y + H, U1, V2},
            {X + W, Y,     U2, V1},
            {X + W, Y + H, U2, V2}
        };
        
        glEnableVertexAttribArray(0);
        glBindTexture(GL_TEXTURE_2D, AssetMemory->Atlas0ID);
        glBindBuffer(GL_ARRAY_BUFFER, RenderData->VertexBufferID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * 4 * sizeof(r32), Vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);
        OffsetX += 30;
    }
}
#endif