#include "Component.h"
#include "GameObject.h"
#include <stdio.h>
#include <iostream>
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light.h"
#include "GameObjectManager.h"
#include "ColliderManager.h"

void GameObject::Initialise() 
{ 
	transform = new Transform(); 
	debug = false; 
	isChild = false; 
	hasChild = false; 
	hasSibling = false; 
	child = nullptr;
	sibling = nullptr;
	timeSinceCreation = 0;

	sceneDepth = 0;

	GameObjectManager::GetInstance()->AddGameObject(this);
}

#pragma region constructors
GameObject::GameObject()
{
	Initialise();
}

GameObject::GameObject(GameObject* _parent)
{
	Initialise();
	_parent->SetChild(this);
}

GameObject::GameObject(Component* _component)
{
	Initialise();
	AttachComponent(_component);
}

GameObject::GameObject(vector<Component*> _components)
{
	Initialise();

	for (unsigned int i = 0; i < _components.size(); i++)
		AttachComponent(_components[i]);
}

GameObject::GameObject(GameObject* _parent, Component* _component)
{
	Initialise();
	_parent->SetChild(this);
	AttachComponent(_component);
}
#pragma endregion All constructors

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < components.size(); i++)
		delete components[i];

	if (transform != nullptr && transform != NULL)
		delete transform;
}

void GameObject::SetChild(GameObject* _child)
{
	if (_child != NULL)
	{
		hasChild = true;
		_child->isChild = true;
		_child->sceneDepth = sceneDepth + 1;
		if (child == nullptr)
			child = _child;
		else child->SetSibling(_child);
		_child->parent = this;
	}
}

void GameObject::SetSibling(GameObject* _sibling)
{
	if (_sibling != NULL)
	{
		hasSibling = true;
		_sibling->sceneDepth = sceneDepth;
		if (sibling == nullptr)
			sibling = _sibling;
		else sibling->SetSibling(_sibling);
	}
}

Component* GameObject::AttachComponent(Component* component) 
{
	component->SetOwningGameObject(this);
	components.push_back(component);
	component->InitBase();
	component->Init();
	return component;
}

void GameObject::DestroyComponent(Component* _component)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (_component == components[i])
		{
			components.erase(components.begin() + i);
			delete _component;
			break;
		}

	}
}

Collider* GameObject::AttachCollider(COLLIDERTYPE type)
{
	switch (type)
	{
	case BOX:
		return (Collider*)AttachComponent(ColliderManager::GetInstance()->AddBoxCollider());
		break;
	case SPHERE:
		return (Collider*)AttachComponent(ColliderManager::GetInstance()->AddSphereCollider());
		break;
	default:
		return nullptr;
		break;
	}
}

void GameObject::Update(float deltaTime)
{
	positionLastFrame = transform->position;


	if (debug)
	{
		if (input->GetKeyDown(SDLK_g))
			Destroy(this);
		//Print transforms and children
		if (input->GetKeyDown(SDLK_RETURN))
		{
			cout << endl << "--- " << name << " ---" << endl << "Transform:" << endl;
			transform->PrintPosition();
			transform->PrintRotation();
			transform->PrintScale();

			cout << "Children: " << endl;
			PrintChildren();
			cout << "Has child - " << hasChild << endl;
			cout << "Has Sibling - " << hasSibling << endl;
			cout << endl;
		}

		//Scale up and down. 5 = Halve, 6 = -0.1, 7 = +0.1, 8 = double, 9 = default
#pragma region Scale commands
		if (input->GetKeyDown(SDLK_5))
		{
			transform->scale *= 0.5f;
			cout << "New scale: "; transform->scale.Print();
		}
		if (input->GetKeyDown(SDLK_6))
		{
			transform->scale -= 0.1f;
			cout << "New scale: "; transform->scale.Print();
		}
		if (input->GetKeyDown(SDLK_7))
		{
			transform->scale += 0.1f;
			cout << "New scale: "; transform->scale.Print();
		}
		if (input->GetKeyDown(SDLK_8))
		{
			transform->scale *= 2.0f;
			cout << "New scale: "; transform->scale.Print();
		}
		if (input->GetKeyDown(SDLK_9))
		{
			transform->scale = Vector3D(1, 1, 1);
			cout << "New scale: "; transform->scale.Print();
		}
#pragma endregion Scale commands

		int speed = (input->GetKey(SDLK_SPACE)) ? 5 : 20;
		if (input->GetKey(SDLK_j))
		{
			transform->position -= transform->RightVector() * (speed * deltaTime);
			transform->PrintPosition();
		}
		if (input->GetKey(SDLK_l))
		{
			transform->position += transform->RightVector() * (speed * deltaTime);
			transform->PrintPosition();
		}
		if (input->GetKey(SDLK_i))
		{
			transform->position += transform->ForwardVector() * (speed * deltaTime);
			transform->PrintPosition();
		}
		if (input->GetKey(SDLK_k))
		{
			transform->position -= transform->ForwardVector() * (speed * deltaTime);
			transform->PrintPosition();
		}

		if (input->GetKey(SDLK_z))
			transform->rotation.x += 10 * deltaTime;
		if (input->GetKey(SDLK_x))
			transform->rotation.y += 10 * deltaTime;
		if (input->GetKey(SDLK_c))
			transform->rotation.z += 10 * deltaTime;

		if (input->GetKeyDown(SDLK_SPACE))
			cout << name << endl;
	}

	for (unsigned int i = 0; i < components.size(); i++)
	{
		components[i]->Update(deltaTime);
		//cout << components[i]->componentTypeName << endl;
	}

	timeSinceCreation += deltaTime;

	if (lifeTime != 0 && waitingOnDestruction == false)
		Destroy(this, lifeTime);
	if (timeSinceCreation > lifeTime && waitingOnDestruction)
		Destroy(this);
}

void GameObject::SetPositionToLastFrame()
{
	transform->PrintPosition();
	transform->position = positionLastFrame;
}

void GameObject::PrintChildren()
{
	if (hasChild)
	{
		cout << child->name << endl;
		child->PrintChildren();
	}
	if (hasSibling)
	{
		cout << sibling->name << endl;
		sibling->PrintChildren();
	}
}

void GameObject::Destroy(GameObject* go, float time)
{
	if (time > 0)
	{
		go->lifeTime = go->timeSinceCreation + time;
		waitingOnDestruction = true;
	}
	else
	{
		if (!undestroyable)
		{
			GameObjectManager::GetInstance()->Destroy(this);
			__raise eOnDestroyed(this);
		}
	}
}

void GameObject::Destroy()
{
	if (!undestroyable)
	{
		GameObjectManager::GetInstance()->Destroy(this);
		__raise eOnDestroyed(this);
	}
}

string GameObject::GetChildren(int depth, bool calledFromObject)
{
	string returnString;
	bool isValid = false;

	if (!isChild)
	{
		returnString = "Root: " + name + '\n';
		if (hasChild)
			returnString += child->GetChildren(1, true);

		isValid = true;
	}
	if (calledFromObject)
	{
		for (int i = 0; i < depth; i++)
			returnString += '\t';
		returnString += name + '\n';

		if (hasChild)
			returnString += child->GetChildren(depth + 1, true);

		if (hasSibling)
			returnString += sibling->GetChildren(depth, true);

		isValid = true;
	}
	if (isValid)
		return returnString;

	else return "Invalid Object";
}

vector<GameObject*> GameObject::GetFamilyPointers() 
{ 
	vector<GameObject*> tree;
	vector<GameObject*> childTree;
	vector<GameObject*> siblingTree;

	tree.push_back(this);

	if (hasChild)
		childTree = child->GetFamilyPointers();
	if (hasSibling)
		siblingTree = sibling->GetFamilyPointers();

	tree.reserve(tree.size() + childTree.size() + siblingTree.size()); 
	tree.insert(tree.end(), childTree.begin(), childTree.end());
	tree.insert(tree.end(), siblingTree.begin(), siblingTree.end());

	return tree;
}

/*
template <class T>
T*  GameObject::GetComponent<T>()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (typeid(components[i]) == <T>)
			return components[i];
	}
	return NULL;
}
*/

Component* GameObject::GetComponent(string componentName)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->componentTypeName == componentName)
			return components[i];
	}
	cout << componentName + " not found in" + name << endl;
	return nullptr;
}

void GameObject::Render(bool calledFromParent)
{

	if (!IsChild())
	{

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();


		glTranslatef(transform->position.x, transform->position.y, transform->position.z);

		glRotatef(transform->rotation.x, 1, 0, 0);
		glRotatef(transform->rotation.y, 0, 1, 0);
		glRotatef(transform->rotation.z, 0, 0, 1);

		glScalef(transform->scale.x, transform->scale.y, transform->scale.z);

		for (unsigned int i = 0; i < components.size(); i++)
			components[i]->Render();

		RenderChildren();

		glPopMatrix();
	}
	else if (calledFromParent)
	{

		RenderSibling();

		glPushMatrix();


		glTranslatef(transform->position.x, transform->position.y, transform->position.z);

		glRotatef(transform->rotation.x, 1, 0, 0);
		glRotatef(transform->rotation.y, 0, 1, 0);
		glRotatef(transform->rotation.z, 0, 0, 1);

		glScalef(transform->scale.x, transform->scale.y, transform->scale.z);

		for (unsigned int i = 0; i < components.size(); i++)
			components[i]->Render();

		RenderChildren();

		glPopMatrix();
	}
}

void GameObject::RenderChildren()
{
	if (hasChild)
		child->Render(true);
}

void GameObject::RenderSibling()
{
	if (hasSibling)
		sibling->Render(true);
}

void GameObject::OnEndOverlap(GameObject* other, bool calledFromSelf, int index)
{
	if (calledFromSelf)
		collidingObjects.erase(collidingObjects.begin() + index);
	else
	{
		for (unsigned int i = 0; i < collidingObjects.size(); i++)
		{
			if (other == collidingObjects[i])
			{
				collidingObjects.erase(collidingObjects.begin() + i);
				break;
			}
		}
	}

	__raise eOnEndOverlap(other);
}

void GameObject::CheckOverlaps(GameObject* other, bool overlapped)
{
	temp = false;

	if (overlapped)
	{
		for (unsigned int i = 0; i < collidingObjects.size(); i++)
		{
			if (other == collidingObjects[i])
			{
				temp = true; //Were already overlapping
				break;
			}
		}

		if (temp)
		{
			OnOverlapping(other);
			other->OnOverlapping(this);
		}
		else
		{
			OnBeginOverlap(other);
			other->OnBeginOverlap(this);
		}
	}
	else
	{
		for (unsigned int i = 0; i < collidingObjects.size(); i++)
		{
			if (other == collidingObjects[i])
			{
				OnEndOverlap(other, true, i);
				other->OnEndOverlap(this, false);

				break;
			}
		}
	}
}