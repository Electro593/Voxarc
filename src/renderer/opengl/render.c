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
    Error(Message);
    Assert(FALSE);
}

internal u32
Renderer_LoadShaders(c08 *VertFileName,
                     c08 *FragFileName)
{
    s32 Result=FALSE, InfoLogLength;
    u32 Vert=0, Frag=0;
    c08 *ProgramErrorMessage;
    string VertCode, FragCode;
    
    u32 Program = OpenGL_CreateProgram();
    
    if(VertFileName) {
        Vert = OpenGL_CreateShader(GL_VERTEX_SHADER);
        VertCode = File_Read(VertFileName, 0, 0);
        OpenGL_ShaderSource(Vert, 1, &VertCode.Text, NULL);
        OpenGL_CompileShader(Vert);
        String_Free(VertCode);
        OpenGL_AttachShader(Program, Vert);
    }
    
    if(FragFileName) {
        Frag = OpenGL_CreateShader(GL_FRAGMENT_SHADER);
        FragCode = File_Read(FragFileName, 0, 0);
        OpenGL_ShaderSource(Frag, 1, &FragCode.Text, NULL);
        OpenGL_CompileShader(Frag);
        String_Free(FragCode);
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
        ID = 131218;
        OpenGL_DebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 1, &ID, FALSE);
    #endif
    
    OpenGL_Enable(GL_DEPTH_TEST);
    OpenGL_Enable(GL_SCISSOR_TEST);
    OpenGL_Enable(GL_CULL_FACE);
    OpenGL_Enable(GL_BLEND);
    OpenGL_CullFace(GL_FRONT);
    OpenGL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer->PCProgram = Renderer_LoadShaders(SHADERS_DIR "pc.vert", SHADERS_DIR "pc.frag");
    Renderer->PTProgram = Renderer_LoadShaders(SHADERS_DIR "pt.vert", SHADERS_DIR "pt.frag");
    Platform_GetFileTime(SHADERS_DIR "pt.vert", 0, 0, Renderer->PTLastModified+0);
    Platform_GetFileTime(SHADERS_DIR "pt.frag", 0, 0, Renderer->PTLastModified+1);
    Mesh_Init(&Renderer->Mesh, Renderer->Heap, &Renderer->PTProgram, MESH_HAS_TEXTURES);
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Renderer->Assetpack, TAG_ATLAS_DESCRIPTOR);
    Assert(Tag);
    assetpack_atlas *Atlas = (assetpack_atlas*)Tag->ValueP;
    
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    OpenGL_TexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, Atlas->Size.X, Atlas->Size.Y, Atlas->Count, 0, GL_RGBA, GL_UNSIGNED_BYTE, Renderer->Assetpack.AssetData+Atlas->DataOffset);
    
    OpenGL_UseProgram(Renderer->PTProgram);
    OpenGL_Uniform1i(Renderer->Mesh.AtlasesSampler, 0);
    OpenGL_Uniform2ui(Renderer->Mesh.AtlasSize, Atlas->Size.X, Atlas->Size.Y);
    
    Heap_Resize(Renderer->Mesh.Storage, (127-32)*sizeof(assetpack_texture));
    Mem_Cpy(Renderer->Mesh.Storage->Data, Renderer->Assetpack.Assets, Renderer->Mesh.Storage->Size);
    Renderer->Mesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
    
    UI_Init(&Renderer->UI, &Renderer->Mesh, Renderer->Heap);
    // string String0 = CString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 1234567890`~!@#$%^&*()-_=+[]{}<>\\|/;:\'\",.");
    string String1 = CString("Hello, world!");
    // u16 Index0 = UI_CreateStringNode(&Renderer->UI, String0, (v2r32){-1,-1}, (v2r32){1,1}, 0.1, Renderer->Assetpack);
    u16 Index1 = UI_CreateStringNode(&Renderer->UI, String1, (v2r32){-1,-1}, (v2r32){2,1}, 0.4, Renderer->Assetpack);
    // ui_node *Node0 = UI_GetNode(&Renderer->UI, Index0);
    ui_node *Node1 = UI_GetNode(&Renderer->UI, Index1);
    Mesh_AddObjects(&Renderer->Mesh, 1, &Node1->Object);
    // Heap_Free(Node0->Object.Indices);
    Heap_Free(Node1->Object.Indices);
    // Heap_Free(Node0->StringData.Lines);
    Heap_Free(Node1->StringData.Lines);
    // Heap_Free(Node0->Object.Vertices);
    Heap_Free(Node1->Object.Vertices);
    // String_Free(Node0->StringData.String);
    String_Free(Node1->StringData.String);
    
    Mesh_Update(&Renderer->Mesh);
    Renderer->DEBUGCounter = 0;
    
    OpenGL_ClearColor(.2,.2,.2,1);
}

internal void
Renderer_Draw(renderer_state *Renderer)
{
    {
        datetime VertTime, FragTime;
        Platform_GetFileTime(SHADERS_DIR "pt.vert", 0, 0, &VertTime);
        Platform_GetFileTime(SHADERS_DIR "pt.frag", 0, 0, &FragTime);
        if(Platform_CmpFileTime(Renderer->PTLastModified[0], VertTime) == LESS ||
           Platform_CmpFileTime(Renderer->PTLastModified[1], FragTime) == LESS)
        {
            OpenGL_DeleteProgram(Renderer->PTProgram);
            Renderer->PTProgram = Renderer_LoadShaders(SHADERS_DIR "pt.vert", SHADERS_DIR "pt.frag");
            
            assetpack_tag *Tag = Assetpack_FindFirstTag(Renderer->Assetpack, TAG_ATLAS_DESCRIPTOR);
            assetpack_atlas *Atlas = (assetpack_atlas*)Tag->ValueP;
            
            Renderer->Mesh.AtlasesSampler = OpenGL_GetUniformLocation(Renderer->PTProgram, "Atlases");
            Renderer->Mesh.AtlasSize = OpenGL_GetUniformLocation(Renderer->PTProgram, "AtlasSize");
            
            OpenGL_UseProgram(Renderer->PTProgram);
            OpenGL_Uniform1i(Renderer->Mesh.AtlasesSampler, 0);
            OpenGL_Uniform2ui(Renderer->Mesh.AtlasSize, Atlas->Size.X, Atlas->Size.Y);
            
            Renderer->PTLastModified[0] = VertTime;
            Renderer->PTLastModified[1] = FragTime;
        }
    }
    
    OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    ui_node *Node = UI_GetNode(&Renderer->UI, 1);
    u32 Loops = Renderer->DEBUGCounter/180;
    r32 Theta = Renderer->DEBUGCounter++*R32_PI/90;
    Node->Object.RotationMatrix = M4x4r32_Rotation(Theta, (v3r32){0,0,1});
    m4x4r32 ModelMatrix = M4x4r32_Mul(M4x4r32_Mul(Node->Object.TranslationMatrix, Node->Object.RotationMatrix), Node->Object.ScalingMatrix);
    Mesh_UpdateMatrix(&Renderer->Mesh, ModelMatrix, 0);
    OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Renderer->Mesh.MatrixSSBO);
    OpenGL_BufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Renderer->Mesh.Matrices->Size, Renderer->Mesh.Matrices->Data);
    
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