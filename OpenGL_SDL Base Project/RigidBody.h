#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "Component.h"
#include "Transform.h"
#include "Camera.h"

class RigidBody : public Component
{
public:
	RigidBody();
	~RigidBody();

	bool collisionOnY; //TEMPORARY
	float airDrag = 0.03f;
	float friction = 0.1f;
	float bounciness = 0.5f;
	float mass = 1;

	bool affectedByGravity = true;
	bool affectedByDrag = true;

	Vector3D GetVelocity() { return velocity; }
	void SetVelocity(Vector3D newVel);

	void Update(float deltaTime);

	void AddForce(Vector3D force);

	float heightLimit = 0;

	bool usesFakeFloor = true;

	bool enabled = true;

	__event void eOnBounce(float posToCamMagnitude, float force);
	__event void eOnHitAnything();
	__event void eOnBecomeStationary(GameObject* go);

private:
	RigidBody * rb;
	Vector3D v3Temp;
	float averageBounciness;
	Vector3D otherVel;
	float otherMass;
	float otherBounciness;
	Camera* cam;
	Vector3D temp;

	bool onFloor = false;
	Vector3D velocity;
};

#endif