/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(INCLUDE_SOURCE) && !defined(INCLUDE_HEADER)
   #include <shared.h>
   
   #define INCLUDE_HEADER
      #include <util/main.c>
      #include <platform/platform.h>
      #include <renderer.h>
      #include "main.c"
      #include <renderer_opengl/main.c>
   #undef INCLUDE_HEADER
   
   #define INCLUDE_SOURCE
      #include "main.c"
   #undef INCLUDE_SOURCE
#else
   #include <base/file.c>
   #include <base/world.c>
   #include <base/ui.c>
   
   #ifdef INCLUDE_HEADER
      #define GAME_FUNCS \
         GAME_FILE_FUNCS \
         WORLD_FUNCS \
         UI_FUNCS \
      
      /* TODO: Unavoidable error sources that might need handling:
       - Computer state
         - Not enough memory
         - File doesn't exist
         - File is open
         - Not enough storage space
       - User input
         - Mouse
         - Keys
       - User output
         - No matching graphics modes
         - Missing hardware
      */
      
      #define TICKS_PER_DAY 480
      
      typedef struct collision_pole {
         u32 IntersectionCount;
         r32 *Intersections;
      } collision_pole;
      
      typedef struct game_state {
			r64 LastTime;
			
         b08 Key1WasDown;
         b08 KeySpaceWasDown;
         b08 TouchingGround;
         v3s32 WalkStep;
         
         heap *WorldHeap;
         region Region;
         
         // TODO: Split pos into PosInRegion, PosInChunk, and PosInBlock. The
         // first two should be s32, the last should be r32. That way, no floating
         // point issues crop up
         v3r32 Dir;
         v3r32 Pos;
         v3s32 AimBlock;
         b08 AimBlockValid;
         u32 AimBlockObjectIndex;
         u32 CrosshairObjectIndex;
         v3r32 Velocity;
         v3r32 Acceleration;
         r32 Mass;
         r32 Gravity;
         
         b08 Flying;
         v2s32 PrevCursorPos;
         
         u32 TimeOfDay;
      } game_state;
      
      typedef struct game_funcs {
         #define EXPORT(R, N, ...) R (*N)(__VA_ARGS__);
         #define X GAME_FUNCS
         #include <x.h>
      } game_funcs;
      
      #if defined(_OPENGL)
         #define RENDERER_NAME "renderer_opengl"
      #else
         #define RENDERER_NAME "renderer"
      #endif
      
      #if defined(_BASE_MODULE)
         #define EXPORT(R, N, ...) \
            internal R N(__VA_ARGS__);
         #define X GAME_FUNCS
         #include <x.h>
      #else
         #define EXPORT(R, N, ...) \
            global R (*N)(__VA_ARGS__);
         #define X GAME_FUNCS
         #include <x.h>
      #endif
   #endif
   
   #ifdef INCLUDE_SOURCE
      global platform_module *RendererModule;
      global renderer_state *Renderer;
      global game_state _G;
      global game_funcs _F;
      
      external API_EXPORT void
      Load(
         platform_state *Platform,
         platform_module *Module)
      {
         _F = (game_funcs){
            #define EXPORT(R, N, ...) N,
            #define X GAME_FUNCS
            #include <x.h>
         };
         
         Module->Data = &_G;
         Module->Funcs = &_F;
         
         #define EXPORT(R, S, N, ...) S##_##N = Platform->Functions.S##_##N;
         #define X PLATFORM_FUNCS
         #include <x.h>
         
         platform_module *UtilModule = Platform_LoadModule("util");
         util_funcs *UtilFuncs = UtilModule->Funcs;
         #define EXPORT(R, N, ...) N = UtilFuncs->N;
         #define X UTIL_FUNCS
         #include <x.h>
         
         RendererModule = Platform_LoadModule(RENDERER_NAME);
         Renderer = RendererModule->Data;
         renderer_funcs *RendererFuncs = RendererModule->Funcs;
         #define EXPORT(R, N, ...) N = RendererFuncs->N;
         #define X RENDERER_FUNCS
         #include <x.h>
      }
      
      external API_EXPORT void
      Init(
         platform_state *Platform)
      {
         game_state *Game = &_G;
         
         u64 RendererHeapSize = 32*1024*1024;
         u64 WorldHeapSize    = 32*1024*1024;
         vptr MemBase = Platform_AllocateMemory(RendererHeapSize+WorldHeapSize);
         
         heap *RendererHeap = Heap_Init(MemBase, RendererHeapSize);
         (u08*)MemBase += RendererHeapSize;
         
         Game->WorldHeap = Heap_Init(MemBase, WorldHeapSize);
         (u08*)MemBase += WorldHeapSize;
         
         random Random = Rand_Init(0);
         // random Random = Rand_Init(Asm_ReadTimeStampCounter());
         
         // File_CreateAssetpack("assets\\0.pack", RendererHeap, 64.0f);
         Renderer->Assetpack = File_LoadAssetpack("assets\\0.pack", RendererHeap);
         
         Game->Flying = TRUE;
         Game->Dir = (v3r32){0,0,0};
         Game->Pos = (v3r32){0,0,0};
         Game->TimeOfDay = TICKS_PER_DAY/4;
         Game->Velocity = (v3r32){0,0,0};
         Game->Acceleration = (v3r32){0,0,0};
			Game->LastTime = Platform_GetTime();
         Renderer->Heap = RendererHeap;
         RendererModule->Init(Platform);
         
         Renderer_SetUniform_V3r32(Renderer, ShaderID_PNM3, Renderer->Shaders[ShaderID_PNM3].Mesh.CameraPos, Game->Pos);
         Renderer_SetUniform_V3r32(Renderer, ShaderID_PNM3, Renderer->Shaders[ShaderID_PNM3].Mesh.LightPos,
            (v3r32){-10000*R32_cos((r32)Game->TimeOfDay/TICKS_PER_DAY*2*R32_PI), 10000*R32_sin((r32)Game->TimeOfDay/TICKS_PER_DAY*2*R32_PI), 0});
         
         Stack_Push();
         
         u32 *TextureBytes = Stack_Allocate(sizeof(u32) * BLOCK_Count);
         for(u32 I = 1; I < BLOCK_Count; I++) {
            assetpack_asset *Asset = FindFirstAssetFromExactTag(Renderer->Assetpack, TAG_BLOCK_TEXTURE, &I);
            TextureBytes[I] = (u64)Asset - (u64)Renderer->Assetpack.Assets;
         }
         
         // World
         {
            Assert(ChunkDims.X  * ChunkDims.Y  * ChunkDims.Z  <= U16_MAX);
            Assert(RegionDims.X * RegionDims.Y * RegionDims.Z <= U16_MAX);
            //TODO: Oblong regions
            Assert(ChunkDims.X  == ChunkDims.Y  && ChunkDims.Y  == ChunkDims.Z);
            Assert(RegionDims.X == RegionDims.Y && RegionDims.Y == RegionDims.Z);
            
            Game->Region.Pos = (v3s32){0};
            Game->Region.Chunks = Heap_Allocate(Game->WorldHeap, 0);
            Game->Region.Nodes = Heap_Allocate(Game->WorldHeap, sizeof(region_node));
            Mem_Set(Game->Region.Nodes->Data, 0, sizeof(region_node));
            
            MakeNonVoxelChunk(Game->WorldHeap, &Random, &Game->Region, &Renderer->Shaders[ShaderID_PNM3].Mesh, (v3s32){0,0,0}, TextureBytes);
            MakeChunk(Game->WorldHeap, &Game->Region, &Renderer->Shaders[ShaderID_PT3].Mesh, (v3s32){0,0,1}, TextureBytes);
            MakeChunk(Game->WorldHeap, &Game->Region, &Renderer->Shaders[ShaderID_PT3].Mesh, (v3s32){1,0,0}, TextureBytes);
            
            u32 ChunkCount = Game->Region.Chunks->Size / sizeof(chunk);
            chunk *Chunks = Game->Region.Chunks->Data;
            u32 PTCount=0, PNM3Count=0;
            for(u32 I = 0; I < ChunkCount; I++) {
               if(Chunks[I].Blocks) PTCount++;
               else PNM3Count++;
            }
            mesh_object **PTObjects   = Stack_Allocate(sizeof(mesh_object*)*PTCount);
            mesh_object **PNM3Objects = Stack_Allocate(sizeof(mesh_object*)*PNM3Count);
            PTCount=0, PNM3Count=0;
            for(u32 I = 0; I < ChunkCount; I++) {
               if(Chunks[I].Blocks) PTObjects[PTCount++] = &Chunks[I].Object;
               else PNM3Objects[PNM3Count++] = &Chunks[I].Object;
            }
            Mesh_AddObjects(&Renderer->Shaders[ShaderID_PT3].Mesh,  PTCount,   PTObjects);
            Mesh_AddObjects(&Renderer->Shaders[ShaderID_PNM3].Mesh, PNM3Count, PNM3Objects);
            for(u32 I = 0; I < ChunkCount; I++)
               Mesh_FreeObject(Chunks[I].Object);
            Mesh_Update(&Renderer->Shaders[ShaderID_PT3].Mesh);
            Mesh_Update(&Renderer->Shaders[ShaderID_PNM3].Mesh);
         }
         
         // Block highlight
         Game->AimBlockObjectIndex = Mesh_ReserveObject(&Renderer->Shaders[ShaderID_P3].Mesh, 24, 0);
         
         // Crosshair
         {
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
            mesh_object *Objects[] = {&Object};
            Mesh_AddObjects(&Renderer->Shaders[ShaderID_PC2].Mesh, 1, Objects);
            Game->CrosshairObjectIndex = Object.Index;
            Mesh_Update(&Renderer->Shaders[ShaderID_PC2].Mesh);
            Mesh_FreeObject(Object);
         }
         
         Platform->ExecutionState = EXECUTION_RUNNING;
         
         Stack_Pop();
      }
		
      external API_EXPORT void
      Update(
         platform_state *Platform)
      {
         game_state *Game = &_G;
			
			r64 CurrentTime = Platform_GetTime();
			r64 DeltaTime = CurrentTime - Game->LastTime;
			Game->LastTime = CurrentTime;
         
         if(Platform->Updates & WINDOW_RESIZED) {
            Renderer_Resize(Platform->WindowSize, &Renderer->OrthographicMatrix, &Renderer->PerspectiveMatrix);
            
            Renderer_SetUniform_M4x4r32(Renderer, ShaderID_PC2, Renderer->Shaders[ShaderID_PC2].Mesh.VPMatrix, 1, FALSE, Renderer->OrthographicMatrix);
            
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
            Game->TimeOfDay = (Game->TimeOfDay+1) % TICKS_PER_DAY;
            Renderer_SetUniform_V3r32(Renderer, ShaderID_PNM3, Renderer->Shaders[ShaderID_PNM3].Mesh.LightPos,
               (v3r32){-10000*R32_cos((r32)Game->TimeOfDay/TICKS_PER_DAY*2*R32_PI), 10000*R32_sin((r32)Game->TimeOfDay/TICKS_PER_DAY*2*R32_PI), 0});
            
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
               if(Platform->Keys[ScanCode_D]) MoveXZ.X++;
               if(Platform->Keys[ScanCode_A]) MoveXZ.X--;
               if(Platform->Keys[ScanCode_W]) MoveXZ.Y++;
               if(Platform->Keys[ScanCode_S]) MoveXZ.Y--;
               
               r32 MoveY = 0;
               if(Platform->Keys[ScanCode_Space]) MoveY++;
               if(Platform->Keys[ScanCode_ShiftLeft] || Platform->Keys[ScanCode_ShiftRight])
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
               Game->Mass = 75; // kg
               Game->Gravity = -9.807; // m/s^2
					r32 WalkForce = 1 * Game->Mass;
					r32 GroundFriction = -15 * Game->Mass;
					r32 AirFriction = -0.05 * Game->Mass;
					
					v3r32 Force = {0};
					
					r32 SinY = R32_sin(Game->Dir.Y);
               r32 CosY = R32_cos(Game->Dir.Y);
					
					// Gravity
					Force.Y += Game->Gravity * Game->Mass;
					
					// Walking
					r32 DeltaFront = (!!Platform->Keys[ScanCode_W] - !!Platform->Keys[ScanCode_S]) * WalkForce * Game->Mass;
					r32 DeltaRight = (!!Platform->Keys[ScanCode_D] - !!Platform->Keys[ScanCode_A]) * WalkForce * Game->Mass;
					Force.X += -SinY*DeltaFront + CosY*DeltaRight;
					Force.Z += CosY*DeltaFront + SinY*DeltaRight;
					
					// Jumping
               if(Game->TouchingGround) {
                  Force.Y += -Game->Gravity * Game->Mass;
						
                  if(Platform->Keys[ScanCode_Space])
                     Game->KeySpaceWasDown = TRUE;
						
                  if(!Platform->Keys[ScanCode_Space] && Game->KeySpaceWasDown) {
                     Game->KeySpaceWasDown = FALSE;
							Force.Y += 6 / DeltaTime * Game->Mass;
						}
					}
					
					// Friction
					Force = V3r32_Add(Force, V3r32_MulS(Game->Velocity, AirFriction));
					Force = V3r32_Add(Force, (v3r32){
						Game->Velocity.X * GroundFriction,
						Game->Velocity.Y,
						Game->Velocity.Z * GroundFriction
					});
               
               Game->Acceleration = V3r32_DivS(Force, Game->Mass);
               Game->Velocity = V3r32_Add(Game->Velocity, V3r32_MulS(Game->Acceleration, DeltaTime));
               
               if(!V3r32_IsEqual(Game->Velocity, (v3r32){0})) {
                  Game->Pos = V3r32_Add(Game->Pos, V3r32_MulS(Game->Velocity, DeltaTime));
                  Moved = TRUE;
                  
                  Game->TouchingGround = FALSE;
                  chunk *Chunks = Game->Region.Chunks->Data;
                  v3r32 PlayerSize = {.6,1.8,.6};
                  v3s32 ChunkPos = {
                     R32_Floor(Game->Pos.X / ChunkDims.X + 0.5f),
                     R32_Floor(Game->Pos.Y / ChunkDims.Y + 0.5f),
                     R32_Floor(Game->Pos.Z / ChunkDims.Z + 0.5f)
                  };
                  v3r32 PosInChunk = {
							Game->Pos.X - ChunkPos.X * ChunkDims.X + ChunkDims.X/2,
							Game->Pos.Y - ChunkPos.Y * ChunkDims.Y + ChunkDims.Y/2,
							Game->Pos.Z - ChunkPos.Z * ChunkDims.Z + ChunkDims.Z/2
						};
						u32 ChunkIndex;
						if (GetChunk(&Game->Region, ChunkPos, &ChunkIndex)) {
							chunk Chunk = Chunks[ChunkIndex];
	                  //TODO: Fix this for multiple chunks
	                  for(s32 X = (s32)PosInChunk.X; X <= (s32)(PosInChunk.X + PlayerSize.X); X++) {
	                     for(s32 Z = (s32)PosInChunk.Z; Z <= (s32)(PosInChunk.Z + PlayerSize.Z); Z++) {
	                        b08 R = CollidesWithBlock(Chunk, (v3u32){X, (u32)(PosInChunk.Y-0.01f), Z}, PosInChunk, PlayerSize);
	                        Game->TouchingGround |= R;
	                     }
	                  }
	                  
	                  if(Game->TouchingGround) {
	                     if(PosInChunk.Y > (s32)PosInChunk.Y)
	                        Game->Pos.Y = (r32)((s32)PosInChunk.Y+1) + Chunk.Pos.Y*ChunkDims.Y - ChunkDims.Y/2;
	                     
	                     Game->Velocity.Y = 0;
	                  }
						}
               }
            }
         }
         
         if(Moved) {
            Renderer_SetUniform_V3r32(Renderer, ShaderID_PNM3, Renderer->Shaders[ShaderID_PNM3].Mesh.CameraPos,
               (v3r32){Game->Pos.X, Game->Pos.Y, Game->Pos.Z});
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
            
            for(u32 I = 0; I < Shader_Count; I++) {
               if(Renderer->Shaders[I].Mesh.Flags & MESH_HAS_PERSPECTIVE) {
                  Renderer_SetUniform_M4x4r32(Renderer, I, Renderer->Shaders[I].Mesh.VPMatrix, 1, FALSE, VPMatrix);
               }
            }
            
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
            
            v3s32 ChunkPos = {
               (s32)((Game->Pos.X + ChunkDims.X*1.5f) / ChunkDims.X) - 1,
               (s32)((Game->Pos.Y + ChunkDims.Y*1.5f) / ChunkDims.Y) - 1,
               (s32)((Game->Pos.Z + ChunkDims.Z*1.5f) / ChunkDims.Z) - 1
            };
            v3r32 PosInChunk = {
               Game->Pos.X + ChunkDims.X/2.0f - ChunkPos.X*ChunkDims.X,
               Game->Pos.Y + ChunkDims.Y/2.0f - ChunkPos.Y*ChunkDims.Y,
               Game->Pos.Z + ChunkDims.Z/2.0f - ChunkPos.Z*ChunkDims.Z
            };
            v3r32 AimBase = {PosInChunk.X, PosInChunk.Y+EyeHeight, PosInChunk.Z};
            
            Game->AimBlockValid = FALSE;
            
            b08 UpdateChunkPos = TRUE;
            block_type *Blocks;
            
            s32 X = (s32)AimBase.X;
            s32 Y = (s32)AimBase.Y;
            s32 Z = (s32)AimBase.Z;
            
            while(TRUE) {
               if(UpdateChunkPos) {
                  u32 ChunkIndex;
                  b08 Found = GetChunk(&Game->Region, ChunkPos, &ChunkIndex);
                  if(!Found) break;
                  
                  chunk *Chunks = Game->Region.Chunks->Data;
                  if(!Chunks[ChunkIndex].Blocks) break;
                  
                  Blocks = Chunks[ChunkIndex].Blocks->Data;
                  UpdateChunkPos = FALSE;
               }
               
               block_type BlockType = Blocks[INDEX_3D(X, Y, Z, ChunkDims.X, ChunkDims.Y)];
               
               if(BlockType != BLOCK_NONE) {
                  Game->AimBlock = (v3s32){X, Y, Z};
                  Game->AimBlockValid = TRUE;
                  break;
               }
               
               r32 Ts[3] = {0};
               b08 Successes[3] = {0};
               
               if(AimDir.X < 0)
                  Successes[0] = RayRectIntersectionA((v3r32){X,   Y,   Z  }, (v3r32){X,   Y+1, Z+1}, (v3r32){-1, 0, 0}, AimBase, AimDir, Ts+0, NULL);
               else
                  Successes[0] = RayRectIntersectionA((v3r32){X+1, Y,   Z  }, (v3r32){X+1, Y+1, Z+1}, (v3r32){ 1, 0, 0}, AimBase, AimDir, Ts+0, NULL);
               
               if(AimDir.Y < 0)
                  Successes[1] = RayRectIntersectionA((v3r32){X,   Y,   Z  }, (v3r32){X+1, Y,   Z+1}, (v3r32){ 0,-1, 0}, AimBase, AimDir, Ts+1, NULL);
               else
                  Successes[1] = RayRectIntersectionA((v3r32){X,   Y+1, Z  }, (v3r32){X+1, Y+1, Z+1}, (v3r32){ 0, 1, 0}, AimBase, AimDir, Ts+1, NULL);
               
               if(AimDir.Z < 0)
                  Successes[2] = RayRectIntersectionA((v3r32){X,   Y,   Z  }, (v3r32){X+1, Y+1, Z  }, (v3r32){ 0, 0,-1}, AimBase, AimDir, Ts+2, NULL);
               else
                  Successes[2] = RayRectIntersectionA((v3r32){X,   Y,   Z+1}, (v3r32){X+1, Y+1, Z+1}, (v3r32){ 0, 0, 1}, AimBase, AimDir, Ts+2, NULL);
               
               u32 MinI = 3;
               r32 MinT = R32_MAX;
               for(u32 I = 0; I < 3; I++) {
                  if(Successes[I] && MinT > Ts[I]) {
                     MinI = I;
                     MinT = Ts[I];
                  }
               }
               
               if(MinI == 0) {
                  if(AimDir.X < 0) X--;
                  else             X++;
               } else if(MinI == 1) {
                  if(AimDir.Y < 0) Y--;
                  else             Y++;
               } else {
                  if(AimDir.Z < 0) Z--;
                  else             Z++;
               }
               
               v3s32 Offset = V3s32_Add((v3s32){X, Y, Z}, ChunkDims);
               v3s32 Dir = V3s32_SubS(V3s32_Div(Offset, ChunkDims), 1);
               if(!V3s32_IsEqual(Dir, (v3s32){0})) {
                  ChunkPos = V3s32_Add(ChunkPos, Dir);
                  UpdateChunkPos = TRUE;
                  
                  AimBase.X -= Dir.X * ChunkDims.X;
                  AimBase.Y -= Dir.Y * ChunkDims.Y;
                  AimBase.Z -= Dir.Z * ChunkDims.Z;
                  
                  X = Offset.X % ChunkDims.X;
                  Y = Offset.Y % ChunkDims.Y;
                  Z = Offset.Z % ChunkDims.Z;
               }
               
               if(MinT > AimRange) break;
            }
            
            if(Game->AimBlockValid) {
               p_vertex *Vertex = Mesh_GetVertices(&Renderer->Shaders[ShaderID_P3].Mesh, Game->AimBlockObjectIndex);
               
               v3s32 B = Game->AimBlock;
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
               
               m4x4r32 Translation = M4x4r32_Translation(ChunkPos.X*16 - 7.5 + B.X, ChunkPos.Y*16 - 7.5 + B.Y, ChunkPos.Z*16 - 7.5 + B.Z);
               m4x4r32 Scaling     = M4x4r32_Scaling(0.5, 0.501, 0.5);
               m4x4r32 Rotation    = M4x4r32_I;
               
               m4x4r32 *Matrix = Mesh_GetMatrix(&Renderer->Shaders[ShaderID_P3].Mesh, Game->AimBlockObjectIndex);
               *Matrix = M4x4r32_Mul(M4x4r32_Mul(Translation, Rotation), Scaling);
               
               s32 *VertexCount = (s32*)Renderer->Shaders[ShaderID_P3].Mesh.VertexCounts->Data + Game->AimBlockObjectIndex;
               *VertexCount = 24;
               
               Mesh_Update(&Renderer->Shaders[ShaderID_P3].Mesh);
            }
         }
         
         Renderer_Draw(Platform, Game, Renderer, Platform->FPS);
      }
      
      external API_EXPORT void
      Unload(
         platform_state *Platform)
      {
         
      }
   #endif
#endif