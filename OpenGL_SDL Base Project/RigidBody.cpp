#include "RigidBody.h"
#include "Constants.h"
#include "Camera.h"
#include <cmath>

RigidBody::RigidBody()
{
	componentTypeName = "RigidBody";

	mass = 1;
	cam = Camera::GetInstance();
	velocity = Vector3D();
}

RigidBody::~RigidBody()
{}

void RigidBody::Update(float deltaTime)
{

	if (enabled)
	{
		gameObject->transform->position += velocity * deltaTime;

		if (usesFakeFloor)
		{
			if (gameObject->transform->position.y < heightLimit)
			{
				__raise eOnHitAnything();
				gameObject->transform->position.y = heightLimit;
				if (abs(velocity.Magnitude()) > 20)
				{
					temp = gameObject->transform->position - cam->transform.position;
					__raise eOnBounce(temp.Magnitude(), velocity.Magnitude());
				}
				velocity.y = -velocity.y * bounciness;
				if (velocity.y < 1)
					velocity.y = 0;
			}
		}

		if ((velocity.x < 1 && velocity.x > -1) && (velocity.z < 1 && velocity.z > -1))
		{
			velocity.x = 0;
			velocity.z = 0;

			if (velocity.y == 0)
				__raise eOnBecomeStationary(gameObject);
		}

		if ((velocity.x != 0 || velocity.z != 0) && affectedByDrag)
		{
			velocity.x *= 1 - airDrag;
			velocity.z *= 1 - airDrag;
			if (onFloor)
			{
				velocity.x *= 1 - friction;
				velocity.z *= 1 - friction;
			}
		}

		if (velocity.y == 0)
			onFloor = true;
		else
			onFloor = false;

		if (affectedByGravity)
			velocity.y += GRAVITY * deltaTime;


		rb = nullptr;
	}

	if (input->GetKey(SDLK_SPACE) && debug)
	{
		cout << "velocity = ";
		velocity.Print();
		cout << endl << "position = ";
		gameObject->transform->PrintPosition();
	}
}

void RigidBody::AddForce(Vector3D force)
{
	velocity += force;
}

void RigidBody::SetVelocity(Vector3D newVel)
{
	if (abs(velocity.Magnitude()) > 20)
	{
		temp = gameObject->transform->position - cam->transform.position;
		__raise eOnBounce(temp.Magnitude(), velocity.Magnitude());
	}

	velocity = newVel;
}