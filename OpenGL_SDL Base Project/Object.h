#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "Input.h"

class Object
{
public:
	Object();
	~Object();

	Input* input;

	virtual void Update(float deltaTime) {}

	std::string name = "Default";

protected:
	bool initialised;

};


#endif