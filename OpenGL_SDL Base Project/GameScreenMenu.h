#ifndef _GAMESCREENMENU_H
#define _GAMESCREENMENU_H

#include "GameScreen.h"

class GameScreenMenu : public GameScreen
{
public:
	GameScreenMenu();
	~GameScreenMenu() {}

	void Update(float deltaTime);
	void Render();

protected:
	int chosenOption = 0;

	string level1Info = "Level 1: \n\nW - Forward\nS - Backwards\nA - Left\nD - Right\n\nSimply make it to\nthe far corner's\nbox to win!";
	string level2Info = "Level 2: \n\nW - Forward\nS - Backwards\nA - Left\nD - Right\nMouse - Aim\nLeft Click - Fire";
	string quitInfo = "Quit the game";

	string* currentInfo;
};

#endif