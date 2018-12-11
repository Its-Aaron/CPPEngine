#ifndef _AITARGETSHOOTER_H
#define _AITARGETSHOOTER_H

#include "Component.h"
#include <random>
class Target;
class ShootingStall;
class Shooter;

class AITargetShooter : public Component
{
public:
	AITargetShooter();
	~AITargetShooter() {}


	void Update(float deltaTime);

	void Init();
	void SetGun(Shooter* _gun) { gun = _gun; hasGun = true; }
	ShootingStall* stall;
	Vector3D accuracy = Vector3D(0, 10, 0);

private:
	Target* target = nullptr;
	Shooter* gun;
	bool TargetIsValid();
	void AcquireTarget();
	void FaceTarget();
	void Fire();
	void NullTarget();
	void PrepareToNullTarget(Target* target) { nullingTarget = true; }

	bool hasGun = false;
	bool nullingTarget = false;

	random_device rd;
};

#endif