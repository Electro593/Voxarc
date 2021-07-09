/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(GAME_VOX_GAME_H_)
#define      GAME_VOX_GAME_H_

// #define VIRTUAL_WINDOW_SIZE_X 1920
// #define VIRTUAL_WINDOW_SIZE_Y 1080
#define VIRTUAL_WINDOW_SIZE_X 1200
#define VIRTUAL_WINDOW_SIZE_Y 900

typedef struct global_state
{
    random Rand;
    stack *Stack;
    
    heap *StrHeap;
    #if defined(_VOX_DEBUG)
        heap_data_DEBUG StrHeapDebugData;
    #endif
} global_state;

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
    global_state GlobalState;
    
    heap *GeneralHeap;
    heap *MeshHeap;
    
    mesh UIMesh;
    
    asset_pack *AssetPack;
    
    // game_config Config;
    // player Player;
    // world World;
    
    m4x4r32 OrthographicMatrix;
    
    v2r32 AspectRatio;
    // v2s32 PrevCursorPos;
    
    u32 TestCounter;
} game_state;

typedef struct game_exports game_exports;

#define GAME__GAME__EXPORTS \
    PROC(void, Game_Init, platform_state *PlatformState, game_state *GameState, game_input *Input) \
    PROC(void, Game_Update, platform_state *PlatformState, game_state *GameState, game_input *Input) \
    PROC(void, Game_Cleanup, platform_state *PlatformState, game_state *GameState, game_input *Input) \

#define GAME__GAME__FUNCS \

#endif