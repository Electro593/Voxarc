#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform mat4 mvpMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition_worldspace;

void main(){

	gl_Position =  mvpMatrix * vec4(vertexPosition_modelspace,1);
	
	Position_worldspace = (modelMatrix * vec4(vertexPosition_modelspace,1)).xyz;
	
	vec3 vertexPosition_cameraspace = (viewMatrix * modelMatrix * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = (viewMatrix * modelMatrix * vec4(lightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	Normal_cameraspace = (viewMatrix * modelMatrix * vec4(vertexNormal_modelspace,0)).xyz;
	
	UV = vertexUV;
}