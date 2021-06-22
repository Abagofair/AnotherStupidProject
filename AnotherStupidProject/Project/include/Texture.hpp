#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <glPortableHeaders.hpp>

class Texture {
public:
    Texture(
        std::string path);

    Texture(
        unsigned int width,
        unsigned int height);

    ~Texture();
    void Bind();
    void Unbind();
    void UpdateTexture(void* pixels);

private:
    Texture();
    Texture(Texture& texture);// = delete;

    unsigned int _glTextureId;
    bool _hasId;
    unsigned short _width;
    unsigned short _height;

    void CopyFromPixels(
        unsigned short width,
        unsigned short height,
        void* pixels);
};