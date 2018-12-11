#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H

#include <SDL.h>
#include "Commons.h"
#include "Constants.h"
#include "ClassIncludes.h"
#include <map>

// MANAGERS
class Input;
class ColliderManager;
class GameObjectManager;
class GameManager;
class Prefabs;

[event_source(native)]
class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime);
	void OutputLine(float x, float y, string text, TEXTALIGNMENT alignment = TCENTRE, Colour colour = Colour());
	int OutputLines(float _x, float _y,  string text, TEXTALIGNMENT alignment = TCENTRE, float newLineOffset = 5.0f, float tabInset = 1.0f);
	void SetGameState(GAMESTATE newState) { cout << "State changed to " << stateNames[newState] << endl; lastState = currentState; currentState = newState; }

	void QuitGame();

	vector<Mesh*> meshes;
	vector<Light*> lights;

	__event void eOnLevelChange(SCREENS newScreen);

	void ChangeLevel(SCREENS newScreen);

protected:

	//------------SINGLETONS
	Input* input;
	ColliderManager* colliderManager;
	GameObjectManager* goManager;
	Camera* cam;
	Prefabs* prefab;

	//-----------FUNCTIONS
	GameObject*		CreateGameObject(string name);
	GameObject*		CreateGameObject(string name, GameObject* parent);
	GameObject*		CreateGameObject(string name, Component* component);
	GameObject*		CreateGameObject(string name, vector<Component*> components);
	GameObject*		CreateGameObject(string name, GameObject* parent, Component* component);
	GameObject*		CreateGameObject(string name, GameObject* parent, vector<Component*> components);
	GameObject* FindGameObject(string name);
	vector<GameObject*> FindGameObjectsByComponent(string componentName);

	Mesh* CreateMesh(char* path, char* texturePath = '\0') { meshes.push_back(new Mesh(path, texturePath)); return meshes[meshes.size() - 1]; }
	void Destroy(GameObject* go, float time = 0.0f);
	void PrintSceneGraph();
	void SetRasterPosition(int x, int y, int stringLength, TEXTALIGNMENT alignment, int fontSize = 24);

	//----------OTHER
	int totalTris;
	int totalQuads;

	bool gettingDeleted = false;

	float mCurrentTime = 0.0f;

	GameObject* lastItemCreated;

	GAMESTATE currentState = PAUSED;
	GAMESTATE lastState = UNKNOWN;

	std::map<GAMESTATE, string> stateNames;
};


#endif //_GAMESCREEN_H