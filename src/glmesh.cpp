#include "glmesh.h"
#include <gltexture.h>
#include <engine_gl.h>

NAMESPACE(opengl)

GLMesh::GLMesh()
{

}

void GLMesh::addBuffer(GLVertexBuffer* buffer)
{
    MeshElement e;
    e.buffer = buffer;
    _elements.push_back(e);
}

void GLMesh::addTexture(GLTexture* texture)
{
    if (_elements.size() == 0)
        return;
    _elements[_elements.size() - 1].textures.push_back(texture);
}

void GLMesh::draw(sage3d::Engine_GL* gl)
{
    for(const MeshElement& e : _elements)
    {
        for(int i = 0; i < e.textures.size(); ++i)
        {
            e.textures[i]->bindForSampler(i);
        }
        gl->drawBuffer(e.buffer);
    }
}

NAMESPACE_END
