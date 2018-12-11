#include "GameObjectManager.h"
#include <algorithm>

static GameObjectManager* instance = 0;

GameObjectManager* GameObjectManager::GetInstance()
{
	if (instance == 0)
		instance = new GameObjectManager();

	return instance;
}

GameObjectManager::GameObjectManager()
{
	input = Input::GetInstance();
}

GameObjectManager::~GameObjectManager()
{}

void GameObjectManager::Update(float deltaTime)
{
	if (waitingForDeletion.size() > 0)
		DeleteQueuedObjects();

	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Update(deltaTime);

	if (debug)
	{
		objects[debugObjectIndex]->debug = false;

		if (input->GetKeyDown(SDLK_m))
			debugObjectIndex++;
		if (input->GetKeyDown(SDLK_n))
			debugObjectIndex--;

		if (debugObjectIndex < 0)
			debugObjectIndex = (int)objects.size();
		if (debugObjectIndex > (int)objects.size())
			debugObjectIndex = 0;

		objects[debugObjectIndex]->debug = true;
	}
}

void GameObjectManager::Render()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

void GameObjectManager::Destroy(GameObject* go, float time)
{
	if (time == 0)
		waitingForDeletion.push_back(go);
	else
		go->Destroy(go, time);
}

void GameObjectManager::Destroy(GameObject* go)
{
	waitingForDeletion.push_back(go);
}

void GameObjectManager::DeleteQueuedObjects()
{
	for (unsigned int i = 0; i < waitingForDeletion.size();)
	{
		for (unsigned int j = 0; j < objects.size(); j++)
		{
			if (objects[j] == waitingForDeletion[i])
			{
				objects.erase(objects.begin() + j);
				break;
			}
		}
		if (waitingForDeletion[i] != NULL)
			delete waitingForDeletion[i];
		waitingForDeletion.erase(waitingForDeletion.begin());
	}
}