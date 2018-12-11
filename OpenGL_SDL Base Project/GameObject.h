#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>
#include <memory>
#include "Object.h"
#include "Input.h"
class Component;
class Transform;
#include "Commons.h"
class Collider;

using namespace std;

[event_source(native)]
class GameObject : public Object
{
public:
	GameObject();
	GameObject(GameObject* _parent);
	GameObject(Component* _component);
	GameObject(vector<Component*> _component);
	GameObject(GameObject* _parent, Component* _component);
	~GameObject();

	bool debug;
	void Update(float deltaTime);
	void Render(bool calledFromParent = false);
	Transform* transform = nullptr;
	
	// Components
	Component* GetComponent(string componentName);
	Component* AttachComponent(Component* component);
	void DestroyComponent(Component* _component);
	Collider* AttachCollider(COLLIDERTYPE type);
	//---

	// Relationships
	void SetChild(GameObject* _child);
	void SetSibling(GameObject* _sibling);
	bool IsChild() { return isChild; }
	bool HasChild() { return hasChild; }

	string GetChildren(int depth = 0, bool calledFromObject = false);
	vector<GameObject*> GetFamilyPointers();

	GameObject* child;
	GameObject* sibling;
	GameObject* parent;
	//---

	void Destroy(GameObject* go, float time = 0.0f);
	void Destroy();
	void CancelDestroy(bool makeUndestroyable) { lifeTime = 0.0f; undestroyable = makeUndestroyable; }

	int sceneDepth;

	void SetPositionToLastFrame();

	//--------EVENTS
	__event void eOnBeginOverlap(GameObject* other);
	__event void eOnOverlapping(GameObject* other);
	__event void eOnEndOverlap(GameObject* other);

	__event void eOnTakeDamage(float damage, GameObject* damageCauser, GameObject* damageInstigator);

	__event void eOnDestroyed(GameObject* thisGO);

	void TakeDamage(float damage, GameObject* damageCauser, GameObject* damageInstigator) { __raise eOnTakeDamage(damage, damageCauser, damageInstigator); }

	void OnBeginOverlap(GameObject* other) { collidingObjects.push_back(other); __raise eOnBeginOverlap(other); }
	void OnOverlapping(GameObject* other) { __raise eOnOverlapping(other); }
	void OnEndOverlap(GameObject* other, bool calledFromSelf, int index = 0);

	void CheckOverlaps(GameObject* other, bool overlapped);
	//

	string GetTag() { return tag; }
	void SetTag(string _tag) { tag = _tag; }
	bool TagIs(string _tag) { return tag == _tag; }

	vector<GameObject*> collidingObjects;

protected:
	float timeSinceCreation = 0;

private:
	vector<Component*> components;
	bool isChild;
	bool hasChild;
	bool hasSibling;
	void Initialise();
	void PrintChildren();

	float lifeTime = 0;
	bool waitingOnDestruction = false;

	void RenderChildren();
	void RenderSibling();

	bool setsLastPosition;
	Vector3D positionLastFrame;

	string tag = "Default";

	bool temp;

	bool undestroyable = false;
};

#endif