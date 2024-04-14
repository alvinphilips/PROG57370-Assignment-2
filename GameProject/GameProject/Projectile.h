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
private:
    Vec2 velocity;
    CircleCollider* collider = nullptr;
private:
    void RpcDestroy(RakNet::BitStream& bitStream);
    void RpcUpdatePosition(RakNet::BitStream& bitStream);
};

#endif
