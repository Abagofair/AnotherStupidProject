#include <Canvas.hpp>

Canvas::Canvas(
    unsigned int width,
    unsigned int height)
{
    //setup orthographic projection for the canvas
    _canvasProjection = glm::ortho<float>(0, (float)width, (float)height, 0, -1.0f, 1.0f);

    //Default weird texture
    _texture = std::make_unique<Texture>(width, height);

    //Default canvas shader
    _canvasShader = std::make_unique<CanvasShader>();

    //Setup a fullscreen quad
    glGenVertexArrays(1, &_glVAOId);
    glGenBuffers(1, &_glVBOId);
    glGenBuffers(1, &_glEBOId);

    glBindVertexArray(_glVAOId);

    glBindBuffer(GL_ARRAY_BUFFER, _glVBOId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Canvas::vertices), Canvas::vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glEBOId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Canvas::indices), Canvas::indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Canvas::~Canvas()
{
    glDeleteVertexArrays(1, &_glVAOId);
    glDeleteBuffers(1, &_glVBOId);
    glDeleteBuffers(1, &_glEBOId);
}

void Canvas::Draw(void* pixels)
{
    _texture->UpdateTexture(pixels);

    _canvasShader->UseWithOrtho(_canvasProjection);
    _texture->Bind();

    glBindVertexArray(_glVAOId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

//Uses existing texture and draws the canvas
void Canvas::Draw()
{
    _canvasShader->UseWithOrtho(_canvasProjection);
    _texture->Bind();

    glBindVertexArray(_glVAOId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}