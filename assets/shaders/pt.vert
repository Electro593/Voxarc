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
out flat vec2 TexturePos;
out flat vec2 TextureSize;

void main()
{
   mat4 MVPMatrix = Matrices[gl_DrawID]*VPMatrix;
   gl_Position = PositionIn*MVPMatrix;
   
   uint Right = TextureData & 1;
   uint Up = (TextureData >> 1) & 1;
   uint DWords = (TextureData >> 2) & 0x3FFFFF;
   uint RepeatX = (TextureData >> 24) & 0xF;
   uint RepeatY = (TextureData >> 28) & 0xF;
   
   texture_data Data;
   Data.Pos.x   = Assets[DWords+0];
   Data.Pos.y   = Assets[DWords+1];
   Data.Size.x  = Assets[DWords+2];
   Data.Size.y  = Assets[DWords+3];
   Data.AtlasIndex = Assets[DWords+4];
   
   // float U = float(Data.Pos.x + Data.Size.x*Right) / float(AtlasSize.x);
   // float V = float(Data.Pos.y + Data.Size.y*Up) / float(AtlasSize.y);
   TextureCoords = vec2(Right*(RepeatX+1), Up*(RepeatY+1));
   
   TexturePos = vec2(Data.Pos) / vec2(AtlasSize);
   TextureSize = vec2(Data.Size) / vec2(AtlasSize);
   
   #if LITTLE_ENDIAN
      AtlasIndex = Data.AtlasIndex & 0xFFFF;
   #else
      AtlasIndex = (Data.AtlasIndex>>16) & 0xFFFF;
   #endif
}