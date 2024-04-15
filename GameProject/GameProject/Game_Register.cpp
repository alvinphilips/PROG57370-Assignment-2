#include "GameCore.h"

void GameRegisterClasses()
{
	REGISTER_TYPECLASS(Player);
	REGISTER_TYPECLASS(GameOver);
	REGISTER_TYPECLASS(Projectile);
	REGISTER_TYPECLASS(Asteroid);
}