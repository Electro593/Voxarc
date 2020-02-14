#include <cmath>

#include "matrices.h"

namespace Voxarc {

mat4f::mat4f()
{
    mat4f(0.0f);
}

mat4f::mat4f(float i)
{
    e[ 0] = i;
    e[ 1] = 0;
    e[ 2] = 0;
    e[ 3] = 0;
    e[ 4] = 0;
    e[ 5] = i;
    e[ 6] = 0;
    e[ 7] = 0;
    e[ 8] = 0;
    e[ 9] = 0;
    e[10] = i;
    e[11] = 0;
    e[12] = 0;
    e[13] = 0;
    e[14] = 0;
    e[15] = i;
}

float &mat4f::operator[](int i)
{
    return e[i];
}

mat4f mat4f::operator*(mat4f m)
{
    mat4f result;
    result[ 0] = e[ 0]*m[ 0] + e[ 4]*m[ 1] + e[ 8]*m[ 2] + e[12]*m[ 3];
    result[ 1] = e[ 1]*m[ 0] + e[ 5]*m[ 1] + e[ 9]*m[ 2] + e[13]*m[ 3];
    result[ 2] = e[ 2]*m[ 0] + e[ 6]*m[ 1] + e[10]*m[ 2] + e[14]*m[ 3];
    result[ 3] = e[ 3]*m[ 0] + e[ 7]*m[ 1] + e[11]*m[ 2] + e[15]*m[ 3];
    result[ 4] = e[ 0]*m[ 4] + e[ 4]*m[ 5] + e[ 8]*m[ 6] + e[12]*m[ 7];
    result[ 5] = e[ 1]*m[ 4] + e[ 5]*m[ 5] + e[ 9]*m[ 6] + e[13]*m[ 7];
    result[ 6] = e[ 2]*m[ 4] + e[ 6]*m[ 5] + e[10]*m[ 6] + e[14]*m[ 7];
    result[ 7] = e[ 3]*m[ 4] + e[ 7]*m[ 5] + e[11]*m[ 6] + e[15]*m[ 7];
    result[ 8] = e[ 0]*m[ 8] + e[ 4]*m[ 9] + e[ 8]*m[10] + e[12]*m[11];
    result[ 9] = e[ 1]*m[ 8] + e[ 5]*m[ 9] + e[ 9]*m[10] + e[13]*m[11];
    result[10] = e[ 2]*m[ 8] + e[ 6]*m[ 9] + e[10]*m[10] + e[14]*m[11];
    result[11] = e[ 3]*m[ 8] + e[ 7]*m[ 9] + e[11]*m[10] + e[15]*m[11];
    result[12] = e[ 0]*m[12] + e[ 4]*m[13] + e[ 8]*m[14] + e[12]*m[15];
    result[13] = e[ 1]*m[12] + e[ 5]*m[13] + e[ 9]*m[14] + e[13]*m[15];
    result[14] = e[ 2]*m[12] + e[ 6]*m[13] + e[10]*m[14] + e[14]*m[15];
    result[15] = e[ 3]*m[12] + e[ 7]*m[13] + e[11]*m[14] + e[15]*m[15];
    return result;
}

namespace Matrices {

mat4f lookAt(vec3f position, vec3f target, vec3f worldUp)
{
    vec3f zAxis = Vectors::normalize(position - target);
    vec3f xAxis = Vectors::normalize(Vectors::cross(worldUp, zAxis));
    vec3f yAxis = Vectors::cross(zAxis, xAxis);
    
    mat4f result = mat4f(1.0f);
    result[ 0] = xAxis[0];
    result[ 4] = xAxis[1];
    result[ 8] = xAxis[2];
    result[ 1] = yAxis[0];
    result[ 5] = yAxis[1];
    result[ 9] = yAxis[2];
    result[ 2] = zAxis[0];
    result[ 6] = zAxis[1];
    result[10] = zAxis[2];
    result[12] = -position[0];
    result[13] = -position[1];
    result[14] = -position[2];
    return result;
}

mat4f perspective(float fov, float aspectRatio, float zNear, float zFar)
{
    float t = tanf(fov / 2.0f);
    mat4f result = mat4f(0.0f);
    result[ 0] = 1.0f / (aspectRatio * t);
    result[ 5] = 1.0f / t;
    result[10] = -(zFar + zNear) / (zFar - zNear);
    result[11] = -1;
    result[14] = -(2 * zFar * zNear) / (zFar - zNear);
    return result;
}

}

}