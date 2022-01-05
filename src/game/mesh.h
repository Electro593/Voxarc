/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_MESH_H_
#define GAME_MESH_H_

//TODO: Consider mapped GPU memory

#define MESH_POS_MAX  511 // 0001 11111111

typedef enum mesh_flags
{
    MESH_IS_DYNAMIC   = 0x01,
    MESH_HAS_NORMALS  = 0x02,
    MESH_HAS_TEXTURES = 0x04,
    MESH_HAS_COLORS   = 0x08,
    
    MESH_FLAGS_COUNT
} mesh_flags;

typedef struct mesh_object
{
    hvptr Vertices;
    hvptr Indices;
    
    u32 VertexCount;
    s32 IndexCount;
    
    m4x4r32 ModelMatrix;
} mesh_object;

//TODO: Batch rendering of some sort?
typedef struct mesh
{
    hmem Vertices; // bfs32_2_10_10_10_r, [bfs32_2_10_10_10_r], [v4u16], [v4u08]
    hmem Indices; // u32
    s32 *VertexCounts; // First element is 0
    s32 *IndexCounts;
    m4x4r32 *MMatrices;
    
    u64 VertexBufferSize;
    u64 IndexBufferSize;
    u64 StorageBufferSize;
    s32 ObjectCount;
    u32 TotalVertexCount;
    u32 TotalIndexCount;
    
    u32 ProgramID;
    u32 VAOID;
    u32 VBOID;
    u32 EBOID;
    u32 TextureID;
    u32 AtlasCountID;
    u32 SamplerID;
    u32 MPMatricesID;
    
    u08 VertexSize;
    flag08 Flags;
} mesh;

#define GAME__MESH__EXPORTS \

#define GAME__MESH__FUNCS \
    FUNC(bfs32, Mesh_MakePosCoord, v3r32 Coords) \
    FUNC(v3u16, Mesh_MakeTexCoord, v3r32 Bounds, v3r32 Coords) \
    FUNC(void,  Mesh_Init, heap *Heap, mesh *Mesh, flag08 Attributes) \
    FUNC(void,  Mesh_Bind, mesh *Mesh) \
    FUNC(void,  Mesh_Unbind, void) \
    FUNC(void,  Mesh_AddObjects, mesh *Mesh, u32 ObjectCount, mesh_object *Objects) \
    FUNC(void,  Mesh_Finalize, mesh *Mesh) \
    FUNC(void,  Mesh_Update, mesh *Mesh) \
    FUNC(void,  Mesh_Render, mesh *Mesh, m4x4r32 PMatrix, b08 UpdateMatrices) \
    FUNC(void,  Mesh_Free, mesh *Mesh) \

//NOTE: Ranges
// -512 <= Position <= 511
// -512 <= Normal   <= 511
//    0 <= Texture  <= 65535
//    0 <= Color    <= 255

//TODO: Use the extra 2 position bits for ambient occlusion

//NOTE: Layouts
// Attributes: P
// Layout: [-][  PX    PY    PZ  ]
//         [   bfs32_2_10_10_10  ]
//
// Attributes: PN
// Layout: [-][  PX    PY    PZ  ]  [-][  NZ    NY    NX  ]
//         [   bfs32_2_10_10_10  ]  [   bfs32_2_10_10_10  ]
//
// Attributes: PT
// Layout: [-][  PX    PY    PZ  ]  [  TX   TY   TZ  ][ - ]
//         [   bfs32_2_10_10_10  ]  [        v4u16        ]
//
// Attributes: PNT
// Layout: [-][  PX    PY    PZ  ]  [-][  NZ    NY    NX  ]  [  TX   TY   TZ  ][ - ]
//         [   bfs32_2_10_10_10  ]  [   bfs32_2_10_10_10  ]  [        v4u16        ]
//
// Attributes: PC
// Layout: [-][  PX    PY    PZ  ]  [  CR   CG   CB   CA  ]
//         [   bfs32_2_10_10_10  ]  [        v4u08        ]
//
// Attributes: PNC
// Layout: [-][  PX    PY    PZ  ]  [-][  NZ    NY    NX  ]  [  CR   CG   CB   CA  ]
//         [   bfs32_2_10_10_10  ]  [   bfs32_2_10_10_10  ]  [        v4u08        ]
//
// Attributes: PTC
// Layout: [-][  PX    PY    PZ  ]  [  TX   TY   TZ  ][ - ]  [  CR   CG   CB   CA  ]
//         [   bfs32_2_10_10_10  ]  [        v4u16        ]  [        v4u08        ]
//
// Attributes: PNTC
// Layout: [-][  PX    PY    PZ  ]  [-][  NZ    NY    NX  ]  [  TX   TY   TZ  ][ - ]  [  CR   CG   CB   CA  ]
//         [   bfs32_2_10_10_10  ]  [   bfs32_2_10_10_10  ]  [        v4u16        ]  [        v4u08        ]

// typedef struct render_data_3d
// {
//     u32 ProgramID;
    
//     u32 VertexBufferID;
//     u32 ColorBufferID;
//     u32 ElementBufferID;
    
//     s32 ModelMatrixID;
//     s32 ViewMatrixID;
//     s32 MVPMatrixID;
//     s32 LightPositionID;
//     s32 LightPowerID;
//     s32 LightColorID;
//     s32 ChannelMaskID;
//     s32 ChannelRangeID;
//     s32 ChannelRangeShiftID;
//     s32 ChannelShiftID;
    
//     m4x4r32 ViewMatrix;
//     m4x4r32 ProjectionMatrix;
// } render_data_3d;

/* Cube meshData setup
    Vertices:
        -1, -1, -1
        -1, -1,  1
        -1,  1, -1
        -1,  1,  1
         1, -1, -1
         1, -1,  1
         1,  1, -1
         1,  1,  1
    
    Normals:
        -1,  0,  0,
         0, -1,  0,
         0,  0, -1,
         1,  0,  0,
         0,  1,  0,
         0,  0,  1,
    
    UVs:
        ( 0.0+0.5)/96.0, (32.0+0.5)/96.0,
        ( 0.0+0.5)/96.0, (63.0+0.5)/96.0,
        (31.0+0.5)/96.0, (32.0+0.5)/96.0,
        (31.0+0.5)/96.0, (63.0+0.5)/96.0,
        (32.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (32.0+0.5)/96.0, (31.0+0.5)/96.0,
        (32.0+0.5)/96.0, (32.0+0.5)/96.0,
        (32.0+0.5)/96.0, (63.0+0.5)/96.0,
        (32.0+0.5)/96.0, (64.0+0.5)/96.0,
        (32.0+0.5)/96.0, (95.0+0.5)/96.0,
        (63.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (63.0+0.5)/96.0, (31.0+0.5)/96.0,
        (63.0+0.5)/96.0, (32.0+0.5)/96.0,
        (63.0+0.5)/96.0, (63.0+0.5)/96.0,
        (63.0+0.5)/96.0, (64.0+0.5)/96.0,
        (63.0+0.5)/96.0, (95.0+0.5)/96.0,
        (64.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (64.0+0.5)/96.0, (31.0+0.5)/96.0,
        (64.0+0.5)/96.0, (32.0+0.5)/96.0,
        (64.0+0.5)/96.0, (63.0+0.5)/96.0,
        (95.0+0.5)/96.0, ( 0.0+0.5)/96.0,
        (95.0+0.5)/96.0, (31.0+0.5)/96.0,
        (95.0+0.5)/96.0, (32.0+0.5)/96.0,
        (95.0+0.5)/96.0, (63.0+0.5)/96.0
    
    Faces:
        Right
        6, 3, 23,  7, 3, 19,  5, 3, 18,
        6, 3, 23,  5, 3, 18,  4, 3, 22,
        Top
        6, 4, 15,  2, 4, 9,  3, 4, 8,
        6, 4, 15,  3, 4, 8,  7, 4, 14,
        Front
        1, 5, 6,  5, 5, 12,  7, 5, 13,
        1, 5, 6,  7, 5, 13,  3, 5, 7,
        Left
        1, 0, 2,  3, 0, 3,  2, 0, 1,
        1, 0, 2,  2, 0, 1,  0, 0, 0,
        Bottom
        1, 1, 5,  0, 1, 4,  4, 1, 10,
        1, 1, 5,  4, 1, 10,  5, 1, 11,
        Back
        6, 2, 17,  4, 2, 16,  0, 2, 20,
        6, 2, 17,  0, 2, 20,  2, 2, 21
*/

#endif