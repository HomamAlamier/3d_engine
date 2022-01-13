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
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0.f, 0.f, 1.f, 1.f);
}

void Engine_GL::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
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
    if (vb->ebo())
        glDrawElements(e, vb->indicesCount(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(e, 0, vb->vertexCount());

}

void Engine_GL::setBlendFunc(SourceBlendFunc s, DestBlendFunc d)
{
    int t = 0, v = 0;
    switch (s)
    {
        // Source
    case SourceBlendFunc_Zero: t = GL_ZERO; break;
    case SourceBlendFunc_One: t = GL_ONE; break;
    case SourceBlendFunc_SrcAlpha: t = GL_SRC_ALPHA; break;
    case SourceBlendFunc_OneMinusSrcAlpha: t = GL_ONE_MINUS_SRC_ALPHA; break;
    }
    switch (d)
    {
        // Dest
    case DestBlendFunc_Zero: v = GL_ZERO; break;
    case DestBlendFunc_One: v = GL_ONE; break;
    case DestBlendFunc_SrcColor: v = GL_SRC_COLOR; break;
    case DestBlendFunc_OneMinusSrcColor: v = GL_ONE_MINUS_SRC_COLOR; break;
    case DestBlendFunc_SrcAlpha: v = GL_SRC_ALPHA; break;
    case DestBlendFunc_OneMinusSrcAlpha: v = GL_ONE_MINUS_SRC_ALPHA; break;
    case DestBlendFunc_SrcColorPreFog: v = GL_SRC_COLOR; break;
    }
    glBlendFunc(t, v);
}

void Engine_GL::setDepthTestFunc(DepthTestFunc func)
{
    int t = 0;
    switch (func)
    {
    case DepthFunc_Never: t = GL_NEVER; break;
    case DepthFunc_Less: t = GL_LESS; break;
    case DepthFunc_Equal: t = GL_EQUAL; break;
    case DepthFunc_LessOrEqual: t = GL_LEQUAL; break;
    case DepthFunc_Greater: t = GL_GREATER; break;
    case DepthFunc_NotEqual: t = GL_NOTEQUAL; break;
    case DepthFunc_GreaterOrEqual: t = GL_GEQUAL; break;
    case DepthFunc_Always: t = GL_ALWAYS; break;
    }
    glDepthFunc(t);
}

void Engine_GL::setEnabled(GLOptions opt, bool v)
{
    int t = 0;
    switch (opt)
    {
    case DepthTest: t = GL_DEPTH_TEST; break;
    case Blend: t = GL_BLEND; break;
    case AlphaTest: t = GL_ALPHA_TEST; break;
    }
    if (v)
        glEnable(t);
    else
        glDisable(t);
}

void Engine_GL::setDepthMask(bool v)
{
    glDepthMask(v ? GL_TRUE : GL_FALSE);
}

void Engine_GL::setPolygonMode(GLPolygonMode m)
{
    int t = 0;
    switch (m)
    {
    case PolygoneMode_Fill: t = GL_FILL; break;
    case PolygoneMode_Wireframe: t = GL_LINE; break;
    }
    glPolygonMode(GL_FRONT_AND_BACK, t);
}

NAMESPACE_END
