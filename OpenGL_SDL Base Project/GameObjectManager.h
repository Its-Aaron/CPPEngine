#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H

#include "GameObject.h"

class GameObjectManager
{
public:
	static GameObjectManager* GetInstance();
	void AddGameObject(GameObject* go) { objects.push_back(go); }
	void Destroy(GameObject* go, float time);
	void Destroy(GameObject* go);

	void Update(float deltaTime);
	void Render();

	vector<GameObject*> GetObjects() { return objects; }

	void DestroyAll() { for (unsigned int i = 0; i < objects.size(); i++) Destroy(objects[i]); DeleteQueuedObjects(); }

	bool debug = false;

private:
	GameObjectManager();
	~GameObjectManager();

	vector<GameObject*> objects;

	void DeleteQueuedObjects();

	vector<GameObject*> waitingForDeletion;

	int debugObjectIndex = 0;

	Input* input;
};

#endif