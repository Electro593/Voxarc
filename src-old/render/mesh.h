#ifndef VOX_MESH_H_
#define VOX_MESH_H_

#include <vector>

#include <gl/glew.h>

#include "../math/vectors.h"
#include "../math/matrices.h"

namespace Voxarc {

struct PackedVertex
{
    vec3f position;
    vec3f normal;
    vec3f color;
    
    bool operator<(const PackedVertex that) const;
};

class Mesh
{
private:
    GLuint vertexBufferID;
    GLuint normalBufferID;
    GLuint colorBufferID;
    GLuint elementBufferID;
    
    std::vector<vec3f> vertices;
    std::vector<vec3f> normals;
    std::vector<vec3f> colors;
    std::vector<unsigned short> indices;
    
    mat4f modelMatrix;
    
public:
    Mesh();
    ~Mesh();
    
    void create(std::vector<vec3f> &vertices,
                std::vector<vec3f> &normals,
                std::vector<vec3f> &colors,
                std::vector<unsigned short> &inFaces);
    void draw(GLuint mvpMatrixID, mat4f viewMatrix, mat4f projectionMatrix);
};

}

#endif