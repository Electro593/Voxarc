/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


internal void
Game_Init(platform_state *Platform,
          game_state *Game,
          renderer_state *Renderer)
{
    Renderer_Init(Renderer);
    
    
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
    if(Platform->Updates & WINDOW_RESIZED) {
        Renderer_Resize(Platform->WindowSize);
        Platform->Updates &= ~WINDOW_RESIZED;
    }
    
    r32 R = R32_Abs((r32)(s08)Game->DebugCounter)/128.f;
    r32 G = 0;
    r32 B = 0;
    
    OpenGL_ClearColor(R, G, B, 1);
    Game->DebugCounter++;
    
    OpenGL_Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    Renderer_Draw(Renderer);
}