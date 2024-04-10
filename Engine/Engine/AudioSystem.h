#pragma once

#ifndef _AUDIO_SYSTEM_H_
#define _AUDIO_SYSTEM_H_

class SoundAsset;
class MusicAsset;

class AudioSystem final
{
	DECLARE_SINGLETON(AudioSystem)

public:
	void SetMusic(const MusicAsset* music);
	void PlayFX(const SoundAsset* sound);

protected:
	void Load(const std::string& config_file);
	void Initialize();
	void Destroy();

private:
	int sampleRate = 44100;
	friend class Engine;
};

#endif
