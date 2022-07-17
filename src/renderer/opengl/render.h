/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct renderer_state {
    u32 VAO;
    u32 VBO;
    u32 EBO;
    
    u32 PProgram;
    u32 PC2Program;
    u32 PC3Program;
    u32 PTProgram;
    u32 GlyphProgram;
    
    datetime PLastModified[2];
    datetime PC2LastModified[2];
    datetime PC3LastModified[2];
    datetime PTLastModified[2];
    datetime UILastModified[2];
    
    m4x4r32 OrthographicMatrix;
    m4x4r32 PerspectiveMatrix;
    m4x4r32 ViewMatrix;
    
    v2u32 WindowSize;
    
    heap *Heap;
    
    mesh PMesh;
    mesh PC2Mesh;
    mesh PC3Mesh;
    mesh PTMesh;
    mesh GlyphMesh;
    
    assetpack Assetpack;
    
    ui_font Font;
    ui_style Style;
    u32 ObjectIndex;
} renderer_state;