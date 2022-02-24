#version 460

in vec2 TexCoords;
in flat uint AtlasIndex;

uniform sampler2DArray Atlases;

out vec4 FragColor;

void main()
{
    float U = TexCoords.x;
    float V = TexCoords.y;
    FragColor = texture(Atlases, vec3(U,V,AtlasIndex));
}