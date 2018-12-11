#ifndef _SPHERECOLLIDER_H
#define _SPHERECOLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	SphereCollider(float _radius);
	~SphereCollider();
	void Update(float deltaTime);

	float radius;
};

#endif