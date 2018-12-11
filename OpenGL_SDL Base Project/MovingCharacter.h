#ifndef _MovingCharacter_H
#define _MovingCharacter_H

#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"
#include "MeshRenderer.h"



class MovingCharacter : public Component
{
public:
	MovingCharacter();
	~MovingCharacter();

	float moveSpeed;
	float jumpHeight;

	void Update(float deltaTime);

	void OnBeginOverlap(GameObject* other);
	void OnTakeDamage(float amount) { cout << "Ouch, I took " + to_string(amount) + " damage!" << endl; }

	__event void eOnHitByBall(GameObject* theBall);

	bool active = true;

	RigidBody* ball1RB = nullptr;
	RigidBody* ball2RB = nullptr;
	RigidBody* ball3RB = nullptr;

	bool forcedInBounds = false;

	Vector3D boundsMin = Vector3D(-22, 0, -48);
	Vector3D boundsMax = Vector3D(22, 0, 48);


private:
	void Init();
	MeshRenderer* mr;
	void Jump();

	bool canJump;

	Vector3D temp;
};


#endif