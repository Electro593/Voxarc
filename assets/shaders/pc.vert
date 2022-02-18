#version 460 core

layout(location = 0) in vec4 PositionIn;
layout(location = 3) in vec4 ColorIn;

out vec4 VertColor;

void main()
{
    gl_Position = PositionIn;
    VertColor = ColorIn;
}