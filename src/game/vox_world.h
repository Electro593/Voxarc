#ifndef GAME_VOX_WORLD_H_

#include "util/vox_defines.h"
#include "util/vox_memory.h"
#include "util/vox_render.h"
#include "math/vox_m4x4r32.h"
#include "math/vox_v4r32.h"
#include "math/vox_v3u32.h"

#define WORLD_SIZE_X 4
#define WORLD_SIZE_Y 4
#define WORLD_SIZE_Z 4

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16

#define BLOCK_SIZE_X 1
#define BLOCK_SIZE_Y 1
#define BLOCK_SIZE_Z 1

#define CHUNKS_BUILT_PER_TICK 1

//TODO: Greedy mesh with textures

struct block
{
    v4r32 Color;
};

struct chunk
{
    v3u32 Pos;
    block Blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
    u32 MaxVertices;
    mesh_data_3d OpaqueMesh;
    mesh_data_3d TransparentMesh;
    b08 Changed;
    b08 IsInitialized;
};

struct world
{
    u32 ChunksBuiltThisTick;
    memory_handle *Chunks;
};

/*                         2 _____________________________ 6
                           /  _______________________    /  
                         /  / ______________________/  / |  
                       /  / /| |                  /  / / |  
                     /  / /  | |                /  / / | |  
                   /  / / |  | |              /  / /|  | |  
                 /  /_/___|__|_|____________/  / /  |  | |  
             3 /_____________________________/ /    |  | |  
              |   _________________________ 7| |    |  | |  
              |  | |      |  | |          |  | |    |  | |  
              |  | |      |  | |__________|  | |____|  | |  
              |  | |      | 0|____________|  | |____|  | |  
              |  | |     /  / ____________|  | |____/  / / 4
              |  | |   /  / /             |  | |   /  / /   
              |  | | /  / /               |  | | /  / /     
              |  | |  / /                 |  | |  / /       
              |  | |/_/___________________|  | |/ /         
              |  |________________________|  |  /           
              |______________________________|/             
              1                              5              
                                                            
                        y                                   
                        |                                   
                        |                                   
                        |________x                          
                       /                                    
                     /                                      
                   z                                        
                                                            
                                                            */

#define GAME_VOX_WORLD_H_
#endif