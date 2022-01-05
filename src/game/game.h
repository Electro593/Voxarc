/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_GAME_H_
#define GAME_GAME_H_

// #define VIRTUAL_WINDOW_SIZE_X 1920
// #define VIRTUAL_WINDOW_SIZE_Y 1080
#define VIRTUAL_WINDOW_SIZE_X 1200
#define VIRTUAL_WINDOW_SIZE_Y 900

typedef struct game_config
{
    u32 VisionBitsPerChannel;
    u32 VisionChannelBinary;
    u32 FOV;
    r32 Sensitivity;
    v3u32 RenderDistance;
    u32 MoveLeft;
    u32 MoveForward;
    u32 MoveRight;
    u32 MoveBack;
    u32 MoveUp;
    u32 MoveDown;
} game_config;

typedef struct game_input
{
    key_state Keys[KEYCODE_LAST + 1];
    button_state Buttons[BUTTON_LAST + 1];
    v2s32 CursorPos;
    r32 DeltaWheelX;
    r32 DeltaWheelY;
} game_input;

typedef struct game_state
{
    heap *GeneralHeap;
    heap *MeshHeap;
    
    mesh UIMesh;
    
    asset_pack *AssetPack;
    
    game_config Config;
    // player Player;
    // world World;
    
    m4x4r32 OrthographicMatrix;
    
    v2r32 AspectRatio;
    // v2s32 PrevCursorPos;
    
    u32 TestCounter;
} game_state;

typedef struct game_exports game_exports;

#define GAME__GAME__EXPORTS \
    FUNC(void, Game_Init,    platform_state *PlatformState, game_state *GameState) \
    FUNC(void, Game_Update,  platform_state *PlatformState, game_module *GameModule) \
    FUNC(void, Game_Cleanup, util_state *UtilState, game_state *GameState) \

#define GAME__GAME__FUNCS \

#endif