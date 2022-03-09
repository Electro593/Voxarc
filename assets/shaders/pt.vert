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

uniform uvec2 AtlasSize;

out vec2 TexCoords;
out flat uint AtlasIndex;

void main()
{
    gl_Position = PositionIn.xyzw;
    
    uint Right = TextureIndex & 1;
    uint Up = (TextureIndex & 2) >> 1;
    // texture_data Data = TextureData[TextureIndex>>2];
    texture_data Data = TextureData['~'-32];
    
    float XCoord = float(Data.Pos.x + Data.Size.x*Right) / float(AtlasSize.x);
    float YCoord = float(Data.Pos.y + Data.Size.y*Up) / float(AtlasSize.y);
    TexCoords = vec2(XCoord, YCoord);
    
    AtlasIndex = (Data.TheRest >> 0) & 0xFFFF;
}