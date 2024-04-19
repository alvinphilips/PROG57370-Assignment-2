#pragma once

#ifndef ASTEROID_SPAWNER_H
#define ASTEROID_SPAWNER_H

#include "GameCore.h"

class AsteroidSpawner : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(AsteroidSpawner, Component)

public:
    void Initialize() override;
    void Update() override;
private:
    float spawn_timer = 0;
    float spawn_delay = 4;
    float large_asteroid_rate = 0.3f;
    float spawn_position_y = -2;
};


#endif
