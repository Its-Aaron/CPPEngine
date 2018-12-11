#include "BoxCollider.h"
#include "SphereCollider.h"

BoxCollider::BoxCollider()
{
	componentTypeName = "BoxCollider";
	colliderType = BOX;

	dimensions = Vector3D(1, 1, 1);
}

BoxCollider::~BoxCollider()
{
	Collider::~Collider();
}

void BoxCollider::Update(float deltaTime)
{
	if (debug)
	{
		int i = input->GetKey(SDLK_LCTRL) ? -1 : 1;

		if (input->GetKey(SDLK_1))
		{
			dimensions.x += 0.1f * i;
			dimensions.Print();
		}
		if (input->GetKey(SDLK_2))
		{
			dimensions.y += 0.1f * i;
			dimensions.Print();
		}
		if (input->GetKey(SDLK_3))
		{
			dimensions.z += 0.1f * i;
			dimensions.Print();
		}
	}
}