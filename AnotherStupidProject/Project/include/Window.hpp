#pragma once

#include <string>
#include <SDL.h>
#include <glm/glm.hpp>

class Window {
public:
    Window(
        unsigned short width,
        unsigned short height,
        std::string title);
    ~Window();

    void SwapBuffers();
    void CenterMouseInWindow();
    glm::uvec2 WindowDimensions();
private:
    Window();
    Window(Window& window);// = delete;

    unsigned short _width;
    unsigned short _height;
    float _aspectRatio;

    SDL_Window* _window;
    SDL_GLContext _context;
};