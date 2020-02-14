#include <cmath>

#include "vectors.h"

namespace Voxarc {

vec3f::vec3f()
{
    e[0] = 0.0f;
    e[1] = 0.0f;
    e[2] = 0.0f;
}

vec3f::vec3f(float x, float y, float z)
{
    e[0] = x;
    e[1] = y;
    e[2] = z;
}

float &vec3f::operator[](int i)
{
    return e[i];
}

vec4f::vec4f()
{
    e[0] = 0.0f;
    e[1] = 0.0f;
    e[2] = 0.0f;
    e[3] = 0.0f;
}

vec4f::vec4f(float x, float y, float z, float w)
{
    e[0] = x;
    e[1] = y;
    e[2] = z;
    e[3] = w;
}

vec3f vec3f::operator+(vec3f v)
{
    return vec3f(e[0] + v[0],
                 e[1] + v[1],
                 e[2] + v[2]);
}

vec3f vec3f::operator-(vec3f v)
{
    return vec3f(e[0] - v[0],
                 e[1] - v[1],
                 e[2] - v[2]);
}

vec3f vec3f::operator*(float s)
{
    return vec3f(e[0] * s,
                 e[1] * s,
                 e[2] * s);
}

float &vec4f::operator[](int i)
{
    return e[i];
}

vec3f &vec3f::operator+=(vec3f v)
{
    *this = *this + v;
    return *this;
}

vec3f &vec3f::operator-=(vec3f v)
{
    *this = *this - v;
    return *this;
}

namespace Vectors {

float dot(vec3f a, vec3f b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

vec3f cross(vec3f a, vec3f b)
{
    return vec3f(a[1]*b[2] - a[2]*b[1],
                 a[2]*b[0] - a[0]*b[2],
                 a[0]*b[1] - a[1]*b[0]);
}

float length(vec3f v)
{
    return sqrtf(dot(v, v));
}

vec3f normalize(vec3f v)
{
    float len = length(v);
    return vec3f(v[0]/len,
                 v[1]/len,
                 v[2]/len);
}

}

}