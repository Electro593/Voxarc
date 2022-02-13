#version 450

layout(location = 0) in vec3 PositionIn;
layout(location = 2) in vec4 ColorIn;

out vec4 VertColor;

void main()
{
    gl_Position = vec4(PositionIn, 1);
    VertColor = ColorIn;
}