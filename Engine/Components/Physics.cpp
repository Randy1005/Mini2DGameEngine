#include "Components/Physics.h"

std::vector<PhysicsItem*> Physics::allPhysicsItems;

void Physics::AddPhysicsItem(WeakPtr<GameObject>& i_gameObject, bool i_enableGravity, bool i_enableCollision, AABB& i_boundingBox, const Vector2& i_velocity, const Vector2& i_force, float i_mass, float i_dragCoefficient) {
	allPhysicsItems.push_back(new PhysicsItem(i_gameObject, i_enableGravity, i_enableCollision, i_boundingBox, i_velocity, i_force, i_mass, i_dragCoefficient));
}



void Physics::RunAll(float i_dt) {
	const size_t size = allPhysicsItems.size();
	for (auto& physicsItem : Physics::allPhysicsItems) {
		Physics::Tick(physicsItem, physicsItem->force, i_dt);
		if (physicsItem->enableGravity)
			Physics::Tick(physicsItem, Vector2(0.0f, -.1f), i_dt);
	}
}

void Physics::Tick(PhysicsItem* i_PhysicsItem, const Vector2& i_force, float i_dt) {
	StrongPtr<GameObject> tmpOwnerPtr = i_PhysicsItem->pGameObject.CreateOwner();

	if (tmpOwnerPtr) {
		Vector2 accerleration = i_force / i_PhysicsItem->mass;

		Vector2 velocityBeforeUpdate = i_PhysicsItem->velocity;
		i_PhysicsItem->velocity += accerleration * i_dt;

		// update position with mid-point method
		tmpOwnerPtr->SetPosition(Vector2(tmpOwnerPtr->GetPosition() + ((velocityBeforeUpdate + i_PhysicsItem->velocity) / 2.0f) * i_dt));

	}
	else {
		// remove physics item
	}

}

void Physics::CleanUp() {
	for (auto& physicsItem : allPhysicsItems) {
		if (physicsItem->pGameObject) {
			delete physicsItem;
		}
	}
}
