#ifndef UTIL_VOX_FILE_IO_H_

#include "math/vox_v3u32.h"

typedef struct config
{
    u32 VisionBitsPerChannel;
    u32 VisionChannelBinary;
    u32 FOV;
    r32 Sensitivity;
    v3u RenderDistance;
} config;

#define UTIL_VOX_FILE_IO_H_
#endif