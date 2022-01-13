#ifndef GLSHADER_H
#define GLSHADER_H
#include <macros.h>
#include <logger.h>
#include <map>
#include <glm/mat4x4.hpp>
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
    ~GLShader();
    void use();

    void setFloat(float value, const std::string& name);
    void setMatrix4(const glm::mat4& value, const std::string& name);
    void setVec2(const glm::vec2& value, const std::string& name);
    void setVec3(const glm::vec3& value, const std::string& name);
    void setVec4(const glm::vec4& value, const std::string& name);
    void setInt(int value, const std::string& name);
    void setUInt(uint32_t value, const std::string& name);
    void setIntArray(int* value, uint32_t size, const std::string& name);
    void setFloatArray(float* value, uint32_t size, const std::string& name);
private:
    void compile();
    int getUniformId(const std::string&);
    std::map<std::string, int> _idCache;
};

NAMESPACE_END

#endif // GLSHADER_H
