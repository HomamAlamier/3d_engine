#include "engine_gl.h"
#include <GL/glew.h>
#include <sstream>
#include <istream>
#include <fstream>
#include <glvertexbuffer.h>

NAMESPACE(sage3d)

Engine_GL::Engine_GL(Engine* engine)
    : _engine(engine)
{

}

void Engine_GL::clearColorBuffer()
{
    glClear( GL_COLOR_BUFFER_BIT );
}

void Engine_GL::drawBuffer(opengl::GLVertexBuffer* vb)
{
    vb->bind();
    GLenum e;
    switch (vb->drawType())
    {
    case opengl::Lines: e = GL_LINES; break;
    case opengl::Triangles: e = GL_TRIANGLES; break;
    case opengl::Quads: e = GL_QUADS; break;
    }
    glDrawArrays(e, 0, vb->vertexCount());
}

NAMESPACE_END
