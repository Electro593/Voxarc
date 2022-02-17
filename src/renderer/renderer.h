/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_OPENGL)
    #include <renderer/opengl/opengl.h>
    #include <renderer/opengl/render.h>
#endif

#define RENDERER_FUNCS \
    EXPORT(void, Renderer, Init,        renderer_state *Renderer) \
    EXPORT(void, Renderer, Draw,        renderer_state *Renderer) \
    EXPORT(void, Renderer, Resize,      v2u32 NewSize) \
    INTERN(u32,  OpenGL,   LoadShaders, c08 *VertFileName, c08 *FragFileName) \
    INTERN(void API_ENTRY, OpenGL, DebugCallback, u32 Source, u32 Type, u32 ID, u32 Severity, s32 Length, c08 *Message, vptr UserParam) \
