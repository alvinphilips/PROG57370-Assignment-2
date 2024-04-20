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

	if (is_out_of_bounds(GetTransform().position, RenderSystem::Instance().GetWindowSize()))
	{
		owner->Dispose();
		return;
	}

	// if (!NetworkEngine::Instance().IsServer()) return;

	sync_timer -= Time::Instance().DeltaTime();
	Transform& transform = owner->GetTransform();

	// transform.position += velocity * Time::Instance().DeltaTime();

	return;
	if (sync_timer <= 0)
	{
		RakNet::BitStream bs;
		bs.Write<unsigned char>(MSG_SCENE_MANAGER);
		bs.Write<unsigned char>(MSG_RPC);
		//write the scene id
		bs.Write(owner->GetParentScene()->GetUid());
		// Write the entity id
		bs.Write(owner->GetUid());
		//write this id
		bs.Write(GetUid());
		bs.Write(GetHashCode("RpcUpdatePosition"));
		bs.Write(transform.position.x);
		bs.Write(transform.position.y);

		NetworkEngine::Instance().SendPacket(bs);
		sync_timer = sync_delay;
	}
}

void Projectile::SerializeCreate(RakNet::BitStream& bitStream) const
{
	Component::SerializeCreate(bitStream);
	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);
}

void Projectile::DeserializeCreate(RakNet::BitStream& bitStream)
{
	Component::DeserializeCreate(bitStream);
	bitStream.Read(velocity.x);
	bitStream.Read(velocity.y);
}

void Projectile::RpcDestroy(RakNet::BitStream& bitStream)
{
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
