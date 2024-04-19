#pragma once

#include "GameCore.h"

class AsteroidFactory
{
public:
	enum class AsteroidType
	{
		Big,
		Small,
	};

	static Asteroid* CreateAsteroid(Scene* owner_scene, const AsteroidType type, const Vec2 position)
	{
		const auto asteroid_entity = owner_scene->CreateEntity();
		asteroid_entity->GetTransform().position = position;
		const auto asteroid = asteroid_entity->CreateComponent<Asteroid>();
		asteroid->radius = type == AsteroidType::Big ? 40 : 10;
		asteroid->velocity = Vec2(0, 10);

		return asteroid;
	}
};