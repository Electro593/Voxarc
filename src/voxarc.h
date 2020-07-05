#ifndef VOXARC_H_

#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "util/vox_file_io.h"
#include "math/vox_v2s32.h"
#include "math/vox_random.h"
#include "game/vox_world.h"
#include "game/vox_entity.h"

typedef struct game_state
{
    handle_pool *ChunksHandlePool;
    handle_pool *UtilHandlePool;
    random Random;
    config Config;
    render_data RenderData;
    player Player;
    world World;
    u32 Program3DID;
    r32 WindowAspectRatio;
    v2s32 PrevCursorPos;
} game_state;

typedef struct platform_flags
{
    b08 GameIsRunning;
    b08 GameIsFocused;
    b08 AspectRatioChanged;
    b08 CursorIsCaptured;
} platform_flags;

typedef struct game_input
{
    b08 Keys[256];
    v2s32 CursorPos;
} game_input;

#define VOXARC_H_
#endif