#pragma once

#ifndef _COLLISION_SYSTEM_H_
#define _COLLISION_SYSTEM_H_

#include "EngineCore.h"
#include "SDL.h"

#include <utility>
#include <set>

class Collider;
class CircleCollider;
class BoxCollider;

class CollisionSystem final
{
	DECLARE_SINGLETON(CollisionSystem)

public:
	void AddCollider(Collider*);
	void RemoveCollider(Collider*);
	std::list<std::pair<Collider*,Collider*>> BroadPhaseDetection();
	std::set<std::pair<Collider*, Collider*>> NarrowPhaseDetection(const std::list<std::pair<Collider*, Collider*>>& potentialCollisions);
	void ResolveCollision(Collider*, Collider*);

private:
	void Initialize();
	void Update();
	void Destroy();

	bool CircleCircleCollision(Collider*, Collider*);
	bool BoxBoxCollision(Collider*, Collider*);
	bool CircleBoxCollision(Collider*, Collider*);

	std::list<std::pair<Collider*, Collider*>> enterCollisions;
	std::list<std::pair<Collider*, Collider*>> stayCollisions;
	std::list<std::pair<Collider*, Collider*>> exitCollisions;

private:
	std::list<Collider*> colliders;
	std::set<std::pair<Collider*, Collider*>> ongoingCollisions;

	static CollisionSystem* instance;
	std::list<int> idList;
	int idCount = 0;

	friend class Engine;
	friend class Collider;
	friend class BoxCollider;
	friend class CircleCollider;
};

#endif