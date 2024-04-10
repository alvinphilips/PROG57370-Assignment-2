#include "EngineCore.h"
#include "MusicAsset.h"

IMPLEMENT_DYNAMIC_CLASS(MusicAsset)

void MusicAsset::Destroy()
{
	Asset::Destroy();

	Mix_FreeMusic(music);
	music = nullptr;
}

void MusicAsset::Load(json::JSON& json)
{
	Asset::Load(json);
	music = Mix_LoadMUS(filepath.c_str());

#ifdef DEBUG_MUSIC_LOAD
	if (music == nullptr)
	{
		LOG("Could not load music.");
		LOG(Mix_GetError());
	}
	else
	{
		const auto music_title = Mix_GetMusicTitle(music);
		LOG("Music asset: " << music_title);
	}
#endif
};
