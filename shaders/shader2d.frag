#version 330 core

in vec2 TexCoords;

uniform sampler2D Texture0;

out vec4 Color;

void main(void)
{
    //Color = vec4(0.5, 0.5, 0.5, 1.0);
    Color = texture(Texture0, TexCoords);
}