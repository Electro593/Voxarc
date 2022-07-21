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
    Stack_Push();
    
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
        OpenGL_AttachShader(Program, Vert);
    }
    
    if(FragFileName) {
        Frag = OpenGL_CreateShader(GL_FRAGMENT_SHADER);
        FragCode = File_Read(FragFileName, 0, 0);
        OpenGL_ShaderSource(Frag, 1, &FragCode.Text, NULL);
        OpenGL_CompileShader(Frag);
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
    
    Stack_Pop();
    
    return Program;
}

internal void
Renderer_LoadPProgram(renderer_state *Renderer, b08 FirstTime)
{
    c08 *VertName = SHADERS_DIR "p.vert";
    c08 *FragName = SHADERS_DIR "p.frag";
    
    datetime VertTime, FragTime;
    Platform_GetFileTime(VertName, 0, 0, &VertTime);
    Platform_GetFileTime(FragName, 0, 0, &FragTime);
    
    if(FirstTime ||
       Platform_CmpFileTime(Renderer->PLastModified[0], VertTime) == LESS ||
       Platform_CmpFileTime(Renderer->PLastModified[1], FragTime) == LESS)
    {
        if(!FirstTime)
            OpenGL_DeleteProgram(Renderer->PProgram);
        
        Renderer->PProgram = Renderer_LoadShaders(VertName, FragName);
        
        if(FirstTime)
            Mesh_Init(&Renderer->PMesh, Renderer->Heap, &Renderer->PProgram, 0);
        
        Renderer->PMesh.P.VPMatrix = OpenGL_GetUniformLocation(Renderer->PProgram, "VPMatrix");
        Renderer->PMesh.P.Color    = OpenGL_GetUniformLocation(Renderer->PProgram, "Color");
        
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        
        OpenGL_UseProgram(Renderer->PProgram);
        OpenGL_UniformMatrix4fv(Renderer->PMesh.P.VPMatrix, 1, FALSE, VPMatrix);
        OpenGL_Uniform4f(Renderer->PMesh.P.Color, 1, 1, 1, 1);
        
        Renderer->PLastModified[0] = VertTime;
        Renderer->PLastModified[1] = FragTime;
    }
}

internal void
Renderer_LoadPC2Program(renderer_state *Renderer, b08 FirstTime)
{
    c08 *VertName = SHADERS_DIR "pc2.vert";
    c08 *FragName = SHADERS_DIR "pc2.frag";
    
    datetime VertTime, FragTime;
    Platform_GetFileTime(VertName, 0, 0, &VertTime);
    Platform_GetFileTime(FragName, 0, 0, &FragTime);
    
    if(FirstTime ||
       Platform_CmpFileTime(Renderer->PC2LastModified[0], VertTime) == LESS ||
       Platform_CmpFileTime(Renderer->PC2LastModified[1], FragTime) == LESS)
    {
        if(!FirstTime)
            OpenGL_DeleteProgram(Renderer->PC2Program);
        
        Renderer->PC2Program = Renderer_LoadShaders(VertName, FragName);
        
        if(FirstTime)
            Mesh_Init(&Renderer->PC2Mesh, Renderer->Heap, &Renderer->PC2Program, MESH_HAS_ELEMENTS|MESH_HAS_COLORS);
        
        Renderer->PC2Mesh.VPMatrix = OpenGL_GetUniformLocation(Renderer->PC2Program, "VPMatrix");
        
        OpenGL_UseProgram(Renderer->PC2Program);
        OpenGL_UniformMatrix4fv(Renderer->PC2Mesh.VPMatrix, 1, FALSE, Renderer->OrthographicMatrix);
        
        Renderer->PC2LastModified[0] = VertTime;
        Renderer->PC2LastModified[1] = FragTime;
    }
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
       Platform_CmpFileTime(Renderer->PC3LastModified[0], VertTime) == LESS ||
       Platform_CmpFileTime(Renderer->PC3LastModified[1], FragTime) == LESS)
    {
        if(!FirstTime)
            OpenGL_DeleteProgram(Renderer->PC3Program);
        
        Renderer->PC3Program = Renderer_LoadShaders(VertName, FragName);
        
        if(FirstTime)
            Mesh_Init(&Renderer->PC3Mesh, Renderer->Heap, &Renderer->PC3Program, MESH_HAS_ELEMENTS|MESH_HAS_COLORS);
        
        Renderer->PC3Mesh.VPMatrix = OpenGL_GetUniformLocation(Renderer->PC3Program, "VPMatrix");
        
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        
        OpenGL_UseProgram(Renderer->PC3Program);
        OpenGL_UniformMatrix4fv(Renderer->PC3Mesh.VPMatrix, 1, FALSE, VPMatrix);
        
        Renderer->PC3LastModified[0] = VertTime;
        Renderer->PC3LastModified[1] = FragTime;
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
        
        assetpack_atlas Atlas = FindFirstAssetFromExactTag(Renderer->Assetpack, TAG_ATLAS_DESC, &(u32){0})->Atlas;
        m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
        
        if(FirstTime) {
            Renderer->PTMesh.TextureIndex = 0;
            Mesh_Init(&Renderer->PTMesh, Renderer->Heap, &Renderer->PTProgram, MESH_HAS_ELEMENTS|MESH_HAS_TEXTURES);
            
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            OpenGL_SamplerParameteri(Renderer->PTMesh.SamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            OpenGL_TexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, Atlas.Size.X, Atlas.Size.Y, Atlas.Count, 0, GL_RGBA, GL_UNSIGNED_BYTE, Renderer->Assetpack.AssetData+Atlas.DataOffset);
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
        OpenGL_Uniform2ui(Renderer->PTMesh.AtlasSize, Atlas.Size.X, Atlas.Size.Y);
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
            Mesh_Init(&Renderer->GlyphMesh, Renderer->Heap, &Renderer->GlyphProgram, MESH_IS_FOR_UI|MESH_HAS_ELEMENTS|MESH_HAS_TEXTURES|MESH_SHARED_TEXTURE_BUFFER);
            
            Renderer->GlyphMesh.TextureSSBO = Renderer->PTMesh.TextureSSBO;
            Renderer->GlyphMesh.Atlases = Renderer->PTMesh.Atlases;
            Renderer->GlyphMesh.Storage = Renderer->PTMesh.Storage;
            Renderer->GlyphMesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
            
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            OpenGL_SamplerParameteri(Renderer->GlyphMesh.SamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        
        assetpack_atlas Atlas = FindFirstAssetFromExactTag(Renderer->Assetpack, TAG_ATLAS_DESC, &(u32){0})->Atlas;
        
        Renderer->GlyphMesh.AtlasesSampler = OpenGL_GetUniformLocation(Renderer->GlyphProgram, "Atlases");
        Renderer->GlyphMesh.AtlasSize      = OpenGL_GetUniformLocation(Renderer->GlyphProgram, "AtlasSize");
        
        OpenGL_UseProgram(Renderer->GlyphProgram);
        OpenGL_Uniform1i(Renderer->GlyphMesh.AtlasesSampler, 0);
        OpenGL_Uniform2ui(Renderer->GlyphMesh.AtlasSize, Atlas.Size.X, Atlas.Size.Y);
        
        Renderer->UILastModified[0] = VertTime;
        Renderer->UILastModified[1] = FragTime;
    }
}

internal void
Renderer_Resize(v2u32 NewSize, m4x4r32 *OrthographicMatrix, m4x4r32 *PerspectiveMatrix)
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
    
    *OrthographicMatrix = (m4x4r32){
        1/AspectRatio, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
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
    
    Renderer->OrthographicMatrix = M4x4r32_I;
    Renderer->PerspectiveMatrix = M4x4r32_I;
    Renderer->ViewMatrix = M4x4r32_I;
    Renderer->WindowSize = WindowSize;
    
    Renderer_LoadPProgram(Renderer, TRUE);
    Renderer_LoadPC2Program(Renderer, TRUE);
    Renderer_LoadPC3Program(Renderer, TRUE);
    Renderer_LoadPTProgram(Renderer, TRUE);
    Renderer_LoadGlyphProgram(Renderer, TRUE);
    
    Mem_Set(&Renderer->Font, 0, sizeof(ui_font));
    Renderer->Font.Assetpack = Renderer->Assetpack;
    
    ui_style Style;
    Style.Font = &Renderer->Font;
    Style.ZIndex = 0;
    Style.TabSize = 80.0f;
    Style.FontSize = 30.0f;
    Style.Size = (v2u32){800, 800};
    Style.StringOffset = (v2u32){20, 20};
    
    Renderer->Style = Style;
    
    ui_string UIString = MakeUIString(Renderer->Heap, CString("Position (X,Y,Z): "), Style);
    mesh_object Object = MakeUIStringObject(Renderer->Heap, UIString, (v2u32){10, 0}, WindowSize);
    
    mesh_object *Objects[] = {&Object};
    Mesh_AddObjects(&Renderer->GlyphMesh, 1, Objects);
    
    Mesh_Update(&Renderer->GlyphMesh);
    
    Renderer->ObjectIndex = Object.Index;
}

internal void
Renderer_Draw(platform_state *Platform,
              game_state *Game,
              renderer_state *Renderer,
              r32 FPS)
{
    // Hot-reload the shaders
    Renderer_LoadPProgram(Renderer, FALSE);
    Renderer_LoadPC2Program(Renderer, FALSE);
    Renderer_LoadPC3Program(Renderer, FALSE);
    Renderer_LoadPTProgram(Renderer, FALSE);
    Renderer_LoadGlyphProgram(Renderer, FALSE);
    
    
    
    Stack_Push();
    
    string String = CString("Position (X, Y, Z): ");
    String = String_Cat(String, R32_ToString(Game->Pos.X, 4));
    String = String_Cat(String, CString(", "));
    String = String_Cat(String, R32_ToString(Game->Pos.Y, 4));
    String = String_Cat(String, CString(", "));
    String = String_Cat(String, R32_ToString(Game->Pos.Z, 4));
    String = String_Cat(String, CString("\n"));
    
    String = String_Cat(String, CString("Velocity (X, Y, Z): "));
    String = String_Cat(String, R32_ToString(Game->Velocity.X, 4));
    String = String_Cat(String, CString(", "));
    String = String_Cat(String, R32_ToString(Game->Velocity.Y, 4));
    String = String_Cat(String, CString(", "));
    String = String_Cat(String, R32_ToString(Game->Velocity.Z, 4));
    String = String_Cat(String, CString("\n"));
    
    String = String_Cat(String, CString("WalkStep (X, Z): "));
    String = String_Cat(String, S32_ToString(Game->WalkStep.X));
    String = String_Cat(String, CString(", "));
    String = String_Cat(String, S32_ToString(Game->WalkStep.Z));
    String = String_Cat(String, CString("\n"));
    
    String = String_Cat(String, CString("Jump Charge: "));
    String = String_Cat(String, R32_ToString(Game->JumpCharge, 2));
    String = String_Cat(String, CString("\n"));
    
        String = String_Cat(String, CString("Looking At: "));
    if(Game->AimBlockValid) {
        String = String_Cat(String, S32_ToString(Game->AimBlock.X));
        String = String_Cat(String, CString(", "));
        String = String_Cat(String, S32_ToString(Game->AimBlock.Y));
        String = String_Cat(String, CString(", "));
        String = String_Cat(String, S32_ToString(Game->AimBlock.Z));
        String = String_Cat(String, CString("\n"));
    } else {
        String = String_Cat(String, CString("None\n"));
    }
    
    String = String_Cat(String, CString("Direction (Pitch, Yaw): "));
    String = String_Cat(String, R32_ToString(Game->Dir.X, 4));
    String = String_Cat(String, CString(", "));
    String = String_Cat(String, R32_ToString(Game->Dir.Y, 4));
    String = String_Cat(String, CString("\n"));
    
    String = String_Cat(String, CString("FPS: "));
    String = String_Cat(String, R32_ToString(FPS, 1));
    String = String_Cat(String, CString("\n"));
    
    ui_string UIString = MakeUIString(Renderer->Heap, String, Renderer->Style);
    mesh_object Object = MakeUIStringObject(Renderer->Heap, UIString, (v2u32){10, 0}, Renderer->WindowSize);
    Object.Index = Renderer->ObjectIndex;
    
    Mesh_Bind(&Renderer->GlyphMesh);
    
    Mesh_UpdateVertices(&Renderer->GlyphMesh, Object);
    Mesh_UpdateIndices(&Renderer->GlyphMesh, Object);
    Mesh_UpdateMatrix(&Renderer->GlyphMesh, Object);
    
    Mesh_FreeObject(Object);
    
    OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    Mesh_Draw(&Renderer->PTMesh, GL_TRIANGLES);
    
    if(Game->AimBlockValid)
        Mesh_DrawPartial(&Renderer->PMesh, GL_LINES, Game->AimBlockObjectIndex, 1);
    
    if(Platform->CursorIsDisabled)
        Mesh_DrawPartial(&Renderer->PC2Mesh, GL_TRIANGLES, Game->CrosshairObjectIndex, 1);
    
    OpenGL_Disable(GL_DEPTH_TEST);
    Mesh_Draw(&Renderer->GlyphMesh, GL_TRIANGLES);
    OpenGL_Enable(GL_DEPTH_TEST);
    
    Stack_Pop();
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