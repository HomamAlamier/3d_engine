#include "gltexture.h"
#include <GL/glew.h>
#include <memory.h>
#include <map>
#include <gldds.h>
static std::map<std::string, uint32_t> _loadedTexturesCache;
NAMESPACE(opengl)

uint32_t GLTexture::texture_loadDDS(const char* path)
{
   return glLoadDDSTexture(path);
}



GLTexture::GLTexture(const std::string& filename)
    : _textureId(0)
    , _filename(filename)
{
    auto it = _loadedTexturesCache.find(filename);
    if (it != _loadedTexturesCache.end())
    {
        _textureId = it->second;
    }
    else
    {
        uint32_t id = texture_loadDDS(filename.c_str());
        if (id > 0)
        {
            _loadedTexturesCache[filename] = id;
            _textureId = id;
        }
    }
    _logger->debug("Loaded texture {\n\tGLID = %i\n\tpath = %s\n}", _textureId, filename.c_str());
}

void GLTexture::bindForSampler(uint32_t i)
{
    if (i > 32)
        return;
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, _textureId);
}

NAMESPACE_END
