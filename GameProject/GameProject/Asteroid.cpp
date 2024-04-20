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

	Debug::DrawCircle(GetTransform().position, radius, Color(150, 75, 0));

	Transform& transform = GetTransform();

	transform.position = start_position + velocity * (Time::Instance().current_server_time - start_time);

	if (!NetworkEngine::Instance().IsServer()) return;

	if (is_out_of_bounds(GetTransform().position, RenderSystem::Instance().GetWindowSize()))
	{
		owner->Dispose();
		return;
	}

	for (const auto other : collider->OnCollisionEnter())
	{
		if (other->GetOwner()->GetName() == "Player")
		{
			owner->Dispose();
		}
	}
}

void Asteroid::SerializeCreate(RakNet::BitStream& bitStream) const
{
	Component::SerializeCreate(bitStream);
	bitStream.Write(start_position.x);
	bitStream.Write(start_position.y);
	bitStream.Write(velocity.x);
	bitStream.Write(velocity.y);
	bitStream.Write(start_time);
	bitStream.Write(radius);
}

void Asteroid::DeserializeCreate(RakNet::BitStream& bitStream)
{
	Component::DeserializeCreate(bitStream);
	bitStream.Read(start_position.x);
	bitStream.Read(start_position.y);
	bitStream.Read(velocity.x);
	bitStream.Read(velocity.y);
	bitStream.Read(start_time);
	bitStream.Read(radius);
}