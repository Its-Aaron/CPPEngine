#ifndef _CUBE_H
#define _CUBE_H

#include <SDL.h>

class Cube
{
public:
	Cube();
	~Cube();

	void Jump(float height = 8.0f);
	void Update(float deltaTime, SDL_Event e);
	void Render();
	float posX = 0;
	float posY = 0;
	float posZ = 0;
	float size = 1;

private:
	float yVelocity = 0;
	float gravityScale = 25;
	bool canJump = true;
	void CheckVelocity(float deltaTime);
};

#endif