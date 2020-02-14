#ifndef VOX_SHADERHANDLER_H_
#define VOX_SHADERHANDLER_H_

namespace Voxarc {

class ShaderHandler
{
private:
    Logger *logger;
    
    GLint succeeded;
public:
    ShaderHandler(Logger *logger);
    
    std::string readFile(char *filePath);
    GLuint loadShaders(char *vertexShaderPath, char *fragmentShaderPath);
    void reloadShaders(GLuint *programID, char *vertexShaderPath, char *fragmentShaderPath);
    
    GLint getSuccess();
};

}

#endif