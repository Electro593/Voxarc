/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef INCLUDE_HEADER

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

#define WORLD_FUNCS \

#endif



#ifdef INCLUDE_SOURCE

/*

   7 ___________ 6
    /|         /|
   / |        / |
3 /__|_______/2 |
  | 4|_______|__| 5
  |  /       |  /
  | /        | /
  |/_________|/
0             1

*/

global u32 CubePositions[] = {
   0b01100000000110000000011000000001,
   0b01100000000110000000010111111111,
   0b01100000000101111111110111111111,
   0b01100000000101111111111000000001,
   0b01011111111110000000011000000001,
   0b01011111111110000000010111111111,
   0b01011111111101111111110111111111,
   0b01011111111101111111111000000001
};

internal mesh_object
MakePTBlockObject(mesh *Mesh, heap *Heap, v3r32 Pos, u32 BytesFromFirstAsset)
{
   persist u32 Indices[] = {
       0, 1, 2,   0, 2, 3,
       4, 5, 6,   4, 6, 7,
       8, 9,10,   8,10,11,
      12,13,14,  12,14,15,
      16,17,18,  16,18,19,
      20,21,22,  20,22,23
   };
   
   mesh_object Object;
   
   // Object.Vertices = Heap_Allocate(Heap, sizeof(pt_vertex)*4*6);
   Object.Vertices = Heap_Allocate(Heap, sizeof(pt_vertex)*4);
   // Object.Indices  = Heap_Allocate(Heap, sizeof(Indices));
   Object.Indices  = Heap_Allocate(Heap, sizeof(u32)*6);
   
   pt_vertex *Vertex = (vptr)Object.Vertices->Data;
   u32       *Index  = (vptr)Object.Indices->Data;
   
   Assert((BytesFromFirstAsset & 3) == 0 && BytesFromFirstAsset <= 0xFFFFFF);
   
   // *Vertex++ = (pt_vertex){Positions[0], BytesFromFirstAsset | 0b00};
   // *Vertex++ = (pt_vertex){Positions[1], BytesFromFirstAsset | 0b01};
   // *Vertex++ = (pt_vertex){Positions[2], BytesFromFirstAsset | 0b11};
   // *Vertex++ = (pt_vertex){Positions[3], BytesFromFirstAsset | 0b10};
   // *Vertex++ = (pt_vertex){Positions[1], BytesFromFirstAsset | 0b00};
   // *Vertex++ = (pt_vertex){Positions[5], BytesFromFirstAsset | 0b01};
   // *Vertex++ = (pt_vertex){Positions[6], BytesFromFirstAsset | 0b11};
   // *Vertex++ = (pt_vertex){Positions[2], BytesFromFirstAsset | 0b10};
   // *Vertex++ = (pt_vertex){Positions[5], BytesFromFirstAsset | 0b00};
   // *Vertex++ = (pt_vertex){Positions[4], BytesFromFirstAsset | 0b01};
   // *Vertex++ = (pt_vertex){Positions[7], BytesFromFirstAsset | 0b11};
   // *Vertex++ = (pt_vertex){Positions[6], BytesFromFirstAsset | 0b10};
   // *Vertex++ = (pt_vertex){Positions[4], BytesFromFirstAsset | 0b00};
   // *Vertex++ = (pt_vertex){Positions[0], BytesFromFirstAsset | 0b01};
   // *Vertex++ = (pt_vertex){Positions[3], BytesFromFirstAsset | 0b11};
   // *Vertex++ = (pt_vertex){Positions[7], BytesFromFirstAsset | 0b10};
   // *Vertex++ = (pt_vertex){Positions[3], BytesFromFirstAsset | 0b00};
   // *Vertex++ = (pt_vertex){Positions[2], BytesFromFirstAsset | 0b01};
   // *Vertex++ = (pt_vertex){Positions[6], BytesFromFirstAsset | 0b11};
   // *Vertex++ = (pt_vertex){Positions[7], BytesFromFirstAsset | 0b10};
   // *Vertex++ = (pt_vertex){Positions[4], BytesFromFirstAsset | 0b00};
   // *Vertex++ = (pt_vertex){Positions[5], BytesFromFirstAsset | 0b01};
   // *Vertex++ = (pt_vertex){Positions[1], BytesFromFirstAsset | 0b11};
   // *Vertex++ = (pt_vertex){Positions[0], BytesFromFirstAsset | 0b10};
   *Vertex++ = (pt_vertex){CubePositions[0], (0<<28) | (2<<24) | BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){CubePositions[1], (0<<28) | (2<<24) | BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){CubePositions[2], (0<<28) | (2<<24) | BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){CubePositions[3], (0<<28) | (2<<24) | BytesFromFirstAsset | 0b10};
   
   // Mem_Cpy(Object.Indices->Data, Indices, sizeof(Indices));
   Mem_Cpy(Object.Indices->Data, Indices, sizeof(u32)*6);
   
   Object.TranslationMatrix = M4x4r32_Translation(Pos.X, Pos.Y, Pos.Z);
   Object.ScalingMatrix = M4x4r32_Scaling(0.5, 0.5, 0.5);
   Object.RotationMatrix = M4x4r32_I;
   
   return Object;
}

internal b08
GetChunk(region *Region, v3s32 Pos, u32 *ChunkIndexOut)
{
   v3u32 Start = {0};
   v3u32 Dims = RegionDims;
   u32 PrevNode = 0;
   
   region_node *Nodes = Region->Nodes->Data;
   chunk *Chunks = Region->Chunks->Data;
   
   while(TRUE) {
      Dims = V3u32_DivS(Dims, 2);
      v3u32 I = {0, 0, 0};
      if(Pos.X >= Start.X + Dims.X) I.X++, Start.X += Dims.X;
      if(Pos.Y >= Start.Y + Dims.Y) I.Y++, Start.Y += Dims.Y;
      if(Pos.Z >= Start.Z + Dims.Z) I.Z++, Start.Z += Dims.Z;
      
      u32 J = INDEX_3D(I.X, I.Y, I.Z, 2, 2);
      
      if(Nodes[PrevNode].Children[J] == 0)
         return FALSE;
      
      if(Dims.X == 1 && Dims.Y == 1 && Dims.Z == 1) {
         if(ChunkIndexOut) *ChunkIndexOut = Nodes[PrevNode].Chunks[J]-1;
         return TRUE;
      }
      
      PrevNode = Nodes[PrevNode].Children[J];
   }
}

internal chunk *
AddChunkToRegion(region *Region, v3s32 ChunkPos)
{
   chunk *Chunk;
   
   v3u32 Start = {0};
   v3u32 Dims = RegionDims;
   u32 PrevNode = 0;
   region_node *Nodes = Region->Nodes->Data;
   chunk *Chunks = Region->Chunks->Data;
   while(TRUE) {
      Dims = V3u32_DivS(Dims, 2);
      v3u32 I = {0, 0, 0};
      if(ChunkPos.X >= Start.X + Dims.X) I.X++, Start.X += Dims.X;
      if(ChunkPos.Y >= Start.Y + Dims.Y) I.Y++, Start.Y += Dims.Y;
      if(ChunkPos.Z >= Start.Z + Dims.Z) I.Z++, Start.Z += Dims.Z;
      
      u32 J = INDEX_3D(I.X, I.Y, I.Z, 2, 2);
      
      if(Dims.X > 1 && Dims.Y > 1 && Dims.Z > 1) {
         if(Nodes[PrevNode].Children[J] == 0) {
            u32 NodeIndex = Region->Nodes->Size / sizeof(region_node);
            Heap_Resize(Region->Nodes, Region->Nodes->Size+sizeof(region_node));
            Nodes = Region->Nodes->Data;
            
            region_node *Node = Nodes+NodeIndex;
            Mem_Set(Node, 0, sizeof(region_node));
            
            Nodes[PrevNode].Children[J] = NodeIndex;
            PrevNode = NodeIndex;
         } else {
            PrevNode = Nodes[PrevNode].Children[J];
         }
      } else {
         Assert(Nodes[PrevNode].Chunks[J] == 0);
         
         u32 ChunkIndex = Region->Chunks->Size / sizeof(chunk);
         Heap_Resize(Region->Chunks, Region->Chunks->Size+sizeof(chunk));
         Chunks = Region->Chunks->Data;
         
         Chunk = Chunks+ChunkIndex;
         
         Nodes[PrevNode].Chunks[J] = ChunkIndex+1;
         break;
      }
   }
   
   return Chunk;
}

internal void
MakeChunk(heap *Heap, region *Region, mesh *Mesh, v3s32 ChunkPos, u32 *TextureBytes)
{
   chunk *Chunk = AddChunkToRegion(Region, ChunkPos);
   
   // NOTE: Must be <= the 'repeat' max in the pt shaders
   
   Chunk->Pos = ChunkPos;
   Chunk->Blocks = Heap_Allocate(Heap, V3u32_Volume(ChunkDims)*sizeof(block_type));
   block_type *Blocks = Chunk->Blocks->Data;
   Mem_Set(Blocks, BLOCK_NONE, Chunk->Blocks->Size);
   
   for(u32 Y = 0; Y < 4; Y++) {
      block_type Type = BLOCK_NONE;
      if(Y == 0) Type = BLOCK_STONE;
      else if(Y <= 2) Type = BLOCK_DIRT;
      else if(Y == 3) Type = BLOCK_GRASS;
      
      for(u32 Z = 0; Z < ChunkDims.Z; Z++) {
         for(u32 X = 0; X < ChunkDims.X; X++) {
            Blocks[INDEX_3D(X, Y, Z, ChunkDims.X, ChunkDims.Y)] = Type;
         }
      }
   }
   
   Blocks[INDEX_3D(7, 4, 7, ChunkDims.X, ChunkDims.Y)] = BLOCK_GRASS;
   Blocks[INDEX_3D(7, 4, 8, ChunkDims.X, ChunkDims.Y)] = BLOCK_GRASS;
   Blocks[INDEX_3D(8, 4, 7, ChunkDims.X, ChunkDims.Y)] = BLOCK_GRASS;
   Blocks[INDEX_3D(8, 4, 8, ChunkDims.X, ChunkDims.Y)] = BLOCK_GRASS;
   
   Blocks[INDEX_3D(0, 3, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_TEST;
   
   Blocks[INDEX_3D(15, 1, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   Blocks[INDEX_3D(15, 2, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   Blocks[INDEX_3D(15, 3, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   Blocks[INDEX_3D(14, 2, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   Blocks[INDEX_3D(14, 3, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   Blocks[INDEX_3D(14, 2, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   Blocks[INDEX_3D(13, 3, 0, ChunkDims.X, ChunkDims.Y)] = BLOCK_NONE;
   
   Stack_Push();
   
   u32 Size = V3u32_Volume(ChunkDims)*sizeof(u08);
   u08 *Consumed = Stack_Allocate(Size);
   Mem_Set(Consumed, 0, Size);
   
   u32 VertexIndex = 0;
   u32 VertexCount = 12;
   Chunk->Object.Vertices = Heap_Allocate(Heap, VertexCount*4*Mesh->VertexSize);
   Chunk->Object.Indices = Heap_Allocate(Heap, VertexCount*6*sizeof(u32));
   pt_vertex *Vertex = Chunk->Object.Vertices->Data;
   u32       *Index  = Chunk->Object.Indices->Data;
   
   // A highly compacted greedy meshing algorithm
   //
   // Key:
   // - F is the current face. +X (0), -X (1), +Y (2), -Y (3), +Z (4), -Z (5).
   // - S is the current side. X (0), Y (1), or Z (2).
   // - S1 and S2 are the other, non-S sides. They're also the directions
   //   for the UV coordinates, respectively.
   // - D is whether the face is on the negative (-1) or positive side (1).
   // - Flag is to avoid clearing the Consumed bitmap each time.
   // - B is short for Bound. It's to prevent an edge block from trying
   //   to access a block in another chunk.
   // - Min/Max: The bounds for the consolidated block.
   // - P is just the current block's position. Since it gets reset each
   //   iteration, it's also used in the merging loop.
   // - I is the index of the current block.
   // - DI is the index of the next block, as specified by D. Its block
   //   is checked to make sure that the current block isn't being covered.
   // - C is the dimensions of the chunk.
   // - T is just the type of the current block.
   //
   // TODO: Could use a heuristic where, instead of just
   // quitting when the next row is smaller, we could calculate
   // the area and if including the smaller row (and decreasing
   // its length) would end up increasing it, we'd do that.
   // Not sure if it's more efficient though. 
   v3u32 C = ChunkDims;
   for(u32 Z = 0; Z < C.Z; Z++) {
      for(u32 Y = 0; Y < C.Y; Y++) {
         for(u32 X = 0; X < C.X; X++) {
            for(u32 F = 0; F < 6; F++) {
               // Setup
               v3u32 P = {X, Y, Z};
               
               u32 S  = F / 2;
               u32 S1 = (S == 0) ? 2 : 0;
               u32 S2 = (S == 1) ? 2 : 1;
               
               v3s32 D;
               D.E[S ] = (F % 2)*-2 + 1;
               D.E[S1] = 0;
               D.E[S2] = 0;
               
               u08 Flag = 1 << F;
               
               u32 B = (D.E[S] == 1) ? C.E[S]-1 : 0;
               
               v3u32 Max;
               Max.E[S ] = P.E[S];
               Max.E[S1] = C.E[S1]-1;
               Max.E[S2] = C.E[S2]-1;
               v3u32 Min = P;
               
               u32 I = INDEX_3D(X, Y, Z, C.X, C.Y);
               u32 DI = INDEX_3D(X+D.X, Y+D.Y, Z+D.Z, C.X, C.Y);
               
               block_type T = Blocks[I];
               
               // Don't try to merge an invalid or already merged block.
               if((Consumed[I] & Flag) || T == BLOCK_NONE ||
                  (P.E[S] != B && Blocks[DI] != BLOCK_NONE))
                  continue;
               
               // Merge loop:
               // For each 'V' coordinate:
               for(; P.E[S2] < C.E[S2]; P.E[S2]++) {
                  // For each 'U' coordinate:
                  for(P.E[S1] = Min.E[S1]; P.E[S1] <= Max.E[S1]; P.E[S1]++) {
                     I = INDEX_3D(P.X, P.Y, P.Z, C.X, C.Y);
                     DI = INDEX_3D(P.X+D.X, P.Y+D.Y, P.Z+D.Z, C.X, C.Y);
                     // If the current face has already been merged, or
                     // it's a different type, or it's being covered, don't
                     // merge it.
                     if((Consumed[I] & Flag) || Blocks[I] != T || (P.E[S] != B && Blocks[DI] != BLOCK_NONE))
                        break;
                  }
                  // If this is the first iteration (Min.E[S2] stores the
                  // original P.E[S2]), set our max 'U' coordinate.
                  if(P.E[S2] == Min.E[S2]) Max.E[S1] = P.E[S1] - 1;
                  // If we broke from the loop, this row is too short, so
                  // give up on it.
                  else if(P.E[S1] <= Max.E[S1]) break;
                  // We're merging this row, so consume it.
                  for(P.E[S1] = Min.E[S1]; P.E[S1] <= Max.E[S1]; P.E[S1]++)
                     Consumed[INDEX_3D(P.X, P.Y, P.Z, C.X, C.Y)] |= Flag;
               }
               // Set our max 'V' coordinate.
               Max.E[S2] = P.E[S2] - 1;
               
               // Potentially resize.
               if(VertexIndex == VertexCount) {
                  Heap_Resize(Chunk->Object.Vertices, (VertexCount+12)*4*Mesh->VertexSize);
                  Heap_Resize(Chunk->Object.Indices,  (VertexCount+12)*6*sizeof(u32));
                  Vertex = (pt_vertex*)Chunk->Object.Vertices->Data + 4*VertexCount;
                  Index  =       (u32*)Chunk->Object.Indices->Data  + 6*VertexCount;
                  VertexCount += 12;
               }
               
               // Encode the number of repeated blocks to send to the GPU.
               u32 RepU, RepV;
               if(S == 0) RepU = (Max.Z - Min.Z) << 24;
               else       RepU = (Max.X - Min.X) << 24;
               if(S == 1) RepV = (Max.Z - Min.Z) << 28;
               else       RepV = (Max.Y - Min.Y) << 28;
               
               // Map the 0..C coordinates to -1..1.
               v3r32 MinPos, MaxPos;
               MinPos.X = ((r32)(Min.X  ) * 2/C.X) - 1;
               MinPos.Y = ((r32)(Min.Y  ) * 2/C.Y) - 1;
               MinPos.Z = ((r32)(Min.Z  ) * 2/C.Z) - 1;
               MaxPos.X = ((r32)(Max.X+1) * 2/C.X) - 1;
               MaxPos.Y = ((r32)(Max.Y+1) * 2/C.Y) - 1;
               MaxPos.Z = ((r32)(Max.Z+1) * 2/C.Z) - 1;
               
               // Now we create the actual vertex positions
               v3r32 P0, P1, P2, P3;
               
               // E.g. all +X vertices have an X coordinate of MaxPos.X.
               P0.E[S] = (D.E[S] == 1) ? MaxPos.E[S] : MinPos.E[S];
               P1.E[S] = (D.E[S] == 1) ? MaxPos.E[S] : MinPos.E[S];
               P2.E[S] = (D.E[S] == 1) ? MaxPos.E[S] : MinPos.E[S];
               P3.E[S] = (D.E[S] == 1) ? MaxPos.E[S] : MinPos.E[S];
               
               u32 J = (S + D.E[S] + 3) % 3;
               P0.E[J] = MinPos.E[J];
               P1.E[J] = MinPos.E[J];
               P2.E[J] = MaxPos.E[J];
               P3.E[J] = MaxPos.E[J];
               
               J = (S - D.E[S] + 3) % 3;
               P0.E[J] = MinPos.E[J];
               P1.E[J] = MaxPos.E[J];
               P2.E[J] = MaxPos.E[J];
               P3.E[J] = MinPos.E[J];
               
               // Maps the Face index to the UV ordering. Not sure what
               // the pattern is for this, but it works.
               u32 K[] = {0, 1, 2, 1, 1, 0};
               u32 R[] = {0b00, 0b01, 0b11, 0b10, 0b00, 0b01, 0b11};
               
               *Vertex++ = (pt_vertex){Mesh_EncodePosition(P0), RepV | RepU | TextureBytes[T] | R[K[F]+0]};
               *Vertex++ = (pt_vertex){Mesh_EncodePosition(P1), RepV | RepU | TextureBytes[T] | R[K[F]+1]};
               *Vertex++ = (pt_vertex){Mesh_EncodePosition(P2), RepV | RepU | TextureBytes[T] | R[K[F]+2]};
               *Vertex++ = (pt_vertex){Mesh_EncodePosition(P3), RepV | RepU | TextureBytes[T] | R[K[F]+3]};
               
               *Index++ = 4*VertexIndex + 0;
               *Index++ = 4*VertexIndex + 1;
               *Index++ = 4*VertexIndex + 2;
               *Index++ = 4*VertexIndex + 0;
               *Index++ = 4*VertexIndex + 2;
               *Index++ = 4*VertexIndex + 3;
               
               VertexIndex++;
            }
         }
      }
   }
   
   Stack_Pop();
   
   Heap_Resize(Chunk->Object.Vertices, VertexIndex*4*Mesh->VertexSize);
   Heap_Resize(Chunk->Object.Indices,  VertexIndex*6*sizeof(u32));
   
   Chunk->Object.TranslationMatrix = M4x4r32_Translation(ChunkPos.X*16, ChunkPos.Y*16, ChunkPos.Z*16);
   Chunk->Object.ScalingMatrix = M4x4r32_Scaling(8, 8, 8);
   Chunk->Object.RotationMatrix = M4x4r32_I;
}

internal b08
CollidesWithBlock(chunk Chunk, v3u32 BlockPos, v3r32 PlayerPos, v3r32 PlayerSize)
{
   v3r32 PosInChunk;
   PosInChunk.X = PlayerPos.X - Chunk.Pos.X * ChunkDims.X + ChunkDims.X/2;
   PosInChunk.Y = PlayerPos.Y - Chunk.Pos.Y * ChunkDims.Y + ChunkDims.Y/2;
   PosInChunk.Z = PlayerPos.Z - Chunk.Pos.Z * ChunkDims.Z + ChunkDims.Z/2;
   
   if(PosInChunk.X + PlayerSize.X <= BlockPos.X) return FALSE;
   if(PosInChunk.Y + PlayerSize.Y <  BlockPos.Y) return FALSE;
   if(PosInChunk.Z + PlayerSize.Z <= BlockPos.Z) return FALSE;
   if(PosInChunk.X >= BlockPos.X+1) return FALSE;
   if(PosInChunk.Y >  BlockPos.Y+1) return FALSE;
   if(PosInChunk.Z >= BlockPos.Z+1) return FALSE;
   
   block_type *Blocks = Chunk.Blocks->Data;
   block_type Type = Blocks[INDEX_3D(BlockPos.X, BlockPos.Y, BlockPos.Z, ChunkDims.X, ChunkDims.Y)];
   
   if(Type == BLOCK_NONE) return FALSE;
   
   return TRUE;
}


/* TODO:
 - Consider a hybrid scheme, where far-off chunks are loaded as octrees to
   save memory, and nearby chunks are directly accessed?
 - Short indices, like relative pointers, for octree accessing
*/




internal void
MakeNonVoxelChunk(heap *Heap, random *Random, region *Region,
                  mesh *Mesh, v3s32 ChunkPos, u32 *TextureBytes)
{
   chunk *Chunk = AddChunkToRegion(Region, ChunkPos);
   
   Chunk->Pos = ChunkPos;
   Chunk->Blocks = NULL;
   
   u32 SquareCountX = 32;
   u32 SquareCountZ = 32;
   u32 VertexCountX = SquareCountX+1;
   u32 VertexCountZ = SquareCountZ+1;
   u32 VertexCount = VertexCountX * VertexCountZ;
   u32 TriangleCount = 2*SquareCountX*SquareCountZ;
   Chunk->Object.Vertices = Heap_Allocate(Heap, VertexCount*Mesh->VertexSize);
   Chunk->Object.Indices  = Heap_Allocate(Heap, TriangleCount*3*sizeof(u32));
   v3r32 *Normals   = Heap_AllocateA(Heap, TriangleCount*sizeof(v3r32));
   v3r32 *VerticesR = Heap_AllocateA(Heap, VertexCount*sizeof(v3r32));
   pnm_vertex *Vertices = Chunk->Object.Vertices->Data;
   u32        *Index    = Chunk->Object.Indices->Data;
   
   for(u32 Z = 0; Z < VertexCountZ; Z++) {
      for(u32 X = 0; X < VertexCountX; X++) {
         v3r32 Pos = {
            2.0f * X/SquareCountX - 1 + R32_RandRange(Random, -0.01, 0.01),
            R32_RandRange(Random, -0.8, -0.79),
            2.0f * Z/SquareCountZ - 1 + R32_RandRange(Random, -0.01, 0.01)
         };
         
         VerticesR[INDEX_2D(X, Z, VertexCountX)] = Pos;
         
         pnm_vertex *Vertex = Vertices + INDEX_2D(X, Z, VertexCountX);
         *Vertex = (pnm_vertex){Mesh_EncodePosition(Pos), 0, 0};
      }
   }
   
   for(u32 Z = 0; Z < SquareCountZ; Z++) {
      for(u32 X = 0; X < SquareCountX; X++) {
         v3r32 VertexBL = VerticesR[INDEX_2D(X,   Z,   VertexCountX)];
         v3r32 VertexBR = VerticesR[INDEX_2D(X+1, Z,   VertexCountX)];
         v3r32 VertexTL = VerticesR[INDEX_2D(X,   Z+1, VertexCountX)];
         v3r32 VertexTR = VerticesR[INDEX_2D(X+1, Z+1, VertexCountX)];
         
         v3r32 LineX = V3r32_Sub(VertexBR, VertexBL);
         v3r32 LineZ = V3r32_Sub(VertexTL, VertexBL);
         v3r32 LineD = V3r32_Sub(VertexTR, VertexBL);
         
         v3r32 CrossXD = V3r32_Cross(LineD, LineX);
         v3r32 CrossDZ = V3r32_Cross(LineZ, LineD);
         
         Normals[2*INDEX_2D(X, Z, SquareCountX)+0] = CrossXD;
         Normals[2*INDEX_2D(X, Z, SquareCountX)+1] = CrossDZ;
         
         *Index++ = INDEX_2D(X,   Z,   VertexCountX);
         *Index++ = INDEX_2D(X+1, Z,   VertexCountX);
         *Index++ = INDEX_2D(X+1, Z+1, VertexCountX);
         *Index++ = INDEX_2D(X,   Z,   VertexCountX);
         *Index++ = INDEX_2D(X+1, Z+1, VertexCountX);
         *Index++ = INDEX_2D(X,   Z+1, VertexCountX);
      }
   }
   
   for(u32 Z = 0; Z < VertexCountZ; Z++) {
      for(u32 X = 0; X < VertexCountX; X++) {
         v3r32 TL,TM,TR,BL,BM,BR;
         
         if(X == 0 || Z == 0) {
            BL = (v3r32){0};
            BM = (v3r32){0};
         } else {
            BL = Normals[2*INDEX_2D(X-1, Z-1, SquareCountX)+1];
            BM = Normals[2*INDEX_2D(X-1, Z-1, SquareCountX)+0];
         }
         
         if(X == VertexCountX-1 || Z == 0)
            BR = (v3r32){0};
         else
            BR = Normals[2*INDEX_2D(X,   Z-1, SquareCountX)+1];
         
         if(X == 0 || Z == VertexCountZ-1)
            TL = (v3r32){0};
         else
            TL = Normals[2*INDEX_2D(X-1, Z,   SquareCountX)+0];
         
         if(X == VertexCountX-1 || Z == VertexCountZ-1) {
            TM = (v3r32){0};
            TR = (v3r32){0};
         } else {
            TR = Normals[2*INDEX_2D(X,   Z,   SquareCountX)+0];
            TM = Normals[2*INDEX_2D(X,   Z,   SquareCountX)+1];
         }
         
         v3r32 Sum = V3r32_Add(V3r32_Add(V3r32_Add(TL, TM), V3r32_Add(TR, BL)), V3r32_Add(BM, BR));
         v3r32 Normal = V3r32_Norm(Sum);
         
         Vertices[INDEX_2D(X, Z, VertexCountX)].Normal = Mesh_EncodeNormal(Normal);
      }
   }
   
   Heap_FreeA(Normals);
   Heap_FreeA(VerticesR);
   
   Chunk->Object.TranslationMatrix = M4x4r32_Translation(ChunkPos.X*16, ChunkPos.Y*16, ChunkPos.Z*16);
   Chunk->Object.ScalingMatrix = M4x4r32_Scaling(8, 8, 8);
   Chunk->Object.RotationMatrix = M4x4r32_I;
}

#endif