#include <engine.h>
#include <window.h>
#include <engine_gl.h>

NAMESPACE(sage3d)

Engine::Engine(int width, int height)
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
    while (!_window->shouldClose())
    {
        _gl->clearColorBuffer();
        _window->doEvents();
    }
    return 0;
}

NAMESPACE_END
