#pragma once
#include "GameObject/GameObject.h"
#include "Dynamics/Matrix4x4.h"
#include "Components/Physics.h"
#include "PerformanceProfiling/PerformanceProfiler.h"



struct Collideable {
	WeakPtr<GameObject> pGameObject;
	PhysicsItem* pPhysicsItem;
	AABB m_BoundingBox;

	Collideable(WeakPtr<GameObject>& i_gameObject, PhysicsItem* i_physicsItem, AABB& i_BoundingBox) :
		pGameObject(i_gameObject),
		pPhysicsItem(i_physicsItem),
		m_BoundingBox(i_BoundingBox)
	{

	}

};

struct Collision {
	Vector2 m_collisionNormal;
	Collideable* m_collideableObject;

	Collision() :
		m_collisionNormal(Vector2::Zero),
		m_collideableObject(nullptr)
	{

	}


	Collision(Vector2& i_collisionNormal, Collideable* i_collideableObjects) :
		m_collisionNormal(i_collisionNormal),
		m_collideableObject(i_collideableObjects)
	{

	}
};

struct CollisionPair {
	float m_collisionTime;
	Vector2 m_collisionNormal;
	std::pair<Collision*, Collision*> m_collisions;

	CollisionPair(float i_collisionTime, Collision* i_collision1, Collision* i_collision2) :
		m_collisionTime(i_collisionTime),
		m_collisions(std::make_pair(i_collision1, i_collision2))
	{

	}

	~CollisionPair() {
		delete m_collisions.first;
		delete m_collisions.second;
	}
};

namespace CollidingSystem {


	// grab the physicsItems if they have collision enabled
	void ImportPhysicsItems();

	void AddCollideable(WeakPtr<GameObject>& i_gameObject, PhysicsItem* i_physicsItem, AABB& i_boundingBox);

	// do the seperating axis check on world coordinate
	bool SeparatingAxisCheck(Collideable& collideableA, Collideable& collideableB, float i_dt, float o_collisionTime, Vector2& o_collisionNormal);

	// only does debug print for now
	void CheckCollision(float i_dt);

	void GetEarliestCollision(float i_dt);

	void ResolveCollisions(float i_dt);

	void ResolveCollision(CollisionPair& i_collisionPair);

	Vector2 Reflect(Vector2& vectorToReflect, Vector2& normal);

	void AdvanceSimulation(float i_dt);

	void CleanUp();


}
