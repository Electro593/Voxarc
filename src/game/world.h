/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_WORLD_H_
#define GAME_WORLD_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Defines
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//TODO: Make these configurable

#define CHUNK_SIZE 8


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


typedef enum world_chunk_flag_bits
{
    WorldChunkFlag_Dirty = 1,
} world_chunk_flag_bits;
typedef flag08 chunk_flags;

typedef struct world_block
{
    v4u08 Color;
} world_block;

typedef struct world_chunk
{
    world_block *Blocks; // Morton + RLE Encoding
    
    mesh_object Object;
    
    chunk_flags Flags;
} world_chunk;
// RLE Format: <NBlocks><BlockID>[Metadata] ...
// Metadata is an array of data, the size of each element is determined by BlockID


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#define GAME__WORLD__EXPORTS \

#define GAME__WORLD__FUNCS \
    FUNC(void, World_Create_Temp, world_chunk *Chunks, heap *Heap) \


#endif