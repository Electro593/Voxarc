#ifndef VOX_WORLD_H_
#define VOX_WORLD_H_

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

#include "util/vox_defines.h"
#include "math/vox_v3u32.h"
#include "math/vox_v4r32.h"
#include "math/vox_m4x4r32.h"

typedef struct mesh_data
{
    memory_handle *Vertices;
    memory_handle *Colors;
    memory_handle *Indices;
} mesh_data;

typedef struct mesh
{
    mesh_data Data;
    m4r ModelMatrix;
    u32 VertexBufferID;
    u32 ColorBufferID;
    u32 ElementBufferID;
} mesh;

typedef struct render_data
{
    s32 ModelMatrixID;
    s32 ViewMatrixID;
    s32 MVPMatrixID;
    s32 LightPositionID;
    s32 LightPowerID;
    s32 LightColorID;
    s32 ChannelMaskID;
    s32 ChannelRangeID;
    s32 ChannelRangeShiftID;
    s32 ChannelShiftID;
    m4r ViewMatrix;
    m4r ProjectionMatrix;
} render_data;

typedef struct block
{
    v4r Color;
} block;

typedef struct chunk
{
    v3u Pos;
    block Blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
    u32 MaxVertices;
    mesh OpaqueMesh;
    mesh TransparentMesh;
    b08 Changed;
    b08 IsInitialized;
} chunk;

typedef struct world
{
    u32 ChunksBuiltThisTick;
    memory_handle *Chunks;
} world;

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

#endif