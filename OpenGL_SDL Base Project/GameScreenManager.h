#ifndef _GAMESCREENMANAGER_H
#define _GAMESCREENMANAGER_H

#include <SDL.h>
#include <vector>
#include "Commons.h"

class GameScreen;

[event_receiver(native)]
class GameScreenManager
{
public:
	GameScreenManager(SCREENS startScreen);
	~GameScreenManager();

	void Render();
	void Update(float deltaTime);

	void InitiateScreenChange(SCREENS newScreen);
	void ChangeScreens();

private:
	GameScreen* mCurrentScreen = nullptr;
	GameScreen* mPreviousScreen = nullptr;
	GameScreen* mNextScreen = nullptr;
};


#endif //_GAMESCREENMANAGER_H