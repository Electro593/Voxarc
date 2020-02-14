#include <iostream>

#include "math/commonmath.h"

#include "voxarc.h"

namespace Voxarc {

Voxarc::Voxarc(int windowWidth, int windowHeight, char *windowTitle,
               char *loggerPath,
               char *vertexShaderPath, char *fragmentShaderPath)
    : windowWidth(windowWidth), windowHeight(windowHeight), windowTitle(windowTitle),
      loggerPath(loggerPath), logger(new Logger(loggerPath)),
      vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath),
      shaderHandler(new ShaderHandler(logger)),
      camera(vec3f(0, 0, 3), vec3f(0, 1, 0), -90.0f, 0.0f, 3.0f, 0.05f) { }

Voxarc::~Voxarc() { }

void Voxarc::run()
{
    logger->restart();
    
    logger->logInfo("Initializing game\n");
    init();
    
    logger->logInfo("Beginning game\n");
    loop();
    
    logger->logInfo("Closing game\n");
    finish();
}

void Voxarc::init()
{
    logger->logInfo("Creating window\n");
    initWindow();
    
    logger->logInfo("Creating shader program\n");
    initShaders();
    
    block.create(vec3f(1, 1, 1));
}

void Voxarc::loop()
{
    double lastTime = glfwGetTime();
    double fpsCounter = lastTime;
    int frames = 0;
    
    while(!glfwWindowShouldClose(window))
    {
        double currTime = glfwGetTime();
        deltaTime = currTime - lastTime;
        lastTime = currTime;
        if(currTime - fpsCounter >= 1.0)
        {
            printf("%.0f ms/f\n", 1000.0f / (double)frames);
            frames = 0;
            fpsCounter += 1.0;
        }
        frames++;
        
        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            shaderHandler->reloadShaders(&programID, vertexShaderPath, fragmentShaderPath);
        
        double currMouseX, currMouseY;
        glfwGetCursorPos(window, &currMouseX, &currMouseY);
        player.move(deltaTime, currMouseX - mouseX, currMouseY - mouseY,
                    glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS,
                    glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS,
                    glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS,
                    glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS,
                    glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS,
                    glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
        mouseX = currMouseX;
        mouseY = currMouseY;
        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        
        block.getMesh()->draw(mvpMatrixID, player.getViewMatrix(), projectionMatrix);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Voxarc::finish()
{
    delete logger;
    delete shaderHandler;
    
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(programID);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Voxarc::initWindow()
{
    if(!glfwInit())
    {
        logger->logError("Failed to init GLFW\n");
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if(!window)
    {
        logger->logError("Failed to create window\n");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        logger->logError("Failed to init GLEW\n");
        exit(EXIT_FAILURE);
    }
    
    glfwSetWindowUserPointer(window, this);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    
    glfwSetErrorCallback(errorCallback);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
}

void Voxarc::initShaders()
{
    shaderHandler = new ShaderHandler(logger);
    programID = shaderHandler->loadShaders(vertexShaderPath, fragmentShaderPath);
    if(!shaderHandler->getSuccess())
    {
        logger->logError("Failed to load shaders from files %s and %s\n", vertexShaderPath, fragmentShaderPath);
        exit(EXIT_FAILURE);
    }
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    mvpMatrixID = glGetUniformLocation(programID, "mvp");
    
    projectionMatrix = Matrices::perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    
    glUseProgram(programID);
}

void Voxarc::setWindowWidth(int width) { windowWidth = width; }
void Voxarc::setWindowHeight(int width) { windowWidth = width; }


double Voxarc::getDeltaTime() { return deltaTime; }
double Voxarc::getMouseX() { return mouseX; }
double Voxarc::getMouseY() { return mouseY; }
Camera *Voxarc::getCamera() { return &camera; }

void mouseCallback(GLFWwindow *window, double mouseX, double mouseY)
{
    Voxarc *game = (Voxarc*)glfwGetWindowUserPointer(window);
    
    double currMouseX;
    double currMouseY;
    glfwGetCursorPos(window, &currMouseX, &currMouseY);
    double dMouseX = currMouseX - game->getMouseX();
    double dMouseY = currMouseY - game->getMouseY();
    game->setMouseX(currMouseX);
    game->setMouseY(currMouseY);
    
    game->getCamera()->look(game->getDeltaTime(), dMouseX, dMouseY);
    
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    
}

void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    Voxarc *game = (Voxarc*)glfwGetWindowUserPointer(window);
    
    game->setWindowWidth(width);
    game->setWindowHeight(height);
    
    
}

void errorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW ERROR: code %i msg: %s\n", error, description);
}

}

int main()
{
    Voxarc::Voxarc game = Voxarc::Voxarc(800, 600, "Voxarc",
                                         "default.log",
                                         "src/shaders/shader.vert", "src/shaders/shader.frag");
    game.run();
    
    return 0;
}