#ifndef ENGINE_H
#define ENGINE_H
#include <macros.h>
#include <logger.h>
NAMESPACE(sage3d)

class Window;
class Engine_GL;
class Engine
{
    LOGGER(Sage3D, Engine)
    const char GL_MAJOR = 3;
    const char GL_MINOR = 3;
public:
    Engine(int width, int height);
    int exec();
private:
    Window* _window;
    Engine_GL* _gl;
};

NAMESPACE_END

#endif // !ENGINE_H
