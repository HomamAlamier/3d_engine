#ifndef ENGINE_GL_H
#define ENGINE_GL_H
#include <macros.h>
#include <logger.h>


NAMESPACE(opengl)
class GLVertexBuffer;
NAMESPACE_END

NAMESPACE(sage3d)

enum SourceBlendFunc
{
    SourceBlendFunc_Zero = 0,
    SourceBlendFunc_One,
    SourceBlendFunc_SrcAlpha,
    SourceBlendFunc_OneMinusSrcAlpha,
};
enum DestBlendFunc
{
    DestBlendFunc_Zero = 0,
    DestBlendFunc_One,
    DestBlendFunc_SrcColor,
    DestBlendFunc_OneMinusSrcColor,
    DestBlendFunc_SrcAlpha,
    DestBlendFunc_OneMinusSrcAlpha,
    DestBlendFunc_SrcColorPreFog
};
enum DepthTestFunc
{
    DepthFunc_Never,
    DepthFunc_Less,
    DepthFunc_Equal,
    DepthFunc_LessOrEqual,
    DepthFunc_Greater,
    DepthFunc_NotEqual,
    DepthFunc_GreaterOrEqual,
    DepthFunc_Always,
};

enum GLOptions
{
    DepthTest,
    Blend,
    AlphaTest
};

enum GLPolygonMode
{
    PolygoneMode_Fill,
    PolygoneMode_Wireframe,
};

class Engine;
class Engine_GL
{
    LOGGER(Sage3D, Engine_GL)
public:
    Engine_GL(Engine*);
    void init();
    void clearColorBuffer();
    void drawBuffer(opengl::GLVertexBuffer*);
    void setBlendFunc(SourceBlendFunc, DestBlendFunc);
    void setDepthTestFunc(DepthTestFunc);
    void setEnabled(GLOptions, bool);
    void setDepthMask(bool);
    void setPolygonMode(GLPolygonMode);
private:
    Engine* _engine;
};

NAMESPACE_END

#endif // ENGINE_GL_H
