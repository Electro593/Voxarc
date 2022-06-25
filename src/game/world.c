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

internal mesh_object
MakePCBlockObject(mesh *Mesh, heap *Heap, v4u08 Color, v3r32 Pos)
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
   
   Object.Vertices = Heap_Allocate(Heap, Mesh->VertexSize * 8);
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