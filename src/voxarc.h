#ifndef VOXARC_H_

#include "util/vox_defines.h"
#include "math/vox_random.h"
#include "util/vox_assets.h"
#include "util/vox_render.h"
#include "util/vox_debug.h"

struct game_state
{
    handle_pool *ChunksHandlePool;
    handle_pool *DebugHandlePool;
    handle_pool *AssetsHandlePool;
    handle_pool *TagSearchHandlePool;
    handle_pool *UtilHandlePool;
    
    random Random;
    config Config;
    asset_memory AssetMemory;
    render_data_3d RenderData3D;
    render_data_2d RenderData2D;
    debug_data DebugData;
    player Player;
    world World;
    
    v2u32 WindowDimensions;
    v2s32 PrevCursorPos;
};

struct platform_flags
{
    b08 GameIsRunning;
    b08 GameIsFocused;
    b08 DimensionsChanged;
    b08 CursorIsCaptured;
};

struct game_input
{
    b08 Keys[256];
    v2s32 CursorPos;
};

#define VOXARC_H_
#endif