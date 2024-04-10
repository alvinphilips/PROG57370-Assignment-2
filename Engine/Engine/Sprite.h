#pragma once

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "SDL_rect.h"
#include "Renderable.h"

class TextureAsset;

class Sprite : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Sprite, Component)

public:
	void SetSourceRect(SDL_Rect rect);
	void SetTextureAsset(TextureAsset* texAsset);

	inline SDL_Rect& GetTargetRect() { return targetRect; }

	void SerializeCreate(RakNet::BitStream& bitStream) const override;
	void DeserializeCreate(RakNet::BitStream& bitStream) override;
	void SetFilterColor(int r, int g, int b, int a);

protected:
	void Load(json::JSON& node) override;
	void Destroy() override;
	void Update() override;
	void Render() override;
	SDL_RendererFlip SetFlipFromScale(Vec2 scale);

protected:
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_Rect targetRect = { 0,0,0,0 };
	TextureAsset* texture = nullptr;
	IVec2 size = { 0, 0 };
	SDL_Rect sourceRect = { 0,0,0,0 };
	SDL_Color filterColor = { 255, 255, 255 };
};

#endif
