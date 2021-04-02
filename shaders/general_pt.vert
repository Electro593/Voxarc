#version 460 core

layout(location = 0) in vec4 VertPosition;
layout(location = 2) in vec4 VertTexture;

layout(std430, binding = 0) readonly buffer Matrices
{
    mat4 MPMatrices[];
};

out vec4 FragTexture;

void main(void)
{
    gl_Position = MPMatrices[gl_DrawID] * vec4(VertPosition.xyz, 1.0);
    
    FragTexture = VertTexture;
}