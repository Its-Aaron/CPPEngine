#include "Rotator.h"

Rotator::Rotator()
{
	setByOther = false; 
	componentTypeName = "Rotator";
}

Rotator::~Rotator()
{}

void Rotator::Update(float deltaTime)
{
	transform->rotation += (rotSpeeds * (clockwise ? (float)1 : (float)-1)) * deltaTime;
	if (wobble)
	{
		if (CheckForDifference())
			clockwise = !clockwise;
	}
}

void Rotator::Init()
{
	if (!initialised)
	{
		curRot = &gameObject->transform->rotation;
		transform = gameObject->transform;
		if (!setByOther)
			rotSpeeds = Vector3D(0, 10, 0);
		startRot = gameObject->transform->rotation;
	}
}

bool Rotator::CheckForDifference()
{
	temp = clockwise ? startRot + wobbleAmount : startRot - wobbleAmount;

	return clockwise ?	(curRot->x > temp.x || curRot->y > temp.y || curRot->z > temp.z) :
						(curRot->x < temp.x || curRot->y < temp.y || curRot->z < temp.z);
}