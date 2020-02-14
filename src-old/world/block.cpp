#include <vector>

#include <gl/glew.h>

#include "block.h"

namespace Voxarc {

void Block::create(vec3f sizeIn)
{
    size = sizeIn;
    
    std::vector<vec3f> vertices = {
        vec3f(-size[0], -size[1], -size[2]),
        vec3f(-size[0], -size[1],  size[2]),
        vec3f(-size[0],  size[1], -size[2]),
        vec3f(-size[0],  size[1],  size[2]),
        vec3f( size[0], -size[1], -size[2]),
        vec3f( size[0], -size[1],  size[2]),
        vec3f( size[0],  size[1], -size[2]),
        vec3f( size[0],  size[1],  size[2])
    };
    
    std::vector<vec3f> normals = {
        vec3f(-1.0f,  0.0f,  0.0f),
        vec3f( 0.0f, -1.0f,  0.0f),
        vec3f( 0.0f,  0.0f, -1.0f),
        vec3f( 1.0f,  0.0f,  0.0f),
        vec3f( 0.0f,  1.0f,  0.0f),
        vec3f( 0.0f,  0.0f,  1.0f)
    };
    
    std::vector<vec3f> colors = {
        vec3f(1.0f, 0.0f, 0.0f),
        vec3f(0.0f, 1.0f, 0.0f),
        vec3f(0.0f, 0.0f, 1.0f)
    };
    
    std::vector<unsigned short> faces = {
        0, 0, 1,  1, 0, 0,  3, 0, 1, // LEFT
        0, 0, 1,  3, 0, 1,  2, 0, 2,
        
        1, 5, 0,  5, 5, 1,  7, 5, 2, // FRONT
        1, 5, 0,  7, 5, 2,  3, 5, 1,
        
        5, 3, 1,  4, 3, 0,  6, 3, 1, // RIGHT
        5, 3, 1,  6, 3, 1,  7, 3, 2,
        
        4, 2, 0,  0, 2, 1,  2, 2, 2, // BACK
        4, 2, 0,  2, 2, 2,  6, 2, 1,
        
        3, 4, 1,  7, 4, 2,  6, 4, 1, // TOP
        3, 4, 1,  6, 4, 1,  2, 4, 2,
        
        0, 1, 1,  4, 1, 0,  5, 1, 1, // BOTTOM
        0, 1, 1,  5, 1, 1,  1, 1, 0
    };
    
    mesh.create(vertices, normals, colors, faces);
}

Mesh *Block::getMesh() { return &mesh; }

}