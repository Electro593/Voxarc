#version 460 core

layout(location = 0) in vec4 PositionIn;
layout(location = 1) in vec4 NormalIn;
layout(location = 3) in uint MaterialIndex;

struct material {
   vec3 AmbientColor;
   vec3 DiffuseColor;
   vec3 SpecularColor;
   float Shininess;
};

layout(std430, binding = 1) readonly buffer MatrixStorage {
   mat4 Matrices[];
};
layout(std430, binding = 2) readonly buffer MaterialStorage {
   uint Materials[];
};

uniform mat4 VPMatrix;

out vec3 Normal;
out material Material;
out vec3 FragPos;

void main()
{
   mat4 MVPMatrix = Matrices[gl_DrawID]*VPMatrix;
   gl_Position = PositionIn*MVPMatrix;
   FragPos = (PositionIn*Matrices[gl_DrawID]).xyz;
   
   uint Mat0 = Materials[3*MaterialIndex+0];
   uint Mat1 = Materials[3*MaterialIndex+1];
   uint Mat2 = Materials[3*MaterialIndex+2];
   
   Material = material(
      vec3(float((Mat0 >>  0) & 0xFF) / 255.0, float((Mat0 >>  8) & 0xFF) / 255.0, float((Mat0 >> 16) & 0xFF) / 255.0),
      vec3(float((Mat0 >> 24) & 0xFF) / 255.0, float((Mat1 >>  0) & 0xFF) / 255.0, float((Mat1 >>  8) & 0xFF) / 255.0),
      vec3(float((Mat1 >> 16) & 0xFF) / 255.0, float((Mat1 >> 24) & 0xFF) / 255.0, float((Mat2 >>  0) & 0xFF) / 255.0),
      float(1 << ((Mat2 >>  8) & 0xFF))
  );
   
   Normal = NormalIn.xyz;
}