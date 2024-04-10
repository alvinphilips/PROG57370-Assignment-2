#pragma once

#ifndef _TEXTURE_ASSET_H_
#define _TEXTURE_ASSET_H_

#include "Asset.h"
#include "SDL_image.h"

class TextureAsset final : public Asset 
{
    DECLARE_DYNAMIC_DERIVED_CLASS(TextureAsset, Asset)

public:
    IVec2 GetDimensions() const { return IVec2(width, height); }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    SDL_Texture* GetTexture() const { return texture; }

protected:
    void Initialize() override;
    void Destroy() override;

private:
    SDL_Texture* texture = nullptr;
    int32_t width = 1;
    int32_t height = 1;
};

#endif
