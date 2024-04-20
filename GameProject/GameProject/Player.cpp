#include "GameCore.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "TextureAsset.h"
#include "BoxCollider.h"
#include "DebugDraw.h"
#include "NetworkTransform.h"
#include "Rigidbody.h"

#define NDEBUG_PLAYER

IMPLEMENT_DYNAMIC_CLASS(Player)

using namespace gfx;

void Player::Initialize()
{
	Component::Initialize();
	start_pos = owner->GetTransform().position;
	collider = owner->GetComponent<BoxCollider>();
	owner->GetComponent<AnimatedSprite>()->SetFilterColor(0, 0, 0, 0);
	owner->CreateComponent<AsteroidSpawner>();

	GetTransform().position.x = RenderSystem::Instance().GetWindowSize().x * 0.5f;
	NetworkEngine::Instance().on_player_joined.AddListener([this]
		{
			auto pos_factor = 0.5f;
			LOG(NetworkEngine::Instance().GetFriendCount());
			LOG(NetworkEngine::Instance().GetMyIndex());
			if (NetworkEngine::Instance().GetFriendCount() > 1)
			{
				pos_factor = (float) NetworkEngine::Instance().GetMyIndex() / (float) NetworkEngine::Instance().GetFriendCount() - 1;
			}

			GetTransform().position.x = RenderSystem::Instance().GetWindowSize().x * pos_factor;
		});

	GetTransform().position.y = RenderSystem::Instance().GetWindowSize().y * 0.75f;
	RegisterRPC(GetHashCode("RPC"), std::bind(&Player::RPC, this, std::placeholders::_1));
}

void Player::Update()
{
	Debug::DrawCircle(GetTransform().position, 50, Color::MAGENTA, 3, MATH_PI);
	Debug::DrawCircle(GetTransform().position, 20, Color::WHITE, 5, MATH_PI);
	// Debug::DrawRect(GetTransform().position, Vec2(100));
	const InputSystem& input = InputSystem::Instance();

	fire_timer -= Time::Instance().DeltaTime();
	if (input.IsKeyPressed(SDLK_SPACE) && fire_timer <= 0)
	{
		RakNet::BitStream bitStream;

		bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
		bitStream.Write((unsigned char)MSG_RPC);

		//write the scene id
		bitStream.Write(owner->GetParentScene()->GetUid());
		// Write the entity id
		bitStream.Write(owner->GetUid());
		//write this id
		bitStream.Write(GetUid());
		bitStream.Write(GetHashCode("RPC"));

		const Vec2 spawn_position = GetTransform().position;
		const Vec2 spawn_target = input.MousePosition();

		bitStream.Write(spawn_position.x);
		bitStream.Write(spawn_position.y);

		bitStream.Write(spawn_target.x);
		bitStream.Write(spawn_target.y);

		NetworkEngine::Instance().SendPacket(bitStream);

		fire_timer = fire_delay;
	}
}

void Player::Load(json::JSON& node)
{
	Component::Load(node);
	if (node.hasKey("Speed"))
	{
		speed = static_cast<float>(node.at("Speed").ToFloat());
	}

	if (node.hasKey("DeathScene"))
	{
		game_over_scene = GetHashCode(node.at("DeathScene").ToString().c_str());
	}
}

void Player::RPC(RakNet::BitStream& bitStream)
{
	if (!NetworkEngine::Instance().IsServer()) return;

	Vec2 spawn_position;
	Vec2 spawn_target;

	bitStream.Read(spawn_position.x);
	bitStream.Read(spawn_position.y);

	bitStream.Read(spawn_target.x);
	bitStream.Read(spawn_target.y);

	const auto scene = owner->GetParentScene();
	const auto entity = scene->CreateEntity();
	entity->GetTransform().position = spawn_position;
	const auto projectile = entity->CreateComponent<Projectile>();

	Vec2 dir = spawn_target - spawn_position;
	dir.Normalize();
	projectile->velocity = dir * 50.0f;
	projectile->start_time = Time::Instance().TotalTimeRaw();
	projectile->start_position = spawn_position;

	RakNet::BitStream  bs;
	bs.Write<unsigned char>(MSG_SCENE_MANAGER);
	bs.Write<unsigned char>(MSG_CREATE_ENTITY);
	scene->SerializeCreateEntity(entity, bs);
	NetworkEngine::Instance().SendPacket(bs);
}