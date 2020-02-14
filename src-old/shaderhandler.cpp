#include <fstream>
#include <vector>

#include <GL/glew.h>

#include "util/logging.h"

#include "shaderhandler.h"

namespace Voxarc {

ShaderHandler::ShaderHandler(Logger *logger)
    : logger(logger) { }

std::string ShaderHandler::readFile(char *filePath)
{
    std::ifstream file(filePath, std::ios::in|std::ios::binary);
    if(!file)
    {
        logger->logError("Failed to open file %s\n", filePath);
        exit(EXIT_FAILURE);
    }
    
    std::string contents;
    
    file.seekg(0, std::ios::end);
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    
    file.read(&contents[0], contents.size());
    file.close();
    
    return contents;
}

GLuint ShaderHandler::loadShaders(char *vertexShaderPath, char *fragmentShaderPath)
{
    succeeded = false;
    int infoLogLength;
    
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertexShaderCode = readFile(vertexShaderPath).c_str();
    glShaderSource(vertexShaderID, 1, &vertexShaderCode, 0);
    glCompileShader(vertexShaderID);
    
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &succeeded);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(!succeeded)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, 0, &vertexShaderErrorMessage[0]);
        logger->logError("%s\n%s\n", vertexShaderPath, &vertexShaderErrorMessage[0]);
        return 0;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragmentShaderCode;
    fragmentShaderCode = readFile(fragmentShaderPath).c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, 0);
    glCompileShader(fragmentShaderID);
    
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &succeeded);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(!succeeded)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, 0, &fragmentShaderErrorMessage[0]);
        logger->logError("%s\n%s\n", fragmentShaderPath, &fragmentShaderErrorMessage[0]);
        return 0;
    }
    
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    
    glGetProgramiv(programID, GL_LINK_STATUS, &succeeded);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!succeeded)
    {
        std::vector<char> programErrorMessage(infoLogLength);
        glGetProgramInfoLog(programID, infoLogLength, 0, &programErrorMessage[0]);
        logger->logError("%s\n", &programErrorMessage[0]);
        return 0;
    }
    
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    return programID;
}

void ShaderHandler::reloadShaders(GLuint *programID, char *vertexShaderPath, char *fragmentShaderPath)
{
    GLuint reloadedProgram = loadShaders(vertexShaderPath, fragmentShaderPath);
    
    if(succeeded)
    {
        logger->logInfo("Reloaded shader files %s and %s\n", vertexShaderPath, fragmentShaderPath);
        
        glDeleteProgram(*programID);
        *programID = reloadedProgram;
    }
}

GLint ShaderHandler::getSuccess() { return succeeded; }

}