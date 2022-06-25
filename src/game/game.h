/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct global_state {
    stack *Stack;
} global_state;

struct game_state {
    b08 PostInitComplete;
    
    u32 DebugCounter;
    v2s32 PrevCursorPos;
    
    global_state GlobalState;
};

#define GAME_FUNCS \
    RENDERER_FUNCS \
    EXTERN(void, Game, Load,   platform_exports *PlatformFuncs, game_exports *GameFuncs, game_state *GameState, opengl_funcs *OpenGLFuncs) \
    EXPORT(void, Game, Init,   platform_state *Platform, game_state *Game, renderer_state *Renderer) \
    EXTERN(void, Game, Unload, game_state *GameState) \
    EXPORT(void, Game, Update, platform_state *Platform, game_state *Game, renderer_state *Renderer) \

#define EXPORT(ReturnType, Namespace, Name, ...) \
    typedef ReturnType func_##Namespace##_##Name(__VA_ARGS__);
#define EXTERN EXPORT
#define X GAME_FUNCS
#include <x.h>

struct game_exports {
    #define EXPORT(ReturnType, Namespace, Name, ...) \
        func_##Namespace##_##Name *Namespace##_##Name;
    #define X GAME_FUNCS
    #include <x.h>
};

#if defined(_GAME_MODULE)
    #define DEFAULT(ReturnType, Namespace, Name, ...) \
        internal ReturnType Namespace##_##Name(__VA_ARGS__);
    #define EXTERN(ReturnType, Namespace, Name, ...) \
        external API_EXPORT ReturnType Namespace##_##Name(__VA_ARGS__);
    #define X GAME_FUNCS
    #include <x.h>
#else
    #define EXPORT(ReturnType, Namespace, Name, ...) \
        global func_##Namespace##_##Name *Namespace##_##Name;
    #define EXTERN EXPORT
    #define X GAME_FUNCS
    #include <x.h>
#endif