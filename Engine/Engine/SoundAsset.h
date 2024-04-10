#pragma once

#ifndef _SOUND_ASSET_H_
#define _SOUND_ASSET_H_

#include "Asset.h"
#include "SDL_mixer.h"

#ifdef DEBUG_ASSET_LOAD
#define DEBUG_SFX_LOAD
#endif

#ifdef NDEBUG_SFX_LOAD
#undef DEBUG_SFX_LOAD
#endif

class SoundAsset final : public Asset
{
    DECLARE_DYNAMIC_DERIVED_CLASS(SoundAsset, Asset)

public:
    Mix_Chunk* GetSound() const { return sound; };

protected:
    void Load(json::JSON&) override;
    void Destroy() override;

private:
    Mix_Chunk* sound = nullptr;
};

#endif
