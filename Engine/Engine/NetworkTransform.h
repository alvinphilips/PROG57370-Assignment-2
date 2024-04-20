#pragma once

#ifndef NETWORK_TRANSFORM_H_
#define NETWORK_TRANSFORM_H_

#include "Component.h"

class NetworkTransform final : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(NetworkTransform, Component);
public:
    Vec2 prev_position;
    Vec2 prev_velocity;
    Vec2 target_position;
    Vec2 target_velocity;
};

#endif