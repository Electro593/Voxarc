/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef INCLUDE_HEADER
   typedef struct p_vertex {
      v4s16 Position;
      // u32 Position;
   } p_vertex;
   
   typedef struct pnc_vertex {
      v4s16 Position;
      u32 Normal;
      v4u08 Color;
   } pnc_vertex;
   
   typedef struct pnm_vertex {
      v4s16 Position;
      u32 Normal;
      u32 MaterialIndex;
   } pnm_vertex;
   
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
   
   typedef struct material {
      v3u08 AmbientColor;
      v3u08 DiffuseColor;
      v3u08 SpecularColor;
      u08 Shininess;
      
      u08 _Unused[2];
   } material;
   
   typedef enum mesh_flags {
      MESH_HAS_ELEMENTS          = 0x0001,
      MESH_HAS_NORMALS           = 0x0002,
      MESH_HAS_TEXTURES          = 0x0004,
      MESH_HAS_COLORS            = 0x0008,
      
      MESH_GROW_VERTEX_BUFFER    = 0x0010,
      MESH_GROW_INDEX_BUFFER     = 0x0020,
      MESH_GROW_TEXTURE_BUFFER   = 0x0040,
      MESH_GROW_MATRIX_BUFFER    = 0x0080,
      
      MESH_IS_FOR_UI             = 0x0100,
      MESH_IS_DYNAMIC            = 0x0200,
      MESH_SHARED_TEXTURE_BUFFER = 0x0400,
      MESH_HAS_PERSPECTIVE       = 0x0800,
      
      MESH_HAS_MATERIALS         = 0x1000 | MESH_HAS_COLORS,
      
      MESH_IS_DIRTY = MESH_GROW_VERTEX_BUFFER|MESH_GROW_INDEX_BUFFER|MESH_GROW_TEXTURE_BUFFER|MESH_GROW_MATRIX_BUFFER
   } mesh_flags;
   
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
      u32 MaterialSSBO;
      u32 EBO;
      u32 TextureSSBO;
      
      u32 SamplerObject;
      u32 TextureIndex;
      
      u32 Color;
      u32 VPMatrix;
      u32 LightPos;
      u32 CameraPos;
      u32 Atlases;
      u32 AtlasesSampler;
      u32 AtlasSize;
      
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
      heap_handle *Materials;
   } mesh;
   
   #define MESH_FUNCS \
      EXPORT(v4s16, Mesh_EncodePosition, v3r32 P) \
      EXPORT(u32, Mesh_EncodeNormal, v3r32 N) \
      EXPORT(vptr, Mesh_GetVertices, mesh *Mesh, u32 ObjectIndex) \
      EXPORT(m4x4r32*, Mesh_GetMatrix, mesh *Mesh, u32 ObjectIndex) \
      EXPORT(u32, Mesh_ReserveObject, mesh *Mesh, u32 VertexCount, u32 IndexCount) \
      EXPORT(void, Mesh_AddObjects, mesh *Mesh, u32 ObjectCount, mesh_object **Objects) \
      EXPORT(void, Mesh_Update, mesh *Mesh) \
      EXPORT(void, Mesh_FreeObject, mesh_object Object) \
   
   typedef struct game_state game_state;
   typedef struct renderer_state renderer_state;
   
   #define RENDER_FUNCS \
      EXPORT(void, Renderer_Resize, v2u32 NewSize, m4x4r32 *OrthographicMatrix, m4x4r32 *PerspectiveMatrix) \
      EXPORT(void, Renderer_SetUniform_V3r32, struct renderer_state *Renderer, u32 ShaderID, u32 Uniform, v3r32 Value) \
      EXPORT(void, Renderer_SetUniform_M4x4r32, struct renderer_state *Renderer, u32 ShaderID, u32 Uniform, u32 Count, b08 Transpose, m4x4r32 Value) \
      EXPORT(void, Renderer_Draw, struct platform_state *Platform, struct game_state *Game, struct renderer_state *Renderer, r32 FPS) \
   
   #define RENDERER_FUNCS \
      MESH_FUNCS \
      RENDER_FUNCS \
   
   typedef struct renderer_funcs {
      #define EXPORT(R, N, ...) \
         R (*N)(__VA_ARGS__);
      #define X RENDERER_FUNCS
      #include <x.h>
   } renderer_funcs;
   
   #if defined(_RENDERER_MODULE)
      #define EXPORT(R, N, ...) \
         internal R N(__VA_ARGS__);
      #define X RENDERER_FUNCS
      #include <x.h>
   #else
      #define EXPORT(R, N, ...) \
         global R (*N)(__VA_ARGS__);
      #define X RENDERER_FUNCS
      #include <x.h>
   #endif
#endif