#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

uniform mat4 mvp;

out vec3 fragmentColor;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    
    fragmentColor = vertexColor;
}