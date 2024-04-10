#include "EngineCore.h"
#include "Asset.h"

IMPLEMENT_ABSTRACT_CLASS(Asset)

void Asset::Load(json::JSON& json)
{
	Object::Load(json);
	name = filepath;
#ifdef DEBUG_ASSET_LOAD
	LOG("Loading Asset: " + name);
#endif
}
