#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoords;

out vec2 FragmentTexCoords;

void main(void)
{
    gl_Position = vec4(VertexPosition, 1.0);
    FragmentTexCoords = VertexTexCoords;
}