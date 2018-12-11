#include "Shooter.h"
#include "Projectile.h"


Shooter::Shooter(Mesh* _projectile, Transform* _spawnPoint) 
{
	projectile = _projectile;
	spawnPoint = _spawnPoint;
	componentTypeName = "Shooter";
}

void Shooter::Init()
{
	spawnPoint = spawnPoint == nullptr ? transform : spawnPoint;
}

void Shooter::Update(float deltaTime)
{
	elapsedTime += deltaTime;
	if (elapsedTime > fireRate)
		reloading = false;

	if (input->GetMouse(1) && fullAuto)
		Fire();

	if (input->GetMouseDown(1))
		Fire();

	if (autoFire)
		Fire();
}

void Shooter::Fire()
{
	if (elapsedTime > fireRate)
	{
		GameObject* go = new GameObject(new Projectile(projectile, spawnPoint, projectileVelocity));
		Projectile* projScript = (Projectile*)go->GetComponent("Projectile");
		projScript->SetMeshOffsets(projectileMeshOffsets);
		projScript->instigator = owner;
		go->transform->position = spawnPoint->position;

		go->SetTag("Projectile");
		Destroy(go, projectileLifeTime);

		elapsedTime = 0.0f;
		reloading = true;
	}
}