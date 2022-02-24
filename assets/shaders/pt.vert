#version 460

layout(location = 0) in vec4 PositionIn;
layout(location = 2) in uint TextureIndex;

struct texture_data {
    uvec2 Pos;
    uvec2 Size;
    ivec2 Bearing;
    int AdvanceX;
    uint TheRest;
};

layout(std430, binding = 0) readonly buffer Storage {
    texture_data TextureData[];
};

out vec2 TexCoords;
out flat uint AtlasIndex;

void main()
{
    gl_Position = PositionIn.xyzw;
    
    uint Right = TextureIndex & 1;
    uint Up = (TextureIndex & 2) >> 1;
    int ActualTextureIndex = int(TextureIndex >> 2);
    
    texture_data Data = TextureData[ActualTextureIndex];
    vec2 Pos = vec2(Data.Pos);
    vec2 Size = vec2(Data.Size);
    AtlasIndex = (Data.TheRest >> 16) & 0xFFFF;
    
    float XCoord = (Pos.x + Size.x*Right)/256;
    float YCoord = (Pos.y + Size.y*Up)/256;
    TexCoords = vec2(XCoord, YCoord);
}