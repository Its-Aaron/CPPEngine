#include "Cannon.h"
#include "MeshRenderer.h"
#include <random>
#include "GameObjectManager.h"
#include "ColliderManager.h"

Cannon::Cannon(Mesh* ball)
{
	mesh = ball;
	componentTypeName = "Cannon";
}

Cannon::~Cannon()
{
}

void Cannon::Update(float deltaTime)
{
	elapsedTime += deltaTime;

	if (input->GetKeyDown(SDLK_9) && debug)
		Fire();

	if (elapsedTime >= fireRate && automatic)
		Fire();
}

void Cannon::Fire()
{
	mt19937 eng(rd());
	uniform_real_distribution<float> distr(-randomSpreadAmount, randomSpreadAmount);

	GameObject* go = new GameObject();
	MeshRenderer* mr = (MeshRenderer*)go->AttachComponent(new MeshRenderer(mesh));
	mr->offsets.scale = Vector3D(0.05f, 0.05f, 0.05f);
	go->transform->position = gameObject->transform->position;
	go->SetTag("Ball");
	SphereCollider* sc = (SphereCollider*)go->AttachCollider(SPHERE);
	RigidBody* rb = (RigidBody*)go->AttachComponent(new RigidBody());
	if (randomSpreadInRange)
		rb->AddForce( (gameObject->transform->ForwardVector() + Vector3D(distr(eng), distr(eng), distr(eng))) * fireStrength);
	else
		rb->AddForce(gameObject->transform->ForwardVector() * fireStrength);

	__hook(&RigidBody::eOnBounce, rb, &Cannon::MakeBounceNoise);
	__hook(&RigidBody::eOnBecomeStationary, rb, &Cannon::DestroyBall);

	if (randomFireInRange)
	{
		mt19937 eng2(rd());
		uniform_real_distribution<float> distr2(fireRateMin, fireRateMax);

		fireRate = distr2(eng2);
	}
	elapsedTime = 0.0f;
}

void Cannon::MakeBounceNoise(float ballPosToCamLength, float force)
{
	PlayMySound(sBounce, (130 - ballPosToCamLength) * (std::max(0.0f, std::min(force , 50.0f)) / 50.0f), false);
}

void Cannon::DestroyBall(GameObject* go)
{
	__unhook(&RigidBody::eOnBounce, (RigidBody*)go->GetComponent("RigidBody"), &Cannon::MakeBounceNoise);
	go->Destroy();
	//__unhook(&RigidBody::eOnBecomeStationary, (RigidBody*)go->GetComponent("RigidBody"), &Cannon::DestroyBall); Leads to exception
}