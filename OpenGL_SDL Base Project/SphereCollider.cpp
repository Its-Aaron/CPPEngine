#include "SphereCollider.h"
#include "BoxCollider.h"

SphereCollider::SphereCollider()
{
	componentTypeName = "SphereCollider";
	colliderType = SPHERE;
	radius = 1;
}

SphereCollider::SphereCollider(float _radius)
{
	componentTypeName = "SphereCollider";
	colliderType = SPHERE;
	radius = _radius;
}

SphereCollider::~SphereCollider()
{}

void SphereCollider::Update(float deltaTime)
{
	bounds.CalculateBounds(transform->position, transform->scale);
}