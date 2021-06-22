#include <Window.hpp>

Window::Window(
    unsigned short width,
    unsigned short height,
    std::string title) : _width(width), _height(height)
{
    _aspectRatio = (float)width / (float)height;

    _window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL
    );

    _context = SDL_GL_CreateContext(_window);
}

Window::~Window()
{
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(_window);
}

void Window::CenterMouseInWindow()
{
    SDL_WarpMouseInWindow(_window, _width / 2, _height / 2);
}

glm::uvec2 Window::WindowDimensions()
{
    return glm::uvec2(_width, _height);
}
