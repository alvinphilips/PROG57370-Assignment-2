#include "EngineCore.h"
#include "Rigidbody.h"

IMPLEMENT_DYNAMIC_CLASS(Rigidbody)

void Rigidbody::Update()
{
	Component::Update();

	if (!is_kinematic)
	{
		GetTransform().position += velocity * Time::Instance().DeltaTime();
	}
}

void Rigidbody::Serialize(RakNet::BitStream& bitStream) const
{
	Component::Serialize(bitStream);
	bitStream.Write(is_kinematic);
	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);
}

void Rigidbody::Deserialize(RakNet::BitStream& bitStream)
{
	Component::Deserialize(bitStream);
	bitStream.Read(is_kinematic);
	bitStream.Read(velocity.x);
	bitStream.Read(velocity.y);
}

void Rigidbody::SerializeCreate(RakNet::BitStream& bitStream) const
{
	Component::SerializeCreate(bitStream);
	bitStream.Write(is_kinematic);
	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);
}

void Rigidbody::DeserializeCreate(RakNet::BitStream& bitStream)
{
	Component::DeserializeCreate(bitStream);
	bitStream.Read(is_kinematic);
	bitStream.Read(velocity.x);
	bitStream.Read(velocity.y);
}

