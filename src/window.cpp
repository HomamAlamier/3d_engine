#include <window.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

NAMESPACE(sage3d)

Window::Window(char vmajor, char vminor, char antialiasing, int width, int height,
                   const std::string& title, bool fullscreen)
    : _version_major(vmajor)
    , _version_minor(vminor)
    , _antialiasing(antialiasing)
    , _width(width)
    , _height(height)
    , _title(title)
    , _fullscreen(fullscreen)
    , _lastTime(0.0)
    , _fpsCounter(0)
{
}

bool Window::init()
{
    ::glewExperimental = true;
    if (!glfwInit())
    {
        _logger->critical("Failed to initialize GLFW !");
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES, _antialiasing);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    if (!_window)
    {
        _logger->critical("Failed to create window (major: %i, minor: %i)",
                          _version_major, _version_minor);
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent((GLFWwindow*)_window);
    if (glewInit() != GLEW_OK)
    {
        _logger->critical("Failed to initialize glew !");
        glfwTerminate();
        return false;
    }
    _lastTime = glfwGetTime();
    return true;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose((GLFWwindow*)_window) != 0;
}

void Window::doEvents()
{
    glfwSwapBuffers((GLFWwindow*)_window);
    glfwPollEvents();
}

void Window::setTitle(const std::string& title)
{
    glfwSetWindowTitle((GLFWwindow*)_window, title.c_str());
    _title = title;
}

void Window::fpsInTitle()
{
    double tm = glfwGetTime();
    double delta = tm - _lastTime;
    _fpsCounter++;
    if (delta >= 1.0)
    {
        _lastTime = tm;
        glfwSetWindowTitle((GLFWwindow*)_window, (_title + " [ fps = " + std::to_string((double)_fpsCounter / delta) + " ]").c_str());
        _fpsCounter = 0;
    }
}

KeyboardKeyData Window::getKeyboardKeyData()
{
    std::vector<KeyboardKeys> keys;
    for(int i = KEY_SPACE; i < KEY_MENU; ++i)
    {
        if (glfwGetKey((GLFWwindow*)_window, i) == GLFW_PRESS)
        {
            keys.push_back(static_cast<KeyboardKeys>(i));
        }
    }
    return KeyboardKeyData(keys);
}

NAMESPACE_END
