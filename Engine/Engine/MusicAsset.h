#pragma once

#ifndef _MUSIC_ASSET_H_
#define _MUSIC_ASSET_H_

#include "Asset.h"
#include "SDL_mixer.h"

#ifdef DEBUG_ASSET_LOAD
#define DEBUG_MUSIC_LOAD
#endif

#ifdef NDEBUG_MUSIC_LOAD
#undef DEBUG_MUSIC_LOAD
#endif

class MusicAsset final : public Asset 
{
    DECLARE_DYNAMIC_DERIVED_CLASS(MusicAsset, Asset)

public:
    Mix_Music* GetMusic() const { return music; }

protected:
    void Load(json::JSON&) override;
    void Destroy() override;

private:
    Mix_Music* music = nullptr;
};

#endif
