#ifndef _CAMERA_H
#define _CAMERA_H

#include "Component.h"
#include "Transform.h"

class Camera// : public Component
{
public:
	static Camera* GetInstance();
	void Update(float deltaTime);
	void Render();
	Transform GetTransform() { return transform; }

	void SetLockedTransform(bool locked = false, Transform* transform = nullptr) { lockedTransform = transform; lockedToOtherTransform = true; }

	Transform transform;

	bool mobile = false;

private:
	Camera();
	~Camera();

	Input* input;

	//void Init();
	bool initialised;
	bool fps;

	bool lockedToOtherTransform = false;
	Transform* lockedTransform;
};



#endif