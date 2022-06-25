/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
   
   persist u32 Positions[] = {
      0b01100000000110000000011000000001,
      0b01100000000110000000010111111111,
      0b01100000000101111111110111111111,
      0b01100000000101111111111000000001,
      0b01011111111110000000011000000001,
      0b01011111111110000000010111111111,
      0b01011111111101111111110111111111,
      0b01011111111101111111111000000001
   };
   
   mesh_object Object;
   
   Object.Vertices = Heap_Allocate(Heap, sizeof(pc_vertex)*8);
   Object.Indices  = Heap_Allocate(Heap, sizeof(Indices));
   
   pc_vertex *Vertex = (vptr)Object.Vertices->Data;
   u32       *Index  = (vptr)Object.Indices->Data;
   
   *Vertex++ = (pc_vertex){Positions[0], Color};
   *Vertex++ = (pc_vertex){Positions[1], Color};
   *Vertex++ = (pc_vertex){Positions[2], Color};
   *Vertex++ = (pc_vertex){Positions[3], Color};
   *Vertex++ = (pc_vertex){Positions[4], Color};
   *Vertex++ = (pc_vertex){Positions[5], Color};
   *Vertex++ = (pc_vertex){Positions[6], Color};
   *Vertex++ = (pc_vertex){Positions[7], Color};
   
   Mem_Cpy(Object.Indices->Data, Indices, sizeof(Indices));
   
   Object.TranslationMatrix = M4x4r32_Translation(Pos.X, Pos.Y, Pos.Z);
   Object.ScalingMatrix = M4x4r32_Scaling(0.4, 0.4, 0.4);
   Object.RotationMatrix = M4x4r32_I;
   
   return Object;
}

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
   
   persist u32 Positions[] = {
      0b01100000000110000000011000000001,
      0b01100000000110000000010111111111,
      0b01100000000101111111110111111111,
      0b01100000000101111111111000000001,
      0b01011111111110000000011000000001,
      0b01011111111110000000010111111111,
      0b01011111111101111111110111111111,
      0b01011111111101111111111000000001
   };
   
   mesh_object Object;
   
   Object.Vertices = Heap_Allocate(Heap, sizeof(pt_vertex)*4*6);
   Object.Indices  = Heap_Allocate(Heap, sizeof(Indices));
   
   pt_vertex *Vertex = (vptr)Object.Vertices->Data;
   u32       *Index  = (vptr)Object.Indices->Data;
   
   Assert((BytesFromFirstAsset & 3) == 0);
   
   *Vertex++ = (pt_vertex){Positions[0], BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){Positions[1], BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){Positions[2], BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){Positions[3], BytesFromFirstAsset | 0b10};
   
   *Vertex++ = (pt_vertex){Positions[1], BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){Positions[5], BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){Positions[6], BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){Positions[2], BytesFromFirstAsset | 0b10};
   
   *Vertex++ = (pt_vertex){Positions[5], BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){Positions[4], BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){Positions[7], BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){Positions[6], BytesFromFirstAsset | 0b10};
   
   *Vertex++ = (pt_vertex){Positions[4], BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){Positions[0], BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){Positions[3], BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){Positions[7], BytesFromFirstAsset | 0b10};
   
   *Vertex++ = (pt_vertex){Positions[3], BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){Positions[2], BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){Positions[6], BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){Positions[7], BytesFromFirstAsset | 0b10};
   
   *Vertex++ = (pt_vertex){Positions[4], BytesFromFirstAsset | 0b00};
   *Vertex++ = (pt_vertex){Positions[5], BytesFromFirstAsset | 0b01};
   *Vertex++ = (pt_vertex){Positions[1], BytesFromFirstAsset | 0b11};
   *Vertex++ = (pt_vertex){Positions[0], BytesFromFirstAsset | 0b10};
   
   Mem_Cpy(Object.Indices->Data, Indices, sizeof(Indices));
   
   Object.TranslationMatrix = M4x4r32_Translation(Pos.X, Pos.Y, Pos.Z);
   Object.ScalingMatrix = M4x4r32_Scaling(0.5, 0.5, 0.5);
   Object.RotationMatrix = M4x4r32_I;
   
   return Object;
}