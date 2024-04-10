#pragma once

#ifndef _FONT_ASSET_H_
#define _FONT_ASSET_H_

#include "Asset.h"
#include "SDL_ttf.h"

#ifdef DEBUG_ASSET_INIT
#define DEBUG_FONT_INIT
#endif

#ifdef NDEBUG_FONT_INIT
#undef DEBUG_FONT_INIT
#endif

class FontAsset final : public Asset
{
    DECLARE_DYNAMIC_DERIVED_CLASS(FontAsset, Asset)

public:
    void SetFontSize(uint32_t size);
    TTF_Font* GetFont() const { return font; };

protected:
    void Initialize() override;
    void Load(json::JSON&) override;
    void Destroy() override;

private:
    TTF_Font* font = nullptr;
    uint32_t font_size = 12;
};

#endif
