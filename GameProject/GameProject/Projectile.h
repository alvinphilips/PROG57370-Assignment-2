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
private:
    CircleCollider* collider = nullptr;
    float sync_timer = 0;
    float sync_delay = 1;
private:
    void RpcDestroy(RakNet::BitStream& bitStream);
    void RpcUpdatePosition(RakNet::BitStream& bitStream);
};

#endif
