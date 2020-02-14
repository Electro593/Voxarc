#ifndef VOX_VECTORS_H_
#define VOX_VECTORS_H_

namespace Voxarc {

struct vec3f
{
    vec3f();
    vec3f(float x, float y, float z);
    
    float e[3];
    vec3f operator+(vec3f v);
    vec3f operator-(vec3f v);
    vec3f operator*(float s);
    float &operator[](int i);
    vec3f &operator+=(vec3f v);
    vec3f &operator-=(vec3f v);
};

struct vec4f
{
    vec4f();
    vec4f(float x, float y, float z, float w);
    
    float e[4];
    float &operator[] (int i);
};

namespace Vectors {

float dot(vec3f a, vec3f b);
vec3f cross(vec3f a, vec3f b);
float length(vec3f v);
vec3f normalize(vec3f v);

}

}

#endif