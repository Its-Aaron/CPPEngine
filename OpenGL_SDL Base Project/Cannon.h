#ifndef _CANNON_H
#define _CANNON_H

#include "Component.h"
#include "Mesh.h"
#include <random>
#include "RigidBody.h"

class Cannon : public Component
{
public:
	Cannon(Mesh* ball);
	~Cannon();

	bool automatic = false;

	bool randomFireInRange = false;
	bool randomSpreadInRange = false;


	float fireRateMin = 2.0f;
	float fireRateMax = 5.0f;
	float fireRate = 1.0f;
	float randomSpreadAmount = 0.1f;
	float fireStrength = 20.0f;

	void Update(float deltaTime);
	void Fire();

private:
	float elapsedTime = 0.0f;
	Mesh* mesh;
	random_device rd;
	Mix_Chunk* sBounce = Mix_LoadWAV_RW(SDL_RWFromFile("Bounce.wav", "rb"), 1);
	void MakeBounceNoise(float ballPosToCamLength, float yVelocity);
	void DestroyBall(GameObject* go);
};

#endif