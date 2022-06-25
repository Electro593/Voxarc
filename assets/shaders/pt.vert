#version 460 core

#define LITTLE_ENDIAN 1

layout(location = 0) in vec4 PositionIn;
layout(location = 2) in uint TextureData;

struct texture_data {
   uvec2 Pos;
   uvec2 Size;
   uint AtlasIndex;
};

layout(std430, binding = 0) readonly buffer TextureStorage {
   uint Assets[];
};
layout(std430, binding = 1) readonly buffer MatrixStorage {
   mat4 Matrices[];
};

uniform uvec2 AtlasSize;
uniform mat4 VPMatrix;

out vec2 TextureCoords;
out uint AtlasIndex;

void main()
{
   mat4 MVPMatrix = Matrices[gl_DrawID]*VPMatrix;
   gl_Position = PositionIn*MVPMatrix;
   
   uint Right = TextureData & 1;
   uint Up = (TextureData >> 1) & 1;
   uint DWords = TextureData >> 2;
   
   texture_data Data;
   Data.Pos.x   = Assets[DWords+0];
   Data.Pos.y   = Assets[DWords+1];
   Data.Size.x  = Assets[DWords+2];
   Data.Size.y  = Assets[DWords+3];
   Data.AtlasIndex = Assets[DWords+4];
   
   float U = float(Data.Pos.x + Data.Size.x*Right) / float(AtlasSize.x);
   float V = float(Data.Pos.y + Data.Size.y*Up) / float(AtlasSize.y);
   TextureCoords = vec2(U, V);
   
   #if LITTLE_ENDIAN
      AtlasIndex = Data.AtlasIndex & 0xFFFF;
   #else
      AtlasIndex = (Data.AtlasIndex>>16) & 0xFFFF;
   #endif
}