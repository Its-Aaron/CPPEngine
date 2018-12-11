#ifndef _GAMESCREENLEVEL2_H
#define _GAMESCREENLEVEL2_H

#include <SDL.h>
#include "GameScreen.h"
#include <vector>
class Texture;
#include "MovingCharacter.h"

class GameScreenLevel2 : public GameScreen
{
	//--------------------------------------------------------------------------------------------------
public:
	GameScreenLevel2();
	~GameScreenLevel2();

	void Update(float deltaTime);
	void Render();

	//--------------------------------------------------------------------------------------------------

private:
	//---------- FUNCTIONS
	void InstantiateObjects();
	void EndLevel() { cout << "You did it" << endl; }
	void GainPoints(int _points, GameObject* shotBy) { if (shotBy->TagIs("Player")) playerPoints += _points; else aiPoints += _points; }
	void RenderCrosshair();

	//---------- MODELS
	Mesh* court = CreateMesh("MyCourt3.obj");
	Mesh* ball = CreateMesh("basketball.3ds", "basketball.tga");
	Mesh* man = CreateMesh("dummy.obj");
	Mesh* crate = CreateMesh("Crate1.obj");
	Mesh* toilet = CreateMesh("Toilet.3ds", "Toilet.tga");

	//---------- AUDIO
	Mix_Chunk* hitMetal = Mix_LoadWAV_RW(SDL_RWFromFile("hit_metal.wav", "rb"), 1);
	Mix_Chunk* hitCeramic = Mix_LoadWAV_RW(SDL_RWFromFile("hit_ceramic.wav", "rb"), 1);
	Mix_Chunk* hitWood = Mix_LoadWAV_RW(SDL_RWFromFile("hit_wood.wav", "rb"), 1);

	//------------------- OBJECTS
	ShootingStall* stallScript = nullptr;
	Shooter* playerGun = nullptr;
	Shooter* aiGun = nullptr;

	//--------- OTHER
	float timeLeft = 30.0f;
	int playerPoints = 0;
	int aiPoints = 0;
	int round = 1;

};


#endif //_GAMESCREENLEVEL1_H