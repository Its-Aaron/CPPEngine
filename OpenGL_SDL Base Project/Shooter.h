#ifndef _SHOOTER_H
#define _SHOOTER_H

#include "Component.h"
#include "Mesh.h"

class Shooter : public Component
{
public:
	Shooter(Mesh* _projectile, Transform* _spawnPoint = nullptr);
	~Shooter() {}

	void Update(float deltaTime);

	void Fire();

	void Init();

	float fireStrength = 20.0f;
	bool projectilesHaveGravity = false;
	bool projectilesHaveDrag = false;
	Vector3D projectileScale = Vector3D(1, 1, 1);
	Transform* spawnPoint;
	Transform projectileMeshOffsets;
	GameObject* owner = nullptr;

	bool autoFire = false;
	bool fullAuto = false;
	float fireRate = 1.0f;
	float projectileLifeTime = 1.0f;
	float projectileVelocity = 50.0f;

	bool GetReloading() { return reloading; }

private:
	Mesh* projectile;
	float elapsedTime = 0.0f;
	bool reloading = false;
};

#endif