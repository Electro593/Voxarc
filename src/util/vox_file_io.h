#ifndef UTIL_VOX_FILE_IO_H_

#include "math/vox_v3u32.h"

typedef struct config
{
    u32 VisionBitsPerChannel;
    u32 VisionChannelBinary;
    u32 FOV;
    r32 Sensitivity;
    v3u RenderDistance;
    u32 MoveLeft;
    u32 MoveForward;
    u32 MoveRight;
    u32 MoveBack;
    u32 MoveUp;
    u32 MoveDown;
} config;

typedef enum file_type
{
    FileType_Bitmap,
} file_type;

#define UTIL_VOX_FILE_IO_H_
#endif