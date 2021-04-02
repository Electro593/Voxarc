#version 330 core

layout(location = 0) in vec4 VertPosition;
layout(location = 3) in vec4 VertColor;

uniform mat4 MPMatrix;

out vec4 FragColor;

void main(void)
{
    gl_Position = MPMatrix * vec4(VertPosition.xyz, 1.0);
    
    FragColor = VertColor;
}