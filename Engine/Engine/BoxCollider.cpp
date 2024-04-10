#include "EngineCore.h"
#include "BoxCollider.h"

IMPLEMENT_DYNAMIC_CLASS(BoxCollider);

void BoxCollider::SetSize(int width, int height) const
{
#ifndef STRIP_NULL_CHECKS
	if (rect == nullptr) 
	{
		return;
	}
#endif
	*rect = { static_cast<int>(owner->GetTransform().position.x), static_cast<int>(owner->GetTransform().position.y), width, height };
}

SDL_Rect BoxCollider::GetBounds() const
{
#ifndef STRIP_NULL_CHECKS
	if (rect == nullptr) 
	{
		return { 0, 0, 0, 0 };
	}
#endif
	return *rect;
}

float BoxCollider::GetBroadPhaseRadius() const 
{
#ifndef STRIP_NULL_CHECKS
	if (rect == nullptr) 
	{
		return 0.0f;
	}
#endif
	return Vec2(static_cast<float>(rect->w), static_cast<float>(rect->h)).Magnitude();
}