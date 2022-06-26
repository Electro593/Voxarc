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