#include <engine.h>
#include <window.h>
#include <engine_gl.h>

NAMESPACE(sage3d)

Engine::Engine(int width, int height)
    : _drawCallback(nullptr)
    , _initCallback(nullptr)
{
    _window = new Window(GL_MAJOR, GL_MINOR, 1, width, height, "Game");
    _gl = new Engine_GL(this);
    _logger->debug("Engine initialized successfully !");
}

int Engine::exec()
{
    if (!_window->init())
    {
        _logger->critical("Failed to initialize window !");
        return 1;
    }
    _logger->debug("Window initialized successfully !");
    if (_initCallback)
        _initCallback(this);
    _gl->init();
    while (!_window->shouldClose())
    {
        _gl->clearColorBuffer();
        if (_drawCallback)
        {
            _drawCallback(_gl);
        }

        if (getOption(ShowFps))
        {
            _window->fpsInTitle();
        }

        if (_kbCallback)
        {
            KeyboardKeyData kkd = _window->getKeyboardKeyData();
            if (kkd.hasValues())
            {
                _kbCallback(&kkd);
            }
        }
        _window->doEvents();
    }
    return 0;
}

void Engine::setOptions(EngineOptions opt, bool value)
{
    _options[opt] = value;
}

bool Engine::getOption(EngineOptions opt)
{
    auto it = _options.find(opt);
    if (it != _options.end())
    {
        return it->second;
    }
    else
        return false;
}

NAMESPACE_END
