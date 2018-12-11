#include "Cube.h"
#include <time.h>
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "../gl/glut.h"
#include "Constants.h"
#include <stdio.h>
#include <iostream>

using namespace::std;
Cube::Cube()
{
}

Cube::~Cube()
{
}

void Cube::Jump(float height)
{
	if (canJump)
	{
		yVelocity = height;
		posY = 0.0001f;
	}
}

void Cube::Render()
{
	glPushMatrix();
	glTranslatef(posX, posY, posZ);
	glScalef(size, size, size);
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glEnd();
	glPopMatrix();
}

void Cube::CheckVelocity(float deltaTime)
{
	if (posY <= 0)
	{
		yVelocity = 0;
		posY = 0;
		canJump = true;
	}
	else canJump = false;

	posY += yVelocity * deltaTime;

	yVelocity -= gravityScale * deltaTime;
}

void Cube::Update(float deltaTime, SDL_Event e)
{
	CheckVelocity(deltaTime);
	cout << posY << " " << canJump << endl;

	switch (e.type)
	{
	case SDL_KEYDOWN:
		Jump();
		break;
	}
}