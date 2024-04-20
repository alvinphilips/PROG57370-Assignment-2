#include "EngineCore.h"
#include "NetworkTransform.h"

#include "NetworkEngine.h"
#include "Rigidbody.h"
#include "Scene.h"

IMPLEMENT_DYNAMIC_CLASS(NetworkTransform)

void NetworkTransform::Serialize(RakNet::BitStream& bitStream) const
{
	Component::Serialize(bitStream);
	bitStream.Write(server_position.x);
	bitStream.Write(server_position.y);
	bitStream.Write(server_velocity.x);
	bitStream.Write(server_velocity.y);
	bitStream.Write(data.x);
	bitStream.Write(data.y);
}

void NetworkTransform::Deserialize(RakNet::BitStream& bitStream)
{
	Component::Deserialize(bitStream);
	bitStream.Read(server_position.x);
	bitStream.Read(server_position.y);
	bitStream.Read(server_velocity.x);
	bitStream.Read(server_velocity.y);
	bitStream.Read(data.x);
	bitStream.Read(data.y);
}

void NetworkTransform::Initialize()
{
	Component::Initialize();

	rb = owner->GetComponent<Rigidbody>();
	RegisterRPC(GetHashCode("RpcSync"), std::bind(&NetworkTransform::RpcSyncNetworkTransform, this, std::placeholders::_1));
}

void NetworkTransform::Update()
{
	Component::Update();
	Transform& transform = GetTransform();

	data.y -= Time::Instance().DeltaTime();

	// Client-side prediction
	if (NetworkEngine::Instance().IsClient())
	{
		// This is where the server *should* be in x amount of time, assuming our velocity remains the same
		// new_position = server_position + server_velocity * x_time;

		// our position is currently whatever transform.position is, and our velocity is rigidbody.position,
		// we need to get to the same point
		// new_position = client_position + ?? * x_time

		if (data.y <= 0)
		{
			if (rb == nullptr) transform.position += server_velocity * Time::Instance().DeltaTime();
			return;
		}

		Vec2 velocity = (server_position - transform.position) / data.y + server_velocity;

		// TODO: are we trying to interpolate positions or velocity?

		if (rb)
		{
			rb->velocity = velocity;
		}
		else
		{
			transform.position += velocity * Time::Instance().DeltaTime();
		}
	}

	if (NetworkEngine::Instance().IsServer())
	{
		if (rb)
		{
			// Use Rigidbody velocity
			server_velocity = rb->velocity;
		}
		else
		{
			// Use delta-based velocity calculation
			server_velocity = (transform.position - server_position) / Time::Instance().DeltaTime();
			transform.position = server_position + server_velocity * Time::Instance().DeltaTime();
		}

		server_position = transform.position;

		if (data.y <= 0)
		{
			SendSyncRpc();
			data.y = data.x;
		}
	}
}

void NetworkTransform::SendSyncRpc() const
{
	if (!NetworkEngine::Instance().IsServer()) return;

	RakNet::BitStream bitStream;

	bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
	bitStream.Write((unsigned char)MSG_RPC);

	//write the scene id
	bitStream.Write(owner->GetParentScene()->GetUid());
	// Write the entity id
	bitStream.Write(owner->GetUid());
	//write this id
	bitStream.Write(GetUid());
	bitStream.Write(GetHashCode("RpcSync"));

	bitStream.Write(server_position.x);
	bitStream.Write(server_position.y);
	bitStream.Write(server_velocity.x);
	bitStream.Write(server_velocity.y);
	bitStream.Write(data.x);

	NetworkEngine::Instance().SendPacket(bitStream);
}

void NetworkTransform::RpcSyncNetworkTransform(RakNet::BitStream& bitStream)
{
	// Only sync NetworkTransforms on the client(s)
	if (NetworkEngine::Instance().IsServer()) return;

	// The time 'remaining', we use this to adjust the time our client takes to sync to the server's position
	const float network_latency = data.y;

	bitStream.Read(server_position.x);
	bitStream.Read(server_position.y);
	bitStream.Read(server_velocity.x);
	bitStream.Read(server_velocity.y);
	bitStream.Read(data.x);

	data.y = 0;

	data.x -= network_latency;
};

