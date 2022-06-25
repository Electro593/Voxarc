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
    // Assert(FALSE);
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
Renderer_Resize(v2u32 NewSize, m4x4r32 *PerspectiveMatrix)
{
    v2u32 Pos;
    v2u32 Size;
    v2u32 Res = NewSize;
    
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
    
    //CREDIT: https://ogldev.org/www/tutorial12/tutorial12.html
    
    r32 AspectRatio = (r32)NewSize.X / (r32)NewSize.Y;
    r32 FOV = R32_PI/2;
    //TODO: Implement a tan function
    r32 CotHalfFOV = R32_cos(FOV/2) / R32_sin(FOV/2);
    r32 NearZ = 0.1f;
    r32 FarZ = 256;
    r32 ZRange = NearZ - FarZ;
    
    *PerspectiveMatrix = (m4x4r32){
        CotHalfFOV/AspectRatio, 0, 0, 0,
        0, CotHalfFOV, 0, 0,
        0, 0, (-NearZ-FarZ)/ZRange, 2*FarZ*NearZ/ZRange,
        0, 0, 1, 0
    };
    
    OpenGL_Viewport(Pos.X, Pos.Y, Size.X, Size.Y);
    OpenGL_Scissor(Pos.X, Pos.Y, Size.X, Size.Y);
}

internal void
Renderer_Init(renderer_state *Renderer,
              heap *Heap,
              v2u32 WindowSize)
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
    OpenGL_CullFace(GL_BACK);
    OpenGL_Enable(GL_BLEND);
    OpenGL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //
    // PC Mesh
    //
    
    // Renderer->PCProgram = Renderer_LoadShaders(SHADERS_DIR "pc.vert", SHADERS_DIR "pc.frag");
    
    // Platform_GetFileTime(SHADERS_DIR "pc.vert", 0, 0, Renderer->PCLastModified+0);
    // Platform_GetFileTime(SHADERS_DIR "pc.frag", 0, 0, Renderer->PCLastModified+1);
    
    // Mesh_Init(&Renderer->PCMesh, Renderer->Heap, &Renderer->PCProgram, MESH_HAS_COLORS);
    
    //
    // PT Mesh
    //
    
    Renderer->PTProgram = Renderer_LoadShaders(SHADERS_DIR "pt.vert", SHADERS_DIR "pt.frag");
    
    Platform_GetFileTime(SHADERS_DIR "pt.vert", 0, 0, Renderer->PTLastModified+0);
    Platform_GetFileTime(SHADERS_DIR "pt.frag", 0, 0, Renderer->PTLastModified+1);
    
    Mesh_Init(&Renderer->PTMesh, Renderer->Heap, &Renderer->PTProgram, MESH_HAS_TEXTURES);
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Renderer->Assetpack, TAG_ATLAS_DESCRIPTOR);
    Assert(Tag);
    assetpack_atlas *Atlas = (assetpack_atlas*)Tag->ValueP;
    
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    OpenGL_TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    OpenGL_TexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, Atlas->Size.X, Atlas->Size.Y, Atlas->Count, 0, GL_RGBA, GL_UNSIGNED_BYTE, Renderer->Assetpack.AssetData+Atlas->DataOffset);
    
    OpenGL_UseProgram(Renderer->PTProgram);
    OpenGL_Uniform1i(Renderer->PTMesh.AtlasesSampler, 0);
    OpenGL_Uniform2ui(Renderer->PTMesh.AtlasSize, Atlas->Size.X, Atlas->Size.Y);
    
    Heap_Resize(Renderer->PTMesh.Storage, Renderer->Assetpack.Header->AssetsSize);
    Mem_Cpy(Renderer->PTMesh.Storage->Data, Renderer->Assetpack.Assets, Renderer->PTMesh.Storage->Size);
    Renderer->PTMesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
    
    // UI_Init(&Renderer->UI, Renderer->Heap, &Renderer->PTProgram, Renderer->Assetpack, WindowSize);
    // OpenGL_DeleteBuffers(1, &Renderer->UI.Mesh.TextureSSBO);
    // Renderer->UI.Mesh.TextureSSBO = Renderer->Mesh.TextureSSBO;
    // OpenGL_DeleteTextures(1, &Renderer->UI.Mesh.Atlases);
    // Renderer->UI.Mesh.Atlases = Renderer->Mesh.Atlases;
    // Heap_Free(Renderer->UI.Mesh.Storage);
    // Renderer->UI.Mesh.Storage = Renderer->Mesh.Storage;
    // Renderer->UI.Mesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
    
    // ui_node_style Style;
    // Style.BackgroundColor = (v3u08){128,128,0};
    // Style.ZIndex = 1;
    // Style.FontSize = 200.f;
    // Style.Size = (v2u32){WindowSize.X/2, WindowSize.Y/2};
    // UI_CreateNode(&Renderer->UI, CString("a"), Style);
    // UI_Update(&Renderer->UI);
    
    //
    // UI Mesh
    //
    
    // Renderer->UIProgram = Renderer_LoadShaders(SHADERS_DIR "ui.vert", SHADERS_DIR "ui.frag");
    
    //
    // Other
    //
    
    Renderer->DEBUGCounter = 0;
    
    OpenGL_ClearColor(.2,.2,.2,1);
    
    Renderer->Pos = (v3r32){0,1,3};
    Renderer->Dir = (v3r32){0,0,0};
}

internal void
Renderer_Draw(renderer_state *Renderer)
{
    // Hot-reload PC shaders
    // {
    //     datetime VertTime, FragTime;
    //     Platform_GetFileTime(SHADERS_DIR "pc.vert", 0, 0, &VertTime);
    //     Platform_GetFileTime(SHADERS_DIR "pc.frag", 0, 0, &FragTime);
    //     if(Platform_CmpFileTime(Renderer->PCLastModified[0], VertTime) == LESS ||
    //        Platform_CmpFileTime(Renderer->PCLastModified[1], FragTime) == LESS)
    //     {
    //         OpenGL_DeleteProgram(Renderer->PCProgram);
    //         Renderer->PCProgram = Renderer_LoadShaders(SHADERS_DIR "pc.vert", SHADERS_DIR "pc.frag");
            
    //         Renderer->PCMesh.VPMatrix = OpenGL_GetUniformLocation(Renderer->PCProgram, "VPMatrix");
            
    //         m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
            
    //         OpenGL_UseProgram(Renderer->PCProgram);
    //         OpenGL_UniformMatrix4fv(Renderer->PCMesh.VPMatrix, 1, FALSE, VPMatrix);
            
    //         Renderer->PCLastModified[0] = VertTime;
    //         Renderer->PCLastModified[1] = FragTime;
    //     }
    // }
    
    // Hot-reload PT shaders
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
            
            Renderer->PTMesh.AtlasesSampler = OpenGL_GetUniformLocation(Renderer->PTProgram, "Atlases");
            Renderer->PTMesh.AtlasSize      = OpenGL_GetUniformLocation(Renderer->PTProgram, "AtlasSize");
            Renderer->PTMesh.VPMatrix       = OpenGL_GetUniformLocation(Renderer->PTProgram, "VPMatrix");
            
            m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
            
            OpenGL_UseProgram(Renderer->PTProgram);
            OpenGL_Uniform1i(Renderer->PTMesh.AtlasesSampler, 0);
            OpenGL_Uniform2ui(Renderer->PTMesh.AtlasSize, Atlas->Size.X, Atlas->Size.Y);
            OpenGL_UniformMatrix4fv(Renderer->PTMesh.VPMatrix, 1, FALSE, VPMatrix);
            
            Renderer->PTLastModified[0] = VertTime;
            Renderer->PTLastModified[1] = FragTime;
        }
    }
    
    OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // Mesh_Draw(&Renderer->PCMesh);
    
    Mesh_Draw(&Renderer->PTMesh);
    
    // UI_Draw(&Renderer->UI);
    
    Renderer->DEBUGCounter++;
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