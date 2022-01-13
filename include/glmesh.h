#ifndef GLMESH_H
#define GLMESH_H
#include <macros.h>
#include <logger.h>

NAMESPACE(sage3d)
class Engine_GL;
NAMESPACE_END

NAMESPACE(opengl)


class GLVertexBuffer;
class GLTexture;
class GLMesh
{
public:
    GLMesh();
    void addBuffer(GLVertexBuffer*);
    void addTexture(GLTexture*);
    void draw(sage3d::Engine_GL*);
private:
    struct MeshElement
    {
        GLVertexBuffer* buffer;
        std::vector<GLTexture*> textures;
    };
    std::vector<MeshElement> _elements;
};

NAMESPACE_END

#endif // GLMESH_H
