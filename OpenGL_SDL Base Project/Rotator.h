#ifndef _ROTATOR_H
#define _ROTATOR_H

#include "Component.h"
#include "Transform.h"

class Rotator : public Component
{
public:
	Rotator();
	~Rotator();

	void SetWobble(bool wobbles, Vector3D _wobbleAmount = Vector3D(0, 10, 0)) { wobble = wobbles; wobbleAmount = _wobbleAmount; }

	void Update(float deltaTime);

	void Init();

	void SetRotationSpeed(Vector3D speeds) { setByOther = true; rotSpeeds = speeds; }


private:
	Vector3D rotSpeeds;
	Transform* transform;

	Vector3D* curRot;
	Vector3D startRot;
	Vector3D temp;
	bool clockwise = true;
	bool setByOther;
	bool wobble = false;
	Vector3D wobbleAmount = Vector3D(0, 20, 0);
	bool CheckForDifference();
};

#endif