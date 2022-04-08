#version 460

in vec4 Color;
in vec2 TexCoords;
in flat uint AtlasIndex;

uniform sampler2DArray Atlases;

out vec4 FragColor;

float Median(float A, float B, float C)
{
    return max(min(A, B), min(max(A, B), C));
}

void main()
{
    if(Color.w == 0) {
        vec3 P = vec3(TexCoords.xy, AtlasIndex);
        vec4 S = texture(Atlases, P);
        float D = Median(S.r, S.g, S.b) - 0.5;
        #if 1
        float W = clamp(D/fwidth(D) + 0.5, 0, 1);
        FragColor = mix(vec4(0,0,0,0), vec4(1,1,1,1), W);
        #else
        float R = clamp((S.r-0.5)/fwidth(S.r-0.5) + 0.5, 0, 1);
        float G = clamp((S.g-0.5)/fwidth(S.g-0.5) + 0.5, 0, 1);
        float B = clamp((S.b-0.5)/fwidth(S.b-0.5) + 0.5, 0, 1);
        FragColor = vec4(R, G, B, 1);
        #endif
    } else {
        FragColor = Color;
    }
}