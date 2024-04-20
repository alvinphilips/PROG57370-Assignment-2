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
	
	inline static float asteroid_size_big = 40.0f;
	inline static float asteroid_size_small = 10.0f;

	static float GetSize(const AsteroidType type)
	{
		switch (type)
		{
		case AsteroidType::Big:
			return asteroid_size_big;
		case AsteroidType::Small:
			return asteroid_size_small;
		}

		return 0;
	}

	static Asteroid* CreateAsteroid(Scene* owner_scene, const AsteroidType type, const Vec2 position)
	{
		const auto asteroid_entity = owner_scene->CreateEntity();
		asteroid_entity->GetTransform().position = position;
		const auto asteroid = asteroid_entity->CreateComponent<Asteroid>();
		asteroid->radius = GetSize(type);
		asteroid->velocity = Vec2(0, 10);
		asteroid->start_time = Time::Instance().TotalTimeRaw();
		asteroid->start_position = position;

		return asteroid;
	}
};