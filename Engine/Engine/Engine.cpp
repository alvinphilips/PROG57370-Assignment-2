#include "EngineCore.h"
#include "Engine.h"
#include "EngineTime.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "Entity.h"
#include "InputSystem.h"

#include <thread>

#include "AssetManager.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "Component.h"

#include "NetworkEngine.h"

extern void Engine_Register();

void Engine::Initialize()
{
	Engine_Register();

	int serverClientChoice = -1;
	std::cout << "Server [0] or Client [1]: ";
	std::cin >> serverClientChoice;
	bool isServer = serverClientChoice == 0;
	NetworkEngine::Instance().Initialize(isServer);

	// Load the managers
	AudioSystem::Instance().Load("../Assets/AudioSystem.json");
	AssetManager::Instance().Load("../Assets/AssetManager.json");

	AudioSystem::Instance().Initialize();
	AssetManager::Instance().Initialize();

	RenderSystem::Instance().Initialize(!isServer);
	SceneManager::Instance().Load();

	SceneManager::Instance().Initialize();
	InputSystem::Instance().RegisterQuitEventHandler([this] {isRunning = false; });
}

void Engine::Destroy()
{
	Time::Instance().Destroy();
	CollisionSystem::Instance().Destroy();
	SceneManager::Instance().Destroy();
	AssetManager::Instance().Destroy();
	AudioSystem::Instance().Destroy();
	RenderSystem::Instance().Destroy();
}

void Engine::GameLoop()
{
	while (isRunning)
	{
		Time::Instance().Update();

		// --------------------- Network Update ---------------------
		NetworkEngine::Instance().PreUpdate();

		// --------------------- Pre-update Phase ---------------------
		SceneManager::Instance().PreUpdate();

		// --------------------- Input Phase ---------------------
		InputSystem::Instance().Update();

		// --------------------- Update Phase ---------------------
		SceneManager::Instance().Update();
		RenderSystem::Instance().Update();
		CollisionSystem::Instance().Update();

		// --------------------- Post-update Phase ---------------------
		SceneManager::Instance().PostUpdate();
		NetworkEngine::Instance().PostUpdate();
	}
}
