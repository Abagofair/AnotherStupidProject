#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Texture.hpp>
#include <CanvasShader.hpp>

#include <glPortableHeaders.hpp>

class Canvas {
public:
    Canvas(
        unsigned int width,
        unsigned int height);
    ~Canvas();

    //Updates the texture and then draws the canvas
    //Texture has to match existing (width, height) dimensions - else crash
    void Draw(void* pixels);
    //Uses existing texture and draws the canvas
    void Draw();

private:
    Canvas(Canvas& canvas);// = delete;

    glm::mat4 _canvasProjection;

    std::unique_ptr<CanvasShader> _canvasShader;
    std::unique_ptr<Texture> _texture;

    unsigned int _glVBOId;
    unsigned int _glVAOId;
    unsigned int _glEBOId;

    //make this dynamic
    //defined from top-left
    static constexpr float vertices[] =
    {
        // positions           // uv
        800.0f, 0.0f,   0.0f,  1.0f, 0.0f, // top right
        800.0,  600.0f, 0.0f,  1.0f, 1.0f, // bottom right
        0.0f,   600.0f, 0.0f,  0.0f, 1.0f, // bottom left
        0.0f,   0.0f,   0.0f,  0.0f, 0.0f  // top left
    };

    static constexpr unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };
};