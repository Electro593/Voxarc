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
   ENUM(TEST, test) \

typedef enum block_type {
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