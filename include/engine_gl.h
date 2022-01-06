#ifndef ENGINE_GL_H
#define ENGINE_GL_H
#include <macros.h>
#include <logger.h>
NAMESPACE(sage3d)

class Engine;

class GLShader
{
    LOGGER(Sage3D, GLShader)
    READ_PROP_REF(vertexShaderFilename, std::string)
    READ_PROP_REF(fragmentShaderFilename, std::string)
    READ_PROP(programId, unsigned)
    READ_PROP(loaded, bool)
    READ_PROP(compiled, bool)
public:
    GLShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
    void use();
private:
    void compile();
};


class Engine_GL
{
public:
    Engine_GL(Engine*);
    void clearColorBuffer();
private:
    Engine* _engine;
};

NAMESPACE_END

#endif // ENGINE_GL_H
