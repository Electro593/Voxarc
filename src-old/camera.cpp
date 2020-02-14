#include <cmath>

#include <math/commonmath.h>

#include "camera.h"

namespace Voxarc {

Camera::Camera(vec3f position, vec3f up,
               float yaw, float pitch, float movementSpeed, float mouseSpeed)
    : position(position), worldUp(worldUp),
      yaw(yaw), pitch(pitch), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed)
{
    update();
}

void Camera::look(float dTime, float dMouseX, float dMouseY)
{
    yaw += dMouseX * dTime * mouseSpeed;
    pitch += dMouseY * dTime * mouseSpeed;
    
    update();
}

void Camera::move(double dTime,
                  bool bForward, bool bBackward, bool bLeft, bool bRight, bool bUp, bool bDown)
{
    if(bForward)
        position += front * (float)dTime * (float)movementSpeed;
    if(bBackward)
        position -= front * (float)dTime * (float)movementSpeed;
    if(bRight)
        position += right * (float)dTime * (float)movementSpeed;
    if(bLeft)
        position -= right * (float)dTime * (float)movementSpeed;
    if(bUp)
        position += up * (float)dTime * (float)movementSpeed;
    if(bDown)
        position -= up * (float)dTime * (float)movementSpeed;
    
    update();
}

void Camera::update()
{
    front = Vectors::normalize(vec3f(
        cosf(radians(yaw)) * cosf(radians(pitch)),
        sinf(radians(pitch)),
        sinf(radians(yaw)) * cosf(radians(pitch))
    ));
    right = Vectors::normalize(Vectors::cross(front, worldUp));
    up = Vectors::normalize(Vectors::cross(right, front));
}

}