#version 450

layout(location = 0) in vec4 PositionIn;
layout(location = 3) in uint TextureIndex;

uniform sampler2D TextureData;

out vec2 TexCoords;

void main()
{
    gl_Position = PositionIn;
    
    TexCoords = TextureData[TextureIndex];
}