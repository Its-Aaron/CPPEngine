#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Component.h"
#include "RigidBody.h"

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	bool visible;
	virtual void Update(float deltaTime);
	void Init();
	COLLIDERTYPE GetType() { return colliderType; }
	void SetCollided(bool _collided, GameObject* other = nullptr);
	bool GetCollided() { return collided; }
	virtual void CalculateBounds() {}
	bool toBeDestroyed;
	bool isTrigger;
	Vector3D offset;
	bool debug = false;
	Bounds bounds;

protected:
	COLLIDERTYPE colliderType;
	//Transform* transform;
	bool collided;
};

#endif