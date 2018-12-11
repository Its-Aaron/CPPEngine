#include "Projectile.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "ColliderManager.h"
#include "Transform.h"

Projectile::Projectile(Mesh* _mesh, Transform* spawnTransform, float initialVelocity, bool sphereCollider, bool boxCollider)
{
	spawnTrans = spawnTransform;
	mesh = _mesh;
	initVel = initialVelocity;
	usesSphere = sphereCollider;
	usesBox = boxCollider;
	componentTypeName = "Projectile";
}

Projectile::~Projectile() 
{ 
}

void Projectile::Update(float deltaTime)
{
	if (!fired)
	{
		rb->AddForce(spawnTrans->ForwardVector() * initVel);
		fired = true;
	}
}

void Projectile::Init()
{
	rb = (RigidBody*)gameObject->AttachComponent(new RigidBody());
	rb->affectedByGravity = affectedByGravity;
	rb->affectedByDrag = affectedByDrag;
	rb->usesFakeFloor = false;

	mr = (MeshRenderer*)gameObject->AttachComponent(new MeshRenderer(mesh));

	if (usesSphere)
		sc = (SphereCollider*)gameObject->AttachCollider(SPHERE);
	if (usesBox)
		bc = (BoxCollider*)gameObject->AttachCollider(BOX);
}

void Projectile::SetMeshOffsets(Transform offsets)
{
	meshOffsets = offsets;
	if (mr != nullptr)
		mr->offsets = meshOffsets;
}

void Projectile::SetAffectedByGravity(bool affected) 
{
	affectedByGravity = affected; 
	rb->affectedByGravity = affectedByGravity; 
}

void Projectile::OnBeginOverlap(GameObject* other)
{
	other->TakeDamage(damage, gameObject, instigator); //maybe unhook event?
	CheckDestroy();
}