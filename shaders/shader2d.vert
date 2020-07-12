#version 330 core

layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec2 VertexTexture;

uniform mat4 ViewportMatrix;

out vec2 TexCoords;

void main(void)
{
    gl_Position = ViewportMatrix * vec4(VertexPosition, 1.0, 1.0);
    TexCoords = VertexTexture;
}