/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

global_state __Global;

#include <util/scalar.c>
#include <util/vector.c>
#include <util/memory.c>
#include <util/string.c>
#include <game/msdf.c>
#include <renderer/font.c>
#include <game/file.c>
#include <renderer/opengl/mesh.c>
#include <game/ui.c>
#include <renderer/opengl/render.c>
#include <game/world.c>

// TODO handle errors, e.g. make a memory representation of a default
//      file if loading it fails

internal void
Game_Init(platform_state *Platform,
          game_state *Game,
          renderer_state *Renderer)
{
   u64 StackSize = 32*1024*1024;
   u64 RendererHeapSize = 32*1024*1024;
   vptr MemBase = Platform_AllocateMemory(StackSize+RendererHeapSize);
   
   __Global.Stack = Stack_Init(MemBase, StackSize);
   (u08*)MemBase += StackSize;
   
   heap *RendererHeap = Heap_Init(MemBase, RendererHeapSize);
   (u08*)MemBase += RendererHeapSize;
   
   // File_CreateAssetpack("assets\\0.pack", RendererHeap, 60.0f);
   Renderer->Assetpack = File_LoadAssetpack("assets\\0.pack", RendererHeap);
   
   Game->Flying = TRUE;
   Game->Dir = (v3r32){0,0,0};
   Game->Pos = (v3r32){0,0,0};
   Game->Velocity = (v3r32){0,0,0};
   
   Renderer_Init(Renderer, RendererHeap, Platform->WindowSize);
   
   Stack_Push();
   
   u32 *TextureBytes = Stack_Allocate(sizeof(u32) * BLOCK_Count);
   for(u32 I = 1; I < BLOCK_Count; I++) {
      assetpack_tag *Tag = Assetpack_FindExactTag(Renderer->Assetpack, TAG_BLOCK_TEXTURE, (vptr)(u64)I);
      assetpack_asset *Asset = Tag->Assets[0];
      TextureBytes[I] = (u64)((u08*)Asset - (u64)Renderer->Assetpack.Assets);
   }
   
   Game->Chunk = MakeChunk(RendererHeap, &Renderer->PTMesh, (v3s32){0,0,0}, TextureBytes);
   mesh_object *Objects[] = {&Game->Chunk.Object};
   Mesh_AddObjects(&Renderer->PTMesh, 1, Objects);
   Mesh_FreeObject(Game->Chunk.Object);
   
   Mesh_Update(&Renderer->PTMesh);
   
   Game->AimBlockObjectIndex = Mesh_ReserveObject(&Renderer->PMesh, 24, 0);
   
   mesh_object Object;
   Object.Vertices = Heap_Allocate(RendererHeap,  8*sizeof(pc_vertex));
   Object.Indices  = Heap_Allocate(RendererHeap, 12*sizeof(u32));
   v4u08 C = {127, 127, 127, 255};
   u32 Indices[] = {0,1,2,  0,2,3,  4,5,6,  4,6,7};
   Mem_Cpy(Object.Indices->Data, Indices, sizeof(Indices));
   pc_vertex *Vertex = Object.Vertices->Data;
   r32 Width = 0.15;
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){-1, -Width, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){ 1, -Width, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){ 1,  Width, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){-1,  Width, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){-Width, -1, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){ Width, -1, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){ Width,  1, 0}), C};
   *Vertex++ = (pc_vertex){Mesh_EncodePosition((v3r32){-Width,  1, 0}), C};
   r32 Scale = 0.02;
   Object.TranslationMatrix = M4x4r32_I;
   Object.ScalingMatrix     = M4x4r32_Scaling(Scale, Scale, 1);
   Object.RotationMatrix    = M4x4r32_I;
   Objects[0] = &Object;
   Mesh_AddObjects(&Renderer->PC2Mesh, 1, Objects);
   Game->CrosshairObjectIndex = Object.Index;
   Mesh_Update(&Renderer->PC2Mesh);
   Mesh_FreeObject(Object);
   
   Stack_Pop();
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
   if(Platform->Updates & WINDOW_RESIZED) {
      Renderer_Resize(Platform->WindowSize, &Renderer->OrthographicMatrix, &Renderer->PerspectiveMatrix);
      
      OpenGL_UseProgram(Renderer->PC2Program);
      OpenGL_UniformMatrix4fv(Renderer->PC2Mesh.VPMatrix,  1, FALSE, Renderer->OrthographicMatrix);
      
      Renderer->WindowSize = Platform->WindowSize;
   }
   if(Platform->Updates & CURSOR_DISABLED) {
      Game->PrevCursorPos = Platform->CursorPos;
      Platform->Updates &= ~CURSOR_DISABLED;
   }
   
   b08 Moved = FALSE;
   v3r32 DirDelta = (v3r32){0};
   r32 EyeHeight = 1.7;
   
   if(Platform->CursorIsDisabled) {
      if(Platform->Keys[ScanCode_1] == PRESSED) {
         if(!Game->Key1WasDown) {
            Game->Flying = !Game->Flying;
            Game->Key1WasDown = TRUE;
         }
      } else Game->Key1WasDown = FALSE;
      
      
      v2s32 CursorDelta = V2s32_Sub(Platform->CursorPos, Game->PrevCursorPos);
      Game->PrevCursorPos = Platform->CursorPos;
      
      DirDelta.X =  CursorDelta.Y/120.0f;
      DirDelta.Y = -CursorDelta.X/120.0f;
      Game->Dir = V3r32_Add(Game->Dir, DirDelta);
      if(Game->Dir.X >  R32_PI/2) Game->Dir.X =  R32_PI/2;
      if(Game->Dir.X < -R32_PI/2) Game->Dir.X = -R32_PI/2;
      
      
      
      if(Game->Flying) {
         v2r32 MoveXZ = {0};
         if(Platform->Keys[ScanCode_D] != RELEASED)
            MoveXZ.X++;
         if(Platform->Keys[ScanCode_A] != RELEASED)
            MoveXZ.X--;
         if(Platform->Keys[ScanCode_W] != RELEASED)
            MoveXZ.Y++;
         if(Platform->Keys[ScanCode_S] != RELEASED)
            MoveXZ.Y--;
         
         r32 MoveY = 0;
         if(Platform->Keys[ScanCode_Space] != RELEASED)
            MoveY++;
         if(Platform->Keys[ScanCode_ShiftLeft]  != RELEASED ||
            Platform->Keys[ScanCode_ShiftRight] != RELEASED)
            MoveY--;
         
         b08 MoveHoriz = MoveXZ.X != 0 || MoveXZ.Y != 0;
         if(MoveHoriz)
            MoveXZ = V2r32_Norm(MoveXZ);
         
         if(MoveHoriz || MoveY != 0) {
            r32 Step = 0.1;
            v3r32 ToMove = {MoveXZ.X, MoveY, MoveXZ.Y};
            ToMove = V3r32_MulS(ToMove, Step);
            
            Game->Pos.X += ToMove.X*R32_cos(Game->Dir.Y) - ToMove.Z*R32_sin(Game->Dir.Y);
            Game->Pos.Z += ToMove.X*R32_sin(Game->Dir.Y) + ToMove.Z*R32_cos(Game->Dir.Y);
            Game->Pos.Y += ToMove.Y;
            
            Moved = TRUE;
         }
      } else {
         r32 TicksPerSecond = 60;
         r32 WalkTiming[3] = {0.3, 0.05};
         r32 WalkVelocityX = 0.1;
         r32 WalkVelocityZ = 0.1;
         
         s32 State = 0;
         if(Platform->Keys[ScanCode_W] != RELEASED &&
            Platform->Keys[ScanCode_S] == RELEASED &&
            Game->WalkStep.Z >= 0)
            State = 1;
         else if(Platform->Keys[ScanCode_W] == RELEASED &&
                 Platform->Keys[ScanCode_S] != RELEASED &&
                 Game->WalkStep.Z <= 0)
            State = -1;
         
         if(State == 0 && Game->WalkStep.Z != 0) {
            Game->WalkStep.Z -= S32_Sign(Game->WalkStep.Z);
            Game->Velocity.Z *= 4.0f/5;
         } else if(State == 1 && Game->WalkStep.Z < TicksPerSecond*WalkTiming[0]) {
            Game->WalkStep.Z++;
            Game->Velocity.Z = WalkVelocityZ * Game->WalkStep.Z/(TicksPerSecond*WalkTiming[0]);
         } else if(State == -1 && Game->WalkStep.Z > -TicksPerSecond*WalkTiming[0]) {
            Game->WalkStep.Z--;
            Game->Velocity.Z = WalkVelocityZ * Game->WalkStep.Z/(TicksPerSecond*WalkTiming[0]);
         } else if(S32_Abs(Game->WalkStep.Z) == TicksPerSecond*WalkTiming[0]) {
            Game->Velocity.Z = S32_Sign(Game->WalkStep.Z) * WalkVelocityZ;
         } else if(Game->WalkStep.Z == 0)
            Game->Velocity.Z = 0;
         
         State = 0;
         if(Platform->Keys[ScanCode_D] != RELEASED &&
            Platform->Keys[ScanCode_A] == RELEASED &&
            Game->WalkStep.X >= 0)
            State = 1;
         else if(Platform->Keys[ScanCode_D] == RELEASED &&
                 Platform->Keys[ScanCode_A] != RELEASED &&
                 Game->WalkStep.X <= 0)
            State = -1;
         
         if(State == 0 && Game->WalkStep.X != 0) {
            Game->WalkStep.X -= S32_Sign(Game->WalkStep.X);
            Game->Velocity.X *= 1.0f/2;
         } else if(State == 1 && Game->WalkStep.X < TicksPerSecond*WalkTiming[1]) {
            Game->WalkStep.X++;
            Game->Velocity.X = WalkVelocityX * Game->WalkStep.X/(TicksPerSecond*WalkTiming[1]);
         } else if(State == -1 && Game->WalkStep.X > -TicksPerSecond*WalkTiming[1]) {
            Game->WalkStep.X--;
            Game->Velocity.X = WalkVelocityX * Game->WalkStep.X/(TicksPerSecond*WalkTiming[1]);
         } else if(S32_Abs(Game->WalkStep.X) == TicksPerSecond*WalkTiming[1]) {
            Game->Velocity.X = S32_Sign(Game->WalkStep.X) * WalkVelocityX;
         } else if(Game->WalkStep.X == 0)
            Game->Velocity.X = 0;
         
         Game->Mass = 75; // kg
         Game->Gravity = -9.807; // m/s^2
         
         r32 MaxCharge = 0.5 * TicksPerSecond;
         r32 ReleaseTicks = 0.2 * TicksPerSecond;
         r32 JumpVelocity = 5;// realistic: 2.9;
         
         r32 ForceY = Game->Gravity * Game->Mass;
         
         if(Platform->Keys[ScanCode_Space] != RELEASED) {
            Game->KeySpaceWasDown = TRUE;
            
            if(Game->JumpCharge < (MaxCharge/2) * (Game->TouchingGround+1))
               Game->JumpCharge++;
         }
         
         if(Game->TouchingGround) {
            if(Platform->Keys[ScanCode_Space] == RELEASED) {
               if(Game->KeySpaceWasDown) {
                  Game->JumpTime = Game->JumpCharge;
                  Game->KeySpaceWasDown = FALSE;
               }
               
               if(Game->JumpTime > 0) {
                  Game->JumpTime -= MaxCharge / ReleaseTicks;
                  //TODO: Change camera pos here
                  
                  if(Game->JumpTime <= 0) {
                     Game->Velocity.Y = (0.8*Game->JumpCharge/MaxCharge + 0.2) * JumpVelocity / TicksPerSecond;
                     Game->JumpCharge = 0;
                  } else
                     ForceY += Game->Gravity * Game->Mass;
               }
            } else
               ForceY += Game->Gravity * Game->Mass;
         }
         
         //TODO: Terminal velocity
         
         Game->Acceleration.Y = ForceY / Game->Mass / (TicksPerSecond*TicksPerSecond);
         Game->Velocity.Y += Game->Acceleration.Y;
         
         if(!V3r32_IsEqual(Game->Velocity, (v3r32){0})) {
            Game->Pos.X += Game->Velocity.X*R32_cos(Game->Dir.Y) - Game->Velocity.Z*R32_sin(Game->Dir.Y);
            Game->Pos.Z += Game->Velocity.X*R32_sin(Game->Dir.Y) + Game->Velocity.Z*R32_cos(Game->Dir.Y);
            Game->Pos.Y += Game->Velocity.Y;
            
            
            // Stack_Push();
            
            // r32 PlayerRadius = 0.3;
            // r32 PlayerHeight = 1.8;
            // u32 PoleGenRadius = 16;
            // u32 PoleCount = 4*PoleGenRadius*PoleGenRadius;
            
            // collision_pole *Poles = Stack_Allocate(PoleCount*sizeof(collision_pole));
            // for(u32 X = 0; X < 2*PoleGenRadius; X++) {
            //    for(u32 Z = 0; Z < 2*PoleGenRadius; Z++) {
            //       collision_pole *Pole = Poles+INDEX_2D(X, Z, 2*PoleGenRadius);
            //       Pole->IntersectionCount = 0;
            //       Pole->Intersections = Stack_Allocate(8 * sizeof(r32));
                  
            //       u32 IndexCount = Renderer->PTMesh.Indices->Size / sizeof(u32);
            //       u32 VertexCount = Renderer->PTMesh.Vertices->Size / sizeof(pt_vertex);
            //       u32 *Index = Renderer->PTMesh.Indices->Data;
            //       pt_vertex *Vertices = Renderer->PTMesh.Vertices->Data;
                  
            //       for(u32 I = 0; I < IndexCount; I += 3) {
            //          v3r32 P0 = Mesh_UnencodePosition(Vertices[I+0].Position);
            //          v3r32 P1 = Mesh_UnencodePosition(Vertices[I+1].Position);
            //          v3r32 P2 = Mesh_UnencodePosition(Vertices[I+2].Position);
                     
            //          v2r32 P = {X, Z};
            //          v2r32 P20 = {P0.X, P0.Z};
            //          v2r32 P21 = {P1.X, P1.Z};
            //          v2r32 P22 = {P2.X, P2.Z};
                     
            //          r32 E0 = V2r32_Cross(V2r32_Sub(P21, P20), V2r32_Sub(P, P20));
            //          r32 E1 = V2r32_Cross(V2r32_Sub(P22, P21), V2r32_Sub(P, P21));
            //          r32 E2 = V2r32_Cross(V2r32_Sub(P20, P22), V2r32_Sub(P, P22));
                     
            //          if(E0 >= 0 && E1 >= 0 && E2 >= 0) {
                        
            //          }
            //       }
            //    }
            // }
            
            // Stack_Pop();
            
            
            Moved = TRUE;
            
            Game->TouchingGround = FALSE;
            v3r32 PlayerSize = {.6,1.8,.6};
            v3r32 PosInChunk = V3r32_Add(V3r32_Sub(Game->Pos, V3r32_Mul(V3s32_ToV3r32(Game->Chunk.Pos), V3u32_ToV3r32(ChunkDims))), V3r32_DivS(V3u32_ToV3r32(ChunkDims), 2));
            for(s32 X = (s32)PosInChunk.X; X <= (s32)(PosInChunk.X + PlayerSize.X); X++) {
               for(s32 Z = (s32)PosInChunk.Z; Z <= (s32)(PosInChunk.Z + PlayerSize.Z); Z++) {
                  b08 R = CollidesWithBlock(Game->Chunk, (v3u32){X, (u32)PosInChunk.Y, Z}, Game->Pos, PlayerSize);
                  Game->TouchingGround |= R;
               }
            }
            
            if(Game->TouchingGround) {
               if(PosInChunk.Y > (s32)PosInChunk.Y)
                  Game->Pos.Y = (r32)((s32)PosInChunk.Y+1) + Game->Chunk.Pos.Y*ChunkDims.Y - ChunkDims.Y/2;
               
               Game->Velocity.Y = 0;
            }
         }
         
      }
   }
   
   if((Platform->Updates & WINDOW_RESIZED) ||
      Moved ||
      !V3r32_IsEqual(DirDelta, (v3r32){0}))
   {
      r32 Pitch = Game->Dir.X;
      r32 Yaw   = Game->Dir.Y;
      r32 CosYaw = R32_cos(Yaw);
      r32 SinYaw = R32_sin(Yaw);
      r32 CosPitch = R32_cos(Pitch);
      r32 SinPitch = R32_sin(Pitch);
      
      v3r32 Right = { CosYaw,           0,            SinYaw         };
      v3r32 Up    = { SinYaw*SinPitch,  CosPitch,    -CosYaw*SinPitch};
      v3r32 Front = {-SinYaw*CosPitch,  SinPitch,     CosYaw*CosPitch};
      v3r32 Pos   = {-Game->Pos.X,     -(Game->Pos.Y + EyeHeight), -Game->Pos.Z    };
      
      Renderer->ViewMatrix = (m4x4r32){
         Right.X, Right.Y, Right.Z, V3r32_Dot(Right, Pos),
         Up.X,    Up.Y,    Up.Z,    V3r32_Dot(Up,    Pos),
         Front.X, Front.Y, Front.Z, V3r32_Dot(Front, Pos),
         0,       0,       0,       1
      };
      
      m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
      
      OpenGL_UseProgram(Renderer->PProgram);
      OpenGL_UniformMatrix4fv(Renderer->PMesh.VPMatrix,  1, FALSE, VPMatrix);
      
      OpenGL_UseProgram(Renderer->PC3Program);
      OpenGL_UniformMatrix4fv(Renderer->PC3Mesh.VPMatrix,  1, FALSE, VPMatrix);
      
      OpenGL_UseProgram(Renderer->PTProgram);
      OpenGL_UniformMatrix4fv(Renderer->PTMesh.VPMatrix, 1, FALSE, VPMatrix);
      
      Platform->Updates &= ~WINDOW_RESIZED;
   }
   
   if(Platform->CursorIsDisabled) {
      r32 AimRange = 5;
      
      m4x4r32 ViewRotation = Renderer->ViewMatrix;
      ViewRotation.V[0].W = 0;
      ViewRotation.V[1].W = 0;
      ViewRotation.V[2].W = 0;
      ViewRotation.V[3] = (v4r32){0,0,0,1};
      ViewRotation = M4x4r32_Transpose(ViewRotation);
      
      v4r32 AimDir4 = M4x4r32_MulMV(ViewRotation, (v4r32){0, 0, 1, 1});
      v3r32 AimDir = {AimDir4.X, AimDir4.Y, AimDir4.Z};
      
      v3s32 ChunkPos = {(s32)((Game->Pos.Z+ChunkDims.Z/2.0f)/ChunkDims.Z), (s32)((Game->Pos.Y+EyeHeight+ChunkDims.Y/2.0f)/ChunkDims.Y), (s32)((Game->Pos.Z+ChunkDims.Z/2.0f)/ChunkDims.Z)};
      v3r32 PosInChunk = V3r32_Add(V3r32_Sub(Game->Pos, V3r32_Mul(V3s32_ToV3r32(Game->Chunk.Pos), V3u32_ToV3r32(ChunkDims))), V3r32_DivS(V3u32_ToV3r32(ChunkDims), 2));
      v3r32 AimBase = {PosInChunk.X, PosInChunk.Y+EyeHeight, PosInChunk.Z};
      block_type *Blocks = Game->Chunk.Blocks->Data;
      Game->AimBlockValid = FALSE;
      
      // TODO: Consider an incremental approach, using the face the ray
      // exists to determine the text block to test. It would reduce
      // the search cost when radius gets larger.
      {
         v3s32 Min, Max;
         
         if(AimDir.X < 0) {
            Min.X = AimBase.X - AimRange;
            Max.X = AimBase.X;
         } else {
            Min.X = AimBase.X;
            Max.X = AimBase.X + AimRange;
         }
         if(Min.X < 0) Min.X = 0;
         if(Max.X > ChunkDims.X-1) Max.X = ChunkDims.X-1;
         
         if(AimDir.Y < 0) {
            Min.Y = AimBase.Y - AimRange;
            Max.Y = AimBase.Y;
         } else {
            Min.Y = AimBase.Y;
            Max.Y = AimBase.Y + AimRange;
         }
         if(Min.Y < 0) Min.Y = 0;
         if(Max.Y > ChunkDims.Y-1) Max.Y = ChunkDims.Y-1;
         
         if(AimDir.Z < 0) {
            Min.Z = AimBase.Z - AimRange;
            Max.Z = AimBase.Z;
         } else {
            Min.Z = AimBase.Z;
            Max.Z = AimBase.Z + AimRange;
         }
         if(Min.Z < 0) Min.Z = 0;
         if(Max.Z > ChunkDims.Z-1) Max.Z = ChunkDims.Z-1;
         
         r32 MinT = R32_MAX;
         for(u32 X = Min.X; X <= Max.X; X++) {
            for(u32 Y = Min.Y; Y <= Max.Y; Y++) {
               for(u32 Z = Min.Z; Z <= Max.Z; Z++) {
                  u32 I = INDEX_3D(X, Y, Z, ChunkDims.X, ChunkDims.Y);
                  if(Blocks[I] == BLOCK_NONE) continue;
                  
                  if(X == 0 && Y == 3 && Z == 15)
                     NOP;
                  
                  r32 TX, TY, TZ;
                  b08 IX, IY, IZ;
                  
                  if(AimDir.X < 0)
                     IX = RayRectIntersectionA((v3r32){X+1, Y,   Z  }, (v3r32){X+1, Y+1, Z+1}, (v3r32){ 1, 0, 0}, AimBase, AimDir, &TX, NULL);
                  else
                     IX = RayRectIntersectionA((v3r32){X,   Y,   Z  }, (v3r32){X,   Y+1, Z+1}, (v3r32){-1, 0, 0}, AimBase, AimDir, &TX, NULL);
                  
                  if(AimDir.Y < 0)
                     IY = RayRectIntersectionA((v3r32){X,   Y+1, Z  }, (v3r32){X+1, Y+1, Z+1}, (v3r32){ 0, 1, 0}, AimBase, AimDir, &TY, NULL);
                  else
                     IY = RayRectIntersectionA((v3r32){X,   Y,   Z  }, (v3r32){X+1, Y,   Z+1}, (v3r32){ 0,-1, 0}, AimBase, AimDir, &TY, NULL);
                  
                  if(AimDir.Z < 0)
                     IZ = RayRectIntersectionA((v3r32){X,   Y,   Z+1}, (v3r32){X+1, Y+1, Z+1}, (v3r32){ 0, 0, 1}, AimBase, AimDir, &TZ, NULL);
                  else
                     IZ = RayRectIntersectionA((v3r32){X,   Y,   Z  }, (v3r32){X+1, Y+1, Z  }, (v3r32){ 0, 0,-1}, AimBase, AimDir, &TZ, NULL);
                  
                  r32 T = R32_MAX;
                  
                  if(IX && TX >= 0 && TX < T) T = TX;
                  if(IY && TY >= 0 && TY < T) T = TY;
                  if(IZ && TZ >= 0 && TZ < T) T = TZ;
                  
                  if(T < MinT) {
                     MinT = T;
                     Game->AimBlock = (v3s32){X, Y, Z};
                  }
               }
            }
         }
         
         if(MinT != R32_MAX) {
            #define TEST_THING 0
#if TEST_THING
         }
      }
      
      for(u32 R = 0; R <= AimRange; R++) {
         v3r32 AimVector = V3r32_Add(AimBase, V3r32_MulS(AimDir, R));
         v3s32 BlockPos = {R32_Floor(AimVector.X), R32_Floor(AimVector.Y), R32_Floor(AimVector.Z)};
         
         u32 I = INDEX_3D(BlockPos.X, BlockPos.Y, BlockPos.Z, ChunkDims.X, ChunkDims.Y);
         if(BlockPos.X >= 0 && BlockPos.Y >= 0 && BlockPos.Z >= 0 &&
            BlockPos.X < ChunkDims.X && BlockPos.Y < ChunkDims.Y && BlockPos.Z < ChunkDims.Z &&
            Blocks[I] != BLOCK_NONE) {
            Game->AimBlock = BlockPos;
#endif
            Game->AimBlockValid = TRUE;
            
            p_vertex *Vertex = Mesh_GetVertices(&Renderer->PMesh, Game->AimBlockObjectIndex);
            
            #if 1
            v3s32 B = Game->AimBlock;
            v3u32 D = ChunkDims;
            v3r32 P = {(B.X-8)/8.0f, (B.Y-8)/8.0f, (B.Z-8)/8.0f};
            r32 S = 1/8.0f;
            
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+0, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+S, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+0, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+S, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+0, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+S, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+0, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+S, P.Z+S})};
            
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+0, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+0, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+0, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+0, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+0, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+0, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+0, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+0, P.Z+S})};
            
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+S, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+S, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+S, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+S, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+S, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+0, P.Y+S, P.Z+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+S, P.Z+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){P.X+S, P.Y+S, P.Z+S})};
            
            r32 Factor = 0;
            m4x4r32 Translation = M4x4r32_Translation(ChunkPos.X*16, ChunkPos.Y*16, ChunkPos.Z*16);
            m4x4r32 Scaling     = M4x4r32_Scaling(8+Factor, 8+Factor, 8+Factor);
            m4x4r32 Rotation    = M4x4r32_I;
            #else
            v3s32 B = Game->AimBlock;
            v3u32 D = ChunkDims;
            v3r32 P = {(B.X-8), (B.Y-8), (B.Z-8)};
            r32 S = 2;
            
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+0, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+S, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+0, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+S, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+0, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+S, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+0, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+S, -1+S})};
            
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+0, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+0, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+0, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+0, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+0, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+0, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+0, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+0, -1+S})};
            
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+S, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+S, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+S, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+S, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+S, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+0, -1+S, -1+S})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+S, -1+0})};
            *Vertex++ = (p_vertex){Mesh_EncodePosition((v3r32){-1+S, -1+S, -1+S})};
            
            r32 Factor = 0;
            m4x4r32 Translation = M4x4r32_Translation(ChunkPos.X*16 - 7.5 + B.X - Factor/2, ChunkPos.Y*16 - 7.5 + B.Y - Factor/2, ChunkPos.Z*16 - 7.5 + B.Z - Factor/2);
            m4x4r32 Scaling     = M4x4r32_Scaling(0.5+Factor, 0.5+Factor, 0.5+Factor);
            m4x4r32 Rotation    = M4x4r32_I;
            #endif
            
            m4x4r32 *Matrix = Mesh_GetMatrix(&Renderer->PMesh, Game->AimBlockObjectIndex);
            *Matrix = M4x4r32_Mul(M4x4r32_Mul(Translation, Rotation), Scaling);
            Mesh_Update(&Renderer->PMesh);
#if TEST_THING
            break;
#endif
         }
      }
   }
   
   Renderer_Draw(Platform, Game, Renderer, Platform->FPS);
}

external void API_EXPORT
Game_Load(platform_exports *PlatformFuncs,
          game_exports *GameFuncs,
          game_state *GameState,
          opengl_funcs *OpenGLFuncs)
{
   if(PlatformFuncs) {
      #define EXPORT(ReturnType, Namespace, Name, ...) \
         Namespace##_##Name = PlatformFuncs->Namespace##_##Name;
      #define X PLATFORM_FUNCS
      #include <x.h>
   }
   
   if(GameFuncs) {
      #define EXPORT(ReturnType, Namespace, Name, ...) \
         GameFuncs->Namespace##_##Name = Namespace##_##Name;
      #define X GAME_FUNCS
      #include <x.h>
   }
   
   if(GameState) {
      __Global = GameState->GlobalState;
   }
   
   Renderer_Load(OpenGLFuncs);
}

external void API_EXPORT
Game_Unload(game_state *GameState)
{
   if(GameState) {
      GameState->GlobalState = __Global;
   }
}