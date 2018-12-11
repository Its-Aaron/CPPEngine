#include "GameScreen.h"
#include <time.h>
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "../gl/glut.h"
#include "Input.h"
#include "ColliderManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "Prefabs.h"

#include "ClassIncludes.h"


//--------------------------------------------------------------------------------------------------

GameScreen::GameScreen()
{
	prefab = Prefabs::GetInstance();
	cam = Camera::GetInstance();
	input = Input::GetInstance();
	colliderManager = ColliderManager::GetInstance();
	goManager = GameObjectManager::GetInstance();

	/*
	enum GAMESTATE
	{
	MENU,
	PAUSED,
	PLAYING,
	WINSCREEN,
	LOSESCREEN,
	TIESCREEN,
	UNKNOWN
	};
	*/

	stateNames[MENU] = "MENU";
	stateNames[PAUSED] = "PAUSED";
	stateNames[PLAYING] = "PLAYING";
	stateNames[WINSCREEN] = "WINSCREEN";
	stateNames[LOSESCREEN] = "LOSESCREEN";
	stateNames[TIESCREEN] = "TIESCREEN";
	stateNames[UNKNOWN] = "UNKNOWN";

}

//--------------------------------------------------------------------------------------------------

GameScreen::~GameScreen()
{
}

void GameScreen::ChangeLevel(SCREENS newScreen)
{ 
	__raise eOnLevelChange(newScreen);
}


//--------------------------------------------------------------------------------------------------


void GameScreen::QuitGame() 
{ 
	GameManager::GetInstance()->QuitGame(); 
}

void GameScreen::Render()
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		Camera::GetInstance()->Render();

		goManager->Render();

		colliderManager->Render();
}

//--------------------------------------------------------------------------------------------------

void GameScreen::Update(float deltaTime)
{
	if (currentState == PLAYING)
	{
		colliderManager->Update(deltaTime);
		mCurrentTime += deltaTime;

		Camera::GetInstance()->Update(deltaTime);

		goManager->Update(deltaTime);
	}
}

void GameScreen::OutputLine(float x, float y, string text, TEXTALIGNMENT alignment, Colour colour)
{
	glColor3f(colour.r, colour.g, colour.b);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	glColor3f(colour.r, colour.g, colour.b);

	SetRasterPosition(x, y, text.size(), alignment);

	for (unsigned int i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
}

int GameScreen::OutputLines(float _x, float _y, string text, TEXTALIGNMENT alignment, float newLineOffset, float tabInset)
{
	glColor3f(0.0, 0.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

	int extraLines = 0;
	float x = _x;
	float y = _y;

	SetRasterPosition(x, y, text.size(), alignment);

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text[i] == '\n')
		{
			y -= newLineOffset;
			extraLines++;
			SetRasterPosition(_x, y, text.size(), alignment);
		}

		if (text[i] == '\t')
		{
			x += tabInset;
			SetRasterPosition(x, y, text.size(), alignment);
		}


		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	
	return extraLines;
}

void GameScreen::SetRasterPosition(int x, int y, int stringLength,TEXTALIGNMENT alignment, int fontSize)
{
	switch (alignment)
	{
	case TLEFT:
		glRasterPos2f(x, y);
		break;
	case TCENTRE:
		glRasterPos2f(x - ((stringLength / 2) * (fontSize / 2)), y);
		break;
	case TRIGHT:
		glRasterPos2f(x - ((stringLength / 2) * fontSize), y);
		break;
	default:
		glRasterPos2f(x, y);
		break;
	}
}

GameObject* GameScreen::FindGameObject(string name)
{
	//for (unsigned int i = 0; i < objects.size(); i++)
	//{
	//	if (objects[i]->name == name)
	//		return objects[i];
	//}

	return nullptr;
}

vector<GameObject*> GameScreen::FindGameObjectsByComponent(string componentName)
{
	vector<GameObject*> returnVec;
	//for (unsigned int i = 0; i < objects.size(); i++)
	//{
	//	if (objects[i]->GetComponent(componentName) != nullptr)
	//		returnVec.push_back(objects[i]);
	//}

	return returnVec;
}

#pragma region CreateGameObject
GameObject* GameScreen::CreateGameObject(string name)
{
	lastItemCreated = new GameObject();

	lastItemCreated->name = name;

	return lastItemCreated;
}

GameObject* GameScreen::CreateGameObject(string name, GameObject* parent)
{
	lastItemCreated = new GameObject(parent);

	lastItemCreated->name = name;

	return lastItemCreated;
}

GameObject* GameScreen::CreateGameObject(string name, Component* component)
{
	lastItemCreated = new GameObject(component);

	lastItemCreated->name = name;

	return lastItemCreated;
}

GameObject* GameScreen::CreateGameObject(string name, vector<Component*> components)
{
	lastItemCreated = new GameObject();

	for (unsigned int i = 0; i < components.size(); i++)
		lastItemCreated->AttachComponent(components[i]);

	lastItemCreated->name = name;

	return lastItemCreated;
}

GameObject* GameScreen::CreateGameObject(string name, GameObject* parent, Component* component)
{
	lastItemCreated = new GameObject(parent, component);

	lastItemCreated->name = name;

	return lastItemCreated;
}

GameObject* GameScreen::CreateGameObject(string name, GameObject* parent, vector<Component*> components)
{
	lastItemCreated = new GameObject(parent);

	for (unsigned int i = 0; i < components.size(); i++)
		lastItemCreated->AttachComponent(components[i]);

	lastItemCreated->name = name;

	return lastItemCreated;
}
#pragma endregion All variations of CreateGameObject

void GameScreen::PrintSceneGraph()
{
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
	int x = 5;
	int y = 90;
	OutputLine(5, 10, "Tris: " + to_string(totalTris));
	OutputLine(5, 15, "Quads: " + to_string(totalQuads));
	int lineOffset = 3;
	int tabInset = 1;
	for (unsigned int i = 0; i < goManager->GetObjects().size(); i++)
	{
		string currentString = goManager->GetObjects()[i]->GetChildren();
		if (currentString != "Invalid Object")
		{
			int extraLines = OutputLines(x, y, currentString, TLEFT, lineOffset, tabInset);
			y -= extraLines * lineOffset;
		}
	}
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GameScreen::Destroy(GameObject* go, float time)
{
	goManager->Destroy(go, time);
}
//--------------------------------------------------------------------------------------------------