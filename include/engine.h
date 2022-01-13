#ifndef ENGINE_H
#define ENGINE_H
#include <macros.h>
#include <logger.h>
#include <map>
#include <engine_keyboard.h>

NAMESPACE(sage3d)

class Window;
class Engine_GL;
class Engine;

typedef void (*EngineDrawCallback)(Engine_GL*);
typedef void (*EngineInitCallback)(Engine*);
typedef void (*EngineKeyboardCallBack)(KeyboardKeyData*);

enum EngineOptions
{
    ShowFps
};


class Engine
{
    LOGGER(Sage3D, Engine)
    const char GL_MAJOR = 3;
    const char GL_MINOR = 3;
public:
    Engine(int width, int height);
    int exec();
    void setDrawCallback(EngineDrawCallback cb) { _drawCallback = cb; }
    void setInitCallback(EngineInitCallback cb) { _initCallback = cb; }
    void setKeyboardCallback(EngineKeyboardCallBack cb) { _kbCallback = cb; }
    void setOptions(EngineOptions opt, bool value = true);
private:
    bool getOption(EngineOptions opt);
    Window* _window;
    Engine_GL* _gl;
    EngineDrawCallback _drawCallback;
    EngineInitCallback _initCallback;
    EngineKeyboardCallBack _kbCallback;
    std::map<EngineOptions, bool> _options;
};

NAMESPACE_END

#endif // !ENGINE_H
