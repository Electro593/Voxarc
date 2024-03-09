/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(INCLUDE_SOURCE) && !defined(INCLUDE_HEADER)
   #ifdef _RENDERER_OPENGL_MODULE
      #define _RENDERER_MODULE
   #endif
   
   #include <shared.h>
   
   #define INCLUDE_HEADER
      #include <util/main.c>
      #include <platform/platform.h>
      #include <renderer.h>
      #include <base/main.c>
      #include "main.c"
   #undef INCLUDE_HEADER
   
   #define INCLUDE_SOURCE
      #include "main.c"
   #undef INCLUDE_SOURCE
#else
   #ifdef INCLUDE_SOURCE
      global renderer_state _G;
      global renderer_funcs _F;
   #endif
   
   #include "opengl.h"
   #include "mesh.c"
   
   #ifdef INCLUDE_HEADER
      typedef enum shader_id {
         ShaderID_P3,
         ShaderID_PC2,
         ShaderID_PC3,
         ShaderID_PT3,
         ShaderID_Glyph,
         ShaderID_PNM3,
         
         Shader_Count
      } shader_id;

      c08 *ShaderPaths[] = {
         SHADERS_DIR "p.vert",
         SHADERS_DIR "p.frag",
         SHADERS_DIR "pc2.vert",
         SHADERS_DIR "pc2.frag",
         SHADERS_DIR "pc3.vert",
         SHADERS_DIR "pc3.frag",
         SHADERS_DIR "pt.vert",
         SHADERS_DIR "pt.frag",
         SHADERS_DIR "glyph.vert",
         SHADERS_DIR "glyph.frag",
         SHADERS_DIR "pnc3.vert",
         SHADERS_DIR "pnc3.frag",
      };

      typedef struct shader {
         mesh Mesh;
         
         datetime VertLastModified;
         datetime FragLastModified;
         
         u32 Program;
         b08 Initialized;
      } shader;

      typedef struct renderer_state {
         u32 VAO;
         u32 VBO;
         u32 EBO;
         
         shader Shaders[Shader_Count];
         
         m4x4r32 OrthographicMatrix;
         m4x4r32 PerspectiveMatrix;
         m4x4r32 ViewMatrix;
         
         v2u32 WindowSize;
         
         heap *Heap;
         
         assetpack Assetpack;
         
         HEAP(ui_component) Components;
         
         ui_font Font;
         u32 ObjectIndex;
      } renderer_state;
   #endif
   
   #ifdef INCLUDE_SOURCE
      external API_EXPORT void
      Load(
         platform_state *Platform,
         platform_module *Module)
      {
         _F = (renderer_funcs){
            #define EXPORT(R, N, ...) N,
            #define X RENDERER_FUNCS
            #include <x.h>
         };
         
         Module->Data = &_G;
         Module->Funcs = &_F;
         
         #define EXPORT(R, S, N, ...) S##_##N = Platform->Functions.S##_##N;
         #define X PLATFORM_FUNCS
         #include <x.h>
         
         platform_module *UtilModule = Platform_LoadModule("util");
         util_funcs *UtilFuncs = UtilModule->Funcs;
         #define EXPORT(R, N, ...) N = UtilFuncs->N;
         #define X UTIL_FUNCS
         #include <x.h>
         
         platform_module *GameModule = Platform_LoadModule("base");
         game_funcs *GameFuncs = GameModule->Funcs;
         #define EXPORT(R, N, ...) N = GameFuncs->N;
         #define X GAME_FUNCS
         #include <x.h>
         
         Platform_CreateWindow();
         opengl_funcs *OpenGLFuncs = Platform_LoadOpenGL();
         #define IMPORT(R, N, ...) OpenGL_##N = OpenGLFuncs->OpenGL_##N;
         #define X OPENGL_FUNCS
         #include <x.h>
      }
      
      internal void API_ENTRY
      OpenGL_DebugCallback(
         u32 Source,
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
      Renderer_LoadProgram(
         c08 *VertFileName,
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
      Renderer_LoadShader(
         renderer_state *Renderer,
         shader_id ShaderID,
         mesh_flags Flags)
      {
         c08 *VertName = ShaderPaths[2*ShaderID+0];
         c08 *FragName = ShaderPaths[2*ShaderID+1];
         
         datetime VertTime, FragTime;
         Platform_GetFileTime(VertName, 0, 0, &VertTime);
         Platform_GetFileTime(FragName, 0, 0, &FragTime);
         
         shader *Shader = &Renderer->Shaders[ShaderID];
         b32 N  = Flags & MESH_HAS_NORMALS;
         b32 T  = Flags & MESH_HAS_TEXTURES;
         b32 C  = Flags & MESH_HAS_COLORS;
         b32 d3 = Flags & MESH_HAS_PERSPECTIVE;
         b32 UI = Flags & MESH_IS_FOR_UI;
         
         if(!Shader->Initialized ||
            Platform_CmpFileTime(Shader->VertLastModified, VertTime) == LESS ||
            Platform_CmpFileTime(Shader->FragLastModified, FragTime) == LESS)
         {
            Shader->VertLastModified = VertTime;
            Shader->FragLastModified = FragTime;
            
            if(Shader->Initialized)
                OpenGL_DeleteProgram(Shader->Program);
            
            Shader->Program = Renderer_LoadProgram(VertName, FragName);
            
            assetpack_atlas Atlas = FindFirstAssetFromExactTag(Renderer->Assetpack, TAG_ATLAS_DESC, &(u32){0})->Atlas;
            m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
            
            if(!Shader->Initialized) {
               if(T) Shader->Mesh.TextureIndex = 0;
               
               Mesh_Init(&Shader->Mesh, Renderer->Heap, &Shader->Program, Flags);
               Shader->Initialized = TRUE;
               
               if(T && !UI) {
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                  OpenGL_TexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, Atlas.Size.X, Atlas.Size.Y, Atlas.Count, 0, GL_RGBA, GL_UNSIGNED_BYTE, Renderer->Assetpack.AssetData+Atlas.DataOffset);
                  OpenGL_BindSampler(0, Shader->Mesh.SamplerObject);
                  
                  Heap_Resize(Shader->Mesh.Storage, Renderer->Assetpack.Header->AssetsSize);
                  Mem_Cpy(Shader->Mesh.Storage->Data, Renderer->Assetpack.Assets, Shader->Mesh.Storage->Size);
                  Shader->Mesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
               } else if(UI) {
                  Shader->Mesh.TextureSSBO = Renderer->Shaders[ShaderID_PT3].Mesh.TextureSSBO;
                  Shader->Mesh.Atlases     = Renderer->Shaders[ShaderID_PT3].Mesh.Atlases;
                  Shader->Mesh.Storage     = Renderer->Shaders[ShaderID_PT3].Mesh.Storage;
                  
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                  OpenGL_SamplerParameteri(Shader->Mesh.SamplerObject, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
               }
               
               if(FLAG_SET(Flags, MESH_HAS_MATERIALS)) {
                  material Material = {
                     (v3u08){150, 150, 160},
                     (v3u08){170, 170, 170},
                     (v3u08){200, 200, 200},
                     2,
                  };
                  // ((Mat0 >> 16) & 0xFF) / 255.0d
                  Heap_Resize(Shader->Mesh.Materials, sizeof(material));
                  ((material*)Shader->Mesh.Materials->Data)[0] = Material;
                  OpenGL_BindBuffer(GL_SHADER_STORAGE_BUFFER, Shader->Mesh.MaterialSSBO);
                  OpenGL_BufferData(GL_SHADER_STORAGE_BUFFER, Shader->Mesh.Materials->Size, Shader->Mesh.Materials->Data, GL_STATIC_DRAW);
               }
               
               if(T) Shader->Mesh.Flags |= MESH_GROW_TEXTURE_BUFFER;
            }
            
            Shader->Mesh.Color          = OpenGL_GetUniformLocation(Shader->Program, "Color");
            Shader->Mesh.AtlasesSampler = OpenGL_GetUniformLocation(Shader->Program, "Atlases");
            Shader->Mesh.AtlasSize      = OpenGL_GetUniformLocation(Shader->Program, "AtlasSize");
            Shader->Mesh.VPMatrix       = OpenGL_GetUniformLocation(Shader->Program, "VPMatrix");
            Shader->Mesh.LightPos       = OpenGL_GetUniformLocation(Shader->Program, "LightPos");
            Shader->Mesh.CameraPos      = OpenGL_GetUniformLocation(Shader->Program, "CameraPos");
            
            if(!N && !T && !C &&  d3) {
               OpenGL_UniformMatrix4fv(Shader->Mesh.VPMatrix, 1, FALSE, VPMatrix);
               OpenGL_Uniform4f       (Shader->Mesh.Color,    1, 1, 1, 1);
            } else if(!N && !T &&  C && !d3) {
               OpenGL_UniformMatrix4fv(Shader->Mesh.VPMatrix, 1, FALSE, Renderer->OrthographicMatrix);
            } else if(!N && !T &&  C &&  d3) {
               OpenGL_UniformMatrix4fv(Shader->Mesh.VPMatrix, 1, FALSE, VPMatrix);
            } else if(!N &&  T && !C &&  d3) {
               OpenGL_Uniform1i       (Shader->Mesh.AtlasesSampler, 0);
               OpenGL_Uniform2ui      (Shader->Mesh.AtlasSize, Atlas.Size.X, Atlas.Size.Y);
               OpenGL_UniformMatrix4fv(Shader->Mesh.VPMatrix, 1, FALSE, VPMatrix);
            } else if( N && !T &&  C &&  d3) {
               OpenGL_UniformMatrix4fv(Shader->Mesh.VPMatrix, 1, FALSE, VPMatrix);
               OpenGL_Uniform3f(Shader->Mesh.CameraPos, 0, 0, 0);
            } else if(UI) {
               OpenGL_Uniform1i(Shader->Mesh.AtlasesSampler, 0);
               OpenGL_Uniform2ui(Shader->Mesh.AtlasSize, Atlas.Size.X, Atlas.Size.Y);
            }
         }
      }

      internal void
      Renderer_Resize(
         v2u32 NewSize,
         m4x4r32 *OrthographicMatrix,
         m4x4r32 *PerspectiveMatrix)
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
      Renderer_SetUniform_V3r32(
         renderer_state *Renderer,
         u32 ShaderID,
         u32 Uniform,
         v3r32 Value)
      {
         OpenGL_UseProgram(Renderer->Shaders[ShaderID].Program);
         OpenGL_Uniform3f(Uniform, Value.X, Value.Y, Value.Z);
      }

      internal void
      Renderer_SetUniform_M4x4r32(
         renderer_state *Renderer,
         u32 ShaderID,
         u32 Uniform,
         u32 Count,
         b08 Transpose,
         m4x4r32 Value)
      {
         OpenGL_UseProgram(Renderer->Shaders[ShaderID].Program);
         OpenGL_UniformMatrix4fv(Uniform, Count, Transpose, Value);
      }
      
      internal void
      Renderer_Draw(
         platform_state *Platform,
         game_state *Game,
         renderer_state *Renderer,
         r32 FPS)
      {
         // Hot-reload the shaders
         Renderer_LoadShader(Renderer, ShaderID_P3,    0);
         Renderer_LoadShader(Renderer, ShaderID_PC2,   0);
         Renderer_LoadShader(Renderer, ShaderID_PC3,   0);
         Renderer_LoadShader(Renderer, ShaderID_PT3,   0);
         Renderer_LoadShader(Renderer, ShaderID_Glyph, 0);
         Renderer_LoadShader(Renderer, ShaderID_PNM3,  0);
         
         Stack_Push();
         string String = CString("");
         
         String = String_Cat(String, CString("Position (X, Y, Z): "));
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
         
         String = String_Cat(String, CString("Acceleration (X, Y, Z): "));
         String = String_Cat(String, R32_ToString(Game->Acceleration.X, 4));
         String = String_Cat(String, CString(", "));
         String = String_Cat(String, R32_ToString(Game->Acceleration.Y, 4));
         String = String_Cat(String, CString(", "));
         String = String_Cat(String, R32_ToString(Game->Acceleration.Z, 4));
         String = String_Cat(String, CString("\n"));
         
         String = String_Cat(String, CString("WalkStep (X, Z): "));
         String = String_Cat(String, S32_ToString(Game->WalkStep.X));
         String = String_Cat(String, CString(", "));
         String = String_Cat(String, S32_ToString(Game->WalkStep.Z));
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
         
         String = String_Cat(String, CString("Time of Day: "));
         String = String_Cat(String, U32_ToString(Game->TimeOfDay, 10));
         String = String_Cat(String, CString("\n"));
         
         String = String_Cat(String, CString("FPS: "));
         String = String_Cat(String, R32_ToString(FPS, 1));
         String = String_Cat(String, CString("\n"));
         
         ui_component *Componenet = (ui_component*)Renderer->Components->Data + 1;
         Componenet->String = String;
         ui_style InheritedStyle = GetInheritedStyle(Componenet);
         
         ui_string UIString = MakeUIString(Renderer->Heap, String, InheritedStyle);
         mesh_object Object = MakeUIStringObject(Renderer->Heap, UIString, (v2u32){10, 0}, Renderer->WindowSize);
         FreeUIString(UIString);
         
         Object.Index = Renderer->ObjectIndex;
         mesh *GlyphMesh = &Renderer->Shaders[ShaderID_Glyph].Mesh;
         Mesh_Bind(GlyphMesh);
         
         Mesh_UpdateVertices(GlyphMesh, Object);
         Mesh_UpdateIndices(GlyphMesh, Object);
         Mesh_UpdateMatrix(GlyphMesh, Object);
         
         Mesh_FreeObject(Object);
         
         OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
         
         Mesh_Draw(&Renderer->Shaders[ShaderID_PT3].Mesh,  GL_TRIANGLES);
         Mesh_Draw(&Renderer->Shaders[ShaderID_PC3].Mesh,  GL_TRIANGLES);
         Mesh_Draw(&Renderer->Shaders[ShaderID_PNM3].Mesh, GL_TRIANGLES);
         
         if(Game->AimBlockValid)
            Mesh_DrawPartial(&Renderer->Shaders[ShaderID_P3].Mesh, GL_LINES, Game->AimBlockObjectIndex, 1);
         
         if(Platform->CursorIsDisabled)
            Mesh_DrawPartial(&Renderer->Shaders[ShaderID_PC2].Mesh, GL_TRIANGLES, Game->CrosshairObjectIndex, 1);
         
         OpenGL_Disable(GL_DEPTH_TEST);
         Mesh_Draw(GlyphMesh, GL_TRIANGLES);
         OpenGL_Enable(GL_DEPTH_TEST);
         
         Stack_Pop();
      }
      
      external API_EXPORT void
      Init(
         platform_state *Platform)
      {
         renderer_state *Renderer = &_G;
         heap *Heap = Renderer->Heap;
         v2u32 WindowSize = Platform->WindowSize;
         
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
         
         OpenGL_ClearColor(.5,.5,.5,1);
         
         Renderer->OrthographicMatrix = M4x4r32_I;
         Renderer->PerspectiveMatrix = M4x4r32_I;
         Renderer->ViewMatrix = M4x4r32_I;
         Renderer->WindowSize = WindowSize;
         
         Renderer_LoadShader(Renderer, ShaderID_P3,    0);
         Renderer_LoadShader(Renderer, ShaderID_PC2,   MESH_HAS_ELEMENTS|MESH_HAS_COLORS);
         Renderer_LoadShader(Renderer, ShaderID_PC3,   MESH_HAS_ELEMENTS|MESH_HAS_COLORS|MESH_HAS_PERSPECTIVE);
         Renderer_LoadShader(Renderer, ShaderID_PT3,   MESH_HAS_ELEMENTS|MESH_HAS_TEXTURES|MESH_HAS_PERSPECTIVE);
         Renderer_LoadShader(Renderer, ShaderID_Glyph, MESH_HAS_ELEMENTS|MESH_HAS_TEXTURES|MESH_IS_FOR_UI|MESH_SHARED_TEXTURE_BUFFER);
         Renderer_LoadShader(Renderer, ShaderID_PNM3,  MESH_HAS_ELEMENTS|MESH_HAS_NORMALS|MESH_HAS_MATERIALS|MESH_HAS_PERSPECTIVE);
         
         Mem_Set(&Renderer->Font, 0, sizeof(ui_font));
         Renderer->Font.Assetpack = Renderer->Assetpack;
         
         Renderer->Components = Heap_Allocate(Heap, sizeof(ui_component) * 2);
         ui_component *Components = Renderer->Components->Data;
         
         ui_component *Root = Components+0;
         *Root = DEFAULT_COMPONENT;
         Root->Style.Visible = FALSE;
         Root->Style.Padding = (v4u32){20, 20, 20, 20};
         Root->Style.ZIndex = 0;
         Root->Style.FontSize = 30;
         Root->Style.TabSize = 80;
         Root->Style.BackgroundColor = (v4u08){100, 100, 100, 255};
         Root->Style.BorderColor = (v4u08){20, 20, 20, 255};
         Root->Style.BorderSize = (v4u32){2, 2, 2, 2};
         Root->Style.Font = &Renderer->Font;
         
         ui_component *DebugText = Components+1;
         *DebugText = DEFAULT_COMPONENT;
         DebugText->Parent = Root;
         
         Renderer->ObjectIndex = Mesh_ReserveObject(&Renderer->Shaders[ShaderID_Glyph].Mesh, 0, 0);
      }
      
      external API_EXPORT void
      Update(
         platform_state *Platform)
      {
         
      }
      
      external API_EXPORT void
      Unload(
         platform_state *Platform)
      {
         
      }
   #endif
#endif