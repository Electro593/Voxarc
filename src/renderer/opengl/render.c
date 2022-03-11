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
    
    // c08 Char = '#';
    // Tag = Assetpack_FindExactTag(Renderer->Assetpack, TAG_CODEPOINT, Char);
    // Assert(Tag && Tag->AssetCount);
    // assetpack_texture Asset = Tag->Assets[0]->Texture;
    // r32 Scale = 1.0f/MAX(Asset.Size.X, Asset.Size.Y);
    // v2r32 AS = {Asset.Size.X*Scale, Asset.Size.Y*Scale};
    // v2r32 AP = {-AS.X/2, -AS.Y/2};
    
    // struct {
    //     u32 Position;
    //     u32 Texture;
    // } Vertices1[4] = {
    //     {Mesh_EncodePosition((v3r32){AP.X,     AP.Y,     0}), ((Char-32)<<2) | 0b00},
    //     {Mesh_EncodePosition((v3r32){AP.X,     AP.Y+AS.Y,0}), ((Char-32)<<2) | 0b10},
    //     {Mesh_EncodePosition((v3r32){AP.X+AS.X,AP.Y+AS.Y,0}), ((Char-32)<<2) | 0b11},
    //     {Mesh_EncodePosition((v3r32){AP.X+AS.X,AP.Y,     0}), ((Char-32)<<2) | 0b01},
    // };
    
    // u32 Indices1[] = {0,1,2,0,2,3};
    // mesh_object Objects[] = {
    //     { Heap_Allocate(Heap, sizeof(Vertices1)), Heap_Allocate(Heap, sizeof(Indices1)) },
    // };
    
    // Mem_Cpy(Objects[0].Vertices->Data, Vertices1, sizeof(Vertices1));
    // Mem_Cpy(Objects[0].Indices->Data, Indices1, sizeof(Indices1));
    
    // Mesh_AddObjects(&Renderer->Mesh, 1, Objects);
    // Mesh_Update(&Renderer->Mesh);
    
    ui UI;
    UI_Init(&UI, &Renderer->Mesh, Renderer->Heap);
    u16 Index = UI_CreateStringNode(&UI, CString("Hello, personaaaaa. aHow's it going"), 0.25, Renderer->Assetpack);
    ui_node *Node = UI_GetNode(&UI, Index);
    Mesh_AddObjects(&Renderer->Mesh, 1, &Node->Object);
    Heap_Free(Node->Object.Indices);
    Heap_Free(Node->Object.Vertices);
    Heap_Free(Node->StringData.Lines);
    String_Free(Node->StringData.String);
    
    Mesh_Update(&Renderer->Mesh);
    
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