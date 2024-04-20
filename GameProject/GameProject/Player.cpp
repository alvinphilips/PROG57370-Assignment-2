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

	if (!NetworkEngine::Instance().IsServer())
	{
		movement = Vec2::Zero;

		if (input.IsKeyPressed(SDLK_KP_ENTER) && networkedEntity == nullptr)
		{
			networkedEntity = SceneManager::Instance().CreateEntity();
			Sprite* sprite = networkedEntity->CreateComponent<Sprite>();
			TextureAsset* asset = AssetManager::Instance().GetAsset<TextureAsset>("Explosion_435e0fce-7b11-409c-858e-af4bd7fe99c0");
			sprite->SetTextureAsset(asset);
		}

		// Handle horizontal movement
		if (input.IsKeyPressed(SDLK_LEFT) || input.IsKeyPressed(SDLK_a) || input.IsGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
			movement.x -= 1;
			if (networkedEntity) networkedEntity->GetTransform().position.x -= 1;
		}
		if (input.IsKeyPressed(SDLK_RIGHT) || input.IsKeyPressed(SDLK_d) || input.IsGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
			movement.x += 1;
			if (networkedEntity) networkedEntity->GetTransform().position.x += 1;
		}

		// Handle vertical movement
		if (input.IsKeyPressed(SDLK_UP) || input.IsKeyPressed(SDLK_w) || input.IsGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
			movement.y -= 1;
			if (networkedEntity) networkedEntity->GetTransform().position.y -= 1;
		}
		if (input.IsKeyPressed(SDLK_DOWN) || input.IsKeyPressed(SDLK_s) || input.IsGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
			movement.y += 1;
			if (networkedEntity) networkedEntity->GetTransform().position.y += 1;
		}

		// Handle gamepad analog stick input
		if (movement == Vec2::Zero) {
			movement.x = input.GetGamepadAxisState(0, SDL_CONTROLLER_AXIS_LEFTX);
			movement.y = input.GetGamepadAxisState(0, SDL_CONTROLLER_AXIS_LEFTY);
		}

		// Normalize the direction vector if it's not zero
		if (movement != Vec2::Zero) {
			movement.Normalize();
		}

#ifdef DEBUG_PLAYER
		LOG("Input: " << dir.x << ", " << dir.y);
#endif
	}

	//if (NetworkEngine::Instance().IsClient())
	//{
	//	if (movement != Vec2::Zero)
	//	{
	//		RakNet::BitStream bitStream;

	//		bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
	//		bitStream.Write((unsigned char)MSG_RPC);

	//		//write the scene id
	//		bitStream.Write(owner->GetParentScene()->GetUid());
	//		// Write the entity id
	//		bitStream.Write(owner->GetUid());
	//		//write this id
	//		bitStream.Write(GetUid());
	//		bitStream.Write(GetHashCode("RPC"));

	//		bitStream.Write(movement.x);
	//		bitStream.Write(movement.y);

	//		NetworkEngine::Instance().SendPacket(bitStream);
	//	}
	//	return;
	//}

	// Move the player
	if (movement != Vec2::Zero)
	{
		if (networkedEntity)
		{
			networkedEntity->GetTransform().position += movement * (speed * Time::Instance().DeltaTime());
		}
		owner->GetTransform().position += movement * (speed * Time::Instance().DeltaTime());

		if (collider == nullptr)
		{
			LOG("no collider uwu");
			return;
		}
		for (const auto& other : collider->OnCollisionEnter())
		{
			if (other->GetOwner()->GetName() != "Enemy")
			{
				continue;
			}

			Scene* current_scene = SceneManager::Instance().GetActiveScene();
			if (SceneManager::Instance().SetActiveScene(game_over_scene))
			{
				current_scene->SetEnabled(false);
			}

			owner->GetTransform().position = start_pos;
		}
		movement = Vec2::Zero;
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
	const auto rb = entity->CreateComponent<Rigidbody>();
	entity->CreateComponent<NetworkTransform>();

	Vec2 dir = spawn_target - spawn_position;
	dir.Normalize();
	rb->velocity = dir * 50.0f;

	RakNet::BitStream  bs;
	bs.Write<unsigned char>(MSG_SCENE_MANAGER);
	bs.Write<unsigned char>(MSG_CREATE_ENTITY);
	scene->SerializeCreateEntity(entity, bs);
	NetworkEngine::Instance().SendPacket(bs);
}