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

NAMESPACE_END
