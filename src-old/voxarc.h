#ifndef VOX_VOXARC_H_
#define VOX_VOXARC_H_

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include "util/logging.h"
#include "math/matrices.h"
#include "world/block.h"
#include "shaderhandler.h"
#include "camera.h"

namespace Voxarc {

class Voxarc
{
private:
    GLFWwindow *window;
    int windowWidth;
    int windowHeight;
    char *windowTitle;
    
    char *loggerPath;
    Logger *logger;
    
    char *vertexShaderPath;
    char *fragmentShaderPath;
    
    ShaderHandler *shaderHandler;
    GLuint programID;
    GLuint vertexArrayID;
    GLuint mvpMatrixID;
    
    mat4f projectionMatrix;
    
    Camera camera;
    Block block;
    
    double deltaTime;
    double mouseX;
    double mouseY;
    
public:
    Voxarc(int windowWidth, int windowHeight, char *windowTitle,
           char *loggerPath,
           char *vertexShaderPath, char *fragmentShaderPath);
    ~Voxarc();
    
    void run();
    
    void init();
    void loop();
    void finish();
    
    void initWindow();
    void initShaders();
    
    void setWindowWidth(int width);
    void setWindowHeight(int height);
    void setMouseX(double mouseX);
    void setMouseY(double mouseY);
    
    double getDeltaTime();
    double getMouseX();
    double getMouseY();
    Camera *getCamera();
};

void mouseCallback(GLFWwindow *window, double mouseX, double mouseY);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void framebufferResizeCallback(GLFWwindow *window, int width, int height);
void errorCallback(int error, const char *description);

}

#endif