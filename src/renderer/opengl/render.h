/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <renderer/opengl/opengl.h>
#include <renderer/opengl/mesh.h>

typedef struct renderer_state {
    u32 VAO;
    u32 VBO;
    u32 EBO;
    
    u32 PCProgram;
    u32 PTProgram;
    
    heap *Heap;
    mesh Mesh;
} renderer_state;