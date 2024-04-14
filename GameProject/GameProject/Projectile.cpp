#include "GameCore.h"
#include "Projectile.h"
#include "Sprite.h"
#include "TextureAsset.h"
#include "BoxCollider.h"

#define NDEBUG_PLAYER

IMPLEMENT_DYNAMIC_CLASS(Projectile)

void Projectile::Initialize()
{
	Component::Initialize();

	
	RegisterRPC(GetHashCode("RpcUpdatePosition"), std::bind(&Projectile::RpcUpdatePosition, this, std::placeholders::_1));
}

void Projectile::Update()
{
	Component::Update();

	this->owner->GetTransform().position += velocity;
}

void Projectile::RpcDestroy(RakNet::BitStream& bitStream)
{
}

void Projectile::RpcUpdatePosition(RakNet::BitStream& bitStream)
{
}
