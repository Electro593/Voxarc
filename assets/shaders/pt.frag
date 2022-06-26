#version 460 core

in flat vec2 TexturePos;
in flat vec2 TextureSize;
in vec2 TextureCoords;
in flat uint AtlasIndex;

uniform sampler2DArray Atlases;

out vec4 FragColor;

void main()
{
   vec3 P;
   P.x = TexturePos.x + fract(TextureCoords.x)*TextureSize.x;
   P.y = TexturePos.y + fract(TextureCoords.y)*TextureSize.y;
   P.z = AtlasIndex;
   FragColor = texture(Atlases, P);
}