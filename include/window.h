#ifndef WINDOW_H
#define WINDOW_H
#include <macros.h>
#include <logger.h>
NAMESPACE(sage3d)

class Window
{
    LOGGER(Sage3D, Window)
    READ_PROP(width, int)
    READ_PROP(height, int)
    READ_PROP_REF(title, std::string)
    READ_PROP(fullscreen, bool)
    READ_PROP(antialiasing, char)
    READ_PROP(version_major, char)
    READ_PROP(version_minor, char)
    READ_PROP(inited, bool)
public:
    Window(char vmajor, char vminor, char antialiasing, int width, int height,
           const std::string& title, bool fullscreen = false);
    bool init();
    bool shouldClose() const;
    void doEvents();
private:
    void* _window;
};

NAMESPACE_END

#endif // WINDOW_H
