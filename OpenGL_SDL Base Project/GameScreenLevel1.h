#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H

#include <SDL.h>
#include "GameScreen.h"
#include <vector>
class Texture;
#include "MovingCharacter.h"

[event_receiver(native)]
class GameScreenLevel1 : public GameScreen
{
//--------------------------------------------------------------------------------------------------
public:
	GameScreenLevel1();
	~GameScreenLevel1();

	void Update(float deltaTime);
	void Render();

//--------------------------------------------------------------------------------------------------

private:
	//---------- FUNCTIONS
	void InstantiateObjects();
	void CheckForEndLevel(GameObject* other) { if (other->GetTag() == "Denzil") EndLevel(); }
	void EndLevel();
	void GameOver() { PlayMySound(sScream); SetGameState(LOSESCREEN); }
	void IncrementHitCount(GameObject* theBall);

	//---------- MODELS
	Mesh* court = CreateMesh("MyCourt3.obj");
	Mesh* ball = CreateMesh("basketball.3ds", "basketball.tga");
	Mesh* man = CreateMesh("dummy.obj");

	//---------- SOUNDS
	Mix_Chunk* sScream;
	Mix_Chunk* sFanfare;
	
	//----------- OBJECTS
	GameObject* denzil;

	//---------- OTHER
	int hitCount = 0;
	bool gameOver = false;
	bool gameWon = false;
	string timeString;
};


#endif //_GAMESCREENLEVEL1_H