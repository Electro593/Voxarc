/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define BLOCK_TEXTURE_PREFIX "assets\\blocks\\"

#define BLOCK_TYPES \
   ENUM(TEST,  test) \
   ENUM(GRASS, grass) \
   ENUM(DIRT,  dirt) \
   ENUM(STONE, stone) \

typedef enum block_type {
   BLOCK_NONE,
   
   #define ENUM(EnumName, FileName) \
      BLOCK_##EnumName,
   BLOCK_TYPES
   #undef ENUM
   
   BLOCK_Count
} block_type;

global c08 *BlockTexturePaths[BLOCK_Count] = {
   "",
   
   #define ENUM(EnumName, FileName) \
      BLOCK_TEXTURE_PREFIX #FileName ".bmp",
   BLOCK_TYPES
   #undef ENUM
};

typedef struct chunk {
   v3s32 Pos;
   HEAP(block_type) Blocks;
   mesh_object Object;
} chunk;

typedef union region_node {
   u16 Chunks[8];
   u16 Children[8];
} region_node;

typedef struct region {
   v3s32 Pos;
   HEAP(chunk) Chunks;
   HEAP(region_node) Nodes;
} region;

global v3u32 ChunkDims = {16, 16, 16};
global v3u32 RegionDims = {16, 16, 16};