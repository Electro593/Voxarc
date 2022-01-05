/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UTIL_DEBUG_H_
#define UTIL_DEBUG_H_


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      SECTION: Struct Printing
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//SUBSECTION: Functions


#if 0
typedef struct debug_data
{
    handle_pool *Memory;
    mesh_data_2d MeshData;
} debug_data;

internal void
InitDebugData(debug_data *DebugData,
              handle_pool *Pool)
{
    DebugData->Memory = Pool;
    DebugData->MeshData.Vertices = AllocateHandle(Pool, 0);
}

internal void
CreateDebugString(str String,
                  handle_pool *SearchPool,
                  asset_memory *AssetMemory,
                  render_data_2d *RenderData,
                  debug_data *DebugData,
                  r32 Scale,
                  r32 OffsetX,
                  r32 OffsetY,
                  r32 Depth,
                  r32 WindowWidth,
                  r32 WindowHeight)
{
    if(Depth == 1.0f)
    {
        Depth -= R32_EPSILON;
    }
    WindowWidth /= 2.0f;
    WindowHeight /= 2.0f;
    
    u32 StrSize = Str_Len(String);
    u32 DeltaSize = 6 * MESH2D_STRIDE;
    u64 PrevSize = DebugData->MeshData.Vertices->Size;
    
    u32 TextureCount = 0;
    str SpaceTest = String;
    while(*SpaceTest)
    {
        if(Chr_IsPrintableASCII(*SpaceTest++))
        {
            TextureCount++;
        }
    }
    DebugData->MeshData.Vertices = ResizeHandle(DebugData->MeshData.Vertices, PrevSize + (TextureCount * DeltaSize));
    
    u32 PrintedIndex = 0;
    for(u32 Index = 0;
        Index < StrSize;
        ++Index)
    {
        chr Char = String[Index];
        
        memory_handle *Tags[2];
        
        Tags[0] = AllocateHandle(SearchPool, sizeof(voxai_tag));
        ((voxai_tag*)Tags[0]->Base)->ID = AssetTag_Codepoint;
        ((voxai_tag*)Tags[0]->Base)->Value = (r32)Char;
        
        Tags[1] = AllocateHandle(SearchPool, sizeof(voxai_tag));
        ((voxai_tag*)Tags[1]->Base)->ID = AssetTag_IsMonospace;
        ((voxai_tag*)Tags[1]->Base)->Value = (r32)1;
        
        voxai_bitmap *Data = GetVoxaiBitmap(AssetMemory->Info, TagSearch_Exact, 2, Tags);
        voxai_header *Header = GetVoxaiHeader(AssetMemory->Info);
        
        FreeHandle(Tags[0]);
        FreeHandle(Tags[1]);
        
        if(Data && Data->Width && Data->Height)
        {
            r32 W = Scale * ((r32)Data->Width / WindowWidth);
            r32 H = Scale * ((r32)Data->Height / WindowHeight);
            r32 U1 = ((r32)Data->OffsetX) / (r32)Header->AtlasDim;
            r32 V1 = ((r32)Data->OffsetY) / (r32)Header->AtlasDim;
            r32 U2 = ((r32)Data->OffsetX + (r32)Data->Width) / (r32)Header->AtlasDim;
            r32 V2 = ((r32)Data->OffsetY + (r32)Data->Height) / (r32)Header->AtlasDim;
            
            r32 Vertices[] = {
                OffsetX    , OffsetY + H, Depth, U1, V2,
                OffsetX    , OffsetY    , Depth, U1, V1,
                OffsetX + W, OffsetY    , Depth, U2, V1,
                OffsetX    , OffsetY + H, Depth, U1, V2,
                OffsetX + W, OffsetY    , Depth, U2, V1,
                OffsetX + W, OffsetY + H, Depth, U2, V2
            };
            
            Mem_Cpy((u08*)DebugData->MeshData.Vertices->Base + PrevSize + (PrintedIndex * DeltaSize),
                       Vertices, DeltaSize);
            
            
            OffsetX += W;
            ++PrintedIndex;
        }
    }
    
    glUseProgram(RenderData->ProgramID);
    glBindBuffer(GL_ARRAY_BUFFER, RenderData->VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, DebugData->MeshData.Vertices->Size,
                 DebugData->MeshData.Vertices->Base, GL_STATIC_DRAW);
}
#endif

#endif