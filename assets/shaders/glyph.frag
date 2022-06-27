#version 460 core

in vec2 TextureCoords;
in flat uint AtlasIndex;

// uniform vec3 BackgroundColor;
// uniform vec3 ForegroundColor;
uniform sampler2DArray Atlases;

out vec4 FragColor;

float Median(float A, float B, float C)
{
   return max(min(A, B), min(max(A, B), C));
}

void main()
{
   vec3 ForegroundColor = vec3(0,0,0);
   vec3 BackgroundColor = vec3(.2,.2,.2);
   
   vec3 P = vec3(TextureCoords, AtlasIndex);
   vec4 S = texture(Atlases, P);
   float D = Median(S.r, S.g, S.b) - 0.5;
   float W = clamp(D/fwidth(D) + 0.5, 0, 1);
   FragColor = mix(vec4(BackgroundColor,0), vec4(ForegroundColor,1), W);
}