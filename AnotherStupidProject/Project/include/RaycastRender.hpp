#pragma once

#include <Player.hpp>
#include <TextureManager.hpp>
#include <LevelTexture.hpp>
#include <Level.hpp>

#include <memory>

#include <glm/glm.hpp>

class RaycastRender {
public:
	RaycastRender(
		float fov,
		unsigned int renderWidth,
		unsigned int renderHeight
	);

	~RaycastRender();

    void RotateCameraPlane(InputManager::InputEventParameters* params)
    {
        const InputManager::InputEventMouseParameters* castParams = static_cast<InputManager::InputEventMouseParameters*>(params);

        //todo: has to be exact same as player mouse rotate
        double rot = -castParams->relativeMouseX * 0.01f;

        _cameraPlane = MathUtils::RotateVec2(_cameraPlane, rot);
    }

    inline void SetupRenderForLevel(const TextureManager& textureManager)
    {
        _ceilingTexture = textureManager.GetLevelTexture("ceiling");
        _floorTexture = textureManager.GetLevelTexture("floor");
    }

	inline void Render(const Player& player, const Level& currentLevel, const TextureManager& textureManager)
    {
        for (int y = 0; y < _renderHeight; ++y)
        {
            for (int x = 0; x < _renderWidth; ++x)
            {
                //SetPixel(x, y, 0x0);
                pixels[_renderWidth * y + x] = 0x0;
            }
        }

        //floor/ceiling rendering (a bit slow)
       /* glm::vec2 floorCoordinate{ 0, 0 };

        for (int y = 0; y < _renderHeight; ++y)
        {
            glm::vec2 ray_1 = player.forward - _cameraPlane;
            glm::vec2 ray_2 = player.forward + _cameraPlane;

            int p = y - _renderHeight / 2;

            float rowDistance = _posZ / p;

            float floorStepX = rowDistance * (ray_2.x - ray_1.x) / _renderHeight;
            float floorStepY = rowDistance * (ray_2.y - ray_1.y) / _renderWidth;

            floorCoordinate.x = player.position.x + rowDistance * ray_1.x;
            floorCoordinate.y = player.position.y + rowDistance * ray_1.y;

            for (int x = 0; x < _renderWidth; ++x)
            {
                // glm::uvec2 cell(floorCoordinate.x, floorCoordinate.y);

                int cellX = (int)(floorCoordinate.x);
                int cellY = (int)(floorCoordinate.y);

                // get the texture coordinate from the fractional part
                int tx = (int)(_ceilingTexture->width * (floorCoordinate.x - cellX)) & (_ceilingTexture->width - 1);
                int ty = (int)(_ceilingTexture->height * (floorCoordinate.y - cellY)) & (_ceilingTexture->height - 1);
                glm::uvec2 ceilingCoord(tx, ty);

                tx = (int)(_floorTexture->width * (floorCoordinate.x - cellX)) & (_floorTexture->width - 1);
                ty = (int)(_floorTexture->height * (floorCoordinate.y - cellY)) & (_floorTexture->height - 1);
                glm::uvec2 floorCoord(tx, ty);

                //glm::uvec2 ceilingCoord = ceilingTexture->GetFloorAndCeilingCoordinates(floorCoordinate);
                //glm::uvec2 floorCoord = floorTexture->GetFloorAndCeilingCoordinates(floorCoordinate);

                unsigned int ceilingTexel = _ceilingTexture->texels[_ceilingTexture->height * floorCoord.x + floorCoord.y];
                unsigned int floorTexel = _floorTexture->texels[_floorTexture->height * floorCoord.x + floorCoord.y];

                pixels[y * _renderWidth + x] = ceilingTexel;
                pixels[(_renderHeight - y - 1) * _renderWidth + x] = floorTexel;
                //SetPixel(x, y, ceilingTexture->GetTexel(floorCoord.x, floorCoord.y));
                //SetPixel(x, _renderHeight - y - 1, floorTexture->GetTexel(floorCoord.x, floorCoord.y));

                floorCoordinate.x += floorStepX;
                floorCoordinate.y += floorStepY;
            }
        }*/

        for (int x = 0; x < _renderWidth; ++x)
        {
            //calculate ray position and direction
            float cameraX = 2.0f * x / float(_renderWidth) - 1; //x-coordinate in camera space
            glm::vec2 ray = player.forward + _cameraPlane * cameraX;

            //which box of the map we're in
            glm::uvec2 pos(player.position);

            //length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            //length of ray from one x or y-side to next x or y-side
            double deltaDistX = std::abs(1.0f / ray.x);
            double deltaDistY = std::abs(1.0f / ray.y);

            double perpWallDist;

            //what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            short texture = 0; //was there a wall hit?
            int side; //was a NS or a EW wall hit?

            //calculate step and initial sideDist
            if (ray.x < 0)
            {
                stepX = -1;
                sideDistX = (player.position.x - pos.x) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (pos.x + 1.0 - player.position.x) * deltaDistX;
            }

            if (ray.y < 0)
            {
                stepY = -1;
                sideDistY = (player.position.y - pos.y) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (pos.y + 1.0 - player.position.y) * deltaDistY;
            }

            //perform DDA
            while (texture <= 0)
            {
                //jump to next map square, OR in x-direction, OR in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    pos.x += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    pos.y += stepY;
                    side = 1;
                }

                //Check if ray has hit a wall
                texture = currentLevel.IsWallHit(pos.x, pos.y);
            }

            if (texture > 0)
            {
                if (side == 0)
                    perpWallDist = (pos.x - player.position.x + (1.0 - stepX) / 2.0) / ray.x;
                else
                    perpWallDist = (pos.y - player.position.y + (1.0 - stepY) / 2.0) / ray.y;

                //Calculate height of line to draw on screen
                int lineHeight = (int)(_renderHeight / perpWallDist);

                //calculate lowest and highest pixel to fill in current stripe
                int drawStart = -lineHeight / 2 + _renderHeight / 2;
                if (drawStart < 0)
                    drawStart = 0;

                int drawEnd = lineHeight / 2 + _renderHeight / 2;
                if (drawEnd >= _renderHeight)
                    drawEnd = _renderHeight - 1;

                //calculate value of wallX
                double wallX; //where exactly the wall was hit
                if (side == 0)
                    wallX = player.position.y + perpWallDist * ray.y;
                else
                    wallX = player.position.x + perpWallDist * ray.x;

                wallX -= floor((wallX));

                LevelTexture* wallTexture = textureManager.GetLevelTexture(currentLevel.GetWallTexture(texture));

                //x coordinate on the texture
                int texX = int(wallX * double(wallTexture->width));
                if (side == 0 && ray.x > 0) texX = wallTexture->width - texX - 1;
                if (side == 1 && ray.y < 0) texX = wallTexture->width - texX - 1;

                double step = 1.0 * wallTexture->height / lineHeight;
                double texPos = (drawStart - _renderHeight / 2.0 + lineHeight / 2.0) * step;
                for (int y = drawStart; y < drawEnd; ++y)
                {
                    int texY = (int)texPos & (wallTexture->height - 1);
                    texPos += step;

                    //SetPixel(x, y, wallTexel);
                    pixels[y * _renderWidth + x] = wallTexture->texels[wallTexture->height * texX + texY];
                }

                texture = 0;
            }
        }
    }

	unsigned int* pixels;
private:
	float _fov;
	unsigned int _renderWidth;
	unsigned int _renderHeight;
	float _posZ;

	glm::vec2 _cameraPlane;

    LevelTexture* _ceilingTexture;
    LevelTexture* _floorTexture;
    LevelTexture* _wallTextures[];

	void Clear();
	void SetPixel(unsigned int x, unsigned int y, unsigned int color);
	unsigned int GetPixel(unsigned int x, unsigned int y);

};