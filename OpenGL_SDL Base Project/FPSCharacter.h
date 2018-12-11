#ifndef _FPSCHARACTER_H
#define _FPSCHARACTER_H

#include "Component.h"
class Camera;
class BoxCollider;

class FPSCharacter : public Component
{
public:
	FPSCharacter();
	~FPSCharacter() {}

	void Render() { RenderCrosshair(); }
	void Update(float deltaTime);

	void Init();

	void SetRestrictedToBoundsManual(bool restricted, Bounds _bounds = Bounds());
	void SetRestrictedToBoundsCollider(bool restricted, BoxCollider* bc);

private:
	Camera* cam;
	float turnSpeed = 5.0f;
	float moveSpeed = 20.0f;
	float sprintSpeed = 35.0f;
	float currentSpeed;
	Bounds bounds;
	Bounds* bcBounds = nullptr;
	bool restrictedToBounds;

	void Move(float deltaTime);
	void KeepInBounds();
	void RenderCrosshair();
};

#endif