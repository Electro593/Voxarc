#version 330 core

in vec4 FragTexture;

uniform sampler2DArray Sampler;

out vec4 Color;

void main(void)
{
    Color = texture(Sampler, FragTexture.xyz);
}