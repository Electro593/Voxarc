#version 330 core

in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec4 FragmentColor;

uniform uint LightPower;
uniform vec3 LightColor;
uniform vec3 LightPosition_worldspace;
uniform uint ChannelMask;
uniform uint ChannelRange;
uniform uvec4 ChannelShift;
uniform uvec4 ChannelRangeShift;

out vec4 Color;

const float ScreenGamma = 2.2;

void main(void)
{
    uint TempColor = (uint((ChannelRange * FragmentColor.r) + 0.5F) << ChannelShift.r) |
                     (uint((ChannelRange * FragmentColor.g) + 0.5F) << ChannelShift.g) |
                     (uint((ChannelRange * FragmentColor.b) + 0.5F) << ChannelShift.b) |
                     (uint((ChannelRange * FragmentColor.a) + 0.5F) << ChannelShift.a);
    TempColor &= ChannelMask;
    Color = vec4(float((TempColor & ChannelRangeShift.r) >> ChannelShift.r) / float(ChannelRange),
                 float((TempColor & ChannelRangeShift.g) >> ChannelShift.g) / float(ChannelRange),
                 float((TempColor & ChannelRangeShift.b) >> ChannelShift.b) / float(ChannelRange),
                 float((TempColor & ChannelRangeShift.a) >> ChannelShift.a) / float(ChannelRange));
}

/*
void main(void)
{
    vec3 materialDiffuseColor = FragmentColor.rgb;
    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3,0.3,0.3);
    
    float distance = length(lightPosition_worldspace - position_worldspace);
    
    vec3 n = normalize(normal_cameraspace);
    vec3 l = normalize(lightDirection_cameraspace);
    
    vec3 E = normalize(eyeDirection_cameraspace);
    vec3 R = reflect(-l, n);
    
    float cosTheta = clamp(dot(n, l), 0, 1);
    float cosAlpha = clamp(dot(E, R), 0, 1);
    
    color.rgb =
        materialAmbientColor +
        materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
        materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance*distance);
    color.a = FragmentColor.a;
    color = pow(color, vec4(1.0 / screenGamma));
}
*/