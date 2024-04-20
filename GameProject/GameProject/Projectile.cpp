#include "GameCore.h"
#include "Projectile.h"
#include "Sprite.h"
#include "TextureAsset.h"
#include "BoxCollider.h"

#define NDEBUG_PLAYER

using namespace gfx;

IMPLEMENT_DYNAMIC_CLASS(Projectile)

bool is_out_of_bounds(Vec2 position, IVec2 window_size) {
	return position.x < 0 || position.x > window_size.x || position.y < 0 || position.y > window_size.y;
}

void Projectile::Initialize()
{
	Component::Initialize();

	RegisterRPC(GetHashCode("RpcUpdatePosition"), std::bind(&Projectile::RpcUpdatePosition, this, std::placeholders::_1));
}

void Projectile::Update()
{
	Component::Update();

	Debug::DrawCircle(GetTransform().position, 10);

	

	// if (!NetworkEngine::Instance().IsServer()) return;

	Transform& transform = owner->GetTransform();

	transform.position = start_position + velocity * (Time::Instance().current_server_time - start_time);

	if (!NetworkEngine::Instance().IsServer()) return;

	if (is_out_of_bounds(GetTransform().position, RenderSystem::Instance().GetWindowSize()))
	{
		owner->Dispose();
	}
}

void Projectile::SerializeCreate(RakNet::BitStream& bitStream) const
{
	Component::SerializeCreate(bitStream);
	bitStream.Write(start_position.x);
	bitStream.Write(start_position.y);
	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);
	bitStream.Write(start_time);
}

void Projectile::DeserializeCreate(RakNet::BitStream& bitStream)
{
	Component::DeserializeCreate(bitStream);
	bitStream.Read(start_position.x);
	bitStream.Read(start_position.y);
	bitStream.Read(velocity.x);
	bitStream.Read(velocity.y);
	bitStream.Read(start_time);
	Time::Instance().current_server_time = start_time;
}

void Projectile::RpcUpdatePosition(RakNet::BitStream& bitStream)
{
	Transform& transform = owner->GetTransform();
	float value;
	bitStream.Read(value);
	transform.position.x = value;
	bitStream.Read(value);
	transform.position.y = value;
}
