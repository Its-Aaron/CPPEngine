#include "Collider.h"
#include "Transform.h"

Collider::Collider()
{
	visible = false;
	offset = Vector3D();
	toBeDestroyed = false;
	isTrigger = false;
}

#include "ColliderManager.h"

Collider::~Collider()
{
	ColliderManager::GetInstance()->RemoveCollider(this);
}

void Collider::Update(float deltaTime)
{}

void Collider::Init()
{
}

void Collider::SetCollided(bool _collided, GameObject* other)
{
	collided = _collided;

	if (collided)
	{
		
	}
}