#ifndef GLTEXTURE_H
#define GLTEXTURE_H
#include <macros.h>
#include <logger.h>

NAMESPACE(opengl)

class GLTexture
{
    LOGGER(Sage3D, GLTexture)
    READ_PROP(textureId, uint32_t)
    READ_PROP_REF(filename, std::string)
public:
    GLTexture(const std::string& filename);
    void bindForSampler(uint32_t);
private:
    uint32_t texture_loadDDS(const char* path);
};

NAMESPACE_END

#endif // GLTEXTURE_H
