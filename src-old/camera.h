#ifndef VOX_CAMERA_H_
#define VOX_CAMERA_H_

#include "math/matrices.h"
#include "math/vectors.h"

namespace Voxarc {

class Camera
{
private:
    mat4f viewMatrix;
    
    vec3f position;
    vec3f worldUp;
    
    vec3f front;
    vec3f right;
    vec3f up;
    
    float movementSpeed;
    float mouseSpeed;
    
    float yaw;
    float pitch;
    
public:
    Camera(vec3f position, vec3f worldUp,
           float yaw, float pitch, float movementSpeed, float mouseSpeed);
    
    void look(float dTime, float dMouseX, float dMouseY);
    void move(double dTime,
              bool bForward, bool bBackward, bool bLeft, bool bRight, bool bUp, bool bDown);
    void update();
    
    mat4f getViewMatrix();
};

}

#endif