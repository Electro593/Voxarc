#version 450

in vec2 TexCoords

uniform sampler2D Sampler;

out vec4 FragColor

void main()
{
    FragColor = texture(Sampler, TexCoords);
}