/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
MakePCBlockObject(mesh *Mesh, heap *Heap, v3r32 Pos, v4u08 Color)
{
   persist u32 Indices[] = {
      0,1,2,  0,2,3,
      1,5,6,  1,6,2,
      5,4,7,  5,7,6,
      4,0,3,  4,3,7,
      3,2,6,  3,6,7,
      4,5,1,  4,1,0
   };
   
   mesh_object Object;
   
   Object.Vertices = Heap_Allocate(Heap, sizeof(pc_vertex)*8);
   Object.Indices  = Heap_Allocate(Heap, sizeof(Indices));
   
   pc_vertex *Vertex = (vptr)Object.Vertices->Data;
   u32       *Index  = (vptr)Object.Indices->Data;
   
   *Vertex++ = (pc_vertex){CubePositions[0], Color};
   *Vertex++ = (pc_vertex){CubePositions[1], Color};
   *Vertex++ = (pc_vertex){CubePositions[2], Color};
   *Vertex++ = (pc_vertex){CubePositions[3], Color};
   *Vertex++ = (pc_vertex){CubePositions[4], Color};
   *Vertex++ = (pc_vertex){CubePositions[5], Color};
   *Vertex++ = (pc_vertex){CubePositions[6], Color};
   *Vertex++ = (pc_vertex){CubePositions[7], Color};
   
   Mem_Cpy(Object.Indices->Data, Indices, sizeof(Indices));
   
   Object.TranslationMatrix = M4x4r32_Translation(Pos.X, Pos.Y, Pos.Z);
   Object.ScalingMatrix = M4x4r32_Scaling(0.4, 0.4, 0.4);
   Object.RotationMatrix = M4x4r32_I;
   
   return Object;
}

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

internal chunk
MakeChunk(heap *Heap, mesh *Mesh, v3s32 ChunkPos, u32 *TextureBytes)
{
   // NOTE: Must be <= the 'repeat' max in the pt shaders
   v3u32 ChunkDims = {16, 16, 16};
   
   chunk Chunk;
   Chunk.Pos = ChunkPos;
   Chunk.Blocks = Heap_Allocate(Heap, V3u32_Volume(ChunkDims)*sizeof(block_type));
   block_type *Blocks = Chunk.Blocks->Data;
   Mem_Set(Blocks, BLOCK_NONE, Chunk.Blocks->Size);
   
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
   
   Stack_Push();
   
   u32 Size = V3u32_Volume(ChunkDims)*sizeof(u08);
   u08 *Consumed = Stack_Allocate(Size);
   Mem_Set(Consumed, 0, Size);
   
   u32 VertexIndex = 0;
   u32 VertexCount = 12;
   Chunk.Object.Vertices = Heap_Allocate(Heap, VertexCount*4*Mesh->VertexSize);
   Chunk.Object.Indices = Heap_Allocate(Heap, VertexCount*6*sizeof(u32));
   pt_vertex *Vertex = Chunk.Object.Vertices->Data;
   u32       *Index  = Chunk.Object.Indices->Data;
   
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
                  Heap_Resize(Chunk.Object.Vertices, (VertexCount+12)*4*Mesh->VertexSize);
                  Heap_Resize(Chunk.Object.Indices,  (VertexCount+12)*6*sizeof(u32));
                  Vertex = (pt_vertex*)Chunk.Object.Vertices->Data + 4*VertexCount;
                  Index  =       (u32*)Chunk.Object.Indices->Data  + 6*VertexCount;
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
   
   Heap_Resize(Chunk.Object.Vertices, VertexIndex*4*Mesh->VertexSize);
   Heap_Resize(Chunk.Object.Indices,  VertexIndex*6*sizeof(u32));
   
   Chunk.Object.TranslationMatrix = M4x4r32_Translation(ChunkPos.X*16, ChunkPos.Y*16, ChunkPos.Z*16);
   Chunk.Object.ScalingMatrix = M4x4r32_Scaling(8, 8, 8);
   Chunk.Object.RotationMatrix = M4x4r32_I;
   
   return Chunk;
}

//TODO:
// Each chunk is a heap handle
// Octree of blocks per chunk
// Short indices, like relative pointers, since max blocks per chunk is 16*16*16=4096