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
   #include "render.c"
   
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
         
         mesh *GlyphMesh = &Renderer->Shaders[ShaderID_Glyph].Mesh;
         mesh_object *Objects[] = {&Object};
         Mesh_AddObjects(GlyphMesh, 1, Objects);
         
         Mesh_Update(GlyphMesh);
         
         Renderer->ObjectIndex = Object.Index;
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