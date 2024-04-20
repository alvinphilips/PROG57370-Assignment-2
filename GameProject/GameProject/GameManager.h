#pragma once
#pragma once

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameCore.h"

class GameManager final : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(GameManager, Component)

public:
    void Initialize() override;
    void Update() override;
    void SerializeCreate(RakNet::BitStream& bitStream) const override;
    void DeserializeCreate(RakNet::BitStream& bitStream) override;
    Vec2 velocity;
};

#endif
