#include "GameCore.h"
#include "AsteroidSpawner.h"
#include "AsteroidFactory.h"

IMPLEMENT_DYNAMIC_CLASS(AsteroidSpawner)

void AsteroidSpawner::Initialize()
{
	Component::Initialize();
}

void AsteroidSpawner::Update()
{
	Component::Update();

	if (NetworkEngine::Instance().IsClient()) return;

	spawn_timer -= Time::Instance().DeltaTime();

	const auto random_position = Vec2((float) (rand() % RenderSystem::Instance().GetWindowSize().x), spawn_position_y);

	const float random_float = (float) rand() / (float) RAND_MAX;

	const auto asteroid_type = random_float < large_asteroid_rate ? AsteroidFactory::AsteroidType::Big : AsteroidFactory::AsteroidType::Small;

	if (spawn_timer <= 0)
	{
		AsteroidFactory::CreateAsteroid(GetScene(), asteroid_type, random_position);
		spawn_timer = spawn_delay;
	}
}
