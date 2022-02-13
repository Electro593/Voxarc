/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
File_LoadShaders(c08 *VertFileName,
                 c08 *FragFileName)
{
    s32 Result=FALSE, InfoLogLength;
    u32 VertID=0, FragID=0;
    c08 *VertErrorMessage, *FragErrorMessage, *ProgramErrorMessage;
    string VertCode, FragCode;
    
    u32 ProgramID = OpenGL_CreateProgram();
    
    if(VertFileName) {
        VertID = OpenGL_CreateShader(GL_VERTEX_SHADER);
        VertCode = File_Read(VertFileName, 0, 0);
        OpenGL_ShaderSource(VertID, 1, &VertCode.Text, NULL);
        OpenGL_CompileShader(VertID);
        String_Free(VertCode);
        
        OpenGL_GetShaderiv(VertID, GL_COMPILE_STATUS, &Result);
        OpenGL_GetShaderiv(VertID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        VertErrorMessage = Stack_Allocate(InfoLogLength);
        OpenGL_GetShaderInfoLog(VertID, InfoLogLength, NULL, VertErrorMessage);
        if(!Result) {
            Error(VertErrorMessage);
        }
        
        OpenGL_AttachShader(ProgramID, VertID);
    }
    
    if(FragFileName) {
        FragID = OpenGL_CreateShader(GL_FRAGMENT_SHADER);
        FragCode = File_Read(FragFileName, 0, 0);
        OpenGL_ShaderSource(FragID, 1, &FragCode.Text, NULL);
        OpenGL_CompileShader(FragID);
        String_Free(FragCode);
        
        OpenGL_GetShaderiv(FragID, GL_COMPILE_STATUS, &Result);
        OpenGL_GetShaderiv(FragID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        FragErrorMessage = Stack_Allocate(InfoLogLength);
        OpenGL_GetShaderInfoLog(FragID, InfoLogLength, NULL, FragErrorMessage);
        if(!Result) {
            Error(FragErrorMessage);
        }
        
        OpenGL_AttachShader(ProgramID, FragID);
    }
    
    OpenGL_LinkProgram(ProgramID);
    OpenGL_GetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    OpenGL_GetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    ProgramErrorMessage = Stack_Allocate(InfoLogLength);
    OpenGL_GetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
    if(!Result) {
        Error(ProgramErrorMessage);
    }
    
    if(VertFileName) {
        OpenGL_DetachShader(ProgramID, VertID);
        OpenGL_DeleteShader(VertID);
    }
    if(FragFileName) {
        OpenGL_DetachShader(ProgramID, FragID);
        OpenGL_DeleteShader(FragID);
    }
    
    return ProgramID;
}

internal void
Renderer_Init(renderer_state *Renderer)
{
    #if defined(_DEBUG)
        OpenGL_Enable(GL_DEBUG_OUTPUT);
        OpenGL_DebugMessageCallback(OpenGL_DebugCallback, NULL);
        u32 ID = 131185;
        OpenGL_DebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, &ID, FALSE);
    #endif
    
    Renderer->PCShader = File_LoadShaders(SHADERS_DIR "pc.vert", SHADERS_DIR "pc.frag");
    OpenGL_UseProgram(Renderer->PCShader);
    
    OpenGL_GenVertexArrays(1, &Renderer->VAO);
    OpenGL_GenBuffers(1, &Renderer->VBO);
    
    OpenGL_BindVertexArray(Renderer->VAO);
    OpenGL_BindBuffer(GL_ARRAY_BUFFER, Renderer->VBO);
    
    u64 Offset = 0;
    u32 Stride = sizeof(v3r32) + sizeof(v4u08);
    // s32 Stride = sizeof(bfs32) + sizeof(v4u08);
    
    OpenGL_EnableVertexAttribArray(0);
    OpenGL_VertexAttribPointer(0, 3, GL_FLOAT, TRUE, Stride, (vptr)Offset);
    // OpenGL_VertexAttribPointer(0, 4, GL_INT_2_10_10_10_REV, TRUE, Stride, (vptr)Offset);
    Offset += sizeof(v3r32);
    // Offset += sizeof(bfs32);
    
    OpenGL_EnableVertexAttribArray(2);
    OpenGL_VertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, TRUE, Stride, (vptr)Offset);
    Offset += sizeof(v4u08);
    
    Assert(Offset == Stride);
    
    struct vertex {
        v3r32 Pos;
        v4u08 Color;
    } Vertices[] = {
        {{-0.75,-0.75,0}, {255,0,0,255}},
        {{ 0.00, 0.75,0}, {0,255,0,255}},
        {{ 0.75,-0.75,0}, {0,0,255,255}}
    };
    
    OpenGL_BufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

internal void
Renderer_Draw(renderer_state *Renderer)
{
    OpenGL_DrawArrays(GL_TRIANGLES, 0, 3);
}