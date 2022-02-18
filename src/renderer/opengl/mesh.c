/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal void
Mesh_Bind(mesh *Mesh)
{
    OpenGL_BindVertexArray(Mesh->VAO);
    OpenGL_BindBuffer(GL_ARRAY_BUFFER, Mesh->VBO);
    OpenGL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->EBO);
    OpenGL_BindTexture(GL_TEXTURE_BUFFER, Mesh->TextureData);
    
    // OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->MPMatricesID);
    // OpenGL_BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, Mesh->MPMatricesID);
}

internal void
Mesh_Unbind(void)
{
    OpenGL_BindVertexArray(0);
    OpenGL_BindBuffer(GL_ARRAY_BUFFER, 0);
    OpenGL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    OpenGL_BindTexture(GL_TEXTURE_BUFFER, 0);
}

internal void
Mesh_Init(mesh *Mesh,
          heap *Heap,
          u32 Program,
          mesh_flags Flags)
{
    Assert(Mesh);
    
    Mesh->Program = Program;
    Mesh->Flags = Flags;
    
    Mesh->Vertices = Heap_Allocate(Heap, 0);
    Mesh->Indices = Heap_Allocate(Heap, 0);
    // Mesh->MMatrices = Heap_Allocate(Heap, 0);
    
    Mesh->ObjectCount = 0;
    Mesh->VertexOffsets = Heap_Allocate(Heap, sizeof(u32));
    Mesh->IndexOffsets = Heap_Allocate(Heap, sizeof(u32));
    ((u32*)Mesh->VertexOffsets->Data)[0] = 0;
    ((u32*)Mesh->IndexOffsets->Data)[0] = 0;
    
    OpenGL_GenVertexArrays(1, &Mesh->VAO);
    OpenGL_GenBuffers(2, &Mesh->VBO);
    // GL_GenBuffers(1, &Mesh->MPMatricesID);
    OpenGL_GenTextures(1, &Mesh->TextureData);
    
    // Vert
    // Mesh->AtlasCountID = GL_GetUniformLocation(Mesh->Program, "AtlasCount");
    
    // Frag
    Mesh->Sampler = OpenGL_GetUniformLocation(Mesh->Program, "Sampler");
    
    Mesh_Bind(Mesh);
    
    u32 Stride = sizeof(bfs32);
    u64 Offset = 0;
    
    if(Mesh->Flags & MESH_HAS_NORMALS)
        Stride += sizeof(bfs32);
    
    if(Mesh->Flags & MESH_HAS_TEXTURES)
        Stride += sizeof(u32);
    
    if(Mesh->Flags & MESH_HAS_COLORS)
        Stride += sizeof(v4u08);
    
    Mesh->VertexSize = Stride;
    
    OpenGL_EnableVertexAttribArray(0);
    OpenGL_VertexAttribPointer(0, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
    Offset += sizeof(bfs32);
    
    if(Mesh->Flags & MESH_HAS_NORMALS) {
        OpenGL_EnableVertexAttribArray(1);
        OpenGL_VertexAttribPointer(1, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(bfs32);
    }
    
    if(Mesh->Flags & MESH_HAS_TEXTURES) {
        OpenGL_EnableVertexAttribArray(2);
        OpenGL_VertexAttribPointer(2, 1, GL_UNSIGNED_INT, FALSE, Stride, (vptr)Offset);
        Offset += sizeof(u32);
    }
    
    if(Mesh->Flags & MESH_HAS_COLORS) {
        OpenGL_EnableVertexAttribArray(3);
        OpenGL_VertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(v4u08);
    }
    
    Assert(Offset == Stride);
}

internal void
Mesh_AddObjects(mesh *Mesh,
                u32 ObjectCount,
                mesh_object *Objects)
{
    Heap_Resize(Mesh->VertexOffsets, Mesh->VertexOffsets->Size + ObjectCount*sizeof(u32));
    Heap_Resize(Mesh->IndexOffsets, Mesh->IndexOffsets->Size + ObjectCount*sizeof(u32));
    
    u64 AddedVerticesSize=0, AddedIndicesSize=0;
    for(u32 I = 0; I < ObjectCount; I++) {
        AddedVerticesSize += Objects[I].Vertices->Size;
        AddedIndicesSize += Objects[I].Indices->Size;
    }
    Heap_Resize(Mesh->Vertices, Mesh->Vertices->Size + AddedVerticesSize);
    Heap_Resize(Mesh->Indices, Mesh->Indices->Size + AddedIndicesSize);
    
    u32 *VertexOffsets = (u32*)Mesh->VertexOffsets->Data + Mesh->ObjectCount;
    u32 *IndexOffsets = (u32*)Mesh->IndexOffsets->Data + Mesh->ObjectCount;
    for(u32 I = 0; I < ObjectCount; I++) {
        Mem_Cpy(Mesh->Vertices->Data+VertexOffsets[I]*Mesh->VertexSize, Objects[I].Vertices->Data, Objects[I].Vertices->Size);
        Mem_Cpy(Mesh->Indices->Data+IndexOffsets[I]*sizeof(u32), Objects[I].Indices->Data, Objects[I].Indices->Size);
        VertexOffsets[I+1] = VertexOffsets[I] + Objects[I].Vertices->Size / Mesh->VertexSize;
        IndexOffsets[I+1] = IndexOffsets[I] + Objects[I].Indices->Size / sizeof(u32);
    }
    
    Mesh->ObjectCount += ObjectCount;
    Mesh->Flags |= MESH_GROW_VERTEX_BUFFER|MESH_GROW_INDEX_BUFFER;
}

//TODO: Make this a per-object update for shrinking
internal void
Mesh_Update(mesh *Mesh)
{
    u32 DrawType;
    if(Mesh->Flags & MESH_IS_DYNAMIC)
        DrawType = GL_DYNAMIC_DRAW;
    else
        DrawType = GL_STATIC_DRAW;
    
    if(Mesh->Flags & MESH_GROW_VERTEX_BUFFER)
        OpenGL_BufferData(GL_ARRAY_BUFFER, Mesh->Vertices->Size, Mesh->Vertices->Data, DrawType);
    else
        OpenGL_BufferSubData(GL_ARRAY_BUFFER, 0, Mesh->Vertices->Size, Mesh->Vertices->Data);
    
    if(Mesh->Flags & MESH_GROW_INDEX_BUFFER)
        OpenGL_BufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh->Indices->Size, Mesh->Indices->Data, DrawType);
    else
        OpenGL_BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, Mesh->Indices->Size, Mesh->Indices->Data);
    
    // if(StorageBufferSize > Mesh->StorageBufferSize)
    //     GL_BufferData(GL_SHADER_STORAGE_BUFFER, StorageBufferSize, Mesh->MMatrices, DrawType);
    // else
    //     GL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, StorageBufferSize, Mesh->MMatrices);
}

internal void
Mesh_Draw(mesh *Mesh/*,
          m4x4r32 PMatrix,
          b08 UpdateMatrices*/)
{
    Stack_Push();
    
    // if(UpdateMatrices) {
    //     m4x4r32 *MPMatrices = Stack_Allocate(Mesh->ObjectCount * sizeof(m4x4r32));
    //     for(s32 ObjectIndex = 0; ObjectIndex < Mesh->ObjectCount; ++ObjectIndex)
    //         MPMatrices[ObjectIndex] = M4x4r32_Mul(PMatrix, Mesh->MMatrices[ObjectIndex]);
        
    //     GL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Mesh->ObjectCount * sizeof(m4x4r32), MPMatrices);
    // }
    
    OpenGL_UseProgram(Mesh->Program);
    
    vptr *IndexOffsets = Stack_Allocate(Mesh->ObjectCount * sizeof(vptr));
    for(u32 I = 0; I < Mesh->ObjectCount; ++I)
        IndexOffsets[I] = (vptr)(u64)(((u32*)Mesh->IndexOffsets->Data)[I] * sizeof(u32));
    
    OpenGL_MultiDrawElementsBaseVertex(GL_TRIANGLES, (s32*)Mesh->IndexOffsets->Data+1, GL_UNSIGNED_INT,
                                       IndexOffsets, Mesh->ObjectCount, (s32*)Mesh->VertexOffsets->Data);
    
    Stack_Pop();
}

internal void
Mesh_Free(mesh *Mesh)
{
    Heap_Free(Mesh->VertexOffsets);
    Heap_Free(Mesh->IndexOffsets);
    Heap_Free(Mesh->Indices);
    Heap_Free(Mesh->Vertices);
    // Heap_Free(Mesh->MMatrices);
}