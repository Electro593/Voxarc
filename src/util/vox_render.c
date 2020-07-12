#include "util/vox_defines.h"
#include "util/vox_render.h"
#include "math/vox_v3u32.h"
#include "game/vox_world.h"

/* Cube meshData setup
    Vertices:
        -1, -1, -1
        -1, -1,  1
        -1,  1, -1
        -1,  1,  1
         1, -1, -1
         1, -1,  1
         1,  1, -1
         1,  1,  1
    
    Normals:
        -1,  0,  0,
         0, -1,  0,
         0,  0, -1,
         1,  0,  0,
         0,  1,  0,
         0,  0,  1,
    
    UVs:
        ( 0.0+0.5)/96.0, (32.0+0.5)/96.0,
        ( 0.0+0.5)/96.0, (63.0+0.5)/96.0,
        (31.0+0.5)/96.0, (32.0+0.5)/96.0,
        (31.0+0.5)/96.0, (63.0+0.5)/96.0,
        (32.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (32.0+0.5)/96.0, (31.0+0.5)/96.0,
        (32.0+0.5)/96.0, (32.0+0.5)/96.0,
        (32.0+0.5)/96.0, (63.0+0.5)/96.0,
        (32.0+0.5)/96.0, (64.0+0.5)/96.0,
        (32.0+0.5)/96.0, (95.0+0.5)/96.0,
        (63.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (63.0+0.5)/96.0, (31.0+0.5)/96.0,
        (63.0+0.5)/96.0, (32.0+0.5)/96.0,
        (63.0+0.5)/96.0, (63.0+0.5)/96.0,
        (63.0+0.5)/96.0, (64.0+0.5)/96.0,
        (63.0+0.5)/96.0, (95.0+0.5)/96.0,
        (64.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (64.0+0.5)/96.0, (31.0+0.5)/96.0,
        (64.0+0.5)/96.0, (32.0+0.5)/96.0,
        (64.0+0.5)/96.0, (63.0+0.5)/96.0,
        (95.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (95.0+0.5)/96.0, (31.0+0.5)/96.0,
        (95.0+0.5)/96.0, (32.0+0.5)/96.0,
        (95.0+0.5)/96.0, (63.0+0.5)/96.0
    
    Faces:
        Right
        6, 3, 23,  7, 3, 19,  5, 3, 18,
        6, 3, 23,  5, 3, 18,  4, 3, 22,
        Top
        6, 4, 15,  2, 4, 9,  3, 4, 8,
        6, 4, 15,  3, 4, 8,  7, 4, 14,
        Front
        1, 5, 6,  5, 5, 12,  7, 5, 13,
        1, 5, 6,  7, 5, 13,  3, 5, 7,
        Left
        1, 0, 2,  3, 0, 3,  2, 0, 1,
        1, 0, 2,  2, 0, 1,  0, 0, 0,
        Bottom
        1, 1, 5,  0, 1, 4,  4, 1, 10,
        1, 1, 5,  4, 1, 10,  5, 1, 11,
        Back
        6, 2, 17,  4, 2, 16,  0, 2, 20,
        6, 2, 17,  0, 2, 20,  2, 2, 21
*/

local_func void
Render3DMesh(render_data_3d *RenderData,
             mesh_data_3d *MeshData)
{
    glUniformMatrix4fv(RenderData->ModelMatrixID, 1, FALSE, &MeshData->ModelMatrix.E[0]);
    m4x4r32 MVPMatrix = M4x4r32_Multiply(RenderData->ProjectionMatrix,
                                         M4x4r32_Multiply(RenderData->ViewMatrix,
                                                          MeshData->ModelMatrix));
    glUniformMatrix4fv(RenderData->MVPMatrixID, 1, FALSE, &MVPMatrix.E[0]);
    
    //TODO: Try moving VertexAttribPointer to CreateMesh
    //TODO: Make colors a series of unsigned bytes
    //TODO: Store normals as INT_2_10_10_10_REV (store something in 2 bit segment?)
    //TODO: Mode glBufferData to CreateMesh, it only changes after a chunk update
    //TODO: Interleave or pack the VBOs?
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, RenderData->VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, MeshData->Vertices->Size, MeshData->Vertices->Base, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, 0);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, RenderData->ColorBufferID);
    glBufferData(GL_ARRAY_BUFFER, MeshData->Colors->Size, MeshData->Colors->Base, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData->ElementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshData->Indices->Size, MeshData->Indices->Base, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, (s32)(MeshData->Indices->Size / sizeof(u16)),
                   GL_UNSIGNED_SHORT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(Mesh.Data.Vertices->Size / sizeof(v3r)));
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

local_func void
Index3DVertices(memory_handle *Vertices,
                memory_handle *Colors,
                memory_handle *Indices)
{
    u32 IndicesSize = 0;
    u16 OutSize = 0;
    u32 Size = (u32)(Indices->Size / sizeof(u16));
    
    for(u16 Index = 0;
        Index < Size;
        ++Index)
    {
        b08 Found = FALSE;
        u16 PrevIndex = 0;
        v3r32 *VertexData = (v3r32*)Vertices->Base;
        v4r32 *ColorData = (v4r32*)Colors->Base;
        for(;
            PrevIndex < OutSize;
            ++PrevIndex)
        {
            if(R32_Abs((VertexData + Index)->X - (VertexData + PrevIndex)->X) < EPSILON32 &&
               R32_Abs((VertexData + Index)->Y - (VertexData + PrevIndex)->Y) < EPSILON32 &&
               R32_Abs((VertexData + Index)->Z - (VertexData + PrevIndex)->Z) < EPSILON32 &&
               R32_Abs((ColorData + Index)->X - (ColorData + PrevIndex)->X) < EPSILON32 &&
               R32_Abs((ColorData + Index)->Y - (ColorData + PrevIndex)->Y) < EPSILON32 &&
               R32_Abs((ColorData + Index)->Z - (ColorData + PrevIndex)->Z) < EPSILON32 &&
               R32_Abs((ColorData + Index)->W - (ColorData + PrevIndex)->W) < EPSILON32)
            {
                Found = TRUE;
                break;
            }
        }
        
        u16 *Word = (u16*)Indices->Base + IndicesSize++;
        if(Found)
        {
            *Word = PrevIndex;
        }
        else
        {
            VertexData += OutSize;
            *VertexData = *((v3r32*)Vertices->Base + Index);
            ColorData += OutSize;
            *ColorData = *((v4r32*)Colors->Base + Index);
            *Word = OutSize++;
        }
    }
    Vertices = ResizeMemory(Vertices, OutSize * sizeof(v3r32));
    Colors = ResizeMemory(Colors, OutSize * sizeof(v4r32));
}

local_func void
Render2DMesh(render_data_2d *RenderData,
             mesh_data_2d *MeshData)
{
    //TODO: Move this out of game loop, if possible
    glActiveTexture(GL_TEXTURE0 + MeshData->AtlasIndex);
    glBindTexture(GL_TEXTURE_2D, RenderData->Texture0ID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, RenderData->VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, MeshData->Vertices->Size, MeshData->Vertices->Base, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, FALSE, 0, 0);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, RenderData->TextureBufferID);
    glBufferData(GL_ARRAY_BUFFER, MeshData->Textures->Size, MeshData->Textures->Base, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, (s32)(MeshData->Vertices->Size / sizeof(v2r32)));
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}