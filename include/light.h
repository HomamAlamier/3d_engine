#ifndef LIGHT_H
#define LIGHT_H
#include <macros.h>

NAMESPACE(sage3d)

enum LightType
{
    DirectionalLight,
    PointLight
};

class Light
{
    READ_PROP(color, glm::vec3)
    READ_PROP(position, glm::vec3)
    READ_PROP(direction, glm::vec3)
    READ_PROP(ambientStrength, float)
    READ_PROP(specularStrength, float)
public:
    Light();
};

NAMESPACE_END

#endif // LIGHT_H
