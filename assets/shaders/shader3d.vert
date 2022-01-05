#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;

uniform mat4 MVPMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 LightPosition_worldspace;

out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 FragmentColor;

void main(void)
{
    vec3 Position_cameraspace = (ViewMatrix * ModelMatrix * vec4(VertexPosition, 1)).xyz;
    gl_Position = MVPMatrix * vec4(VertexPosition, 1);
    
    Position_worldspace = (ModelMatrix * vec4(VertexPosition, 1)).xyz;
    
    EyeDirection_cameraspace = -Position_cameraspace;
    
    LightDirection_cameraspace = (ViewMatrix * vec4(LightPosition_worldspace, 1)).xyz;
    LightDirection_cameraspace += EyeDirection_cameraspace;
    
    FragmentColor = VertexColor;
}