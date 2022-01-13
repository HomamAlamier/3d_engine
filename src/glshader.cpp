#include "glshader.h"
#include <GL/glew.h>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
NAMESPACE(opengl)

GLShader::GLShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
    : _vertexShaderFilename(vertexShaderFilename)
    , _fragmentShaderFilename(fragmentShaderFilename)
    , _programId(0)
    , _loaded(false)
    , _compiled(false)
{
}

GLShader::~GLShader()
{
    glDeleteProgram(_programId);
}

void GLShader::compile()
{
    _compiled = true;
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(_vertexShaderFilename, std::ios::in);
    if(vertexShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << vertexShaderStream.rdbuf();
        vertexShaderCode = sstr.str();
        vertexShaderStream.close();
    }
    else
    {
        _logger->error("Cannot open vertex shader file %s !", _vertexShaderFilename.c_str());
        return;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(_fragmentShaderFilename, std::ios::in);
    if(fragmentShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << fragmentShaderStream.rdbuf();
        fragmentShaderCode = sstr.str();
        fragmentShaderStream.close();
    }
    else
    {
        _logger->warning("Cannot open fragment shader file %s !", _fragmentShaderFilename.c_str());
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile Vertex Shader
    _logger->debug("Compiling shader : %s", _vertexShaderFilename.c_str());
    char const * vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        _logger->error("%s", &vertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    _logger->debug("Compiling shader : %s", _fragmentShaderFilename.c_str());
    char const * fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
        _logger->error("%s", &fragmentShaderErrorMessage[0]);
    }

    // Link the program
    _logger->debug("Linking program...");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        std::vector<char> ProgramErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
        _logger->error("%s", &ProgramErrorMessage[0]);
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    _programId = programID;
    _loaded = true;
}

void GLShader::use()
{
    if (!_compiled)
        compile();
    if (!_loaded)
        return;
    glUseProgram(_programId);
}

int GLShader::getUniformId(const std::string& name)
{
    auto it = _idCache.find(name);
    if (it != _idCache.end())
    {
        return it->second;
    }
    else
    {
        int id = glGetUniformLocation(_programId, name.c_str());
        if (id != -1)
            _idCache[name] = id;
        return id;
    }
}

void GLShader::setFloat(float value, const std::string& name)
{
    glUniform1f(getUniformId(name), value);
}

void GLShader::setVec2(const glm::vec2& value, const std::string& name)
{
    glUniform2fv(getUniformId(name), 1, glm::value_ptr(value));
}

void GLShader::setVec3(const glm::vec3& value, const std::string& name)
{
    glUniform3fv(getUniformId(name), 1, glm::value_ptr(value));
}

void GLShader::setVec4(const glm::vec4& value, const std::string& name)
{
    glUniform4fv(getUniformId(name), 1, glm::value_ptr(value));
}

void GLShader::setMatrix4(const glm::mat4& value, const std::string& name)
{
    glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::setInt(int value, const std::string& name)
{
    glUniform1i(getUniformId(name), value);
}

void GLShader::setUInt(uint32_t value, const std::string& name)
{
    glUniform1i(getUniformId(name), value);
}

void GLShader::setIntArray(int *value, uint32_t size, const std::string &name)
{
    glUniform1iv(getUniformId(name), sizeof(int) * size, value);
}

void GLShader::setFloatArray(float *value, uint32_t size, const std::string &name)
{
    glUniform1fv(getUniformId(name), sizeof(float) * size, value);
}

NAMESPACE_END
