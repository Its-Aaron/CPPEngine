#include "AITargetShooter.h"
#include "Target.h"
#include "ShootingStall.h"
#include "Shooter.h"

AITargetShooter::AITargetShooter()
{
	componentTypeName = "AITargetShooter";
}

void AITargetShooter::Update(float deltaTime)
{
	if (nullingTarget)
		NullTarget();

	if (hasGun && stall != nullptr)
	{
		if (!TargetIsValid())
			AcquireTarget();

		if (TargetIsValid())
			Fire();
	}
}

void AITargetShooter::Init()
{}

bool AITargetShooter::TargetIsValid()
{
	return (target != nullptr && target != NULL && !target->beenHit);
}

void AITargetShooter::AcquireTarget()
{
	if (stall->GetTargets().size() > 0)
	{
		target = stall->GetTargets()[stall->GetTargets().size() - 1];
		__hook(&Target::eOnDestroyNextFrame, target, &AITargetShooter::PrepareToNullTarget);
	}
	else
		target = nullptr;
}


void AITargetShooter::NullTarget()
{ 
	__unhook(&Target::eOnDestroyNextFrame, target, &AITargetShooter::PrepareToNullTarget);
	target = nullptr;
	nullingTarget = false;
}

#include <glut.h>

void AITargetShooter::FaceTarget() 
{
	mt19937 eng(rd());
	//uniform_real_distribution<float>accX(-accuracy.x, accuracy.x);
	uniform_real_distribution<float>accY(-accuracy.y, accuracy.y);
	//uniform_real_distribution<float>accZ(-accuracy.z, accuracy.z);

	/*
	Vector3D temp = (target->transform->position - transform->position).Normalise();
	Vector3D temp2 = Vector3D(0,0,1).Cross(temp);// +Vector3D((float)accX(eng), (float)accY(eng), (float)accZ(eng));
	float rotAngle = std::acos(Vector3D(0, 0, 1).Dot(temp));
	

	Vector3D nTarget = (target->transform->position - transform->position).Normalise();
	Vector3D axis = nTarget.Cross(transform->ForwardVector()).Normalise();
	float angle = std::acos(transform->ForwardVector().Dot(nTarget));

	transform->rotation = (axis * angle) * 360;

		cout << "Axis - ";
		axis.Print();
		cout  << "Angle - " << angle << endl << "Transform - ";
		transform->rotation.Print();
		cout << endl;
		*/

	transform->rotation.y = FLerp(50, -16, target->GetDistanceAlpha()) + accY(eng);
}

void  AITargetShooter::Fire()
{
	if (!gun->GetReloading())
	{
		FaceTarget();
		gun->Fire();
	}
}