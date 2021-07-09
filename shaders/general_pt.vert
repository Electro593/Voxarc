#version 460 core

layout(location = 0) in vec4 VertPosition;
layout(location = 2) in vec4 VertTexture;

uniform uint AtlasCount;

layout(std430, binding = 0) readonly buffer Matrices
{
    mat4 MPMatrices[];
};

out vec4 FragTexture;

void main(void)
{
    vec3 Pos = VertPosition.xyz;
    Pos.x = Pos.x * 1.1 + 0.1;
    Pos.y = Pos.y * 1.1 - 0.1;
    gl_Position = MPMatrices[gl_DrawID] * vec4(Pos, 1.0);
    
    FragTexture = VertTexture;
    FragTexture.z *= AtlasCount;
}