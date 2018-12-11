#include "Input.h"
#include <iostream>

static Input* instance = 0;

Input* Input::GetInstance()
{
	if (instance == 0)
		instance = new Input();

	return instance;
}

Input::Input()
{
	for (int i = 0; i < 284; i++)
	{
		if (i == 0 || (i > 3 && i < 284))
		{
			keyDownFromScancodes[i] = false;
			keyUpFromScancodes[i] = false;
			keyHeldFromScancodes[i] = false;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		mouseButtonDown[i] = false;
		mouseButtonUp[i] = false;
		mouseButtonHeld[i] = false;
	}

	xMotion = 0;
	yMotion = 0;
}

Input::~Input()
{
}

void Input::CheckEventInfo(SDL_Event *k)
{
	switch (k->type)
	{
	case SDL_KEYUP:
		keyUpFromScancodes[k->key.keysym.scancode] = true;
		keyHeldFromScancodes[k->key.keysym.scancode] = false;
		break;

	case SDL_KEYDOWN:
		if (!keyHeldFromScancodes[k->key.keysym.scancode])
		{
			keyDownFromScancodes[k->key.keysym.scancode] = true;
			keyHeldFromScancodes[k->key.keysym.scancode] = true;
		}
		else
			keyDownFromScancodes[k->key.keysym.scancode] = false;
		break;

	case SDL_MOUSEBUTTONUP:
		mouseButtonUp[k->button.button] = true;
		mouseButtonHeld[k->button.button] = false;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!mouseButtonHeld[k->button.button])
		{
			mouseButtonDown[k->button.button] = true;
			mouseButtonHeld[k->button.button] = true;
		}
		else
			mouseButtonDown[k->button.button] = false;
		break;
		
	case SDL_MOUSEMOTION:
		xMotion += k->motion.xrel;
		yMotion += k->motion.yrel;
		break;
		
	}
}

void Input::Update()
{
	for (int i = 0; i < 284; i++)
	{
		if (i == 0 || (i > 3 && i < 284))
		{
			keyDownFromScancodes[i] = false;
			keyUpFromScancodes[i] = false;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		mouseButtonDown[i] = false;
		mouseButtonUp[i] = false;
	}

	xMotion = 0;
	yMotion = 0;
	
}