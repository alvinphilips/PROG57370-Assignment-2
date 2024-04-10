#include "EngineCore.h"
#include "TextureAsset.h"
#include "RenderSystem.h"

IMPLEMENT_DYNAMIC_CLASS(TextureAsset);

void TextureAsset::Initialize()
{
	Asset::Initialize();

	SDL_Surface* image = IMG_Load(filepath.c_str());

#ifdef DEBUG_TEXTURE_INIT
	if (image == nullptr)
	{
		LOG("Could not load Image: " << IMG_GetError());
	}
#endif

	texture = SDL_CreateTextureFromSurface(&RenderSystem::Instance().GetRenderer(), image);
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

#ifdef DEBUG_TEXTURE_INIT
	if (texture == nullptr)
	{
		LOG("Could not load Texture: " << SDL_GetError());
	} else
	{
		LOG("Loaded a " << width << "x" << height << "texture");
	}
#endif

	SDL_FreeSurface(image);
}

void TextureAsset::Destroy()
{
	Asset::Destroy();

	SDL_DestroyTexture(texture);
	texture = nullptr;
}
