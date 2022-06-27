/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

internal u32
Mesh_EncodePosition(v3r32 P)
{
    P = V3r32_Clamp(P, -1, 1);
    P = V3r32_MulS(P, 511);
    v3s32 I = V3r32_ToV3s32(P);
    u32 E = (1<<30)|((I.Z&0x3FF)<<20)|((I.Y&0x3FF)<<10)|(I.X&0x3FF);
    return E;
}

internal void
Mesh_Bind(mesh *Mesh)
{
    OpenGL_UseProgram(*Mesh->Program);
    
    OpenGL_BindVertexArray(Mesh->VAO);
    OpenGL_BindBuffer(GL_ARRAY_BUFFER, Mesh->VBO);
    OpenGL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->EBO);
    OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->MatrixSSBO);
    OpenGL_BindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Mesh->MatrixSSBO);
    
    // if(!(Mesh->Flags & MESH_SHARED_TEXTURE_BUFFER)) {
        OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->TextureSSBO);
        OpenGL_BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, Mesh->TextureSSBO);
    // }
    
    if(Mesh->Flags & MESH_HAS_TEXTURES) {
        OpenGL_BindTexture(GL_TEXTURE_2D_ARRAY, Mesh->Atlases);
        OpenGL_BindSampler(Mesh->TextureIndex, Mesh->SamplerObject);
    }
}

internal void
Mesh_Unbind(mesh *Mesh)
{
    OpenGL_BindVertexArray(0);
    OpenGL_BindBuffer(GL_ARRAY_BUFFER, 0);
    OpenGL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    
    if(Mesh->Flags & MESH_HAS_TEXTURES) {
        OpenGL_BindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
}

internal void
Mesh_Init(mesh *Mesh,
          heap *Heap,
          u32 *Program,
          mesh_flags Flags)
{
    Assert(Mesh);
    
    Assert(Program);
    Mesh->Program = Program;
    
    Mesh->Flags = Flags;
    Mesh->ObjectCount = 0;
    Mesh->Vertices = Heap_Allocate(Heap, 0);
    Mesh->Indices = Heap_Allocate(Heap, 0);
    Mesh->Matrices = Heap_Allocate(Heap, 0);
    
    if(!(Flags & MESH_SHARED_TEXTURE_BUFFER))
        Mesh->Storage = Heap_Allocate(Heap, 0);
    
    Mesh->VertexOffsets = Heap_Allocate(Heap, sizeof(u32));
    ((u32*)Mesh->VertexOffsets->Data)[0] = 0;
    Mesh->IndexOffsets = Heap_Allocate(Heap, sizeof(u32));
    ((u32*)Mesh->IndexOffsets->Data)[0] = 0;
    
    OpenGL_GenVertexArrays(1, &Mesh->VAO);
    OpenGL_GenBuffers(3, &Mesh->VBO); // VBO, EBO, MatrixSSBO
    
    if(Flags & MESH_HAS_TEXTURES) {
        OpenGL_ActiveTexture(GL_TEXTURE0 + Mesh->TextureIndex);
        OpenGL_GenSamplers(1, &Mesh->SamplerObject);
        
        if(!(Flags & MESH_SHARED_TEXTURE_BUFFER)) {
            OpenGL_GenBuffers(1, &Mesh->TextureSSBO);
            OpenGL_GenTextures(1, &Mesh->Atlases);
        }
    }
    
    Mesh_Bind(Mesh);
    
    u64 Offset = 0;
    u32 Stride = sizeof(u32);
    if(Mesh->Flags & MESH_HAS_NORMALS)
        Stride += sizeof(u32);
    if(Mesh->Flags & MESH_HAS_TEXTURES)
        Stride += sizeof(u32);
    if(Mesh->Flags & MESH_HAS_COLORS)
        Stride += sizeof(v4u08);
    Mesh->VertexSize = Stride;
    
    OpenGL_EnableVertexAttribArray(0);
    if(Mesh->Flags & MESH_IS_FOR_UI) {
        OpenGL_VertexAttribPointer(0, 2, GL_SHORT, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(u32);
    } else {
        OpenGL_VertexAttribPointer(0, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(u32);
    }
    
    if(Mesh->Flags & MESH_HAS_NORMALS) {
        OpenGL_EnableVertexAttribArray(1);
        OpenGL_VertexAttribPointer(1, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
        Offset += sizeof(u32);
    }
    
    if(Mesh->Flags & MESH_HAS_TEXTURES) {
        OpenGL_EnableVertexAttribArray(2);
        OpenGL_VertexAttribIPointer(2, 1, GL_UNSIGNED_INT, Stride, (vptr)Offset);
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
                mesh_object **Objects)
{
    Heap_Resize(Mesh->VertexOffsets, Mesh->VertexOffsets->Size + ObjectCount*sizeof(u32));
    Heap_Resize(Mesh->IndexOffsets,  Mesh->IndexOffsets->Size  + ObjectCount*sizeof(u32));
    Heap_Resize(Mesh->Matrices,      Mesh->Matrices->Size      + ObjectCount*sizeof(m4x4r32));
    
    u64 AddedVerticesSize=0, AddedIndicesSize=0;
    for(u32 I = 0; I < ObjectCount; I++) {
        AddedVerticesSize += (*Objects[I]).Vertices->Size;
        AddedIndicesSize += (*Objects[I]).Indices->Size;
    }
    Heap_Resize(Mesh->Vertices, Mesh->Vertices->Size + AddedVerticesSize);
    Heap_Resize(Mesh->Indices, Mesh->Indices->Size + AddedIndicesSize);
    
    u32 *VertexOffsets = (u32*)Mesh->VertexOffsets->Data + Mesh->ObjectCount;
    u32 *IndexOffsets = (u32*)Mesh->IndexOffsets->Data + Mesh->ObjectCount;
    for(u32 I = 0; I < ObjectCount; I++) {
        Mem_Cpy((u08*)Mesh->Vertices->Data+VertexOffsets[I]*Mesh->VertexSize, (*Objects[I]).Vertices->Data, (*Objects[I]).Vertices->Size);
        Mem_Cpy((u08*)Mesh->Indices->Data+IndexOffsets[I]*sizeof(u32), (*Objects[I]).Indices->Data, (*Objects[I]).Indices->Size);
        m4x4r32 ModelMatrix = M4x4r32_Mul(M4x4r32_Mul((*Objects[I]).TranslationMatrix, (*Objects[I]).RotationMatrix), (*Objects[I]).ScalingMatrix);
        ((m4x4r32*)Mesh->Matrices->Data)[Mesh->ObjectCount+I] = ModelMatrix;
        VertexOffsets[I+1] = VertexOffsets[I] + (*Objects[I]).Vertices->Size / Mesh->VertexSize;
        IndexOffsets[I+1] = IndexOffsets[I] + (*Objects[I]).Indices->Size / sizeof(u32);
        (*Objects[I]).Index = Mesh->ObjectCount + I;
    }
    
    Mesh->ObjectCount += ObjectCount;
    Mesh->Flags |= MESH_GROW_VERTEX_BUFFER|MESH_GROW_INDEX_BUFFER|MESH_GROW_MATRIX_BUFFER;
}

internal void
Mesh_UpdateVertices(mesh *Mesh,
                    mesh_object *Object)
{
    u32 ObjectIndex = Object->Index;
    u32 CurrOffset = ((u32*)Mesh->VertexOffsets->Data)[ObjectIndex]*Mesh->VertexSize;
    u32 NextOffset = ((u32*)Mesh->VertexOffsets->Data)[ObjectIndex+1]*Mesh->VertexSize;
    u32 MaxOffset  = ((u32*)Mesh->VertexOffsets->Data)[Mesh->ObjectCount]*Mesh->VertexSize;
    u32 OldSize = NextOffset - CurrOffset;
    u32 NewSize = Object->Vertices->Size;
    s32 DeltaSize = NewSize - OldSize;
    if(DeltaSize > 0) {
        u08 *PrevData = Mesh->Vertices->Data;
        heap *Heap = Heap_GetHeap(Mesh->Vertices);
        Heap_FreeBlock(Heap, Mesh->Vertices);
        Heap_AllocateBlock(Heap, Mesh->Vertices, Mesh->Vertices->Size+DeltaSize);
        Mem_Cpy(Mesh->Vertices->Data, PrevData, CurrOffset);
        Mem_Cpy((u08*)Mesh->Vertices->Data+CurrOffset+NewSize, PrevData+NextOffset, MaxOffset-NextOffset);
    } else if(DeltaSize < 0) {
        Mem_Cpy((u08*)Mesh->Vertices->Data+CurrOffset+NewSize, (u08*)Mesh->Vertices->Data+NextOffset, MaxOffset-NextOffset);
        Heap_Resize(Mesh->Vertices, Mesh->Vertices->Size+DeltaSize);
    }
    Mem_Cpy((u08*)Mesh->Vertices->Data+CurrOffset, Object->Vertices->Data, NewSize);
    if(DeltaSize != 0) {
        for(u32 I = ObjectIndex+1; I <= Mesh->ObjectCount; I++)
            ((u32*)Mesh->VertexOffsets->Data)[I] += DeltaSize/Mesh->VertexSize;
        OpenGL_BufferData(GL_ARRAY_BUFFER, Mesh->Vertices->Size, Mesh->Vertices->Data,
                          (Mesh->Flags & MESH_IS_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    } else
        OpenGL_BufferSubData(GL_ARRAY_BUFFER, CurrOffset, NewSize, Object->Vertices->Data);
}

internal void
Mesh_UpdateIndices(mesh *Mesh,
                   mesh_object *Object)
{
    u32 ObjectIndex = Object->Index;
    u32 CurrOffset = ((u32*)Mesh->IndexOffsets->Data)[ObjectIndex]*sizeof(u32);
    u32 NextOffset = ((u32*)Mesh->IndexOffsets->Data)[ObjectIndex+1]*sizeof(u32);
    u32 MaxOffset  = ((u32*)Mesh->IndexOffsets->Data)[Mesh->ObjectCount]*sizeof(u32);
    u32 OldSize = NextOffset - CurrOffset;
    u32 NewSize = Object->Indices->Size;
    s32 DeltaSize = NewSize - OldSize;
    if(DeltaSize > 0) {
        u08 *PrevData = Mesh->Indices->Data;
        heap *Heap = Heap_GetHeap(Mesh->Indices);
        Heap_FreeBlock(Heap, Mesh->Indices);
        Heap_AllocateBlock(Heap, Mesh->Indices, Mesh->Indices->Size+DeltaSize);
        Mem_Cpy(Mesh->Indices->Data, PrevData, CurrOffset);
        Mem_Cpy((u08*)Mesh->Indices->Data+CurrOffset+NewSize, PrevData+NextOffset, MaxOffset-NextOffset);
    } else if(DeltaSize < 0) {
        Mem_Cpy((u08*)Mesh->Indices->Data+CurrOffset+NewSize, (u08*)Mesh->Indices->Data+NextOffset, MaxOffset-NextOffset);
        Heap_Resize(Mesh->Indices, Mesh->Indices->Size+DeltaSize);
    }
    Mem_Cpy((u08*)Mesh->Indices->Data+CurrOffset, Object->Indices->Data, NewSize);
    OpenGL_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->EBO);
    if(DeltaSize != 0) {
        for(u32 I = ObjectIndex+1; I <= Mesh->ObjectCount; I++)
            ((u32*)Mesh->IndexOffsets->Data)[I] += DeltaSize/sizeof(u32);
        OpenGL_BufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh->Indices->Size, Mesh->Indices->Data,
                          (Mesh->Flags & MESH_IS_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    } else
        OpenGL_BufferSubData(GL_ELEMENT_ARRAY_BUFFER, CurrOffset, NewSize, Object->Indices->Data);
}

internal void
Mesh_UpdateMatrix(mesh *Mesh,
                  m4x4r32 Matrix,
                  mesh_object Object)
{
    ((m4x4r32*)Mesh->Matrices->Data)[Object.Index] = Matrix;
    OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->MatrixSSBO);
    OpenGL_BufferSubData(GL_SHADER_STORAGE_BUFFER, Object.Index*sizeof(m4x4r32), sizeof(m4x4r32), Matrix.E);
}

//TODO: Make this a per-object update for shrinking
internal void
Mesh_Update(mesh *Mesh)
{
    Mesh_Bind(Mesh);
    
    u32 DrawType;
    if(Mesh->Flags & MESH_IS_DYNAMIC)
        DrawType = GL_DYNAMIC_DRAW;
    else
        DrawType = GL_STATIC_DRAW;
    
    if(Mesh->Flags & MESH_GROW_VERTEX_BUFFER) {
        OpenGL_BufferData(GL_ARRAY_BUFFER, Mesh->Vertices->Size, Mesh->Vertices->Data, DrawType);
        Mesh->Flags &= ~MESH_GROW_VERTEX_BUFFER;
    } else
        OpenGL_BufferSubData(GL_ARRAY_BUFFER, 0, Mesh->Vertices->Size, Mesh->Vertices->Data);
    
    if(Mesh->Flags & MESH_GROW_INDEX_BUFFER) {
        OpenGL_BufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh->Indices->Size, Mesh->Indices->Data, DrawType);
        Mesh->Flags &= ~MESH_GROW_INDEX_BUFFER;
    } else
        OpenGL_BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, Mesh->Indices->Size, Mesh->Indices->Data);
    
    OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->TextureSSBO);
    if(Mesh->Flags & MESH_GROW_TEXTURE_BUFFER) {
        OpenGL_BufferData(GL_SHADER_STORAGE_BUFFER, Mesh->Storage->Size, Mesh->Storage->Data, DrawType);
        Mesh->Flags &= ~MESH_GROW_TEXTURE_BUFFER;
    } else
        OpenGL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Mesh->Storage->Size, Mesh->Storage->Data);
    
    OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Mesh->MatrixSSBO);
    if(Mesh->Flags & MESH_GROW_MATRIX_BUFFER) {
        OpenGL_BufferData(GL_SHADER_STORAGE_BUFFER, Mesh->Matrices->Size, Mesh->Matrices->Data, DrawType);
        Mesh->Flags &= ~MESH_GROW_MATRIX_BUFFER;
    } else
        OpenGL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Mesh->Matrices->Size, Mesh->Matrices->Data);
    
    Mesh->Flags &= ~MESH_IS_DIRTY;
}

internal void
Mesh_Draw(mesh *Mesh)
{
    if(!Mesh->Program) return;
    if(Mesh->ObjectCount == 0) return;
    Assert(!(Mesh->Flags & MESH_IS_DIRTY));
    
    Stack_Push();
    
    Mesh_Bind(Mesh);
    // OpenGL_UseProgram(*Mesh->Program);
    
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
    Heap_Free(Mesh->Storage);
    Heap_Free(Mesh->Indices);
    Heap_Free(Mesh->Vertices);
}

internal void
Mesh_FreeObject(mesh_object Object)
{
    Heap_Free(Object.Vertices);
    Heap_Free(Object.Indices);
}