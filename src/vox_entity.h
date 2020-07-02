#ifndef VOX_ENTITY_H_

#include "util/vox_defines.h"
#include "math/vox_v3r32.h"
#include "math/vox_m4x4r32.h"

typedef struct player
{
    r32 Yaw;
    r32 Pitch;
    r32 Speed;
    v3r Pos;
} player;

#define VOX_ENTITY_H_
#endif