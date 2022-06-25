#version 460 core

in vec2 TextureCoords;
in flat uint AtlasIndex;

uniform sampler2DArray Atlases;

out vec4 FragColor;

void main()
{
   vec3 P = vec3(TextureCoords, AtlasIndex);
   FragColor = texture(Atlases, P);
}