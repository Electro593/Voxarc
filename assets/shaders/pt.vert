#version 460

layout(location = 0) in uint PositionIn;
layout(location = 2) in uint TextureIndex;

struct texture_data {
   uvec2 Pos;
   uvec2 Size;
   vec2 SizeR;
   vec2 Bearing;
   float AdvanceX;
   uint TheRest;
};

layout(std430, binding = 0) readonly buffer TextureStorage {
   texture_data TextureData[];
};
layout(std430, binding = 1) readonly buffer MatrixStorage {
   mat4 Matrices[];
};

uniform uvec2 AtlasSize;
uniform mat4 VPMatrix;

out vec4 Color;
out vec2 TexCoords;
out flat uint AtlasIndex;

void main()
{
   vec4 Position = vec4(float((PositionIn>> 0)&0xFFF),
                        float((PositionIn>>12)&0xFFF),
                        float((PositionIn>>24)&0xFF),
                        1);
   
   if((TextureIndex & 0x80000000) != 0) {
      Color.x = float((TextureIndex>>18)&0xFF)/255;
      Color.y = float((TextureIndex>>10)&0xFF)/255;
      Color.z = float((TextureIndex>> 2)&0xFF)/255;
      Color.w = 1;
   } else {
      uint Right = TextureIndex & 1;
      uint Up = (TextureIndex & 2) >> 1;
      texture_data Data = TextureData[TextureIndex>>2];
      
      float XCoord = float(Data.Pos.x + Data.Size.x*Right) / float(AtlasSize.x);
      float YCoord = float(Data.Pos.y + Data.Size.y*Up) / float(AtlasSize.y);
      TexCoords = vec2(XCoord, YCoord);
      
      AtlasIndex = (Data.TheRest >> 0) & 0xFFFF;
      
      Color.w = 0;
   }
   
   mat4 MVPMatrix = Matrices[gl_DrawID]*VPMatrix;
   gl_Position = Position*MVPMatrix;
}