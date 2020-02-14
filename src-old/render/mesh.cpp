#include <map>

#include "mesh.h"

namespace Voxarc {

bool PackedVertex::operator<(const PackedVertex pv) const
{
    return memcmp((void*)this, (void*)&pv, sizeof(PackedVertex)) > 0;
}

Mesh::Mesh() { }

void Mesh::create(std::vector<vec3f> &inVertices,
                  std::vector<vec3f> &inNormals,
                  std::vector<vec3f> &inColors,
                  std::vector<unsigned short> &inFaces)
{
    std::vector<vec3f> tempVertices;
    std::vector<vec3f> tempNormals;
    std::vector<vec3f> tempColors;
    
    for(int i = 0; i < inFaces.size(); i += 3)
    {
        tempVertices.push_back(inVertices[inFaces[i+0]]);
        tempNormals.push_back(inNormals[inFaces[i+1]]);
        tempColors.push_back(inColors[inFaces[i+2]]);
    }
    
    std::map<PackedVertex, unsigned short> vertexToIndex;
    
    for(unsigned int i = 0; i < tempVertices.size(); i++)
    {
        PackedVertex vertex = {tempVertices[i], tempNormals[i], tempColors[i]};
        
        unsigned short result;
        bool found;
        std::map<PackedVertex, unsigned short>::iterator iter = vertexToIndex.find(vertex);
        if(iter == vertexToIndex.end())
            found = false;
        else
        {
            result = iter->second;
            found = true;
        }
        
        if(found)
            indices.push_back(result);
        else
        {
            vertices.push_back(tempVertices[i]);
            normals.push_back(tempNormals[i]);
            colors.push_back(tempColors[i]);
            unsigned short newIndex = static_cast<unsigned short>(vertices.size() - 1);
            indices.push_back(newIndex);
            vertexToIndex[vertex] = newIndex;
        }
    }
    
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3f), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3f), &normals[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3f), &colors[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
    
    modelMatrix = mat4f(1.0f);
}

void Mesh::draw(GLuint mvpMatrixID, mat4f viewMatrix, mat4f projectionMatrix)
{
    mat4f mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0]);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, nullptr);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &normalBufferID);
    glDeleteBuffers(1, &colorBufferID);
    glDeleteBuffers(1, &elementBufferID);
}

}