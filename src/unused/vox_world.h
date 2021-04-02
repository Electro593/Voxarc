/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Andrew Seiler                                       * 
 *                                                                         * 
 *  This program is free software: you can redistribute it and/or modify   * 
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      * 
 *  (at your option) any later version.                                    * 
 *                                                                         * 
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           * 
 *  GNU General Public License for more details.                           * 
 *                                                                         * 
 *  You should have received a copy of the GNU General Public License      * 
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  * 
 *                                                                         * 
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(GAME_VOX_WORLD_H_)

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

typedef struct block
{
    v4r32 Color;
} block;

typedef struct chunk
{
    v3u32 Pos;
    block Blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
    u32 MaxVertices;
    mesh_data_3d OpaqueMesh;
    mesh_data_3d TransparentMesh;
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

internal block
CreateBlock(v4r32 Color)
{
    block Block = {0};
    Block.Color = Color;
    
    return Block;
}

internal block
GetBlockFromRelativePos(world *World,
                        v3u32 ChunkPos,
                        v3s32 BlockPos)
{
    v3s32 Direction = V3s32_3x1((s32)R32_Floor((r32)BlockPos.X / (r32)CHUNK_SIZE_X),
                                (s32)R32_Floor((r32)BlockPos.Y / (r32)CHUNK_SIZE_Y),
                                (s32)R32_Floor((r32)BlockPos.Z / (r32)CHUNK_SIZE_Z));
    
    if(ChunkPos.X + Direction.X >= 0 &&
       ChunkPos.X + Direction.X < WORLD_SIZE_X &&
       ChunkPos.Y + Direction.Y >= 0 &&
       ChunkPos.Y + Direction.Y < WORLD_SIZE_Y &&
       ChunkPos.Z + Direction.Z >= 0 &&
       ChunkPos.Z + Direction.Z < WORLD_SIZE_Z)
    {
        v3u32 ChunkPosToSearch = V3u32_3x1(ChunkPos.X + Direction.X,
                                     ChunkPos.Y + Direction.Y,
                                     ChunkPos.Z + Direction.Z);
        
        
        v3u32 NewBlockPos = V3u32_3x1(BlockPos.X - ((ChunkPosToSearch.X - ChunkPos.X) * CHUNK_SIZE_X),
                                  BlockPos.Y - ((ChunkPosToSearch.Y - ChunkPos.Y) * CHUNK_SIZE_Y),
                                  BlockPos.Z - ((ChunkPosToSearch.Z - ChunkPos.Z) * CHUNK_SIZE_Z));
        
        //TODO: Make an array of memory handles, and sort them by xyz to make this easier
        for(size ChunkIndex = 0;
            ChunkIndex < World->Chunks->Size / sizeof(chunk);
            ++ChunkIndex)
        {
            chunk *CurrChunk = (chunk*)World->Chunks->Base + ChunkIndex;
            if(CurrChunk && V3u32_Equal(CurrChunk->Pos, ChunkPosToSearch))
            {
                return CurrChunk->Blocks[INDEX_3D(NewBlockPos.X, NewBlockPos.Y, NewBlockPos.Z,
                                        CHUNK_SIZE_X, CHUNK_SIZE_Y)];
                break;
            }
        }
    }
    return CreateBlock(V4r32_4x1(0.0f, 0.0f, 0.0f, 0.0f));
}

internal chunk
CreateChunk(memory_handle *Handle,
            game_config Config,
            random *Random,
            v3u32 Pos)
{
    //u32 VertexCount = 6 * 2 * 3 * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
    
    chunk Chunk;
    u32 MaxVertices = 0;
    Chunk.MaxVertices = 0;
    for(s32 X = 0; X < CHUNK_SIZE_X; X++)
    {
        for(s32 Z = 0; Z < CHUNK_SIZE_Z; Z++)
        {
            u32 YMax = CHUNK_SIZE_Y - 1;
            u32 YMin = 0;
            for(s32 Y = 0; Y < CHUNK_SIZE_Y; Y++)
            {
                v4r32 Color;
                Chunk.MaxVertices += 6 * 2 * 3;
                if((Pos.Y * CHUNK_SIZE_Y) + Y <= YMax && (Pos.Y * CHUNK_SIZE_Y) + Y >= YMin)
                {
                    Color = V4r32_4x1(255.0f, 255.0f, 255.0f, 255.0f);
                    r32 Gradient = (r32)Y / (r32)(Config.RenderDistance.Y * CHUNK_SIZE_Y);
                    Color = V4r32_Mul(Color, V4r32_4x1(Gradient, Gradient, Gradient, 1.0f));
                    MaxVertices += 6 * 2 * 3;
                }
                else
                {
                    Color = V4r32_4x1(0.0f, 0.0f, 0.0f, 0.0f);
                }
                
                Chunk.Blocks[INDEX_3D(X, Y, Z, CHUNK_SIZE_X, CHUNK_SIZE_Y)] = CreateBlock(Color);
            }
        }
    }
    
    Chunk.Pos = Pos;
    Chunk.Changed = TRUE;
    
    u32 MaxTVertices = Chunk.MaxVertices - MaxVertices;
    
    handle_pool *HandlePool = GetHandlePool(Handle);
    
    Chunk.OpaqueMesh.Vertices = AllocateHandle(HandlePool, MaxVertices * sizeof(v3r32));
    Chunk.OpaqueMesh.Colors = AllocateHandle(HandlePool, MaxVertices * sizeof(v4r32));
    Chunk.OpaqueMesh.Indices = AllocateHandle(HandlePool, 1);
    //Chunk.TransparentMesh.Data.Vertices = AllocateHandle(Handle, MaxTVertices * sizeof(v3r));
    //Chunk.TransparentMesh.Data.Colors = AllocateHandle(Handle, MaxTVertices * sizeof(v4r));
    //Chunk.TransparentMesh.Data.Indices = AllocateHandle(Handle, 1);
    
    return Chunk;
}

internal void
BuildChunk(world *World,
           chunk *Chunk,
           render_data_3d *RenderData)
{
    //TODO: Calcualte this
    u32 MaxIndices = 6 * 2 * 3 * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
    
    mesh_data_3d *OData = &Chunk->OpaqueMesh;
    //mesh_data *TData = &Chunk->TransparentMesh.Data;
    u16 OIndex = 0;
    //u16 TIndex = 0;
    
    v3r32 Scale = V3r32_3x1(1.0f, 1.0f, 1.0f);
    v3u32 ChunkSize = V3u32_3x1(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
    v3r32 BlockSize = V3r32_Mul(Scale, V3r32_3x1(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z));
    
    u32 FaceOffsetY = CHUNK_SIZE_Y * CHUNK_SIZE_Z;
    u32 FaceOffsetZ = FaceOffsetY + CHUNK_SIZE_Z * CHUNK_SIZE_X;
    v3u32 FaceOffsets = V3u32_3x1(0, FaceOffsetY, FaceOffsetZ);
    v4r32 Faces[CHUNK_SIZE_Y*CHUNK_SIZE_Z + CHUNK_SIZE_Z*CHUNK_SIZE_X + CHUNK_SIZE_X*CHUNK_SIZE_Y];
    
    u32 Axis1, Axis2, Axis3;
    
    v3u32 Pos;
    v3s32 Dir;
    
    for(Axis1 = 0;
        Axis1 < 3;
        ++Axis1)
    {
        Axis2 = (Axis1 + 1) % 3;
        Axis3 = (Axis1 + 2) % 3;
        
        Dir.X = Dir.Y = Dir.Z = 0;
        
        v3r32 DAxis2;
        DAxis2.X = DAxis2.Y = DAxis2.Z = 0;
        v3r32 DAxis3;
        DAxis3.X = DAxis3.Y = DAxis3.Z = 0;
        
        for(s32 Sign = -1;
            Sign < 2;
            Sign += 2)
        {
            Dir.E[Axis1] = Sign;
            
            for(Pos.E[Axis1] = 0;
                Pos.E[Axis1] < ChunkSize.E[Axis1];
                ++Pos.E[Axis1])
            {
                b08 Empty = TRUE;
                
                for(Pos.E[Axis2] = 0;
                    Pos.E[Axis2] < ChunkSize.E[Axis2];
                    ++Pos.E[Axis2])
                {
                    for(Pos.E[Axis3] = 0;
                        Pos.E[Axis3] < ChunkSize.E[Axis3];
                        ++Pos.E[Axis3])
                    {
                        block Block1 = GetBlockFromRelativePos(World, Chunk->Pos, V3s32_3x1(Pos.X, Pos.Y, Pos.Z));
                        block Block2 = GetBlockFromRelativePos(World, Chunk->Pos, V3s32_3x1(Pos.X + Dir.X, Pos.Y + Dir.Y, Pos.Z + Dir.Z));
                        
                        if(Block1.Color.W > R32_EPSILON)
                        {
                            Empty = FALSE;
                            
                            if(Block1.Color.W - Block2.Color.W > R32_EPSILON)
                            {
                                Faces[FaceOffsets.E[Axis1] + INDEX_2D(Pos.E[Axis2], Pos.E[Axis3], ChunkSize.E[Axis2])] = Block1.Color;
                                continue;
                            }
                        }
                        Faces[FaceOffsets.E[Axis1] + INDEX_2D(Pos.E[Axis2], Pos.E[Axis3], ChunkSize.E[Axis2])] = V4r32_1(0.0f);
                    }
                }
                
                if(Empty)
                {
                    continue;
                }
                
                for(u32 A3Index = 0;
                    A3Index < ChunkSize.E[Axis3];
                    ++A3Index)
                {
                    for(u32 A2Index = 0;
                        A2Index < ChunkSize.E[Axis2];
                        )
                    {
                        u32 Width = 1;
                        u32 Height = 1;
                        
                        if(Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index, A3Index, ChunkSize.E[Axis2])].W)
                        {
                            while(A2Index + Width < ChunkSize.E[Axis2] &&
                                  Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index + Width, A3Index, ChunkSize.E[Axis2])].W &&
                                  V4r32_Equal(Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index + Width, A3Index, ChunkSize.E[Axis2])],
                                              Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index, A3Index, ChunkSize.E[Axis2])]))
                            {
                                Width++;
                            }
                            
                            for(;
                                A3Index + Height < ChunkSize.E[Axis3];
                                ++Height)
                            {
                                for(u32 GreedyWidth = 0;
                                    GreedyWidth < Width;
                                    ++GreedyWidth)
                                {
                                    if(!Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index + GreedyWidth, A3Index + Height, ChunkSize.E[Axis2])].W ||
                                       V4r32_NotEqual(Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index + GreedyWidth, A3Index + Height, ChunkSize.E[Axis2])],
                                                      Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index, A3Index, ChunkSize.E[Axis2])]))
                                    {
                                        goto done;
                                    }
                                }
                            }
                            done:
                            
                            v4r32 Color = V4r32_DivS(Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index, A3Index, ChunkSize.E[Axis2])], 255.0f);
                            
                            v3r32 FPos;
                            FPos.E[Axis1] = Pos.E[Axis1] * BlockSize.E[Axis1];
                            FPos.E[Axis2] = A2Index * BlockSize.E[Axis2];
                            FPos.E[Axis3] = A3Index * BlockSize.E[Axis3];
                            
                            DAxis2.E[Axis2] = Width * BlockSize.E[Axis2];
                            DAxis3.E[Axis3] = Height * BlockSize.E[Axis3];
                            
                            if(Sign > 0)
                            {
                                FPos.E[Axis1] += BlockSize.E[Axis1];
                            }
                            
                            v3r32 GreedyVertices[4];
                            GreedyVertices[0] = FPos;
                            GreedyVertices[1] = V3r32_Add(FPos, DAxis2);
                            GreedyVertices[2] = V3r32_Add(FPos, V3r32_Add(DAxis2, DAxis3));
                            GreedyVertices[3] = V3r32_Add(FPos, DAxis3);
                            
                            u32 FaceIndices[] = {0, 1, 2, 0, 2, 3, 0, 3, 2, 0, 2, 1};
                            
                            for(s32 VertexIndex = 0;
                                VertexIndex < 6;
                                ++VertexIndex)
                            {
                                if(Color.A > (1.0f - R32_EPSILON))
                                {
                                    *((v3r32*)OData->Vertices->Base + OIndex) = GreedyVertices[FaceIndices[(Sign > 0) ?
                                                                                           0 + VertexIndex :
                                                                                           6 + VertexIndex]];
                                    *((v4r32*)OData->Colors->Base + OIndex) = Color;
                                    ++OIndex;
                                }
                                else
                                {
                                    //*(TVerticesData + TIndex) = GreedyVertices[FaceIndices[(Sign > 0) ?
                                    //                                                       0 + VertexIndex :
                                    //                                                       6 + VertexIndex]];
                                    //*(TColorsData + TIndex) = Color;
                                }
                            }
                            
                            for(u32 GreedyHeight = 0;
                                GreedyHeight < Height;
                                ++GreedyHeight)
                            {
                                Mem_Set(&Faces[FaceOffsets.E[Axis1] + INDEX_2D(A2Index, A3Index + GreedyHeight, ChunkSize.E[Axis2])],
                                          0, Width * sizeof(v4r32));
                            }
                        }
                        
                        A2Index += Width;
                    }
                }
            }
        }
    }
    
    //TODO: Collapse IndexVertices into the above loop?
    
    OData->Indices = ResizeHandle(OData->Indices, OIndex * sizeof(u16));
    //TIndices = ResizeHandleHandle(TIndices, TIndex * sizeof(u16));
    Index3DVertices(OData->Vertices, OData->Colors, OData->Indices);
    //IndexVertices(TData->Vertices, TColors, TIndices);
    Chunk->OpaqueMesh.ModelMatrix = M4x4r32_Translate(M4x4r32_0(), (r32)Pos.X * CHUNK_SIZE_X,
                                                                   (r32)Pos.Y * CHUNK_SIZE_Y,
                                                                   (r32)Pos.Z * CHUNK_SIZE_Z);
    //Chunk->TransparentMesh = CreateMesh(Chunk->TransparentMesh.Data, Chunk->Pos);
}

internal world
CreateWorld(memory_handle *Handle,
            game_config Config)
{
    world World;
    v3u32 Size = Config.RenderDistance;
    u32 RenderVolume = Size.X * Size.Y * Size.Z;
    World.Chunks = AllocateHandle(GetHandlePool(Handle), RenderVolume * sizeof(chunk));
    World.ChunksBuiltThisTick = 0;
    return World;
}

internal void
RenderWorld(memory_handle *Handle,
            world *World,
            game_config Config,
            random *Random,
            render_data_3d *RenderData)
{
    World->ChunksBuiltThisTick = 0;
    
    v3u32 Size = Config.RenderDistance;
    
    for(u32 X = 0;
        X < Size.X;
        ++X)
    {
        for(u32 Y = 0;
            Y < Size.Y;
            ++Y)
        {
            for(u32 Z = 0;
                Z < Size.Z;
                ++Z)
            {
                chunk *Chunk = (chunk*)World->Chunks->Base + INDEX_3D(X, Y, Z, Size.X, Size.Y);
                if(World->ChunksBuiltThisTick < CHUNKS_BUILT_PER_TICK)
                {
                    if(!Chunk->IsInitialized)
                    {
                        *Chunk = CreateChunk(Handle, Config, Random, V3u32_3x1(X, Y, Z));
                        Chunk->IsInitialized = TRUE;
                    }
                    
                    if(Chunk->Changed)
                    {
                        BuildChunk(World, Chunk, RenderData);
                        Chunk->Changed = FALSE;
                        ++World->ChunksBuiltThisTick;
                    }
                }
                if(Chunk->IsInitialized)
                {
                    Render3DMesh(RenderData, &Chunk->OpaqueMesh);
                }
            }
        }
    }
}

#define GAME_VOX_WORLD_H_
#endif