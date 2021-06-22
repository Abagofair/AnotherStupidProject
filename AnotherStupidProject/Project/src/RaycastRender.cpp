#include <RaycastRender.hpp>

RaycastRender::RaycastRender(float fov, unsigned int renderWidth, unsigned int renderHeight) : _fov(fov), _renderWidth(renderWidth), _renderHeight(renderHeight)
{
    _cameraPlane = glm::vec2(0, _fov);

    pixels = new unsigned int [renderWidth * renderHeight];
    _posZ = 0.5 * _renderHeight;
}

RaycastRender::~RaycastRender()
{
    delete[] pixels;
}

void RaycastRender::Clear()
{
    for (int y = 0; y < _renderHeight; ++y)
    {
        for (int x = 0; x < _renderWidth; ++x)
        {
            SetPixel(x, y, 0x0);
        }
    }
}

void RaycastRender::SetPixel(unsigned int x, unsigned int y, unsigned int color)
{
    pixels[y * _renderWidth + x] = color;
}

unsigned int RaycastRender::GetPixel(unsigned int x, unsigned int y)
{
    return pixels[y * _renderWidth + x];
}