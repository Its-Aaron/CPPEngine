#ifndef _TARGET_H
#define _TARGET_H

#include "Component.h"
#include "Transform.h"
class Mesh;
class BoxCollider;

[event_source(native)]
class Target : public Component
{
public:
	Target(Mesh* mesh, Mix_Chunk* sound, int points = 5);
	~Target() { }


	void Update(float deltaTime);

	void Init();

	void SetPositions(Vector3D _startPos, Vector3D _endPos);

	int points = 5;
	float movementSpeed = 5.0f;
	float fallSpeed = 180;
	Vector3D endPos;
	bool beenHit = false;

	float GetDistanceAlpha() { return ((endPos - transform->position).Magnitude() / startDistance); }

	__event void eOnTargetHit(int pointValue, GameObject* shotBy);
	__event void eOnDestroyNextFrame(Target* target);
	
private:
	void OnTakeDamage(float damage, GameObject* damageCauser, GameObject* damageInstigator);
	void DestroyNextFrame();

	bool falling = false;
	bool raising = false;
	bool destroyingNextFrame = false;

	BoxCollider* bc;

	float startDistance;

	Mesh* mesh;
	Mix_Chunk* sound;
};

#endif