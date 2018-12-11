#include "Component.h"

Component::Component()
{
	gameObject = nullptr;
	initialised = false;
}

Component::~Component()
{
	UnhookEvents();
}

void Component::HookEvents()
{
	__hook(&GameObject::eOnBeginOverlap, gameObject, &Component::OnBeginOverlap);
	__hook(&GameObject::eOnOverlapping, gameObject, &Component::OnOverlapping);
	__hook(&GameObject::eOnEndOverlap, gameObject, &Component::OnEndOverlap);
	__hook(&GameObject::eOnTakeDamage, gameObject, &Component::OnTakeDamage);
}

void Component::UnhookEvents()
{
	if (gameObject != nullptr)
	{
		__unhook(&GameObject::eOnBeginOverlap, gameObject, &Component::OnBeginOverlap);
		__unhook(&GameObject::eOnOverlapping, gameObject, &Component::OnOverlapping);
		__unhook(&GameObject::eOnEndOverlap, gameObject, &Component::OnEndOverlap);
		__unhook(&GameObject::eOnTakeDamage, gameObject, &Component::OnTakeDamage);
	}
}

void Component::Destroy()
{
	gameObject->Destroy();
}

void Component::Destroy(GameObject* go, float lifeTime)
{
	if (lifeTime <= 0.0f)
		go->Destroy();
	else
		go->Destroy(go, lifeTime);
}