/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

global_state __Global;

#include <util/scalar.c>
#include <util/vector.c>
#include <util/memory.c>
#include <util/string.c>
#include <util/set.c>
#include <game/msdf.c>
#include <renderer/font.c>
#include <game/file.c>
#include <renderer/opengl/mesh.c>
#include <game/ui.c>
#include <renderer/opengl/render.c>
#include <game/world.c>


internal void
Game_Init(platform_state *Platform,
          game_state *Game,
          renderer_state *Renderer)
{
   
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
   
}

external void API_EXPORT
Game_Load(platform_exports *PlatformFuncs,
          game_exports *GameFuncs,
          game_state *GameState,
          opengl_funcs *OpenGLFuncs)
{
   if(PlatformFuncs) {
      #define EXPORT(ReturnType, Namespace, Name, ...) \
         Namespace##_##Name = PlatformFuncs->Namespace##_##Name;
      #define X PLATFORM_FUNCS
      #include <x.h>
   }
   
   if(GameFuncs) {
      #define EXPORT(ReturnType, Namespace, Name, ...) \
         GameFuncs->Namespace##_##Name = Namespace##_##Name;
      #define X GAME_FUNCS
      #include <x.h>
   }
   
   if(GameState) {
      __Global = GameState->GlobalState;
   }
   
   Renderer_Load(OpenGLFuncs);
}