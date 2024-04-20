#pragma once

#ifndef NETWORK_TRANSFORM_H_
#define NETWORK_TRANSFORM_H_

#include "Component.h"
#include "Transform.h"

class Rigidbody;

class NetworkTransform final : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(NetworkTransform, Component);
public:
    void Serialize(RakNet::BitStream& bitStream) const override;
    void Deserialize(RakNet::BitStream& bitStream) override;

    void Initialize() override;
    void Update() override;

    Rigidbody* rb = nullptr;
    Vec2 server_position;
    Vec2 server_velocity;
    Vec2 data; // For the server, this is: server sync delay | server sync timer

    void SendSyncRpc() const;
    void RpcSyncNetworkTransform(RakNet::BitStream& bitStream);
};

#endif