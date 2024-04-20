#include "GameCore.h"
#include "GameManager.h"

IMPLEMENT_DYNAMIC_CLASS(GameManager)

void GameManager::Initialize()
{
	Component::Initialize();

	if (!NetworkEngine::Instance().IsServer()) return;

	NetworkEngine::Instance().on_player_joined.AddListener([]
	{
		if (NetworkEngine::Instance().AreWeGameYet())
		{
			
		}
	});
}
