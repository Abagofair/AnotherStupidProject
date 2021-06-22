#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glPortableHeaders.hpp>

class CanvasShader {
public:
    CanvasShader();
    ~CanvasShader();

    void Use();
    void UseWithOrtho(const glm::mat4& ortho);
    void SetOrthoTransform(const glm::mat4& ortho);

private:
    CanvasShader(CanvasShader& canvasShader);// = delete;

    unsigned int _glShaderProgramId;

    std::string _vertexShaderSource = "#version 300 es\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "uniform mat4 ortho;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   TexCoord = aTexCoord;\n"
        "   gl_Position = ortho * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    std::string _fragmentShaderSource = "#version 300 es\n"
        "in mediump vec2 TexCoord;\n"
        "out mediump vec4 FragColor;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(ourTexture, TexCoord);\n"
        "}\0";
};