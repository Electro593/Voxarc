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
   Game->Pos = (v3r32){0,1,0};
   Game->Velocity = (v3r32){0,0,0};
   
   Renderer_Init(Renderer, RendererHeap, Platform->WindowSize);
   
   Stack_Push();
   
   u32 *TextureBytes = Stack_Allocate(sizeof(u32) * BLOCK_Count);
   for(u32 I = 1; I < BLOCK_Count; I++) {
      assetpack_tag *Tag = Assetpack_FindExactTag(Renderer->Assetpack, TAG_BLOCK_TEXTURE, (vptr)(u64)I);
      assetpack_asset *Asset = Tag->Assets[0];
      TextureBytes[I] = (u64)((u08*)Asset - (u64)Renderer->Assetpack.Assets);
   }
   
   chunk Chunk = MakeChunk(RendererHeap, &Renderer->PTMesh, (v3s32){0,0,0}, TextureBytes);
   mesh_object *Objects[] = {&Chunk.Object};
   Mesh_AddObjects(&Renderer->PTMesh, 1, Objects);
   Mesh_FreeObject(Chunk.Object);
   
   Mesh_Update(&Renderer->PTMesh);
   
   Stack_Pop();
}

internal void
Game_Update(platform_state *Platform,
            game_state *Game,
            renderer_state *Renderer)
{
   if(Platform->Updates & WINDOW_RESIZED) {
      Renderer_Resize(Platform->WindowSize, &Renderer->PerspectiveMatrix);
      Renderer->WindowSize = Platform->WindowSize;
   }
   if(Platform->Updates & CURSOR_DISABLED) {
      Game->PrevCursorPos = Platform->CursorPos;
      Platform->Updates &= ~CURSOR_DISABLED;
   }
   
   b08 Moved = FALSE;
   v3r32 DirDelta = (v3r32){0};
   
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
      if(Game->Pos.Y == 0) {
         if(Platform->Keys[ScanCode_Space] != RELEASED) {
            Game->KeySpaceWasDown = TRUE;
            
            if(Game->JumpCharge < MaxCharge)
               Game->JumpCharge++;
         }
         
         if(Platform->Keys[ScanCode_Space] == RELEASED) {
            if(Game->KeySpaceWasDown) {
               Game->JumpTime = Game->JumpCharge;
               Game->KeySpaceWasDown = FALSE;
            }
            
            if(Game->JumpTime > 0) {
               Game->JumpTime -= MaxCharge / ReleaseTicks;
               // Change camera pos here
               
               if(Game->JumpTime <= 0) {
                  Game->Velocity.Y = (Game->JumpCharge / MaxCharge) * JumpVelocity / TicksPerSecond;
                  Game->JumpCharge = 0;
               } else
                  ForceY += Game->Gravity * Game->Mass;
            }
         } else
            ForceY += Game->Gravity * Game->Mass;
      }
      
      Game->Acceleration.Y = ForceY / Game->Mass / (TicksPerSecond*TicksPerSecond);
      Game->Velocity.Y += Game->Acceleration.Y;
      
      if(!V3r32_IsEqual(Game->Velocity, (v3r32){0})) {
         Game->Pos.X += Game->Velocity.X*R32_cos(Game->Dir.Y) - Game->Velocity.Z*R32_sin(Game->Dir.Y);
         Game->Pos.Z += Game->Velocity.X*R32_sin(Game->Dir.Y) + Game->Velocity.Z*R32_cos(Game->Dir.Y);
         Game->Pos.Y += Game->Velocity.Y;
         
         Moved = TRUE;
      }
      
      if(Game->Pos.Y < 0) {
         Game->Pos.Y = 0;
         Game->Velocity.Y = 0;
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
      // v3r32 Right = { SinYaw,           0,           -CosYaw         };
      // v3r32 Up    = {-CosYaw*SinPitch,  CosPitch,    -SinYaw*SinPitch};
      // v3r32 Front = { CosYaw*CosPitch,  SinPitch,     SinYaw*CosPitch};
      v3r32 Pos   = {-Game->Pos.X,     -Game->Pos.Y, -Game->Pos.Z    };
      
      Renderer->ViewMatrix = (m4x4r32){
         Right.X, Right.Y, Right.Z, V3r32_Dot(Right, Pos),
         Up.X,    Up.Y,    Up.Z,    V3r32_Dot(Up,    Pos),
         Front.X, Front.Y, Front.Z, V3r32_Dot(Front, Pos),
         0,       0,       0,       1
      };
      
      m4x4r32 VPMatrix = M4x4r32_Mul(Renderer->PerspectiveMatrix, Renderer->ViewMatrix);
      
      OpenGL_UseProgram(Renderer->PCProgram);
      OpenGL_UniformMatrix4fv(Renderer->PCMesh.VPMatrix,  1, FALSE, VPMatrix);
      
      OpenGL_UseProgram(Renderer->PTProgram);
      OpenGL_UniformMatrix4fv(Renderer->PTMesh.VPMatrix, 1, FALSE, VPMatrix);
      
      Platform->Updates &= ~WINDOW_RESIZED;
   }
   
   Renderer_Draw(Game, Renderer, Platform->FPS);
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