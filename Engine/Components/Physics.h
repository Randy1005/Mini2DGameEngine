#pragma once
#include "GameObject/GameObject.h"
#include "Console/ConsolePrint.h"

struct AABB {
	Vector2 Center;
	Vector2 Extents;

	AABB(Vector2 i_center, Vector2 i_extents) :
		Center(i_center),
		Extents(i_extents)
	{

	}
};

struct PhysicsItem {
	WeakPtr<GameObject> pGameObject;
	bool enableGravity;
	bool enableCollison;
	AABB boundingBox;
	Vector2 velocity;
	Vector2 force;
	float mass;
	float dragCoefficient;

	PhysicsItem(WeakPtr<GameObject>& i_gameObject, bool i_enableGravity, bool i_enableCollision, AABB& i_boundingBox, const Vector2& i_velocity, const Vector2& i_force, float i_mass, float i_dragCoefficient) :
		pGameObject(i_gameObject),
		enableGravity(i_enableGravity),
		enableCollison(i_enableCollision),
		boundingBox(i_boundingBox),
		velocity(i_velocity),
		force(i_force),
		mass(i_mass),
		dragCoefficient(i_dragCoefficient)
	{ }



	


};

class Physics {
public:
	static void AddPhysicsItem(WeakPtr<GameObject>& i_gameObject, bool i_enableGravity, bool i_enableCollision, AABB& i_boundingBox, const Vector2& i_velocity, const Vector2& i_force, float i_mass, float i_dragCoefficient);
	static void RunAll(float i_dt);
	static void Tick(PhysicsItem* i_PhysicsItem, const Vector2& i_force, float i_dt);
	static void CleanUp();

	static std::vector<PhysicsItem*> allPhysicsItems;

private:
	
};
