#ifndef UTIL_VOX_FILE_IO_H_

#include "util/vox_defines.h"

struct config
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
};

enum file_type
{
    FileType_Bitmap,
};

#define UTIL_VOX_FILE_IO_H_
#endif