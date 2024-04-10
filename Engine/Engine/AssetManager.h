#pragma once

#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "Asset.h"

struct AssetMapEntry
{
	Asset* asset;
	unsigned int ref_count;
};

class AssetManager final
{
	DECLARE_SINGLETON(AssetManager)

public:
	void Initialize();
	void Destroy();
	void AddAsset(Asset* asset);

	void LoadSceneAsset(std::string& guid);
	void LoadSceneAsset(STRCODE id);
	void UnloadSceneAsset(std::string& guid);
	void UnloadSceneAsset(STRCODE id);

	Asset* GetAsset(std::string guid);
	Asset* GetAsset(STRCODE id);

	template <typename T = Asset>
	T* GetAsset(std::string guid)
	{
		return (T*)GetAsset(guid);
	}

	template <typename T = Asset>
	T* GetAsset(STRCODE id)
	{
		return (T*)GetAsset(id);
	}

	void RemoveAsset(std::string guid);
	void RemoveAsset(STRCODE id);

	void Load(const std::string& config_file);

private:
	std::string assetDirectory = "../Assets";
	bool recursiveSearch = true;
	std::map<STRCODE, AssetMapEntry> assets;
};

#endif
