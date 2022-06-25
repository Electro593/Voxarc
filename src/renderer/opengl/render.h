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
    
    u32 PCProgram;
    // u32 PTProgram;
    // u32 UIProgram;
    datetime PCLastModified[2];
    // datetime PTLastModified[2];
    
    m4x4r32 PerspectiveMatrix, ViewMatrix;
    
    u32 DEBUGCounter;
    
    v3r32 Pos;
    v3r32 Dir;
    
    heap *Heap;
    mesh PCMesh;//, Mesh, OtherUIMesh;
    // ui UI;
    assetpack Assetpack;
} renderer_state;