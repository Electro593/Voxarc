/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum shader_id {
    ShaderID_P3,
    ShaderID_PC2,
    ShaderID_PC3,
    ShaderID_PT3,
    ShaderID_Glyph,
    ShaderID_PNM3,
    
    Shader_Count
} shader_id;

c08 *ShaderPaths[] = {
    SHADERS_DIR "p.vert",
    SHADERS_DIR "p.frag",
    SHADERS_DIR "pc2.vert",
    SHADERS_DIR "pc2.frag",
    SHADERS_DIR "pc3.vert",
    SHADERS_DIR "pc3.frag",
    SHADERS_DIR "pt.vert",
    SHADERS_DIR "pt.frag",
    SHADERS_DIR "glyph.vert",
    SHADERS_DIR "glyph.frag",
    SHADERS_DIR "pnc3.vert",
    SHADERS_DIR "pnc3.frag",
};

typedef struct shader {
    mesh Mesh;
    
    datetime VertLastModified;
    datetime FragLastModified;
    
    u32 Program;
    b08 Initialized;
} shader;

typedef struct renderer_state {
    u32 VAO;
    u32 VBO;
    u32 EBO;
    
    shader Shaders[Shader_Count];
    
    m4x4r32 OrthographicMatrix;
    m4x4r32 PerspectiveMatrix;
    m4x4r32 ViewMatrix;
    
    v2u32 WindowSize;
    
    heap *Heap;
    
    assetpack Assetpack;
    
    ui_font Font;
    ui_style Style;
    u32 ObjectIndex;
} renderer_state;