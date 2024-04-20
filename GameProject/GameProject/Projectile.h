#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameCore.h"

class CircleCollider;

class Projectile : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Projectile, Component)

public:
    void Initialize() override;
    void Update() override;
    void SerializeCreate(RakNet::BitStream& bitStream) const override;
    void DeserializeCreate(RakNet::BitStream& bitStream) override;
    Vec2 velocity;
    CircleCollider* collider = nullptr;
    Vec2 start_position;
    float start_time = 0;
private:
    void RpcUpdatePosition(RakNet::BitStream& bitStream);
};

#endif
