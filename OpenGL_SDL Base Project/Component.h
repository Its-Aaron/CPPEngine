#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <SDL.h>
#include "GameObject.h"
#include "Commons.h"
#include <memory>
#include <stdio.h>

[event_receiver(native)]
class Component : public Object
{
public:
	Component();
	virtual ~Component();

	virtual void Update(float deltaTime) {}

	virtual void Render() {}

	void InitBase() { transform = gameObject->transform; }
	virtual void Init() {}

	virtual void OnBeginOverlap(GameObject* other) {}
	virtual void OnOverlapping(GameObject* other) {}
	virtual void OnEndOverlap(GameObject* other) {}

	virtual void OnTakeDamage(float damage, GameObject* damageCauser, GameObject* damageInstigator) {}

	void HookEvents();

	void UnhookEvents();

	void SetOwningGameObject(GameObject* owner) { gameObject = owner; HookEvents(); }

	string componentTypeName;

	GameObject* gameObject;
	Transform* transform;

	bool debug = false;

	void Destroy();
	void Destroy(GameObject* go, float lifeTime = 0.0f);

protected:
	bool initialised;
};

#endif