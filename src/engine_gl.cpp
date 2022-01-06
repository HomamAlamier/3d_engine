#include "engine_gl.h"
#include <GL/glew.h>
#include <sstream>
#include <istream>
#include <fstream>
NAMESPACE(sage3d)

GLShader::GLShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
    : _vertexShaderFilename(vertexShaderFilename)
    , _fragmentShaderFilename(fragmentShaderFilename)
    , _programId(0)
    , _loaded(false)
    , _compiled(false)
{
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
}

void GLShader::use()
{
    if (!_compiled)
        compile();
    if (!_loaded)
        return;
    glUseProgram(_programId);
}

Engine_GL::Engine_GL(Engine* engine)
    : _engine(engine)
{

}

void Engine_GL::clearColorBuffer()
{
    glClear( GL_COLOR_BUFFER_BIT );
}

NAMESPACE_END
