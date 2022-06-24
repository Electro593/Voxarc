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
    
    // https://ogldev.org/www/tutorial12/tutorial12.html
    
    r32 AspectRatio = (r32)NewSize.X / (r32)NewSize.Y;
    r32 FOV = R32_PI/2;
    //TODO: Implement a tan function
    r32 CotHalfFOV = R32_cos(FOV/2) / R32_sin(FOV/2);
    r32 NearZ = 0.1f;
    r32 FarZ = 256;
    r32 ZRange = NearZ - FarZ;
    
    *PerspectiveMatrix = (m4x4r32){
        CotHalfFOV, 0, 0, 0,
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
    OpenGL_CullFace(GL_FRONT);
    OpenGL_Enable(GL_BLEND);
    OpenGL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer->PCProgram = Renderer_LoadShaders(SHADERS_DIR "pc.vert", SHADERS_DIR "pc.frag");
    Renderer->PTProgram = Renderer_LoadShaders(SHADERS_DIR "pt.vert", SHADERS_DIR "pt.frag");
    Renderer->UIProgram = Renderer_LoadShaders(SHADERS_DIR "ui.vert", SHADERS_DIR "ui.frag");
    Platform_GetFileTime(SHADERS_DIR "pc.vert", 0, 0, Renderer->PCLastModified+0);
    Platform_GetFileTime(SHADERS_DIR "pc.frag", 0, 0, Renderer->PCLastModified+1);
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
    
    UI_Init(&Renderer->UI, Renderer->Heap, &Renderer->PTProgram, Renderer->Assetpack, WindowSize);
    OpenGL_DeleteBuffers(1, &Renderer->UI.Mesh.TextureSSBO);
    Renderer->UI.Mesh.TextureSSBO = Renderer->Mesh.TextureSSBO;
    OpenGL_DeleteTextures(1, &Renderer->UI.Mesh.Atlases);
    Renderer->UI.Mesh.Atlases = Renderer->Mesh.Atlases;
    Heap_Free(Renderer->UI.Mesh.Storage);
    Renderer->UI.Mesh.Storage = Renderer->Mesh.Storage;
    Renderer->UI.Mesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
    
    ui_node_style Style;
    Style.BackgroundColor = (v3u08){128,128,0};
    Style.ZIndex = 1;
    Style.FontSize = 200.f;
    Style.Size = (v2u32){WindowSize.X/2, WindowSize.Y/2};
    UI_CreateNode(&Renderer->UI, CString("a"), Style);
    UI_Update(&Renderer->UI);
    
    Renderer->DEBUGCounter = 0;
    
    OpenGL_ClearColor(.2,.2,.2,1);
    
    // Mesh_Init(&Renderer->OtherUIMesh, Renderer->Heap, &Renderer->UIProgram, MESH_HAS_COLORS|MESH_IS_FOR_OTHER_UI);
    
    
    // mesh_object Object;
    // mesh_object *Objects[] = { &Object };
    // Object.Indices = Heap_Allocate(Renderer->Heap, 6*sizeof(u32));
    // Object.Vertices = Heap_Allocate(Renderer->Heap, 4*Renderer->Mesh.VertexSize);
    // pt_vertex *Vertices = (vptr)Object.Vertices->Data;
    // *Vertices++ = (pt_vertex){(1<<30)|(0<<20)|((-512&0x3FF)<<10)|(-512&0x3FF), (('a'-' ')<<2) | 0b00};
    // *Vertices++ = (pt_vertex){(1<<30)|(0<<20)|(( 511&0x3FF)<<10)|(-512&0x3FF), (('a'-' ')<<2) | 0b10};
    // *Vertices++ = (pt_vertex){(1<<30)|(0<<20)|(( 511&0x3FF)<<10)|( 511&0x3FF), (('a'-' ')<<2) | 0b11};
    // *Vertices++ = (pt_vertex){(1<<30)|(0<<20)|((-512&0x3FF)<<10)|( 511&0x3FF), (('a'-' ')<<2) | 0b01};
    
    // u32 *Indices = (u32*)Object.Indices->Data;
    // *Indices++ = 0;
    // *Indices++ = 1;
    // *Indices++ = 2;
    // *Indices++ = 0;
    // *Indices++ = 2;
    // *Indices++ = 3;
    
    // Object.TranslationMatrix = M4x4r32_I;
    // Object.ScalingMatrix = M4x4r32_Scaling(0.5,0.5,1);
    // Object.RotationMatrix = M4x4r32_I;
    
    // Mesh_AddObjects(&Renderer->Mesh, 1, Objects);
    // Mesh_Update(&Renderer->Mesh);
    
    // r32 n = 0.1f;
    // r32 f = 100.0f;
    // r32 l = -1.0f;
    // r32 r = 1.0f;
    // r32 t = 1.0f;
    // r32 b = -1.0f;
    // Renderer->PerspectiveMatrix = (m4x4r32){
    //     2*n/(r-l), 0,         (r+l)/(r-l), 0,
    //     0,         2*n/(t-b), (t+b)/(t-b), 0,
    //     0,         0,         -(f+n)/(f-n), -2*f*n/(f-n),
    //     0,         0,         -1,          0
    // };
    Renderer->Pos = (v3r32){0,0,2};
    Renderer->Dir = (v3r32){0,0,0};
    
    // mesh_object Object;
    // mesh_object *Objects[] = { &Object };
    
    // Object.Indices = Heap_Allocate(Heap, 6*sizeof(u32));
    // Object.Vertices = Heap_Allocate(Heap, 4*Renderer->OtherUIMesh.VertexSize);
    
    // u32 *Indices = (vptr)Object.Indices->Data;
    // struct {
    //     v4r32 Pos;
    //     v4u08 Color;
    // } *Vertices = (vptr)Object.Vertices->Data;
    
    // *Indices++ = 0;
    // *Indices++ = 1;
    // *Indices++ = 2;
    // *Indices++ = 0;
    // *Indices++ = 2;
    // *Indices++ = 3;
    
    // *Vertices++ = (pt_vertex){ (1024<<12)|(1024), (u32)(1<<31)|(128<<18)|(0<<10)|(0<<2) };
    // *Vertices++ = (pt_vertex){ (3072<<12)|(1024), (u32)(1<<31)|(128<<18)|(0<<10)|(0<<2) };
    // *Vertices++ = (pt_vertex){ (3072<<12)|(3072), (u32)(1<<31)|(128<<18)|(0<<10)|(0<<2) };
    // *Vertices++ = (pt_vertex){ (1024<<12)|(3072), (u32)(1<<31)|(128<<18)|(0<<10)|(0<<2) };
    // Vertices[0].Pos   = (v4r32){ -0.5, 0.5, 1, -1 };
    // Vertices[1].Pos   = Vertices[0].Pos;
    // Vertices[2].Pos   = Vertices[0].Pos;
    // Vertices[3].Pos   = Vertices[0].Pos;
    // Vertices[0].Color = (v4u08){ 128, 0, 0, 255 };
    // Vertices[1].Color = Vertices[0].Color;
    // Vertices[2].Color = Vertices[0].Color;
    // Vertices[3].Color = Vertices[0].Color;
    
    // OpenGL_Uniform2ui(Renderer->OtherUIMesh.ViewSize, WindowSize.X, WindowSize.Y);
    // OpenGL_Uniform1f(OpenGL_GetUniformLocation(Renderer->UIProgram, "testIn"), 1);
    
    // Object.TranslationMatrix = M4x4r32_Translation(-1, -1, 0);
    // Object.ScalingMatrix = M4x4r32_Scaling(2.0f/4095, 2.0f/4095, 1);
    // Object.RotationMatrix = M4x4r32_I;
    
    // Mesh_AddObjects(&Renderer->OtherUIMesh, 1, Objects);
    // Mesh_Update(&Renderer->OtherUIMesh);
    
    // Heap_Free(Object.Vertices);
    // Heap_Free(Object.Indices);
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
    
    // OpenGL_UniformMatrix4fv(Renderer->OtherUIMesh.VPMatrix, 1, FALSE, VPMatrix);
    
    // Mesh_Draw(&Renderer->Mesh);
    // Mesh_Draw(&Renderer->OtherUIMesh);
    
    UI_Draw(&Renderer->UI);
    
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