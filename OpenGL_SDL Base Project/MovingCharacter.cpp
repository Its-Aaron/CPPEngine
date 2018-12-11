#include "MovingCharacter.h"

MovingCharacter::MovingCharacter()
{
	moveSpeed = 10;
	jumpHeight = 20;
	canJump = true;
	componentTypeName = "MovingCharacter";
}

MovingCharacter::~MovingCharacter()
{}

void MovingCharacter::Init()
{
	transform = gameObject->transform;
	mr = (MeshRenderer*)gameObject->GetComponent("MeshRenderer");
	initialised = true;
}

void MovingCharacter::Update(float deltaTime)
{
	if (active)
	{
		if (input->GetKey(SDLK_a))
			transform->position -= transform->RightVector() * (moveSpeed * deltaTime);
		if (input->GetKey(SDLK_d))
			transform->position += transform->RightVector() * (moveSpeed * deltaTime);
		if (input->GetKey(SDLK_w))
			transform->position += transform->ForwardVector() * (moveSpeed * deltaTime);
		if (input->GetKey(SDLK_s))
			transform->position -= transform->ForwardVector() * (moveSpeed * deltaTime);

		if (forcedInBounds)
		{
			if (transform->position.x < boundsMin.x)
				transform->position.x = boundsMin.x;

			if (transform->position.x > boundsMax.x)
				transform->position.x = boundsMax.x;

			if (transform->position.z < boundsMin.z)
				transform->position.z = boundsMin.z;

			if (transform->position.z > boundsMax.z)
				transform->position.z = boundsMax.z;
		}

		if (transform->position.y <= 0)
			canJump = true;
		else canJump = false;
	}




	if (ball1RB != nullptr)
	{
		temp = (transform->position + Vector3D(0, 0, -3)) - ball1RB->gameObject->transform->position;
		ball1RB->AddForce(Vector3D(temp.x, 0, temp.z) * deltaTime * 12);
	}
	if (ball2RB != nullptr)
	{
		temp = (transform->position + Vector3D(0, 0, -6)) - ball2RB->gameObject->transform->position;
		ball2RB->AddForce(Vector3D(temp.x, 0, temp.z) * deltaTime * 12);
	}
	if (ball3RB != nullptr)
	{
		temp = (transform->position + Vector3D(0, 0, -9)) - ball3RB->gameObject->transform->position;
		ball3RB->AddForce(Vector3D(temp.x, 0, temp.z) * deltaTime * 12);
	}
}

void MovingCharacter::OnBeginOverlap(GameObject* other)
{
	if (other->TagIs("Ball"))
		__raise eOnHitByBall(other);
}