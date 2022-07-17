#version 460 core

layout(location = 0) in vec4 PositionIn;

layout(std430, binding = 1) readonly buffer MatrixStorage {
   mat4 Matrices[];
};

uniform mat4 VPMatrix;

void main()
{
   mat4 MVPMatrix = Matrices[gl_DrawID]*VPMatrix;
   gl_Position = PositionIn*MVPMatrix;
}