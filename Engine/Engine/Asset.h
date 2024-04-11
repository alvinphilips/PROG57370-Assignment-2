#pragma once

#ifndef _ASSET_H_
#define _ASSET_H_

#include "Object.h"

#ifdef DEBUG_ASSET_ALL
#define DEBUG_ASSET_INIT
#define DEBUG_ASSET_LOAD
#endif

#ifdef NDEBUG_ASSET_INIT
#undef DEBUG_ASSET_INIT
#endif

#ifdef NDEBUG_ASSET_LOAD
#undef DEBUG_ASSET_LOAD
#endif

#ifdef DEBUG_AUDIO_LOAD
#define DEBUG_MUSIC_LOAD
#define DEBUG_SFX_LOAD
#endif

class Asset : public Object
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Asset, Object)

public:
	void Load(json::JSON&) override;
	void SetFilepath(const std::string& path) { filepath = path; }

protected:
	std::string filepath;

	friend class AssetManager;
};

#endif
