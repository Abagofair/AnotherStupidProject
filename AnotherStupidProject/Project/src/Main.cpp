#include <math.h>
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include <Window.hpp>
#include <Canvas.hpp>

#include <glPortableHeaders.hpp>

#include <InputManager.hpp>
#include <RaycastRender.hpp>
#include <Player.hpp>
#include <TextureManager.hpp>
#include <Level.hpp>

#include <Windows.h>

std::unique_ptr<Window> window;

void Render()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
            }
            break;
        default:
            break;
        }
    }

    glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /*glUseProgram(shaderProgram);
    texture->Bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);*/

    window->SwapBuffers();
}

constexpr int windowWidth = 1280;
constexpr int windowHeight = 960;

constexpr int renderWidth = 800;
constexpr int renderHeight = 600;

constexpr int mapWidth = 24;
constexpr int mapHeight = 24;

/// <summary>
/// Window setup and GL context creation
/// </summary>
void Initialize()
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = std::make_unique<Window>(windowWidth, windowHeight, "El Doomio");

#ifndef __EMSCRIPTEN__
    gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
#endif
}

glm::vec2 RotateVec2(const glm::vec2& v, float angle)
{
    glm::vec2 rotated;

    double oldDirX = v.x;
    rotated.x = v.x * cos(angle) - v.y * sin(angle);
    rotated.y = oldDirX * sin(angle) + v.y * cos(angle);

    return rotated;
}

int main(int argc, char* args[])
{
    Initialize();

    InputManager* inputManager = InputManager::GetInstance();

    Canvas canvas(renderWidth, renderHeight);

    RaycastRender raycastRender(0.66, renderWidth, renderHeight);
    TextureManager textureManager;

    raycastRender.SetupRenderForLevel(textureManager);

    Level currentLevel;
      
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(render, 0, 1);
#endif

    glm::vec2 playerPosition(22, 12);
    glm::vec2 playerForward(-1, 0);
    glm::vec2 playerRight(0, 1);

    Player player(playerPosition, playerForward, playerRight);

    int exit = 0;

    window->CenterMouseInWindow();
    glm::uvec2 windowDimensions = window->WindowDimensions();

    SDL_SetRelativeMouseMode((SDL_bool)true);

    InputManager::InputEventSubscriber w { 1, SDL_KEYDOWN, SDLK_w, std::bind(&Player::MoveForward, &player, std::placeholders::_1) };
    InputManager::InputEventSubscriber a { 2, SDL_KEYDOWN, SDLK_s, std::bind(&Player::MoveBack, &player, std::placeholders::_1) };
    InputManager::InputEventSubscriber s { 3, SDL_KEYDOWN, SDLK_a, std::bind(&Player::MoveLeft, &player, std::placeholders::_1) };
    InputManager::InputEventSubscriber d { 4, SDL_KEYDOWN, SDLK_d, std::bind(&Player::MoveRight, &player, std::placeholders::_1) };
    InputManager::InputEventSubscriber mouseRotate { 5, SDL_MOUSEMOTION, (SDL_KeyCode)0, std::bind(&Player::MouseRotate, &player, std::placeholders::_1) };
    InputManager::InputEventSubscriber cameraRotate { 6, SDL_MOUSEMOTION, (SDL_KeyCode)0, std::bind(&RaycastRender::RotateCameraPlane, &raycastRender, std::placeholders::_1) };

    inputManager->Subscribe(w);
    inputManager->Subscribe(a);
    inputManager->Subscribe(s);
    inputManager->Subscribe(d);
    inputManager->Subscribe(mouseRotate);
    inputManager->Subscribe(cameraRotate);

    while (!exit)
    {
        inputManager->ReadEventQueue();
        inputManager->Consume();

        raycastRender.Render(player, currentLevel, textureManager);

        glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        canvas.Draw((void*)raycastRender.pixels);

        window->SwapBuffers();
    }

    IMG_Quit();

    return 0;
}