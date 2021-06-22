#include <TextureManager.hpp>

TextureManager::TextureManager()
{
	//todo: handle this via some file format
	LevelTexture* wall = CreateLevelTexture("wall", "assets\\wolf2.png");
	LevelTexture* floor = CreateLevelTexture("floor", "assets\\wolf7.png");
	LevelTexture* ceiling = CreateLevelTexture("ceiling", "assets\\wolf4.png");

	_wallTextures[wall->name] = wall;
	_wallTextures[floor->name] = floor;
	_wallTextures[ceiling->name] = ceiling;
}

TextureManager::~TextureManager()
{
	for (auto& keyValue : _wallTextures)
	{
		delete[] keyValue.second->texels;
		delete keyValue.second;
	}
}

unsigned int TextureManager::GetTexel(std::string name, const glm::vec2 floor) const
{
	const auto& iter = _wallTextures.find(name);
	if (iter != _wallTextures.end())
	{
		const auto& levelTexture = iter->second;
		glm::uvec2 texCoords = levelTexture->GetFloorAndCeilingCoordinates(floor);
		return levelTexture->texels[levelTexture->height * texCoords.x + texCoords.y];
	}

	return 0xf;
}

LevelTexture* TextureManager::CreateLevelTexture(std::string name, std::string filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	if (surface == nullptr)
	{
		throw std::exception("Could not image load file when creating level texture");
	}

	unsigned int* texels = (unsigned int*)malloc(sizeof(unsigned int) * surface->h * surface->w);
	if (texels == nullptr)
	{
		throw std::exception("Could not allocate LevelTexture->texels");
	}

	for (int y = 0; y < surface->h; ++y)
	{
		for (int x = 0; x < surface->w; ++x)
		{
			Uint32 t = static_cast<uint32_t*>(surface->pixels)[y * surface->w + x];
			Uint8 r, g, b, a;
			SDL_GetRGBA(t, surface->format, &r, &g, &b, &a);
			//abgr to rgba
			//Uint32 test = 0x0 | (r << 24) | (g << 16) | (b << 8) | a;
			texels[y * surface->w + x] = t;
		}
	}

	LevelTexture* levelTexture = new LevelTexture(name, surface->w, surface->h, texels);

	SDL_FreeSurface(surface);

	return levelTexture;
}