/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
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