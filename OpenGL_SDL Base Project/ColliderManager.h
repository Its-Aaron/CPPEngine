#ifndef _COLLIDERMANAGER_H
#define _COLLIDERMANAGER_H

#include "Collider.h"
#include <vector>
#include "BoxCollider.h"
#include "SphereCollider.h"

class ColliderManager
{
public:
	static ColliderManager* GetInstance();
	vector<Collider*> GetColliders() { return colliders; }
	BoxCollider* AddBoxCollider();
	SphereCollider* AddSphereCollider();
	void RemoveCollider(Collider* collider);

	void Update(float deltaTime);
	void Render();

private:
	vector<Collider*> colliders;
	void CheckCollisions(Collider* colliderA, Collider* colliderB);
	bool SphereSphereCheck(SphereCollider* sphereA, SphereCollider* sphereB);
	bool SphereBoxCheck(SphereCollider* sphere, BoxCollider* box);
	bool BoxBoxCheck(BoxCollider* boxA, BoxCollider* boxB);
	void SetBoxBounds(bool boxA, BoxCollider* box);
	ColliderManager();
	~ColliderManager();

	BoxCollider* box1;
	BoxCollider* box2;
	SphereCollider* sphere1;
	SphereCollider* sphere2;

	Vector3D tempVec;

	float distSquared;
	float sumOfBoundingRadii;
	bool hasCollided;

	float xMaxA, xMinA, yMaxA, yMinA, zMaxA, zMinA;
	float xMaxB, xMinB, yMaxB, yMinB, zMaxB, zMinB;

	BoxCollider* boxTemp;
	SphereCollider* sphereTemp;


	void CalculateNewVelocities(GameObject* objA, GameObject* objB);

	// Velocity calculation variables
	Vector3D aVel, bVel, muA, muB;
	float aMass, bMass, mAmB;;
	float aBounciness, bBounciness, averageBounciness;
	RigidBody* aRB;
	RigidBody* bRB;

	Vector3D v3TempA, v3TempB;
	//

	bool temp;

};

#endif