/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
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


internal bfs32_1x2_3x10
Mesh_MakePosCoord(v3r32 Coords)
{
    bfs32 Result;
    
    // [-1.0f, 1.0f]
    // [-511.0f, 511.0f]
    // [-511, 511]
    
    //TODO: Do something better than clamping
    Coords = V3r32_Clamp(V3r32_1x1(-1.0f), Coords, V3r32_1x1(1.0f));
    v4r32 Scaled = V4r32_1x3_1x1(V3r32_Mul_V_S(Coords, MESH_POS_MAX), 0.0f);
    // Result = V4r32_To_Bfs32_Round(SWIZZLE(Scaled, SWIZZLE_WZYX), BFF_1X2_3X10);
    
    v4r32 Swizzled = {Scaled.W, Scaled.Z, Scaled.Y, Scaled.X};
    Result = V4r32_RoundTo_Bfs32(Swizzled, BFF_1X2_3X10);
    
    return Result;
}

internal v3u16
Mesh_MakeTexCoord(v3r32 Bounds,
                  v3r32 Coords)
{
    v3u16 Result;
    v3r32 Ratio = V3r32_Div(Coords, Bounds);
    v3r32 Scaled = V3r32_Mul_V_S(Ratio, U16_MAX);
    
    Result = V3r32_RoundTo_V3u16(Scaled);
    
    return Result;
}

internal void
Mesh_Init(heap *Heap,
          mesh *Mesh,
          flag08 Flags)
{
    ASSERT(Mesh);
    
    Mesh->Flags = Flags;
    
    Mesh->Vertices = Heap_Allocate(Heap, 0);
    Mesh->Indices = Heap_Allocate(Heap, 0);
    Mesh->MMatrices = Heap_Allocate(Heap, 0);
    
    Mesh->ObjectCount = 0;
    Mesh->VertexCounts = Heap_Allocate(Heap, sizeof(s32)); // To make BaseVertices easier
    Mesh->IndexCounts = Heap_Allocate(Heap, 0);
    
    GL_GenVertexArrays(1, &Mesh->VAOID);
    GL_GenBuffers(1, &Mesh->VBOID);
    GL_GenBuffers(1, &Mesh->EBOID);
    GL_GenBuffers(1, &Mesh->MPMatricesID);
    
    // Vert
    Mesh->AtlasCountID = GL_GetUniformLocation(Mesh->ProgramID, "AtlasCount");
    
    // Frag
    Mesh->SamplerID = GL_GetUniformLocation(Mesh->ProgramID, "Sampler");
    
    Mesh_Bind(Mesh);
    
    u32 Stride = sizeof(bfs32);
    u64 Offset = 0;
    
    if(FLAG_TEST(Flags, MESH_HAS_NORMALS))
    {
        Stride += sizeof(bfs32);
    }
    
    if(FLAG_TEST(Flags, MESH_HAS_TEXTURES))
    {
        Stride += sizeof(v4u16);
    }
    
    if(FLAG_TEST(Flags, MESH_HAS_COLORS))
    {
        Stride += sizeof(v4u08);
    }
    
    Mesh->VertexSize = (u08)Stride;
    
    GL_EnableVertexAttribArray(0);
    GL_VertexAttribPointer(0, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
    Offset += sizeof(bfs32);
    
    if(FLAG_TEST(Flags, MESH_HAS_NORMALS))
    {
        GL_EnableVertexAttribArray(1);
        GL_VertexAttribPointer(1, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(bfs32);
    }
    
    if(FLAG_TEST(Flags, MESH_HAS_TEXTURES))
    {
        GL_EnableVertexAttribArray(2);
        GL_VertexAttribPointer(2, 4, GL_UNSIGNED_SHORT, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(v4u16);
    }
    
    if(FLAG_TEST(Flags, MESH_HAS_COLORS))
    {
        GL_EnableVertexAttribArray(3);
        GL_VertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(v4u08);
    }
    
    ASSERT(Offset == Stride);
}

internal void
Mesh_Bind(mesh *Mesh)
{
    GL_BindVertexArray(Mesh->VAOID);
    GL_BindBuffer(GL_ARRAY_BUFFER, Mesh->VBOID);
    GL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->EBOID);
    
    GL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->MPMatricesID);
    GL_BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, Mesh->MPMatricesID);
}

internal void
Mesh_Unbind(void)
{
    GL_BindVertexArray(0);
    GL_BindBuffer(GL_ARRAY_BUFFER, 0);
    GL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

internal void
Mesh_AddObjects(mesh *Mesh,
                u32 ObjectCount,
                mesh_object *Objects)
{
    ASSERT(Mesh->ObjectCount + ObjectCount <= S32_MAX);
    
    s32 PrevObjectCount = Mesh->ObjectCount;
    u32 PrevTotalVertexCount = Mesh->TotalVertexCount;
    u32 PrevTotalIndexCount = Mesh->TotalIndexCount;
    
    Mesh->ObjectCount += ObjectCount;
    Heap_Resize(&Mesh->VertexCounts, (Mesh->ObjectCount + 1) * sizeof(s32));
    Heap_Resize(&Mesh->IndexCounts, Mesh->ObjectCount * sizeof(s32));
    Heap_Resize(&Mesh->MMatrices, Mesh->ObjectCount * sizeof(m4x4r32));
    
    mesh_object *AddedObjectCursor = Objects;
    for(u32 ObjectIndex = 0;
        ObjectIndex < ObjectCount;
        ++ObjectIndex)
    {
        Mesh->TotalVertexCount += AddedObjectCursor->VertexCount;
        Mesh->TotalIndexCount += AddedObjectCursor->IndexCount;
        Mesh->VertexCounts[PrevObjectCount + ObjectIndex + 1] = AddedObjectCursor->VertexCount;
        Mesh->IndexCounts[PrevObjectCount + ObjectIndex] = AddedObjectCursor->IndexCount;
        Mesh->MMatrices[PrevObjectCount + ObjectIndex] = AddedObjectCursor->ModelMatrix;
        
        AddedObjectCursor++;
    }
    Heap_Resize(&Mesh->Vertices, Mesh->TotalVertexCount * Mesh->VertexSize);
    Heap_Resize(&Mesh->Indices, Mesh->TotalIndexCount * sizeof(s32));
    
    mem VerticesCursor = Mesh->Vertices + (PrevTotalVertexCount * Mesh->VertexSize);
    mem IndicesCursor = Mesh->Indices + (PrevTotalIndexCount * sizeof(u32));
    for(u32 ObjectIndex = 0;
        ObjectIndex < ObjectCount;
        ++ObjectIndex)
    {
        mesh_object *Object = Objects + ObjectIndex;
        
        Mem_Cpy(VerticesCursor, Object->Vertices, Object->VertexCount * Mesh->VertexSize);
        Mem_Cpy(IndicesCursor, Object->Indices, Object->IndexCount * sizeof(u32));
        
        Heap_Free(Object->Vertices);
        Heap_Free(Object->Indices);
        
        VerticesCursor += (Object->VertexCount * Mesh->VertexSize);
        IndicesCursor += (Object->IndexCount * sizeof(u32));
        ++Object;
    }
}

internal void
Mesh_Finalize(mesh *Mesh)
{
    u32 DrawType;
    if(FLAG_TEST(Mesh->Flags, MESH_IS_DYNAMIC))
    {
        DrawType = GL_DYNAMIC_DRAW;
    }
    else
    {
        DrawType = GL_STATIC_DRAW;
    }
    
    Mesh->VertexBufferSize = Mesh->TotalVertexCount * Mesh->VertexSize;
    Mesh->IndexBufferSize = Mesh->TotalIndexCount * sizeof(u32);
    Mesh->StorageBufferSize = Mesh->ObjectCount * sizeof(m4x4r32);
    
    GL_BufferData(GL_ARRAY_BUFFER, Mesh->VertexBufferSize, Mesh->Vertices, DrawType);
    //NOTE: Index buffer won't change, but it will still need to be resized
    GL_BufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh->IndexBufferSize, Mesh->Indices, DrawType);
    GL_BufferData(GL_SHADER_STORAGE_BUFFER, Mesh->StorageBufferSize, NULL, DrawType);
}

internal void
Mesh_Update(mesh *Mesh)
{
    u32 DrawType;
    if(FLAG_TEST(Mesh->Flags, MESH_IS_DYNAMIC))
    {
        DrawType = GL_DYNAMIC_DRAW;
    }
    else
    {
        DrawType = GL_STATIC_DRAW;
    }
    
    u64 VertexBufferSize = Mesh->TotalVertexCount * Mesh->VertexSize;
    u64 IndexBufferSize = Mesh->TotalIndexCount * sizeof(u32);
    u64 StorageBufferSize = Mesh->ObjectCount * sizeof(m4x4r32);
    
    if(VertexBufferSize > Mesh->VertexBufferSize)
        GL_BufferData(GL_ARRAY_BUFFER, VertexBufferSize, Mesh->Vertices, DrawType);
    else
        GL_BufferSubData(GL_ARRAY_BUFFER, 0, VertexBufferSize, Mesh->Vertices);
    
    if(IndexBufferSize > Mesh->IndexBufferSize)
        GL_BufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, Mesh->Indices, DrawType);
    else
        GL_BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, IndexBufferSize, Mesh->Indices);
    
    if(StorageBufferSize > Mesh->StorageBufferSize)
        GL_BufferData(GL_SHADER_STORAGE_BUFFER, StorageBufferSize, Mesh->MMatrices, DrawType);
    else
        GL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, StorageBufferSize, Mesh->MMatrices);
}

internal void
Mesh_Render(mesh *Mesh,
            m4x4r32 PMatrix,
            b08 UpdateMatrices)
{
    Stack_Push();
    
    if(UpdateMatrices)
    {
        m4x4r32 *MPMatrices = Stack_Allocate(Mesh->ObjectCount * sizeof(m4x4r32));
        for(s32 ObjectIndex = 0;
            ObjectIndex < Mesh->ObjectCount;
            ++ObjectIndex)
        {
            MPMatrices[ObjectIndex] = M4x4r32_Mul(PMatrix, Mesh->MMatrices[ObjectIndex]);
        }
        GL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Mesh->ObjectCount * sizeof(m4x4r32), MPMatrices);
    }
    
    //TODO: Consider sending the block array over to the GPU, and having a
    //      compute shader calculate the vertices and directly copy those to
    //      GPU memory, so it won't have to be stored on CPU memory
    vptr *IndexOffsets = Stack_Allocate(Mesh->ObjectCount * sizeof(vptr));
    Mem_Zero(IndexOffsets, Mesh->ObjectCount * sizeof(vptr));
    
    GL_MultiDrawElementsBaseVertex(GL_TRIANGLES, Mesh->IndexCounts, GL_UNSIGNED_INT,
                                   IndexOffsets, Mesh->ObjectCount, Mesh->VertexCounts);
    
    Stack_Pop();
}

internal void
Mesh_Free(mesh *Mesh)
{
    Heap_Free(Mesh->IndexCounts);
    Heap_Free(Mesh->VertexCounts);
    Heap_Free(Mesh->Indices);
    Heap_Free(Mesh->Vertices);
    Heap_Free(Mesh->MMatrices);
}

//NOTE: Only call GL_BufferData if the data has changed
//NOTE: Split up VBOs into hot/cold types
//NOTE: Indices should be cold, vertices should be hot
//TODO: Hot-reloading shaders

#if 0
internal void
Render2DMesh(render_data_2d *RenderData,
             asset_memory *AssetMemory,
             mesh_data_2d *MeshData)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, AssetMemory->Atlas0ID);
    glUseProgram(RenderData->ProgramID);
    glDrawArrays(GL_TRIANGLES, 0, (s32)(MeshData->Vertices->Size / MESH2D_STRIDE));
    
    //TODO: Move this out of game loop, if possible
    // glUseProgram(RenderData->ProgramID);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, RenderData->Texture0ID);
    
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, RenderData->VertexBufferID);
    // glBufferData(GL_ARRAY_BUFFER, MeshData->Vertices->Size, MeshData->Vertices->Base, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 2, GL_FLOAT, FALSE, 0, 0);
    
    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, RenderData->TextureBufferID);
    // glBufferData(GL_ARRAY_BUFFER, MeshData->Textures->Size, MeshData->Textures->Base, GL_STATIC_DRAW);
    // glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, 0, 0);
    
    // glDrawArrays(GL_TRIANGLES, 0, (s32)(MeshData->Vertices->Size / sizeof(v2r32)));
    
    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);
}

internal void
Render3DMesh(render_data_3d *RenderData,
             mesh_data_3d *MeshData)
{
    glUniformMatrix4fv(RenderData->ModelMatrixID, 1, FALSE, &MeshData->ModelMatrix.E[0]);
    m4x4r32 MVPMatrix = M4x4r32_Mul_M4x4r32(RenderData->ProjectionMatrix,
                        M4x4r32_Mul_M4x4r32(RenderData->ViewMatrix,
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

internal void
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
            if(R32_Abs((VertexData + Index)->X - (VertexData + PrevIndex)->X) < R32_EPSILON &&
               R32_Abs((VertexData + Index)->Y - (VertexData + PrevIndex)->Y) < R32_EPSILON &&
               R32_Abs((VertexData + Index)->Z - (VertexData + PrevIndex)->Z) < R32_EPSILON &&
               R32_Abs((ColorData + Index)->X - (ColorData + PrevIndex)->X) < R32_EPSILON &&
               R32_Abs((ColorData + Index)->Y - (ColorData + PrevIndex)->Y) < R32_EPSILON &&
               R32_Abs((ColorData + Index)->Z - (ColorData + PrevIndex)->Z) < R32_EPSILON &&
               R32_Abs((ColorData + Index)->W - (ColorData + PrevIndex)->W) < R32_EPSILON)
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
    Vertices = ResizeHandle(Vertices, OutSize * sizeof(v3r32));
    Colors = ResizeHandle(Colors, OutSize * sizeof(v4r32));
}
#endif