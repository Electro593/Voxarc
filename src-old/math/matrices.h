#ifndef VOX_MATRICES_H_
#define VOX_MATRICES_H_

#include "vectors.h"

namespace Voxarc {

struct mat4f
{
    mat4f();
    mat4f(float i);
    
    float e[16];
    float &operator[](int i);
    mat4f operator*(mat4f m);
};

namespace Matrices {

mat4f lookAt(vec3f position, vec3f target, vec3f worldUp);
mat4f perspective(float fov, float aspectRatio, float zNear, float zFar);

}

}

#endif