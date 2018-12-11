#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "Transform.h"
class MeshRenderer;
class Mesh;
class RigidBody;
class BoxCollider;
class SphereCollider;

class Projectile : public Component
{
public:
	Projectile(Mesh* _mesh, Transform* spawnTransform, float initialVelocity = 20.0f, bool sphereCollider = true, bool boxCollider = false);
	~Projectile();
	void Update(float deltaTime);
	void Init();
	void SetMeshOffsets(Transform offsets);
	GameObject* instigator = nullptr;

	float damage = 10.0f;
	
	void SetAffectedByGravity(bool affected);
	
	bool destroyedOnImpact = true;

	Transform meshOffsets;
private:
	RigidBody* rb;
	Mesh* mesh;
	Transform* spawnTrans;
	MeshRenderer* mr = nullptr;
	bool usesSphere;
	bool usesBox;
	SphereCollider* sc = nullptr;
	BoxCollider* bc = nullptr;
	float initVel;
	bool fired = false;
	bool affectedByGravity = false;
	bool affectedByDrag = false;

	void CheckDestroy() { if (destroyedOnImpact) Destroy(); }
	void OnBeginOverlap(GameObject* other);
};

#endif