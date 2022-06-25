#version 460 core

layout(location = 0) in vec4 PositionIn;
layout(location = 3) in vec4 ColorIn;

layout(std430, binding = 1) readonly buffer MatrixStorage {
    mat4 Matrices[];
};

uniform mat4 VPMatrix;

out vec4 VertColor;

void main()
{
    mat4 MVPMatrix = Matrices[gl_DrawID]*VPMatrix;
    
    gl_Position = PositionIn*MVPMatrix;
    
    VertColor = ColorIn;
}