#include "EngineCore.h"
#include "FontAsset.h"
#include "RenderSystem.h"

IMPLEMENT_DYNAMIC_CLASS(FontAsset)

void FontAsset::Initialize()
{
	Asset::Initialize();

	TTF_Init();
	font = TTF_OpenFont(filepath.c_str(), font_size);

#ifdef DEBUG_FONT_INIT
	if (font == nullptr)
	{
		LOG(TTF_GetError());
	} else
	{
		LOG("Font asset: " << TTF_FontFaceFamilyName(font));
	}
#endif
}

void FontAsset::Destroy()
{
	Asset::Destroy();

	TTF_CloseFont(font);
	font = nullptr;
	TTF_Quit();
}

void FontAsset::Load(json::JSON& node)
{
	Asset::Load(node);
	
	if (node.hasKey("FontSize"))
	{
		font_size = node.at("FontSize").ToInt();
	}
}

void FontAsset::SetFontSize(const uint32_t size)
{
#ifndef STRIP_NULL_CHECKS
	if (font == nullptr)
	{
		LOG("ERROR: Font not set");
		return;
	}
#endif
	font_size = size;
	TTF_SetFontSize(font, size);
};
