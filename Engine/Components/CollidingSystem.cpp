#include "Components/CollidingSystem.h"

std::vector<Collideable*> allCollideables;
CollisionPair earliestCollisionPair(0, new Collision(), new Collision());

void CollidingSystem::AddCollideable(WeakPtr<GameObject>& i_gameObject, PhysicsItem* i_physicsItem, AABB& i_boundingBox) {
	allCollideables.push_back(new Collideable(i_gameObject, i_physicsItem, i_boundingBox));
}

void CollidingSystem::ImportPhysicsItems() {
	PERFORMANCE_MARKER

	for (auto& physicsItem : Physics::allPhysicsItems) {
		if (physicsItem->enableCollison) {
			WeakPtr<GameObject> goObserver(physicsItem->pGameObject);

			AddCollideable(goObserver, physicsItem, physicsItem->boundingBox);
		}
	}
}

bool CollidingSystem::SeparatingAxisCheck(Collideable& collideableA, Collideable& collideableB, float i_dt, float o_collisionTime, Vector2& o_CollisionNormal) {
	StrongPtr<GameObject> gameObjectTmpOwnerA = collideableA.pGameObject.CreateOwner();
	StrongPtr<GameObject> gameObjectTmpOwnerB = collideableB.pGameObject.CreateOwner();

	// get world transform matrices for A, B
	Matrix4x4 translocalToWorldMat_A = Matrix4x4::CreateTranslation(
		gameObjectTmpOwnerA->GetPosition().X(),
		gameObjectTmpOwnerA->GetPosition().Y());

	Matrix4x4 translocalToWorldMat_B = Matrix4x4::CreateTranslation(
		gameObjectTmpOwnerB->GetPosition().X(),
		gameObjectTmpOwnerB->GetPosition().Y());

	// use 'no rotation' for now
	Matrix4x4 rotlocalToWorldMat_A = Matrix4x4::CreateRotation(0.0f);
	Matrix4x4 rotlocalToWorldMat_B = Matrix4x4::CreateRotation(0.0f);

	// calculate toWorld matrix
	Matrix4x4 localToWorldMat_A = Matrix4x4::Multiply(translocalToWorldMat_A, rotlocalToWorldMat_A);
	Matrix4x4 localToWorldMat_B = Matrix4x4::Multiply(translocalToWorldMat_B, rotlocalToWorldMat_B);

	// calculate toLocal matrix
	Matrix4x4 worldToLocalMat_A(localToWorldMat_A);
	Matrix4x4 worldToLocalMat_B(localToWorldMat_B);
	if (!worldToLocalMat_A.Invert()) DEBUG_PRINT("local to world matrix A not invertible");
	if (!worldToLocalMat_B.Invert()) DEBUG_PRINT("local to world matrix B not invertible");

	// transform boundingbox center into world space
	Vector4 localCenter_A(collideableA.m_BoundingBox.Center, 0.0f, 1.0f);
	Vector4 localCenter_B(collideableB.m_BoundingBox.Center, 0.0f, 1.0f);
	Vector4 worldCenter_A = Matrix4x4::Multiply(localToWorldMat_A, localCenter_A);
	Vector4 worldCenter_B = Matrix4x4::Multiply(localToWorldMat_B, localCenter_B);

	// transform boundingbox extents into world space
	Vector4 localExtents_A(collideableA.m_BoundingBox.Extents, 0.0f, 1.0f);
	Vector4 localExtents_B(collideableB.m_BoundingBox.Extents, 0.0f, 1.0f);
	Vector4 worldExtentsX_A = localToWorldMat_A.GetColumn(0).normalized() * collideableA.m_BoundingBox.Extents.X();
	Vector4 worldExtentsY_A = localToWorldMat_A.GetColumn(1).normalized() * collideableA.m_BoundingBox.Extents.Y();

	Vector4 worldExtentsX_B = localToWorldMat_B.GetColumn(0).normalized() * collideableB.m_BoundingBox.Extents.X();
	Vector4 worldExtentsY_B = localToWorldMat_B.GetColumn(1).normalized() * collideableB.m_BoundingBox.Extents.Y();


	// now we have all the stuff in world space, we check if each axis has A, B overlapped
	Vector4 worldX(1, 0, 0, 0);
	Vector4 worldY(0, 1, 0, 0);

	// project A, B onto world x axis
	float centerOnX_A = Vector4::dot(worldX, worldCenter_A);
	float extentsAlongX_A =
		fabs(Vector4::dot(worldX, worldExtentsX_A)) + fabs(Vector4::dot(worldX, worldExtentsY_A));

	float centerOnX_B = Vector4::dot(worldX, worldCenter_B);
	float extentsAlongX_B =
		fabs(Vector4::dot(worldX, worldExtentsX_A)) + fabs(Vector4::dot(worldX, worldExtentsY_A));

	// check overlap on x axis
	bool worldOverlapX = fabs(centerOnX_A - centerOnX_B) < (extentsAlongX_A + extentsAlongX_B);

	// project A, B onto world y axis
	float centerOnY_A = Vector4::dot(worldY, worldCenter_A);
	float extentsAlongY_A =
		fabs(Vector4::dot(worldY, worldExtentsX_A)) + fabs(Vector4::dot(worldY, worldExtentsY_A));

	float centerOnY_B = Vector4::dot(worldY, worldCenter_B);
	float extentsAlongY_B =
		fabs(Vector4::dot(worldY, worldExtentsY_A)) + fabs(Vector4::dot(worldY, worldExtentsY_A));

	// check overlap on y axis
	bool worldOverlapY = fabs(centerOnY_A - centerOnY_B) < (extentsAlongY_A + extentsAlongY_B);


	// if already overlapped on 2 axes, then it's certain these 2 collideable collided
	if (worldOverlapX && worldOverlapY) {
		// calculate collision normals
		// get the projected length of (center_A - center_B), signed

		// on x axis
		float projectedLength_X = centerOnX_A - centerOnX_B;

		// on y axis
		float projectedLength_Y = centerOnY_A - centerOnY_B;

		// we pick the longer one to use as collision normal
		if (fabs(projectedLength_X) >= fabs(projectedLength_Y)) {
			Vector4 collisionNormal = (projectedLength_X * worldX).normalized();
			o_CollisionNormal = Vector2(collisionNormal.X(), collisionNormal.Y());
		}
		else {
			Vector4 collisionNormal = (projectedLength_Y * worldY).normalized();
			o_CollisionNormal = Vector2(collisionNormal.X(), collisionNormal.Y());
		}


		return true;
	}
	else {

		// if not already overlapped on 2 axes, then it's not necessarily 'no collision'
		// we need to go into frames and check

		// calculate edges on x and y axis
		float rightEdgeX_A = centerOnX_A + extentsAlongX_A;
		float leftEdgeX_A = centerOnX_A - extentsAlongX_A;
		float rightEdgeX_B = centerOnX_B + extentsAlongX_B;
		float leftEdgeX_B = centerOnX_B - extentsAlongX_B;

		float rightEdgeY_A = centerOnY_A + extentsAlongY_A;
		float leftEdgeY_A = centerOnY_A - extentsAlongY_A;
		float rightEdgeY_B = centerOnY_B + extentsAlongY_B;
		float leftEdgeY_B = centerOnY_B - extentsAlongY_B;


		// get relative velocity of A to B first
		Vector2 relativeVelocity_AB = gameObjectTmpOwnerA->GetVelocity() - gameObjectTmpOwnerB->GetVelocity();

		// calculate time to open gap and close gap along x axis
		float timeOpenX = static_cast<float>(rightEdgeX_B - leftEdgeX_A) / relativeVelocity_AB.X();
		float timeCloseX = static_cast<float>(leftEdgeX_B = rightEdgeX_A) / relativeVelocity_AB.X();

		// swap them if t_open is earlier than t_close (this means A is coming from right of B)
		if (timeOpenX < timeCloseX) {
			float tmp = timeOpenX;
			timeOpenX = timeCloseX;
			timeCloseX = tmp;
		}

		// calculate time to open gap and close gap along y axis
		float timeOpenY = static_cast<float>(rightEdgeY_B - leftEdgeY_A) / relativeVelocity_AB.Y();
		float timeCloseY = static_cast<float>(leftEdgeY_B = rightEdgeY_A) / relativeVelocity_AB.Y();

		// swap them if t_open is earlier than t_close (this means A is coming from right of B)
		if (timeOpenY < timeCloseY) {
			float tmp = timeOpenY;
			timeOpenY = timeCloseY;
			timeCloseY = tmp;
		}

		// DEBUG_PRINT("gap open / close: %f, %f, %f, %f", timeOpenX, timeOpenY, timeCloseX, timeCloseY);


		float endFrameTime = i_dt;
		// clamp these values to endframe or 0
		timeOpenX = (timeOpenX > endFrameTime) ? endFrameTime : (timeOpenX < 0.0f) ? 0.0f : timeOpenX;
		timeOpenY = (timeOpenY > endFrameTime) ? endFrameTime : (timeOpenY < 0.0f) ? 0.0f : timeOpenY;
		timeCloseX = (timeCloseX > endFrameTime) ? endFrameTime : (timeCloseX < 0.0f) ? 0.0f : timeCloseX;
		timeCloseY = (timeCloseY > endFrameTime) ? endFrameTime : (timeCloseY < 0.0f) ? 0.0f : timeCloseY;


		// check for extreme cases: guaranteed no collision
		// t_open == 0 || t_close == endFrame
		// !overlapOnX && relativeVelocity.x == 0
		// !overlapOnY && relativeVelocity,y == 0
		if (timeOpenX == 0 || timeOpenY == 0 || timeCloseX == endFrameTime || timeCloseY == endFrameTime)
			return false;

		if (!worldOverlapX && Math::isZero(relativeVelocity_AB.X()))
			return false;

		if (!worldOverlapY && Math::isZero(relativeVelocity_AB.Y()))
			return false;


		// get latest t_close and earliest t_open
		// if earliest t_open < latest t_close, a new separtion opened before the last closed, not collided
		// if earliest t_open > latest t_close, all seperation closed before new gap opend, collided
		float earliestTimeOpen = min(timeOpenX, timeOpenY);
		float latestTimeClose = max(timeCloseX, timeCloseY);

		if (earliestTimeOpen < latestTimeClose) return false;
		else {


			// calculate collision normals
			// get the projected length of (center_A - center_B), signed

			// on x axis
			float projectedLength_X = centerOnX_A - centerOnX_B;

			// on y axis
			float projectedLength_Y = centerOnY_A - centerOnY_B;

			// we pick the longer one to use as collision normal
			if (fabs(projectedLength_X) >= fabs(projectedLength_Y)) {
				Vector4 collisionNormal = (projectedLength_X * worldX).normalized();
				o_CollisionNormal = Vector2(collisionNormal.X(), collisionNormal.Y());
			}
			else {
				Vector4 collisionNormal = (projectedLength_Y * worldY).normalized();
				o_CollisionNormal = Vector2(collisionNormal.X(), collisionNormal.Y());
			}

			// output collision time in this frame
			o_collisionTime = latestTimeClose;

			return true;
		}

	}

}

// this is just a debug spew method, not really useful later for Collision Responses
void CollidingSystem::CheckCollision(float i_dt) {
	const size_t collideableCount = allCollideables.size();

	for (size_t i = 0; i < collideableCount - 1; i++) {
		for (size_t j = i + 1; j < collideableCount; j++) {

			Vector2 collisionNormal;
			float collisionTime = i_dt;

			if (SeparatingAxisCheck(*allCollideables[i], *allCollideables[j], i_dt, collisionTime, collisionNormal)) {
				DEBUG_PRINT("!!!!! COLLIDED !!!!!");
				DEBUG_PRINT("Collision Normal: [%f, %f]", collisionNormal.X(), collisionNormal.Y());
				DEBUG_PRINT("Collision Time: %f sec", collisionTime);
			}
			else
				DEBUG_PRINT("no collision.");
		}
	}
}

void CollidingSystem::GetEarliestCollision(float i_dt) {

	const size_t collideableCount = allCollideables.size();
	for (size_t i = 0; i < collideableCount - 1; i++) {
		for (size_t j = i + 1; j < collideableCount; j++) {

			Vector2 collisionNormal;
			float collisionTime = i_dt;

			if (SeparatingAxisCheck(*allCollideables[i], *allCollideables[j], i_dt, collisionTime, collisionNormal)) {
				earliestCollisionPair.m_collisionTime = collisionTime;

				earliestCollisionPair.m_collisions.first->m_collisionNormal = collisionNormal;
				earliestCollisionPair.m_collisions.first->m_collideableObject = allCollideables[i];

				earliestCollisionPair.m_collisions.second->m_collisionNormal = -collisionNormal;
				earliestCollisionPair.m_collisions.second->m_collideableObject = allCollideables[j];
			}
			else {
				earliestCollisionPair.m_collisions.first->m_collideableObject = nullptr;
				earliestCollisionPair.m_collisions.second->m_collideableObject = nullptr;
			}

		}
	}

}


void CollidingSystem::ResolveCollisions(float i_dt) {
	float frameTimeLeft = i_dt;

	GetEarliestCollision(frameTimeLeft);

	if (earliestCollisionPair.m_collisions.first->m_collideableObject == nullptr ||
		earliestCollisionPair.m_collisions.second->m_collideableObject == nullptr) {
		return;

	}

	ResolveCollision(earliestCollisionPair);
	frameTimeLeft -= earliestCollisionPair.m_collisionTime;


}

void CollidingSystem::ResolveCollision(CollisionPair& i_collisionPair) {
	StrongPtr<GameObject> tmpGameObjectOwner1 = i_collisionPair.m_collisions.first->m_collideableObject->pGameObject.CreateOwner();
	StrongPtr<GameObject> tmpGameObjectOwner2 = i_collisionPair.m_collisions.second->m_collideableObject->pGameObject.CreateOwner();

	// get the magnitude of new velocity using elastic collision formula
	Vector2 originalVelocity1 = i_collisionPair.m_collisions.first->m_collideableObject->pPhysicsItem->velocity;
	Vector2 originalVelocity2 = i_collisionPair.m_collisions.second->m_collideableObject->pPhysicsItem->velocity;

	float originalSpeed1_X = originalVelocity1.X();
	float originalSpeed1_Y = originalVelocity1.Y();
	float originalSpeed2_X = originalVelocity2.X();
	float originalSpeed2_Y = originalVelocity2.Y();

	float mass1 = i_collisionPair.m_collisions.first->m_collideableObject->pPhysicsItem->mass;
	float mass2 = i_collisionPair.m_collisions.second->m_collideableObject->pPhysicsItem->mass;

	float newSpeed1_X = ((mass1 - mass2) / (mass1 + mass2)) * originalSpeed1_X + (2 * mass2 / (mass1 + mass2)) * originalSpeed2_X;
	float newSpeed2_X = ((mass2 - mass1) / (mass1 + mass2)) * originalSpeed2_X + (2 * mass1 / (mass1 + mass2)) * originalSpeed1_X;
	float newSpeed1_Y = ((mass1 - mass2) / (mass1 + mass2)) * originalSpeed1_Y + (2 * mass2 / (mass1 + mass2)) * originalSpeed2_Y;
	float newSpeed2_Y = ((mass2 - mass1) / (mass1 + mass2)) * originalSpeed2_Y + (2 * mass1 / (mass1 + mass2)) * originalSpeed1_Y;

	// *reflect the velocity vectors*

	// then set velocities

	i_collisionPair.m_collisions.first->m_collideableObject->pPhysicsItem->velocity = Vector2(newSpeed1_X, newSpeed1_Y);
	i_collisionPair.m_collisions.second->m_collideableObject->pPhysicsItem->velocity = Vector2(newSpeed2_X, newSpeed2_Y);


}

Vector2 CollidingSystem::Reflect(Vector2& vectorToReflect, Vector2& normal) {
	float magnitudeMirrorVector = 2 * Vector2::dot(vectorToReflect, normal);

	return (vectorToReflect - magnitudeMirrorVector * normal);
}



void CollidingSystem::AdvanceSimulation(float i_dt) {
	Physics::RunAll(i_dt);
}

void CollidingSystem::CleanUp() {
	for (auto& collideable : allCollideables) {
		if (collideable) {
			delete collideable;
		}
	}

}
