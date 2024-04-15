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
    float sync_timer = 0;
    float sync_delay = 5;
private:
    void RpcDestroy(RakNet::BitStream& bitStream);
    void RpcUpdatePosition(RakNet::BitStream& bitStream);
};

#endif
