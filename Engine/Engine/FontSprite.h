#pragma once

#ifndef _FONT_SPRITE_H_
#define _FONT_SPRITE_H_

#include "Sprite.h"
#include "SDL.h"

class FontAsset;

class FontSprite : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(FontSprite, Sprite)

public:
	void SetText(std::string text);
	void SetFont(FontAsset* font);
	void SetFontColor(int r, int g, int b, int a);

protected:
	void Initialize() override;
	void Destroy() override;
	void Render() override;

	void Save(json::JSON&) const;
	void Load(json::JSON&) override;

private:
	void RegenerateOutput();

private:
	std::string text = "DEFAULT TEXT";
	FontAsset* font = nullptr;
	SDL_Texture* output = nullptr;
	SDL_Color fontColor = { 255,255,255,255 };
};

#endif