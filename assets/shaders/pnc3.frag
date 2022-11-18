#version 460 core

struct material {
   vec3 AmbientColor;
   vec3 DiffuseColor;
   vec3 SpecularColor;
   float Shininess;
};

in vec3 Normal;
in material Material;
in vec3 FragPos;

uniform vec3 CameraPos;
uniform vec3 LightPos;

out vec4 FragColor;

void main()
{
    float LightStrength    = 1;
    float AmbientStrength  = 0.1;
    float DiffuseStrength  = 1;
    float SpecularStrength = 0.5;
    
    vec3 LightColor   = LightStrength*vec3(1, 1, 1);
    vec3 LightDir     = normalize(LightPos - FragPos);
    vec3 ViewDir      = normalize(CameraPos - FragPos);
    vec3 ReflectedDir = reflect(-LightDir, Normal);
    
    float DiffuseFactor = max(dot(Normal, LightDir), 0.0);
    float SpecularFactor = pow(max(dot(ViewDir, ReflectedDir), 0.0), Material.Shininess);
    
    vec3 Ambient  = AmbientStrength  * Material.AmbientColor;
    vec3 Diffuse  = DiffuseStrength  * Material.DiffuseColor  * DiffuseFactor;
    vec3 Specular = SpecularStrength * Material.SpecularColor * SpecularFactor;
    
    FragColor = vec4(Ambient + Diffuse + Specular, 1);
}