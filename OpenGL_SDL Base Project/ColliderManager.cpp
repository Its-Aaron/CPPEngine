#include "ColliderManager.h"
#include "Transform.h"

static ColliderManager* instance = 0;

ColliderManager* ColliderManager::GetInstance()
{

	if (instance == 0)
		instance = new ColliderManager();

	return instance;
}

ColliderManager::ColliderManager()
{}

ColliderManager::~ColliderManager()
{}

BoxCollider* ColliderManager::AddBoxCollider()
{
	BoxCollider* collider = new BoxCollider();
	colliders.push_back(collider);
	return collider;
}

SphereCollider* ColliderManager::AddSphereCollider()
{
	SphereCollider* collider = new SphereCollider();
	colliders.push_back(collider);
	return collider;
}


void ColliderManager::RemoveCollider(Collider* collider)
{
	for (unsigned int i = 0; i < colliders.size(); i++)
	{
		if (collider == colliders[i])
		{
			colliders.erase(colliders.begin() + i);
			break;
		}
	}
}

#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "../gl/glut.h"

void ColliderManager::Update(float deltaTime)
{
	for (unsigned int i = 0; i < colliders.size(); i++) //check me
	{
		if (colliders[i]->debug)
			colliders[i]->Update(deltaTime);
		for (unsigned int x = i + 1; x < colliders.size(); x++)
			CheckCollisions(colliders[i], colliders[x]);
	}
}

void ColliderManager::Render()
{
	for (unsigned int i = 0; i < colliders.size(); i++)
	{
		if (colliders[i]->visible)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			switch (colliders[i]->GetType())
			{
			case BOX:
				boxTemp = (BoxCollider*)colliders[i];

				glTranslatef(
					boxTemp->gameObject->transform->position.x + boxTemp->offset.x, 
					boxTemp->gameObject->transform->position.y + boxTemp->offset.y,
					boxTemp->gameObject->transform->position.z + boxTemp->offset.z
				);
				//boxTemp->gameObject->transform->position.Print();
				/*
				glRotatef(boxTemp->GetTransform()->rotation.x, 1, 0, 0);
				glRotatef(boxTemp->GetTransform()->rotation.y, 0, 1, 0);
				glRotatef(boxTemp->GetTransform()->rotation.z, 0, 0, 1);
				*/
				glScalef(
					boxTemp->dimensions.x * boxTemp->transform->scale.x,
					boxTemp->dimensions.y * boxTemp->transform->scale.y,
					boxTemp->dimensions.z * boxTemp->transform->scale.z
				);

				glColor3f(1.0, 0.0, 0.0);
				glutWireCube(1);
				break;
			case SPHERE:
				sphereTemp = (SphereCollider*)colliders[i];

				glTranslatef(
					sphereTemp->gameObject->transform->position.x + sphereTemp->offset.x, 
					sphereTemp->gameObject->transform->position.y + sphereTemp->offset.y,
					sphereTemp->gameObject->transform->position.z + sphereTemp->offset.z);

				/*
				glRotatef(boxTemp->GetTransform()->rotation.x, 1, 0, 0);
				glRotatef(boxTemp->GetTransform()->rotation.y, 0, 1, 0);
				glRotatef(boxTemp->GetTransform()->rotation.z, 0, 0, 1);
				*/

				glColor3f(0.0, 1.0, 0.0);
				glutWireSphere(sphereTemp->radius, 20, 8);

				break;
			default:
				break;
			}
			glColor3f(1, 1, 1);
			glPopMatrix();
		}
	}
}

void ColliderManager::CheckCollisions(Collider* colliderA, Collider* colliderB)
{
	switch (colliderA->GetType())
	{
	case BOX:	
		switch (colliderB->GetType())
		{
		case BOX:
			BoxBoxCheck((BoxCollider*)colliderA, (BoxCollider*)colliderB);
			break;
		case SPHERE:
			SphereBoxCheck((SphereCollider*)colliderB, (BoxCollider*)colliderA);
			break;
		default:
			cout << "Incorrect collider type";
			break;
		}
		break;
		//------------
	case SPHERE:
		switch (colliderA->GetType())
		{
		case BOX:
			SphereBoxCheck((SphereCollider*)colliderA, (BoxCollider*)colliderB);
			break;
		case SPHERE:
			SphereSphereCheck((SphereCollider*)colliderA, (SphereCollider*)colliderB);
			break;
		default:
			cout << "Incorrect collider type";
			break;
		}
		break;
		//-------------
	default:
		cout << "Incorrect collider type";
		break;
	}
}


bool ColliderManager::SphereSphereCheck(SphereCollider* sphereA, SphereCollider* sphereB)
{
	distSquared = sphereA->gameObject->transform->position.DistSquared(sphereB->gameObject->transform->position);

	sumOfBoundingRadii = sphereA->radius + sphereB->radius;

	hasCollided = distSquared < (sumOfBoundingRadii * sumOfBoundingRadii) ? true : false;

	sphereA->gameObject->CheckOverlaps(sphereB->gameObject, hasCollided);

	if (hasCollided)
	{
		//CalculateNewVelocities(sphereA->gameObject, sphereB->gameObject);
		//cout << "Collided SphereSphere" << endl;
	}

	return hasCollided;
}

bool ColliderManager::SphereBoxCheck(SphereCollider* sphere, BoxCollider* box)
{
	box->CalculateBounds();
	//SetBoxBounds(true, box);
	tempVec = sphere->gameObject->transform->position;

	hasCollided = 
		(
			box->bounds.max.x < tempVec.x - sphere->radius ||
			box->bounds.min.x > tempVec.x + sphere->radius ||
			box->bounds.max.y < tempVec.y - sphere->radius ||
			box->bounds.min.y > tempVec.y + sphere->radius ||
			box->bounds.max.z < tempVec.z - sphere->radius ||
			box->bounds.min.z > tempVec.z + sphere->radius
/*
			xMaxA < tempVec.x - sphere->radius ||
			xMinA > tempVec.x + sphere->radius ||
			yMaxA < tempVec.y - sphere->radius ||
			yMinA > tempVec.y + sphere->radius ||
			zMaxA < tempVec.z - sphere->radius ||
			zMinA > tempVec.z + sphere->radius
*/
		) ? false : true;



	sphere->gameObject->CheckOverlaps(box->gameObject, hasCollided);

	if (hasCollided)
	{
		//CalculateNewVelocities(sphere->gameObject, box->gameObject);
		//cout << "Collided SphereBox" << endl;
	}


	return hasCollided;
}

bool ColliderManager::BoxBoxCheck(BoxCollider* boxA, BoxCollider* boxB)
{
/*
	SetBoxBounds(true, boxA);
	SetBoxBounds(false, boxB);
*/

	boxA->CalculateBounds();
	boxB->CalculateBounds();

	hasCollided = 
		(
			boxA->bounds.max.x < boxB->bounds.min.x ||
			boxA->bounds.min.x > boxB->bounds.max.x ||
			boxA->bounds.max.y < boxB->bounds.min.y ||
			boxA->bounds.min.y > boxB->bounds.max.y ||
			boxA->bounds.max.z < boxB->bounds.min.z ||
			boxA->bounds.min.z > boxB->bounds.max.z
			) ? false : true;

	boxA->SetCollided(hasCollided);
	boxB->SetCollided(hasCollided);

	boxA->gameObject->CheckOverlaps(boxB->gameObject, hasCollided);

	if (hasCollided)
	{
		//CalculateNewVelocities(boxA->gameObject, boxB->gameObject);
		//cout << "Collided BoxBox" << endl;
	}

	return hasCollided;
}

void ColliderManager::SetBoxBounds(bool boxA, BoxCollider* box)
{
	tempVec = box->gameObject->transform->position + box->offset;

	if (boxA)
	{
		xMaxA = tempVec.x + (box->dimensions.x * box->gameObject->transform->scale.x) / 2;
		xMinA = tempVec.x - (box->dimensions.x * box->gameObject->transform->scale.x) / 2;
		yMaxA = tempVec.y + (box->dimensions.y * box->gameObject->transform->scale.y) / 2;
		yMinA = tempVec.y - (box->dimensions.y * box->gameObject->transform->scale.y) / 2;
		zMaxA = tempVec.z + (box->dimensions.z * box->gameObject->transform->scale.z) / 2;
		zMinA = tempVec.z - (box->dimensions.z * box->gameObject->transform->scale.z) / 2;
	}
	else
	{
		xMaxB = tempVec.x + (box->dimensions.x * box->gameObject->transform->scale.x) / 2;
		xMinB = tempVec.x - (box->dimensions.x * box->gameObject->transform->scale.x) / 2;
		yMaxB = tempVec.y + (box->dimensions.y * box->gameObject->transform->scale.y) / 2;
		yMinB = tempVec.y - (box->dimensions.y * box->gameObject->transform->scale.y) / 2;
		zMaxB = tempVec.z + (box->dimensions.z * box->gameObject->transform->scale.z) / 2;
		zMinB = tempVec.z - (box->dimensions.z * box->gameObject->transform->scale.z) / 2;
	}
}


void ColliderManager::CalculateNewVelocities(GameObject* objA, GameObject* objB)
{
	aRB = (RigidBody*)objA->GetComponent("RigidBody");
	bRB = (RigidBody*)objB->GetComponent("RigidBody");

	cout << "Calculating" << endl;

	objA->SetPositionToLastFrame();
	objB->SetPositionToLastFrame();



	if (aRB != nullptr)
	{
		aVel = aRB->GetVelocity();
		aMass = aRB->mass;
		aBounciness = aRB->bounciness;
	}
	else
	{
		aVel = Vector3D();
		aMass = 2;
		aBounciness = 0.5f;
	}
	if (bRB != nullptr)
	{
		bVel = bRB->GetVelocity();
		bMass = bRB->mass;
		bBounciness = bRB->bounciness;
	}
	else
	{
		bVel = Vector3D();
		bMass = 2;
		bBounciness = 0.5f;
	}

	averageBounciness = (aBounciness + bBounciness) / 2;

	muA = aVel * aMass;
	muB = bVel * bMass;
	mAmB = aMass + bMass;

	v3TempA = bVel - aVel; //Ub - Ua
	v3TempA = v3TempA * averageBounciness; //CR(Ub - Ua)
	v3TempA = v3TempA * bMass; //MbCR(Ub - Ua)
	v3TempA = (muA + muB + v3TempA * bMass) / mAmB;

	v3TempB = aVel - bVel; //Ua - Ub
	v3TempB = v3TempB * averageBounciness; //CR(Ua - Ub)
	v3TempB = v3TempB * aMass; //MaCR(Ua - Ub)
	v3TempB = (muA + muB + v3TempB * aMass) / mAmB;


	if (aRB != nullptr)
		aRB->SetVelocity(v3TempA);
	if (bRB != nullptr)
		bRB->SetVelocity(v3TempB);
}