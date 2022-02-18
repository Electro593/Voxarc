/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum mesh_flags {
    MESH_IS_DYNAMIC         = 0x01,
    MESH_HAS_NORMALS        = 0x02,
    MESH_HAS_TEXTURES       = 0x04,
    MESH_HAS_COLORS         = 0x08,
    MESH_GROW_VERTEX_BUFFER = 0x10,
    MESH_GROW_INDEX_BUFFER  = 0x20,
} mesh_flags;

typedef struct mesh_object {
    heap_handle *Vertices;
    heap_handle *Indices;
} mesh_object;

typedef struct mesh {
    u32 Program;
    u32 VAO, VBO, EBO;
    u32 TextureData;
    u32 Sampler;
    
    mesh_flags Flags;
    u32 VertexSize;
    
    u32 ObjectCount;
    heap_handle *VertexOffsets;
    heap_handle *IndexOffsets;
    
    heap_handle *Vertices;
    heap_handle *Indices;
} mesh;