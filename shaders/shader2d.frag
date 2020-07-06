#version 330 core

in vec2 FragmentTexCoords;

uniform sampler2D Sampler;

out vec4 Color;

void main(void)
{
    Color = texture(Sampler, FragmentTexCoords);
}