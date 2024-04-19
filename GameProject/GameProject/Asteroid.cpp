#include "GameCore.h"
#include "Asteroid.h"

#include "CircleCollider.h"

#define NDEBUG_PLAYER

using namespace gfx;

IMPLEMENT_DYNAMIC_CLASS(Asteroid)

static bool is_out_of_bounds(Vec2 position, IVec2 window_size) {
	return position.x < 0 || position.x > window_size.x || position.y > window_size.y;
}

void Asteroid::Initialize()
{
	Component::Initialize();
	collider = owner->CreateComponent<CircleCollider>();
	collider->SetRadius(radius);
}

void Asteroid::Update()
{
	Component::Update();

	Debug::DrawCircle(GetTransform().position, radius, to_be_destroyed ? Color::RED : Color(150, 75, 0));

	if (is_out_of_bounds(GetTransform().position, RenderSystem::Instance().GetWindowSize()))
	{
		owner->Dispose();
		return;
	}

	Transform& transform = owner->GetTransform();

	transform.position += velocity * Time::Instance().DeltaTime();

	if (!NetworkEngine::Instance().IsServer()) return;

	for (const auto other : collider->OnCollisionEnter())
	{
		if (other->GetOwner()->GetName() == "Player")
		{
			LOG("ouch");
			to_be_destroyed = true;
		}
	}

	sync_timer -= Time::Instance().DeltaTime();

	if (sync_timer <= 0 && false)
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

void Asteroid::SerializeCreate(RakNet::BitStream& bitStream) const
{
	Component::SerializeCreate(bitStream);
	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);
	bitStream.Write(radius);
}

void Asteroid::DeserializeCreate(RakNet::BitStream& bitStream)
{
	Component::DeserializeCreate(bitStream);
	bitStream.Read(velocity.x);
	bitStream.Read(velocity.y);
	bitStream.Read(radius);
}