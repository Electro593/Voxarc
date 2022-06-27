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
Renderer_LoadPC3Program(renderer_state *Renderer, b08 FirstTime)
{
    c08 *VertName = SHADERS_DIR "pc3.vert";
    c08 *FragName = SHADERS_DIR "pc3.frag";
    
    datetime VertTime, FragTime;
    Platform_GetFileTime(VertName, 0, 0, &VertTime);
    Platform_GetFileTime(FragName, 0, 0, &FragTime);
    
    if(FirstTime ||
       Platform_CmpFileTime(Renderer->PCLastModified[0], VertTime) == LESS ||
       Platform_CmpFileTime(Renderer->PCLastModified[1], FragTime) == LESS)
    {
        if(!FirstTime)
            OpenGL_DeleteProgram(Renderer->PCProgram);
        
        Renderer->PCProgram = Renderer_LoadShaders(VertName, FragName);
        
        if(FirstTime)
            Mesh_Init(&Renderer->PCMesh, Renderer->Heap, &Renderer->PCProgram, MESH_HAS_COLORS);
        
        Renderer->PCMesh.VPMatrix = OpenGL_GetUniformLocation(Renderer->PCProgram, "VPMatrix");
        
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        
        OpenGL_UseProgram(Renderer->PCProgram);
        OpenGL_UniformMatrix4fv(Renderer->PCMesh.VPMatrix, 1, FALSE, VPMatrix);
        
        Renderer->PCLastModified[0] = VertTime;
        Renderer->PCLastModified[1] = FragTime;
    }
}

internal void
Renderer_LoadPTProgram(renderer_state *Renderer, b08 FirstTime)
{
    c08 *VertName = SHADERS_DIR "pt.vert";
    c08 *FragName = SHADERS_DIR "pt.frag";
    
    datetime VertTime, FragTime;
    Platform_GetFileTime(VertName, 0, 0, &VertTime);
    Platform_GetFileTime(FragName, 0, 0, &FragTime);
    
    if(FirstTime ||
       Platform_CmpFileTime(Renderer->PTLastModified[0], VertTime) == LESS ||
       Platform_CmpFileTime(Renderer->PTLastModified[1], FragTime) == LESS)
    {
        if(!FirstTime)
            OpenGL_DeleteProgram(Renderer->PTProgram);
        
        Renderer->PTProgram = Renderer_LoadShaders(VertName, FragName);
        
        assetpack_tag *Tag = Assetpack_FindFirstTag(Renderer->Assetpack, TAG_ATLAS_DESCRIPTOR);
        assetpack_atlas *Atlas = (assetpack_atlas*)Tag->ValueP;
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        
        if(FirstTime) {
            Renderer->PTMesh.TextureIndex = 0;
            Mesh_Init(&Renderer->PTMesh, Renderer->Heap, &Renderer->PTProgram, MESH_HAS_TEXTURES);
            
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            OpenGL_TexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, Atlas->Size.X, Atlas->Size.Y, Atlas->Count, 0, GL_RGBA, GL_UNSIGNED_BYTE, Renderer->Assetpack.AssetData+Atlas->DataOffset);
            OpenGL_BindSampler(0, Renderer->PTMesh.SamplerObject);
            
            Heap_Resize(Renderer->PTMesh.Storage, Renderer->Assetpack.Header->AssetsSize);
            Mem_Cpy(Renderer->PTMesh.Storage->Data, Renderer->Assetpack.Assets, Renderer->PTMesh.Storage->Size);
            Renderer->PTMesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
        }
        
        Renderer->PTMesh.AtlasesSampler = OpenGL_GetUniformLocation(Renderer->PTProgram, "Atlases");
        Renderer->PTMesh.AtlasSize      = OpenGL_GetUniformLocation(Renderer->PTProgram, "AtlasSize");
        Renderer->PTMesh.VPMatrix       = OpenGL_GetUniformLocation(Renderer->PTProgram, "VPMatrix");
        
        OpenGL_UseProgram(Renderer->PTProgram);
        OpenGL_Uniform1i(Renderer->PTMesh.AtlasesSampler, 0);
        OpenGL_Uniform2ui(Renderer->PTMesh.AtlasSize, Atlas->Size.X, Atlas->Size.Y);
        OpenGL_UniformMatrix4fv(Renderer->PTMesh.VPMatrix, 1, FALSE, VPMatrix);
        
        Renderer->PTLastModified[0] = VertTime;
        Renderer->PTLastModified[1] = FragTime;
    }
}

internal void
Renderer_LoadGlyphProgram(renderer_state *Renderer, b08 FirstTime)
{
    c08 *VertName = SHADERS_DIR "glyph.vert";
    c08 *FragName = SHADERS_DIR "glyph.frag";
    
    datetime VertTime, FragTime;
    Platform_GetFileTime(VertName, 0, 0, &VertTime);
    Platform_GetFileTime(FragName, 0, 0, &FragTime);
    
    if(FirstTime ||
       Platform_CmpFileTime(Renderer->UILastModified[0], VertTime) == LESS ||
       Platform_CmpFileTime(Renderer->UILastModified[1], FragTime) == LESS)
    {
        if(!FirstTime)
            OpenGL_DeleteProgram(Renderer->GlyphProgram);
        
        Renderer->GlyphProgram = Renderer_LoadShaders(VertName, FragName);
        
        if(FirstTime) {
            Renderer->GlyphMesh.TextureIndex = 0;
            Mesh_Init(&Renderer->GlyphMesh, Renderer->Heap, &Renderer->GlyphProgram, MESH_IS_FOR_UI|MESH_HAS_TEXTURES|MESH_SHARED_TEXTURE_BUFFER);
            
            Renderer->GlyphMesh.TextureSSBO = Renderer->PTMesh.TextureSSBO;
            Renderer->GlyphMesh.Atlases = Renderer->PTMesh.Atlases;
            Renderer->GlyphMesh.Storage = Renderer->PTMesh.Storage;
            Renderer->GlyphMesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
            
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        assetpack_tag *Tag = Assetpack_FindFirstTag(Renderer->Assetpack, TAG_ATLAS_DESCRIPTOR);
        assetpack_atlas *Atlas = (assetpack_atlas*)Tag->ValueP;
        
        Renderer->GlyphMesh.AtlasesSampler = OpenGL_GetUniformLocation(Renderer->GlyphProgram, "Atlases");
        Renderer->GlyphMesh.AtlasSize      = OpenGL_GetUniformLocation(Renderer->GlyphProgram, "AtlasSize");
        
        OpenGL_UseProgram(Renderer->GlyphProgram);
        OpenGL_Uniform1i(Renderer->GlyphMesh.AtlasesSampler, 0);
        OpenGL_Uniform2ui(Renderer->GlyphMesh.AtlasSize, Atlas->Size.X, Atlas->Size.Y);
        
        Renderer->UILastModified[0] = VertTime;
        Renderer->UILastModified[1] = FragTime;
    }
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
    
    OpenGL_ClearColor(.2,.2,.2,1);
    
    Renderer->DEBUGCounter = 0;
    Renderer->Pos = (v3r32){0,1,-4};
    Renderer->Dir = (v3r32){0,0,0};
    Renderer->ViewMatrix = M4x4r32_I;
    Renderer->PerspectiveMatrix = M4x4r32_I;
    
    assetpack_tag *Tag = Assetpack_FindFirstTag(Renderer->Assetpack, TAG_ATLAS_DESCRIPTOR);
    Assert(Tag);
    assetpack_atlas *Atlas = (assetpack_atlas*)Tag->ValueP;
    
    Renderer_LoadPC3Program(Renderer, TRUE);
    Renderer_LoadPTProgram(Renderer, TRUE);
    Renderer_LoadGlyphProgram(Renderer, TRUE);
    
    ui_font Font = {0};
    Font.Assetpack = Renderer->Assetpack;
    ui_string UIString = MakeUIString(Renderer->Heap, CString("Hello,\nHow are you today?"), &Font);
    mesh_object Object = MakeUIStringObject(Renderer->Heap, UIString, (v2u32){20, 20}, WindowSize);
    
    // Heap_Free(UIString.Lines);
    
    
    // c08 Codepoint = 'A';
    // Tag = Assetpack_FindExactTag(Renderer->Assetpack, TAG_CODEPOINT, (vptr)(u64)Codepoint);
    // u32 Bytes = (u64)Tag->Assets[0] - (u64)Renderer->Assetpack.Assets;
    // Assert((Bytes & 3) == 0);
    
    // mesh_object Object;
    // Object.Vertices = Heap_Allocate(Renderer->Heap, 4*sizeof(glyph_vertex));
    // Object.Indices = Heap_Allocate(Renderer->Heap, 6*sizeof(u32));
    // u32 *Index = Object.Indices->Data;
    // glyph_vertex *Vertex = Object.Vertices->Data;
    // *Vertex++ = (glyph_vertex){{-0x7FFF, -4096}, (0<<24) | Bytes | 0b00};
    // *Vertex++ = (glyph_vertex){{ 0x7FFF, -4096}, (0<<24) | Bytes | 0b01};
    // *Vertex++ = (glyph_vertex){{ 0x7FFF,  4096}, (0<<24) | Bytes | 0b11};
    // *Vertex++ = (glyph_vertex){{-0x7FFF,  4096}, (0<<24) | Bytes | 0b10};
    // *Index++ = 0;
    // *Index++ = 1;
    // *Index++ = 2;
    // *Index++ = 0;
    // *Index++ = 2;
    // *Index++ = 3;
    // Object.TranslationMatrix = M4x4r32_I;
    // Object.ScalingMatrix = M4x4r32_I;
    // Object.RotationMatrix = M4x4r32_I;
    mesh_object *Objects[] = {&Object};
    Mesh_AddObjects(&Renderer->GlyphMesh, 1, Objects);
    Mesh_FreeObject(Object);
    
    Mesh_Update(&Renderer->GlyphMesh);
}

internal void
Renderer_Draw(renderer_state *Renderer)
{
    // Hot-reload the shaders
    Renderer_LoadPC3Program(Renderer, FALSE);
    Renderer_LoadPTProgram(Renderer, FALSE);
    Renderer_LoadGlyphProgram(Renderer, FALSE);
    
    OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    Mesh_Draw(&Renderer->PCMesh);
    Mesh_Draw(&Renderer->PTMesh);
    Mesh_Draw(&Renderer->GlyphMesh);
    
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