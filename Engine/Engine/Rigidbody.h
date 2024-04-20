#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "Transform.h"

class Rigidbody final : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Rigidbody, Component)
public:
	void SerializeCreate(RakNet::BitStream& bitStream) const override;
	void DeserializeCreate(RakNet::BitStream& bitStream) override;
private:
	void Update() override;
    void Serialize(RakNet::BitStream& bitStream) const override;
    void Deserialize(RakNet::BitStream& bitStream) override;
public:
	bool is_kinematic = false;
	Vec2 velocity;
};

#endif