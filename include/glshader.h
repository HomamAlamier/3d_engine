#ifndef GLSHADER_H
#define GLSHADER_H
#include <macros.h>
#include <logger.h>

NAMESPACE(opengl)

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

NAMESPACE_END

#endif // GLSHADER_H
