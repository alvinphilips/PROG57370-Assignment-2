#pragma once

#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

#include"EngineCore.h"
#include"Collider.h"
#include"SDL.h"

class BoxCollider : public Collider
{
	DECLARE_DYNAMIC_DERIVED_CLASS(BoxCollider, Collider)
public:
	void SetSize(int width, int height) const;
	SDL_Rect GetBounds() const;
	ColliderType GetType() const override { return ColliderType::Box; };
	float GetBroadPhaseRadius() const override;
	bool HandleCollision(Collider* other) override { return false; };
};


#endif
