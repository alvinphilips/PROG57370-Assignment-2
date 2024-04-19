#include "EngineCore.h"
#include "FontSprite.h"
#include <utility>
#include "SDL_ttf.h"
#include "RenderSystem.h"
#include "Entity.h"
#include "FontAsset.h"
#include "AssetManager.h"

IMPLEMENT_DYNAMIC_CLASS(FontSprite);

void FontSprite::Initialize()
{
	RegenerateOutput();
}

void FontSprite::Destroy()
{
	Sprite::Destroy();

	if (output != nullptr)
	{
		SDL_DestroyTexture(output);
		output = nullptr;
	}
}

void FontSprite::Render()
{
#ifndef STRIP_NULL_CHECKS
	if (output == nullptr)
	{
		return;
	}
#endif

	const Transform& transform = owner->GetTransform();

	SDL_RenderCopyEx(
		&RenderSystem::Instance().GetRenderer(),
		output,
		nullptr,
		&targetRect,
		(double)transform.rotation,
		nullptr,
		flip
	);
}

void FontSprite::Save(json::JSON& document) const
{
	document["Text"] = text;

	json::JSON subObject = json::JSON::Object();
	subObject["R"] = fontColor.r;
	subObject["G"] = fontColor.g;
	subObject["B"] = fontColor.b;
	subObject["A"] = fontColor.a;

	document["FontColor"] = subObject;

	document["Font"] = font->GetGuid();
}

void FontSprite::Load(json::JSON& node)
{
	Sprite::Load(node);

	if (node.hasKey("Text"))
	{
		text = node["Text"].ToString();
	}

	if (node.hasKey("FontColor"))
	{
		json::JSON colorNode = node["FontColor"];
		fontColor.r = static_cast<Uint8>(colorNode["R"].ToInt());
		fontColor.g = static_cast<Uint8>(colorNode["G"].ToInt());
		fontColor.b = static_cast<Uint8>(colorNode["B"].ToInt());
		fontColor.a = static_cast<Uint8>(colorNode["A"].ToInt());
	}

	const std::string guid = node["Font"].ToString();

	font = AssetManager::Instance().GetAsset<FontAsset>(guid);
	RegenerateOutput();
}

void FontSprite::SetText(std::string text)
{
	this->text = std::move(text);
	RegenerateOutput();
}

void FontSprite::SetFont(FontAsset* font)
{
	this->font = font;
	RegenerateOutput();
}

void FontSprite::SetFontColor(int r, int g, int b, int a)
{
	fontColor.r = static_cast<Uint8>(r);
	fontColor.g = static_cast<Uint8>(g);
	fontColor.b = static_cast<Uint8>(b);
	fontColor.a = static_cast<Uint8>(a);
	RegenerateOutput();
}

void FontSprite::RegenerateOutput()
{
#ifndef STRIP_NULL_CHECKS
	if (font == nullptr)
	{
		return;
	}
#endif

	SDL_Surface* textSurface = TTF_RenderText_Solid(font->GetFont(), text.c_str(), fontColor);

	if (RenderSystem::Instance().HasRenderer())
	{
		SDL_DestroyTexture(output);
		output = SDL_CreateTextureFromSurface(&RenderSystem::Instance().GetRenderer(), textSurface);

		SDL_QueryTexture(output, nullptr, nullptr, &sourceRect.w, &sourceRect.h);
	} else
	{
		sourceRect.w = textSurface->w;
		sourceRect.h = textSurface->h;
	}
	SDL_FreeSurface(textSurface);
}