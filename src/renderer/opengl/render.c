/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <renderer/opengl/mesh.c>

internal void API_ENTRY
OpenGL_DebugCallback(u32 Source,
                     u32 Type,
                     u32 ID,
                     u32 Severity,
                     s32 Length,
                     c08 *Message,
                     vptr UserParam)
{
    Assert(FALSE);
}

internal u32
OpenGL_LoadShaders(c08 *VertFileName,
                   c08 *FragFileName)
{
    s32 Result=FALSE, InfoLogLength;
    u32 Vert=0, Frag=0;
    c08 *VertErrorMessage, *FragErrorMessage, *ProgramErrorMessage;
    string VertCode, FragCode;
    
    u32 Program = OpenGL_CreateProgram();
    
    if(VertFileName) {
        Vert = OpenGL_CreateShader(GL_VERTEX_SHADER);
        VertCode = File_Read(VertFileName, 0, 0);
        OpenGL_ShaderSource(Vert, 1, &VertCode.Text, NULL);
        OpenGL_CompileShader(Vert);
        String_Free(VertCode);
        
        OpenGL_GetShaderiv(Vert, GL_COMPILE_STATUS, &Result);
        OpenGL_GetShaderiv(Vert, GL_INFO_LOG_LENGTH, &InfoLogLength);
        VertErrorMessage = Stack_Allocate(InfoLogLength);
        OpenGL_GetShaderInfoLog(Vert, InfoLogLength, NULL, VertErrorMessage);
        if(!Result) Error(VertErrorMessage);
        
        OpenGL_AttachShader(Program, Vert);
    }
    
    if(FragFileName) {
        Frag = OpenGL_CreateShader(GL_FRAGMENT_SHADER);
        FragCode = File_Read(FragFileName, 0, 0);
        OpenGL_ShaderSource(Frag, 1, &FragCode.Text, NULL);
        OpenGL_CompileShader(Frag);
        String_Free(FragCode);
        
        OpenGL_GetShaderiv(Frag, GL_COMPILE_STATUS, &Result);
        OpenGL_GetShaderiv(Frag, GL_INFO_LOG_LENGTH, &InfoLogLength);
        FragErrorMessage = Stack_Allocate(InfoLogLength);
        OpenGL_GetShaderInfoLog(Frag, InfoLogLength, NULL, FragErrorMessage);
        if(!Result) Error(FragErrorMessage);
        
        OpenGL_AttachShader(Program, Frag);
    }
    
    OpenGL_LinkProgram(Program);
    OpenGL_GetProgramiv(Program, GL_LINK_STATUS, &Result);
    OpenGL_GetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
    ProgramErrorMessage = Stack_Allocate(InfoLogLength);
    OpenGL_GetProgramInfoLog(Program, InfoLogLength, NULL, ProgramErrorMessage);
    if(!Result) Error(ProgramErrorMessage);
    
    if(VertFileName) {
        OpenGL_DetachShader(Program, Vert);
        OpenGL_DeleteShader(Vert);
    }
    if(FragFileName) {
        OpenGL_DetachShader(Program, Frag);
        OpenGL_DeleteShader(Frag);
    }
    
    return Program;
}

internal void
Renderer_Resize(v2u32 NewSize)
{
    v2u32 Pos;
    v2u32 Size;
    v2u32 Res = {1, 1};
    
    if(NewSize.X/Res.X < NewSize.Y/Res.Y) {
        Size.X = NewSize.X;
        Size.Y = (u32)((r32)Size.X*Res.Y/Res.X);
        Pos.X = 0;
        Pos.Y = (NewSize.Y - Size.Y)/2;
    } else {
        Size.Y = NewSize.Y;
        Size.X = (u32)((r32)Size.Y*Res.X/Res.Y);
        Pos.X = (NewSize.X - Size.X)/2;
        Pos.Y = 0;
    }
    
    OpenGL_Viewport(Pos.X, Pos.Y, Size.X, Size.Y);
    OpenGL_Scissor(Pos.X, Pos.Y, Size.X, Size.Y);
}

internal void
Renderer_Init(renderer_state *Renderer,
              heap *Heap)
{
    Renderer->Heap = Heap;
    
    #if defined(_DEBUG)
        OpenGL_Enable(GL_DEBUG_OUTPUT);
        OpenGL_DebugMessageCallback(OpenGL_DebugCallback, NULL);
        u32 ID = 131185;
        OpenGL_DebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, &ID, FALSE);
    #endif
    
    OpenGL_Enable(GL_DEPTH_TEST);
    OpenGL_Enable(GL_SCISSOR_TEST);
    OpenGL_Enable(GL_CULL_FACE);
    OpenGL_CullFace(GL_FRONT);
    
    Renderer->PCProgram = OpenGL_LoadShaders(SHADERS_DIR "pc.vert", SHADERS_DIR "pc.frag");
    // Renderer->PTProgram = OpenGL_LoadShaders(SHADERS_DIR "pt.vert", SHADERS_DIR "pt.frag");
    
    Mesh_Init(&Renderer->Mesh, Renderer->Heap, Renderer->PCProgram, MESH_HAS_COLORS);
    struct {
        u32 Position;
        v4u08 Color;
    } Vertices[] = {
        {0b01000000000010001000001000100000, {255,0,0,255}},
        {0b01000000000001111000001000100000, {0,255,0,255}},
        {0b01000000000001111000000111100000, {0,0,255,255}},
        {0b01000000000010001000000111100000, {255,255,0,255}},
    };
    
    u32 Indices[] = {0,1,2,0,2,3};
    mesh_object Object;
    Object.Vertices = Heap_Allocate(Heap, sizeof(Vertices));
    Object.Indices = Heap_Allocate(Heap, sizeof(Indices));
    Mem_Cpy(Object.Vertices->Data, Vertices, sizeof(Vertices));
    Mem_Cpy(Object.Indices->Data, Indices, sizeof(Indices));
    Mesh_AddObjects(&Renderer->Mesh, 1, &Object);
    Mesh_Update(&Renderer->Mesh);
}

internal void
Renderer_Draw(renderer_state *Renderer)
{
    OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    Mesh_Draw(&Renderer->Mesh);
}

internal void
Renderer_Load(opengl_funcs *OpenGLFuncs)
{
    if(OpenGLFuncs) {
        #define IMPORT(ReturnType, Name, ...) \
            OpenGL_##Name = OpenGLFuncs->OpenGL_##Name;
        #define X OPENGL_FUNCS
        #include <x.h>
    }
}