/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         * 
 *  Copyright (C) 2020 Aria Seiler                                       * 
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


internal void
World_CreateBlock(world_block *Block)
{
    Block->Color = V4u08_4x1(127, 0, 31, 255);
}

internal void
World_Create_Temp(world_chunk *Chunks,
                  heap *Heap)
{
    u32 ChunkCount = 1;
    u32 BlocksPerChunk = CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE;
    u32 ChunkSize = sizeof(world_chunk) + BlocksPerChunk*sizeof(world_block);
    Chunks = Heap_Allocate(Heap, ChunkCount * ChunkSize);
    
    for(u32 ChunkIndex = 0;
        ChunkIndex < ChunkCount;
        ++ChunkIndex)
    {
        world_chunk *Chunk = Chunks + ChunkCount;
        
        Mesh_Init(Heap, &Chunk->Mesh, MESH_HAS_COLORS);
        
        for(u32 BlockIndex = 0;
            BlockIndex < BlocksPerChunk;
            ++BlockIndex)
        {
            World_CreateBlock(Chunk->Blocks + BlockIndex);
        }
        
    }
    
    Mesh_AddObjects();
}