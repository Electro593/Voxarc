/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum mesh_flags {
    MESH_HAS_ELEMENTS          = 0x001,
    MESH_HAS_NORMALS           = 0x002,
    MESH_HAS_TEXTURES          = 0x004,
    MESH_HAS_COLORS            = 0x008,
    
    MESH_GROW_VERTEX_BUFFER    = 0x010,
    MESH_GROW_INDEX_BUFFER     = 0x020,
    MESH_GROW_TEXTURE_BUFFER   = 0x040,
    MESH_GROW_MATRIX_BUFFER    = 0x080,
    
    MESH_IS_FOR_UI             = 0x100,
    MESH_IS_DYNAMIC            = 0x200,
    MESH_SHARED_TEXTURE_BUFFER = 0x400,
    
    MESH_IS_DIRTY = MESH_GROW_VERTEX_BUFFER|MESH_GROW_INDEX_BUFFER|MESH_GROW_TEXTURE_BUFFER|MESH_GROW_MATRIX_BUFFER
} mesh_flags;

typedef struct p_vertex {
    v4s16 Position;
    // u32 Position;
} p_vertex;

typedef struct pc_vertex {
    v4s16 Position;
    // u32 Position;
    v4u08 Color;
} pc_vertex;

typedef struct pt_vertex {
    v4s16 Position;
    // u32 Position;
    u32 Texture;
} pt_vertex;

typedef struct glyph_vertex {
    v2s16 Position;
    u32 Texture;
} glyph_vertex;

typedef struct mesh_object {
    heap_handle *Vertices;
    heap_handle *Indices;
    m4x4r32 ScalingMatrix;
    m4x4r32 TranslationMatrix;
    m4x4r32 RotationMatrix;
    u32 Index;
} mesh_object;

typedef struct mesh {
    u32 *Program;
    
    u32 VAO;
    
    u32 VBO;
    u32 MatrixSSBO;
    u32 EBO;
    u32 TextureSSBO;
    
    u32 TextureIndex;
    u32 SamplerObject;
    
    union {
        struct {
            u32 VPMatrix;
            u32 Atlases;
            u32 AtlasesSampler;
            u32 AtlasSize;
            u32 Color;
        };
        
        struct {
            u32 VPMatrix;
            u32 Color;
        } P;
        
        struct {
            u32 VPMatrix;
        } PC3;
        
        struct {
            u32 VPMatrix;
            u32 Atlases;
            u32 AtlasesSampler;
            u32 AtlasSize;
        } PT;
    };
    
    mesh_flags Flags;
    u32 VertexSize;
    
    u32 ObjectCount;
    HEAP(s32) VertexOffsets;
    HEAP(s32) VertexCounts;
    HEAP(s32) IndexOffsets;
    HEAP(s32) IndexCounts;
    
    heap_handle *Vertices;
    heap_handle *Indices;
    heap_handle *Storage;
    heap_handle *Matrices;
} mesh;