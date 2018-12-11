#ifndef _BOXCOLLIDER_H
#define _BOXCOLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();

	void Update(float deltaTime);

	void CalculateBounds() { bounds.CalculateBounds(transform->position, dimensions * transform->scale); }

	Vector3D dimensions;
};

#endif