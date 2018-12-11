#ifndef _INPUT_H
#define _INPUT_H

#include "SDL.h"
#include <iostream>

#define DEBUG SDLK_RETURN
#define UP SDLK_UP
#define DOWN SDLK_DOWN
#define LEFT SDLK_LEFT
#define RIGHT SDLK_RIGHT
#define JUMP SDLK_SPACE
#define ROTATE SDLK_COMMA
#define LMB SDL_BUTTON_LEFT;
#define MMB SDL_BUTTON_MIDDLE;
#define RMB SDL_BUTTON_RIGHT;

class Input
{
public:
	static Input* GetInstance();

	bool GetKeyDown(SDL_Keycode x) { return keyDownFromScancodes[SDL_GetScancodeFromKey(x)]; }
	bool GetKeyUp(SDL_Keycode x) { return keyUpFromScancodes[SDL_GetScancodeFromKey(x)]; }
	bool GetKey(SDL_Keycode x) { return keyHeldFromScancodes[SDL_GetScancodeFromKey(x)]; }

	bool GetMouseDown(int x) { return mouseButtonDown[x]; }
	bool GetMouseUp(int x) { return mouseButtonUp[x]; }
	bool GetMouse(int x) { return mouseButtonHeld[x]; }

	std::pair<int, int> GetMouseMotion() { return std::make_pair(xMotion, yMotion); }

	void Update();
	void CheckEventInfo(SDL_Event *k);

private:
	Input();
	~Input();

	bool keyDownFromScancodes[284];
	bool keyUpFromScancodes[284];
	bool keyHeldFromScancodes[284];
	bool mouseButtonDown[5];
	bool mouseButtonUp[5];
	bool mouseButtonHeld[5];

	int xMotion;
	int yMotion;
};

#endif