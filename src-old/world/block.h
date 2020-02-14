#ifndef VOX_BLOCK_H_
#define VOX_BLOCK_H_

#include "../math/vectors.h"
#include "../render/mesh.h"

namespace Voxarc {

class Block
{
private:
    vec3f size;
    Mesh mesh;
    
public:
    void create(vec3f size);
    Mesh *getMesh();
};

}

#endif