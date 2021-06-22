#pragma once

#include <Texture.hpp>

Texture::Texture(
    std::string path)
{
    _hasId = false;

    SDL_Surface* surface = IMG_Load(path.c_str());

    _width = surface->w;
    _height = surface->h;

    CopyFromPixels(_width, _height, surface->pixels);

    SDL_FreeSurface(surface);
}

Texture::Texture(
    unsigned int width,
    unsigned int height) : _width(width), _height(height)
{
    _hasId = false;

    std::size_t size = sizeof(uint32_t) * width * height;
    uint32_t* whitePixels = (uint32_t*)(malloc(size));
    if (whitePixels == nullptr)
        throw;

    for (size_t i = 0; i < width * height; ++i)
    {
        if (i % 100 == 0 || i % 105 == 0)
        {
            whitePixels[i] = 0x00000000;
        }
        else
        {
            whitePixels[i] = 0xFFFFFFFF;
        }
    }

    CopyFromPixels(_width, _height, static_cast<void*>(whitePixels));

    free(whitePixels);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_glTextureId);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _glTextureId);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UpdateTexture(void* pixels)
{
    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    Unbind();
}

void Texture::CopyFromPixels(
    unsigned short width,
    unsigned short height,
    void* pixels)
{
    if (_hasId)
    {
        throw;
    }

    glGenTextures(1, &_glTextureId);
    glBindTexture(GL_TEXTURE_2D, _glTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    _hasId = true;
}