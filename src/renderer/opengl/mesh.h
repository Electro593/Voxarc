/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum mesh_flags {
    MESH_IS_DYNAMIC          = 0x01,
    MESH_HAS_NORMALS         = 0x02,
    MESH_HAS_TEXTURES        = 0x04,
    MESH_HAS_COLORS          = 0x08,
    MESH_GROW_VERTEX_BUFFER  = 0x10,
    MESH_GROW_INDEX_BUFFER   = 0x20,
    MESH_GROW_TEXTURE_BUFFER = 0x40,
    MESH_GROW_MATRIX_BUFFER  = 0x80,
} mesh_flags;

typedef struct mesh_object {
    heap_handle *Vertices;
    heap_handle *Indices;
    m4x4r32 ScalingMatrix;
    m4x4r32 TranslationMatrix;
    m4x4r32 RotationMatrix;
} mesh_object;

typedef struct mesh {
    u32 *Program;
    u32 VAO;
    u32 VBO, EBO, MatrixSSBO, TextureSSBO;
    u32 Atlases;
    u32 AtlasesSampler, AtlasSize;
    
    mesh_flags Flags;
    u32 VertexSize;
    
    u32 ObjectCount;
    heap_handle *VertexOffsets;
    heap_handle *IndexOffsets;
    
    heap_handle *Vertices;
    heap_handle *Indices;
    heap_handle *Storage;
    heap_handle *Matrices;
} mesh;