#ifndef ENGINE_GL_H
#define ENGINE_GL_H
#include <macros.h>
#include <logger.h>


NAMESPACE(opengl)
class GLVertexBuffer;
NAMESPACE_END

NAMESPACE(sage3d)

class Engine;
class Engine_GL
{
    LOGGER(Sage3D, Engine_GL)
public:
    Engine_GL(Engine*);
    void clearColorBuffer();
    void drawBuffer(opengl::GLVertexBuffer*);
private:
    Engine* _engine;
};

NAMESPACE_END

#endif // ENGINE_GL_H
