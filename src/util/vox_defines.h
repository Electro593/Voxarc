#ifndef UTIL_VOX_DEFINES_H_

typedef       unsigned char       b08;
typedef                int        b32;

typedef                char       c08;
typedef const          char      cc08;
typedef       unsigned short      c16;
typedef const unsigned short     cc16;

typedef                char       s08;
typedef                short      s16;
typedef                int        s32;
typedef                long long  s64;

typedef       unsigned char       u08;
typedef const unsigned char      cu08;
typedef       unsigned short      u16;
typedef       unsigned int        u32;
typedef       unsigned long long  u64;

typedef                float      r32;
typedef                double     r64;

#ifdef _WIN64
typedef       unsigned long long  size;
#else
typedef       unsigned int        size;
#endif

typedef                void *     vptr;
typedef const          void *    cvptr;

#define NULL ((void *)0)

#define IGNORE -1
#define FALSE 0
#define TRUE 1

#define S08_MIN 0x80
#define S16_MIN 0x8000
#define S32_MIN 0x80000000
#define S64_MIN 0x8000000000000000
#define S08_MAX 0x7F
#define S16_MAX 0x7FFF
#define S32_MAX 0x7FFFFFFF
#define S64_MAX 0x7FFFFFFFFFFFFFFF
#define U08_MAX 0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFF
#define R32_MAX 3.40282e38
#define R32_MIN -1.17549e38

#define local_var  static
#define local_func static
#define global_var static
#define global_func

#ifdef _VOX_DEBUG
#define Assert(Condition) if(!(Condition)) (*(int*)0 = 0)
#define NOP {u32 _Breakpoint_ = 0;}
#else
#define Assert(Condition)
#endif

#define MAX_PATH_SIZE 256
#define MAX_ATLAS_DIM 512
#define BYTES_PER_PIXEL 4
#define ASSET_INFO_FILENAME "assets/info.voxai"
#if _VOX_AP_BITMAP
#define ASSET_ATLAS_FILENAME "assets/atlas0.bmp"
#else
#define ASSET_ATLAS_FILENAME "assets/atlas0.voxaa"
#endif
#define VOXAI_MAGIC_NUMBER (((u32)('v') << 0) | ((u32)('a') << 8) | ((u32)('a') << 16) | ((u32)('i') << 24))
#define VOXAI_VERSION 0 //TODO: Increment this

#define Kilobytes(Count) ((Count) * 1024ULL)
#define Megabytes(Count) (Kilobytes(Count) * 1024ULL)
#define Gigabytes(Count) (Megabytes(Count) * 1024ULL)
#define Terabytes(Count) (Gigabytes(Count) * 1024ULL)

#define OffsetOf(Struct, Member) ((size_t)&(((Struct*)0)->Member))

#define Align16(Num) (((Num) + 15) & ~15)

//NOTE: Struct forward declaration

//NOTE: game/vox_world.h
typedef struct block block;
typedef struct chunk chunk;
typedef struct world world;

//NOTE: game/vox_entity.h
typedef struct player player;

//NOTE: math/vox_m3x3r32.h
typedef union m3x3r32 m3x3r32;

//NOTE: math/vox_m4x4r32.h
typedef union m4x4r32 m4x4r32;

//NOTE: math/vox_math.h
typedef union convert_r32_s32 convert_r32_s32;

//NOTE: math/vox_random.h
typedef struct random random;

//NOTE: math/vox_v2r32.h
typedef union v2r32 v2r32;

//NOTE: math/vox_v2r64.h
typedef union v2r64 v2r64;

//NOTE: math/vox_v2s32.h
typedef union v2s32 v2s32;

//NOTE: math/vox_v2u32.h
typedef union v2u32 v2u32;

//NOTE: math/vox_v3r32.h
typedef union v3r32 v3r32;

//NOTE: math/vox_v3s32.h
typedef union v3s32 v3s32;

//NOTE: math/vox_v3u32.h
typedef union v3u32 v3u32;

//NOTE: math/vox_v4r32.h
typedef union v4r32 v4r32;

//NOTE: math/vox_v4u32.h
typedef union v4u32 v4u32;

//NOTE: util/vox_assets.h
typedef enum asset_type asset_type;
typedef enum asset_tag asset_tag;
typedef enum tag_search tag_search;
typedef struct voxai_header voxai_header;
typedef struct voxai_type voxai_type;
typedef struct voxai_tag voxai_tag;
typedef struct voxai_asset voxai_asset;
typedef struct voxai_bitmap voxai_bitmap;
typedef struct asset_memory asset_memory;

//NOTE: util/vox_debug.h
typedef struct debug_data debug_data;

//NOTE: util/vox_file_io.h
typedef struct config config;
typedef enum file_type file_type;

//NOTE: util/vox_memory.h
typedef struct memory_handle memory_handle;
typedef struct data_pool data_pool;
typedef struct handle_pool handle_pool;

//NOTE: util/vox_render.h
typedef struct mesh_data_3d mesh_data_3d;
typedef struct render_data_3d render_data_3d;
typedef struct mesh_data_2d mesh_data_2d;
typedef struct render_data_2d render_data_2d;

//NOTE: voxarc.h
typedef struct game_state game_state;
typedef struct platform_flags platform_flags;
typedef struct game_input game_input;

//NOTE: Function forward declaration

//NOTE: game/vox_world.c
local_func block
CreateBlock(v4r32 Color);

local_func block
GetBlockFromRelativePos(world *World,
                        v3u32 ChunkPos,
                        v3s32 BlockPos);

local_func chunk
CreateChunk(memory_handle *Handle,
            config Config,
            random *Random,
            v3u32 Pos);

local_func void
BuildChunk(world *World,
           chunk *Chunk,
           render_data_3d *RenderData);

local_func world
CreateWorld(memory_handle *Handle,
            config Config);

local_func void
RenderWorld(memory_handle *Handle,
            world *World,
            config Config,
            random *Random,
            render_data_3d *RenderData);

//NOTE: gmae/vox_entity.c
local_func void
UpdatePlayer(player *Player,
             m4x4r32 *ViewMatrix,
             v2s32 DeltaMouse,
             r32 Sensitivity,
             game_input *Input,
             config *Config);

//NOTE: math/vox_3x3r32.h
inline m3x3r32
M3x3r32_0(void);

inline m3x3r32
M3x3r32_3(v3r32 Col1,
          v3r32 Col2,
          v3r32 Col3);

inline v3r32
M3x3r32_Multiply(m3x3r32 M,
                 v3r32 V);

inline m3x3r32
Inverse(m3x3r32 M);

//NOTE: math/vox_4x4r32.h
inline m4x4r32
M4x4r32_0(void);

inline m4x4r32
M4x4r32_4_4_4_4(v4r32 Col1,
                v4r32 Col2,
                v4r32 Col3,
                v4r32 Col4);

inline m4x4r32
M4x4r32_Multiply(m4x4r32 A,
                 m4x4r32 B);

inline v4r32
M4x4r32_MultiplyV(m4x4r32 M,
                  v4r32 V);

local_func m4x4r32
M4x4r32_Inverse(m4x4r32 M);

inline m4x4r32
M4x4r32_Translate(m4x4r32 M,
                  r32 X,
                  r32 Y,
                  r32 Z);

inline m4x4r32
M4x4r32_Translation(r32 X,
                    r32 Y,
                    r32 Z);

inline m4x4r32
M4x4r32_RotateX(m4x4r32 M,
                r32 T);

inline m4x4r32
M4x4r32_RotateY(m4x4r32 M,
                r32 T);

inline m4x4r32
M4x4r32_Perspective(r32 FOV,
                    r32 AspectRatio,
                    r32 ZNear,
                    r32 ZFar);

inline m4x4r32
M4x4r32_Orthographic(r32 Left,
                     r32 Right,
                     r32 Bottom,
                     r32 Top);

//NOTE: math/vox_math.h
inline r32
R32_Abs(r32 N);

inline s32
S32_Abs(s32 N);

inline r32
Floor(r32 N);

inline r32
R32_Lerp(r32 A,
         r32 B,
         r32 T);

inline r32
S32_Lerp(s32 A,
         s32 B,
         r32 T);

inline r32
Radians(r32 D);

inline r32
Sin(r32 R);

inline r32
Cos(r32 R);

inline r32
Tan(r32 R);

inline r32
SqrtP(r32 N,
      u32 Precision);

inline r32
Sqrt(r32 N);

//NOTE: math/vox_random.h
inline random
InitRand(u32 Seed);

inline u32
RandInt(random *Random);

inline s32
RandRange(random *Random,
          s32 Min,
          s32 Max);

//NOTE: math/vox_v2r32.h
inline v2r32
V2r32_1(r32 N);

inline v2r32
V2r32_1_1(r32 X,
          r32 Y);

inline v2r32
V2r32_Add(v2r32 A,
          v2r32 B);

inline v2r32
V2r32_Subtract(v2r32 A,
               v2r32 B);

inline v2r32
V2r32_Multiply(v2r32 A,
               v2r32 B);

inline v2r32
V2r32_Divide(v2r32 A,
             v2r32 B);

inline v2r32
V2r32_DivideS(v2r32 V,
              r32 S);

inline r32
V2r32_Dot(v2r32 A,
          v2r32 B);

inline r32
V2r32_Length(v2r32 V);

inline v2r32
V2r32_Normalize(v2r32 V);

inline b08
V2r32_Equal(v2r32 A,
            v2r32 B);

inline b08
V2r32_LessThan(v2r32 A,
               v2r32 B);

inline b08
V2r32_GreaterThan(v2r32 A,
                  v2r32 B);

//NOTE: math/vox_v2r64.h
inline v2r64
V2r64_1(r32 N);

inline v2r64
V2r64_1_1(r32 X,
          r32 Y);

inline v2r64
V2r64_Add(v2r64 A,
          v2r64 B);

inline v2r64
V2r64_Subtract(v2r64 A,
               v2r64 B);

inline v2r64
V2r64_Multiply(v2r64 A,
               v2r64 B);

inline v2r64
V2r64_Divide(v2r64 A,
             v2r64 B);

inline v2r64
V2r64_DivideS(v2r64 V,
              r32 S);

inline r32
V2r64_Dot(v2r64 A,
          v2r64 B);

inline r32
V2r64_Length(v2r64 V);

inline v2r64
V2r64_Normalize(v2r64 V);

inline b08
V2r64_Equal(v2r64 A,
            v2r64 B);

inline b08
V2r64_LessThan(v2r64 A,
               v2r64 B);

inline b08
V2r64_GreaterThan(v2r64 A,
                  v2r64 B);

//NOTE: math/vox_v2s32.h
inline v2s32
V2s32_1(s32 N);

inline v2s32
V2s32_1_1(s32 X,
          s32 Y);

inline s32
V2s32_Dot(v2s32 A,
          v2s32 B);

inline r32
V2s32_Length(v2s32 V);

inline b08
V2s32_Equal(v2s32 A,
            v2s32 B);

inline b08
V2s32_NotEqual(v2s32 A,
               v2s32 B);

inline b08
V2s32_LessThan(v2s32 A,
               v2s32 B);

inline b08
V2s32_GreaterThan(v2s32 A,
                  v2s32 B);

inline v2s32
V2s32_Add(v2s32 A,
          v2s32 B);

inline v2s32
V2s32_Subtract(v2s32 A,
               v2s32 B);

inline v2s32
V2s32_Multiply(v2s32 A,
               v2s32 B);

inline v2s32
V2s32_Divide(v2s32 A,
             v2s32 B);

inline v2s32
V2s32_DivideS(v2s32 V,
              s32 S);

//NOTE: math/vox_v2u32.h
inline v2u32
V2u32_1(u32 N);

inline v2u32
V2u32_1_1(u32 X,
          u32 Y);

inline u32
V2u32_Dot(v2u32 A,
          v2u32 B);

inline r32
V2u32_Length(v2u32 V);

inline b08
V2u32_Equal(v2u32 A,
            v2u32 B);

inline b08
V2u32_LessThan(v2u32 A,
               v2u32 B);

inline b08
V2u32_GreaterThan(v2u32 A,
                  v2u32 B);

inline v2u32
V2u32_Add(v2u32 A,
          v2u32 B);

inline v2u32
V2u32_Subtract(v2u32 A,
               v2u32 B);

inline v2u32
V2u32_Multiply(v2u32 A,
               v2u32 B);

inline v2u32
V2u32_Divide(v2u32 A,
             v2u32 B);

inline v2u32
V2u32_DivideS(v2u32 V,
              u32 S);

//NOTE: math/vox_v3r32.h
inline v3r32
V3r32_1(r32 N);

inline v3r32
V3r32_2_1(v2r32 XY,
          r32 Z);

inline v3r32
V3r32_1_1_1(r32 X,
            r32 Y,
            r32 Z);

inline v3r32
V3r32_Add(v3r32 A,
          v3r32 B);

inline v3r32
V3r32_Subtract(v3r32 A,
               v3r32 B);

inline v3r32
V3r32_Multiply(v3r32 A,
               v3r32 B);

inline v3r32
V3r32_MultiplyS(v3r32 V,
                r32 S);

inline v3r32
V3r32_Divide(v3r32 A,
             v3r32 B);

inline v3r32
V3r32_DivideS(v3r32 V,
              r32 S);

inline r32
V3r32_Dot(v3r32 A,
          v3r32 B);

inline v3r32
V3r32_Cross(v3r32 A,
            v3r32 B);

inline r32
V3r32_Length(v3r32 V);

inline v3r32
V3r32_Normalize(v3r32 V);

inline b08
V3r32_Equal(v3r32 A,
            v3r32 B);

inline b08
V3r32_LessThan(v3r32 A,
               v3r32 B);

inline b08
V3r32_GreaterThan(v3r32 A,
                  v3r32 B);

//NOTE: math/vox_v3s32.h
inline v3s32
V3s32_1(s32 N);

inline v3s32
V3s32_2_1(v2s32 XY,
          s32 Z);

inline v3s32
V3s32_1_1_1(s32 X,
            s32 Y,
            s32 Z);

inline v3s32
V3s32_Add(v3s32 A,
          v3s32 B);

inline v3s32
V3s32_Subtract(v3s32 A,
               v3s32 B);

inline v3s32
V3s32_Multiply(v3s32 A,
               v3s32 B);

inline v3s32
V3s32_MultiplyS(v3s32 V,
                s32 S);

inline v3s32
V3s32_Divide(v3s32 A,
             v3s32 B);

inline v3s32
V3s32_DivideS(v3s32 V,
              s32 S);

inline s32
V3s32_Dot(v3s32 A,
          v3s32 B);

inline v3s32
V3s32_Cross(v3s32 A,
            v3s32 B);

inline r32
V3s32_Length(v3s32 V);

inline v3s32
V3s32_Normalize(v3s32 V);

inline b08
V3s32_Equal(v3s32 A,
            v3s32 B);

inline b08
V3s32_LessThan(v3s32 A,
               v3s32 B);

inline b08
V3s32_GreaterThan(v3s32 A,
                  v3s32 B);

//NOTE: math/vox_v3u32.h
inline v3u32
V3u32_1(u32 N);

inline v3u32
V3u32_2_1(v2u32 XY,
          u32 Z);

inline v3u32
V3u32_1_1_1(u32 X,
            u32 Y,
            u32 Z);

inline v3u32
V3u32_Add(v3u32 A,
          v3u32 B);

inline v3u32
V3u32_AddS(v3u32 V,
           u32 S);

inline v3u32
V3u32_Subtract(v3u32 A,
               v3u32 B);

inline v3u32
V3u32_Multiply(v3u32 A,
               v3u32 B);

inline v3u32
V3u32_MultiplyS(v3u32 V,
                u32 S);

inline v3u32
V3u32_Divide(v3u32 A,
             v3u32 B);

inline v3u32
V3u32_DivideS(v3u32 V,
              u32 S);

inline u32
V3u32_Dot(v3u32 A,
          v3u32 B);

inline v3u32
V3u32_Cross(v3u32 A,
            v3u32 B);

inline r32
V3u32_Length(v3u32 V);

inline v3u32
V3u32_Normalize(v3u32 V);

inline b08
V3u32_Equal(v3u32 A,
            v3u32 B);

inline b08
V3u32_LessThan(v3u32 A,
               v3u32 B);

inline b08
V3u32_GreaterThan(v3u32 A,
                  v3u32 B);

//NOTE: math/vox_v4r32.h
inline v4r32
V4r32_1(r32 N);

inline v4r32
V4r32_3_1(v3r32 XYZ,
          r32 W);

inline v4r32
V4r32_2_1_1(v2r32 XY,
            r32 Z,
            r32 W);

inline v4r32
V4r32_2_2(v2r32 XY,
          v2r32 ZW);

inline v4r32
V4r32_1_1_1_1(r32 X,
              r32 Y,
              r32 Z,
              r32 W);

inline v4r32
V4r32_Add(v4r32 A,
          v4r32 B);

inline v4r32
V4r32_Subtract(v4r32 A,
               v4r32 B);

inline v4r32
V4r32_Multiply(v4r32 A,
               v4r32 B);

inline v4r32
V4r32_MultiplyS(v4r32 V,
                r32 S);

inline v4r32
V4r32_Divide(v4r32 A,
             v4r32 B);

inline v4r32
V4r32_DivideS(v4r32 V,
              r32 S);

inline r32
V4r32_Dot(v4r32 A,
          v4r32 B);

inline r32
V4r32_Length(v4r32 V);

inline v4r32
V4r32_Normalize(v4r32 V);

inline b08
V4r32_Equal(v4r32 A,
            v4r32 B);

inline b08
V4r32_NotEqual(v4r32 A,
               v4r32 B);

inline b08
V4r32_LessThan(v4r32 A,
               v4r32 B);

inline b08
V4r32_GreaterThan(v4r32 A,
                  v4r32 B);

//NOTE: math/vox_v4u32.h
inline v4u32
V4u32_1(u32 N);

inline v4u32
V4u32_3_1(v3u32 XYZ,
          u32 W);

inline v4u32
V4u32_2_1_1(v2u32 XY,
            u32 Z,
            u32 W);

inline v4u32
V4u32_2_2(v2u32 XY,
          v2u32 ZW);

inline v4u32
V4u32_1_1_1_1(u32 X,
              u32 Y,
              u32 Z,
              u32 W);

inline v4u32
V4u32_Add(v4u32 A,
          v4u32 B);

inline v4u32
V4u32_Subtract(v4u32 A,
               v4u32 B);

inline v4u32
V4u32_Multiply(v4u32 A,
               v4u32 B);

inline v4u32
V4u32_MultiplyS(v4u32 V,
                u32 S);

inline v4u32
V4u32_Divide(v4u32 A,
             v4u32 B);

inline v4u32
V4u32_DivideS(v4u32 V,
              u32 S);

inline u32
V4u32_Dot(v4u32 A,
          v4u32 B);

inline r32
V4u32_Length(v4u32 V);

inline v4u32
V4u32_Normalize(v4u32 V);

inline b08
V4u32_Equal(v4u32 A,
            v4u32 B);

inline b08
V4u32_LessThan(v4u32 A,
               v4u32 B);

inline b08
V4u32_GreaterThan(v4u32 A,
                  v4u32 B);

//NOTE: util/vox_assets.c
inline voxai_header *
GetVoxaiHeader(memory_handle *Voxai);

inline memory_handle *
LoadVoxai(handle_pool *Pool,
          c08 *Path);

inline memory_handle *
LoadVoxaa(handle_pool *Pool,
          c08 *Path);

inline voxai_type *
GetVoxaiType(memory_handle *Voxai,
             asset_type Type);

inline voxai_asset *
GetVoxaiAssetFromExactTags(memory_handle *Voxai,
                           voxai_type *Type,
                           u32 TagsCount,
                           memory_handle **Tags);

inline voxai_bitmap *
GetVoxaiBitmap(memory_handle *Voxai,
               tag_search SearchType,
               u32 TagsCount,
               memory_handle **Tags);

//NOTE: util/vox_debug.c
local_func void
InitDebugData(debug_data *DebugData,
              handle_pool *Pool);

local_func void
CreateDebugString(c08 *String,
                  handle_pool *SearchPool,
                  asset_memory *AssetMemory,
                  render_data_2d *RenderData,
                  debug_data *DebugData,
                  u32 OffsetX,
                  u32 OffsetY);

//NOTE: util/vox_file_io.c
local_func memory_handle *
VoxReadFile(handle_pool *HandlePool,
            c08 *FilePath);

local_func config
ReadConfig(memory_handle *Handle,
           c08 *ConfigFilePath);

local_func u32
LoadShaders(memory_handle *Handle,
            c08 *VertexShaderFilePath,
            c08 *FragmentShaderFilePath);

//NOTE: util/vox_memory.h
inline void
SetMemory(vptr Dest,
          u32 Data,
          size Size);

inline void
CopyMemory(vptr Dest,
           cvptr Src,
           size Size);

inline handle_pool *
GetHandlePool(memory_handle *Handle);

inline void
CopyHandles(memory_handle *Dest,
            memory_handle *Src);

inline handle_pool *
CreateHandlePool(size Size);

inline void
DefragmentMemoryPools(handle_pool *HandlePool);

inline vptr
FindFreeMemory(handle_pool *HandlePool,
               size Size);

inline memory_handle *
FindUnusedHandle(handle_pool *HandlePool);

inline memory_handle *
AllocateMemory(handle_pool *HandlePool,
               size Size);

inline void
FreeMemory(memory_handle *Handle);

inline memory_handle *
ResizeMemory(memory_handle *Handle,
             size Size);

inline s32
CompareMemory(vptr A,
              vptr B,
              size Size);

inline s32
CompareArrays(c08 *A,
              c08 *B);

inline u32
PointerCount(void *Ptr);

//NOTE: util/vox_render.c
local_func void
Render3DMesh(render_data_3d *RenderData,
             mesh_data_3d *MeshData);

local_func void
Index3DVertices(memory_handle *Vertices,
                memory_handle *Colors,
                memory_handle *Indices);

local_func void
Render2DMesh(render_data_2d *RenderData,
             mesh_data_2d *MeshData);

//NOTE: util/vox_string.h
inline b08
String_EqualC(memory_handle *A,
              c08 *B);

inline b08
String_Equal(memory_handle *A,
             memory_handle *B);

inline c08
ToLower(c08 C);

inline b08
IsSpace(c08 C);

inline b08
IsDigit(c08 C);

inline b08
IsBinary(c08 C);

inline b08
IsHex(c08 C);

local_func u32
String_ToU32(vptr Base);

local_func r32
String_ToR32(vptr Base);

inline v3u32
String_ToV3u32(vptr Base);

inline memory_handle *
String_Contatenate(memory_handle *A,
                   memory_handle *B);

inline memory_handle *
String_ContatenateCS(char *A,
                     memory_handle *B);

#define UTIL_VOX_DEFINES_H_
#endif