#pragma once

#ifndef ASTEROID_H
#define ASTEROID_H

#include "GameCore.h"

class CircleCollider;

class Asteroid : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Asteroid, Component)

public:
    void Initialize() override;
    void Update() override;
    void SerializeCreate(RakNet::BitStream& bitStream) const override;
    void DeserializeCreate(RakNet::BitStream& bitStream) override;
    Vec2 velocity;
    float radius = 20;
private:
    CircleCollider* collider = nullptr;
    Vec2 start_position;
    float start_time = 0;

    friend class AsteroidFactory;
    friend class AsteroidSpawner;
};

#endif
