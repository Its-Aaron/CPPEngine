#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

class GameManager;
static GameManager* instance = 0;

class GameManager
{
public:
	static GameManager* GetInstance() { if (instance == 0) instance = new GameManager(); return instance; }

	bool QuittingGame() { return quitCalled; }

	void QuitGame() { quitCalled = true; }

private:
	GameManager() {}
	~GameManager() {}

	bool quitCalled = false;
};


#endif