#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 Normal_cameraspace;

void main(void)
{
    vec3 A = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 B = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 VertexNormal = cross(A, B);
    Normal_cameraspace = (ViewMatrix * ModelMatrix * vec4(VertexNormal, 0)).xyz;
    
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    
    EndPrimitive();
}