/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                         *
 *  Copyright (C) 2020 Andrew Seiler                                       *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.  *
 *                                                                         *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//TODO: Signed distance text rendering, to keep things looking sharp

internal void
UI_CreateStringObject(heap *Heap,
                      mesh *Mesh,
                      mesh_object *OutObject,
                      asset_pack *AssetPack,
                      str String,
                      v3r32 Pos,
                      r32 LetterHeight)
{
    ASSERT(WITHIN(-1.0f, Pos.X, 1.0f));
    ASSERT(WITHIN(-1.0f, Pos.Y, 1.0f));
    ASSERT(WITHIN(-1.0f, Pos.Z, 1.0f));
    ASSERT(WITHIN(0, LetterHeight, VIRTUAL_WINDOW_SIZE_Y));
    ASSERT(Mesh->Flags == MESH_HAS_TEXTURES);
    
    u32 PrintableCharCount = ChrArr_CountPrintableASCII(String);
    r32 SizeRatio = LetterHeight / ASSET_FONT_DEFAULT_HEIGHT;
    u32 Indices[] = {0, 1, 2, 0, 2, 3};
    chr *C = String;
    
    v2r32 Advance, Bearing, ScaledAssetSize;
    r32 Ascent =  (Asset_GetTag(AssetPack->Groups[0].GroupTags, AssetPack->Groups[0].GroupTagCount, AssetTagID_Ascent)->Value.R32 / VIRTUAL_WINDOW_SIZE_Y) * SizeRatio;
    r32 Descent = (Asset_GetTag(AssetPack->Groups[0].GroupTags, AssetPack->Groups[0].GroupTagCount, AssetTagID_Descent)->Value.R32 / VIRTUAL_WINDOW_SIZE_Y) * SizeRatio;
    r32 LineGap = (Asset_GetTag(AssetPack->Groups[0].GroupTags, AssetPack->Groups[0].GroupTagCount, AssetTagID_LineGap)->Value.R32 / VIRTUAL_WINDOW_SIZE_Y) * SizeRatio;
    Advance.Y = Ascent - Descent + LineGap;
    
    OutObject->VertexCount = 4 * PrintableCharCount;
    OutObject->IndexCount = 6 * PrintableCharCount;
    OutObject->Vertices = Heap_Allocate(Heap, OutObject->VertexCount * Mesh->VertexSize);
    OutObject->Indices = Heap_Allocate(Heap, OutObject->IndexCount * sizeof(u32));
    u08 *VertexCursor = OutObject->Vertices;
    u32 *IndexCursor = OutObject->Indices;
    v2r32 Cursor = V2r32_2x1(-1.0f, 1.0f - Ascent);
    
    v3r32 TexBounds = V3r32_3x1((r32)AssetPack->AtlasSize.X,
                                (r32)AssetPack->AtlasSize.Y,
                                (r32)AssetPack->AtlasCount);
    
    for(u32 PrintableCharIndex = 0;
        PrintableCharIndex < PrintableCharCount;
        ++C)
    {
        if(*C == '\n')
        {
            Cursor.X = -1.0f;
            Cursor.Y -= Advance.Y;
            continue;
        }
        
        asset_tag Tags[1] = {AssetTagID_Codepoint, *C};
        r32 Weights[1] = {1.0f};
        asset_info *Asset = Asset_GetFromTags(AssetPack, AssetGroupID_Font_Cour, Tags, Weights, 1);
        r32 *AdvanceArr = (r32*)Asset_GetExtra(AssetPack, Asset_GetTag(Asset->Tags, Asset->TagCount, AssetTagID_Advance));
        if(*(C+1) != '\0')
            Advance.X = (AdvanceArr[*(C+1)-32] / VIRTUAL_WINDOW_SIZE_X) * SizeRatio;
        else
            Advance.X = 0;
        
        if(*C == ' ')
        {
            goto IncrementCursor;
        }
        
        if(Chr_IsPrintableASCII(*C))
        {
            Bearing.X = (Asset_GetTag(Asset->Tags, Asset->TagCount, AssetTagID_BearingX)->Value.R32 / VIRTUAL_WINDOW_SIZE_X) * SizeRatio;
            Bearing.Y = (Asset_GetTag(Asset->Tags, Asset->TagCount, AssetTagID_BearingY)->Value.R32 / VIRTUAL_WINDOW_SIZE_Y) * SizeRatio;
            ScaledAssetSize.X = ((r32)Asset->Size.X / VIRTUAL_WINDOW_SIZE_X) * SizeRatio;
            ScaledAssetSize.Y = ((r32)Asset->Size.Y / VIRTUAL_WINDOW_SIZE_Y) * SizeRatio;
            
            v2r32 PosCoords[4];
            PosCoords[0] = V2r32_2x1(Bearing.X, Bearing.Y + ScaledAssetSize.Y);
            PosCoords[1] = V2r32_2x1(Bearing.X, Bearing.Y);
            PosCoords[2] = V2r32_2x1(Bearing.X + ScaledAssetSize.X, Bearing.Y);
            PosCoords[3] = V2r32_2x1(Bearing.X + ScaledAssetSize.X, Bearing.Y + ScaledAssetSize.Y);
            
            v3r32 TexCoords[4];
            TexCoords[0] = V3r32_3x1((r32)Asset->Offset.X + 0.5f,                      (r32)Asset->Offset.Y + (r32)Asset->Size.Y - 0.5f, (r32)Asset->AtlasIndex);
            TexCoords[1] = V3r32_3x1((r32)Asset->Offset.X + 0.5f,                      (r32)Asset->Offset.Y + 0.5f,                      (r32)Asset->AtlasIndex);
            TexCoords[2] = V3r32_3x1((r32)Asset->Offset.X + (r32)Asset->Size.X - 0.5f, (r32)Asset->Offset.Y + 0.5f,                      (r32)Asset->AtlasIndex);
            TexCoords[3] = V3r32_3x1((r32)Asset->Offset.X + (r32)Asset->Size.X - 0.5f, (r32)Asset->Offset.Y + (r32)Asset->Size.Y - 0.5f, (r32)Asset->AtlasIndex);
            
            for(u32 VertexIndex = 0;
                VertexIndex < 4;
                ++VertexIndex)
            {
                v3r32 Pos = V3r32_3x1(Cursor.X + PosCoords[VertexIndex].X,
                                      Cursor.Y + PosCoords[VertexIndex].Y,
                                      0.0f);
                *(bfs32*)VertexCursor = Mesh_MakePosCoord(Pos);
                bfs32_1x2_3x10_d PosDEBUG = *(bfs32_1x2_3x10_d*)VertexCursor; UNUSED(PosDEBUG);
                VertexCursor += sizeof(bfs32);
                
                *(v3u16*)VertexCursor = Mesh_MakeTexCoord(TexBounds, TexCoords[VertexIndex]);
                VertexCursor += sizeof(v3u16);
                
                VertexCursor += sizeof(u16);
            }
            
            for(u32 IndexIndex = 0;
                IndexIndex < 6;
                ++IndexIndex)
            {
                *IndexCursor++ = (u32)(PrintableCharIndex * 4) + Indices[IndexIndex];
            }
            
            ++PrintableCharIndex;
            
            IncrementCursor:
            Cursor.X += Advance.X;
        }
    }
    
    OutObject->ModelMatrix = M4x4r32_ModelMatrix(V3r32_3x1(Pos.X + 1.0f, Pos.Y - 1.0f, Pos.Z),
                                                 V3r32_3x1(1.0f, 1.0f, 1.0f));
}