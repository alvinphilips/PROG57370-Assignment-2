#include "EngineCore.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Scene.h"

#include <filesystem>

using std::filesystem::directory_iterator;
using std::filesystem::recursive_directory_iterator;

Asset* HandleAssetEntry(const std::filesystem::directory_entry& entry)
{
	// Skip directories
	if (entry.is_directory())
	{
		return nullptr;
	}

	// Check if the file is a .asset type
	if (entry.path().extension().compare(".asset") != 0)
	{
		return nullptr;
	}

	std::cout << "Found Asset: " << entry.path() << std::endl;

	std::ifstream inputStream(entry.path());
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON node = json::JSON::Load(str);

	if (!node.hasKey("AssetType"))
	{
		LOG("File " << entry.path() << " does not have a 'AssetType' field. Skipping.");
		return nullptr;
	}

	const auto assetType = node.at("AssetType").ToString();
	Asset* asset = (Asset*)CreateObject(assetType.c_str());
	std::string path = entry.path().relative_path().replace_extension("").generic_string();
	asset->SetFilepath(path);
	asset->Load(node);
	return asset;
}

void AssetManager::Initialize()
{
	if (recursiveSearch)
	{
		for (const auto& entry : recursive_directory_iterator(assetDirectory))
		{
			AddAsset(HandleAssetEntry(entry));
		}
	}
	else
	{
		for (const auto& entry : directory_iterator(assetDirectory))
		{
			AddAsset(HandleAssetEntry(entry));
		}
	}
}

void AssetManager::AddAsset(Asset* asset)
{
	if (asset == nullptr)
	{
		return;
	}
	AssetMapEntry entry;
	entry.asset = asset;
	entry.ref_count = 0;
	assets.insert_or_assign(asset->GetUid(), entry);
}

void AssetManager::LoadSceneAsset(std::string& guid)
{
	STRCODE id = GetHashCode(guid.c_str());
	return LoadSceneAsset(id);
}

void AssetManager::LoadSceneAsset(const STRCODE id)
{
	const auto found_asset = assets.find(id);
	if (found_asset == assets.end())
	{
		LOG("Could not find Asset with id: " << id);
		return;
	}

	auto& [asset, ref_count] = found_asset->second;
	if (ref_count == 0)
	{
		asset->Initialize();
	}
	ref_count++;
}


void AssetManager::UnloadSceneAsset(std::string& guid)
{
	const STRCODE id = GetHashCode(guid.c_str());
	return UnloadSceneAsset(id);
}

void AssetManager::UnloadSceneAsset(const STRCODE id)
{
	const auto found_asset = assets.find(id);
	if (found_asset == assets.end())
	{
		LOG("Could not find Asset with id: " << id);
		return;
	}

	found_asset->second.ref_count--;
	if (found_asset->second.ref_count == 0)
	{
		RemoveAsset(id);
	}
}

Asset* AssetManager::GetAsset(std::string guid)
{
	STRCODE id = GetHashCode(guid.c_str());
	return GetAsset(id);
}

Asset* AssetManager::GetAsset(const STRCODE id)
{
	if (const auto found = assets.find(id); found != assets.end())
	{
		return found->second.asset;
	}

	LOG("Could not find Asset with id: " << id);
	return nullptr;
}

void AssetManager::RemoveAsset(std::string guid)
{
	const STRCODE id = GetHashCode(guid.c_str());
	RemoveAsset(id);
}

void AssetManager::RemoveAsset(const STRCODE id)
{
	if (const auto found = assets.find(id); found != assets.end())
	{
		found->second.asset->Destroy();
	}
	assets.erase(id);
}

void AssetManager::Load(const std::string& config_file)
{
	std::ifstream inputStream(config_file.c_str());
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON node = json::JSON::Load(str);

	if (node.hasKey("AssetDirectory"))
	{
		assetDirectory = node.at("AssetDirectory").ToString();
	}

	if (node.hasKey("RecursiveSearch"))
	{
		recursiveSearch = node.at("RecursiveSearch").ToBool();
	}
}

void AssetManager::Destroy()
{
	for (auto& [_, asset] : assets)
	{
		delete asset.asset;
		asset.ref_count = 0;
	}
	assets.clear();
}
