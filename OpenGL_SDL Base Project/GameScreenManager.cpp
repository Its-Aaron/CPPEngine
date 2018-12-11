#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenMenu.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "Commons.h"
#include "GameObjectManager.h"

//--------------------------------------------------------------------------------------------------

GameScreenManager::GameScreenManager(SCREENS startScreen)
{
	InitiateScreenChange(startScreen);
}

//--------------------------------------------------------------------------------------------------

GameScreenManager::~GameScreenManager()
{
	//delete mCurrentScreen;
	mCurrentScreen = NULL;
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::Render()
{
	if (mCurrentScreen != nullptr)
		mCurrentScreen->Render();
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::Update(float deltaTime)
{
	if (mCurrentScreen != nullptr)
		mCurrentScreen->Update(deltaTime);

	if (mNextScreen != nullptr)
		ChangeScreens();
}

//--------------------------------------------------------------------------------------------------

void GameScreenManager::InitiateScreenChange(SCREENS newScreen)
{
	if (mCurrentScreen != nullptr)
	{
		//__unhook(&GameScreen::eOnLevelChange, mCurrentScreen, &GameScreenManager::InitiateScreenChange);
		mPreviousScreen = mCurrentScreen;
		GameObjectManager::GetInstance()->DestroyAll();
	}

		GameScreenMenu* tempScreen0;
		GameScreenLevel1* tempScreen1;
		GameScreenLevel2* tempScreen2;

		//Initialise the new screen.
		switch (newScreen)
		{
		case SCREEN_INTRO:
			break;

		case SCREEN_MENU:
			tempScreen0 = new GameScreenMenu();
			mNextScreen = (GameScreen*)tempScreen0;
			__hook(&GameScreen::eOnLevelChange, mNextScreen, &GameScreenManager::InitiateScreenChange);
			tempScreen0 = NULL;
			break;

		case SCREEN_LEVEL1:
			tempScreen1 = new GameScreenLevel1();
			mNextScreen = (GameScreen*)tempScreen1;
			__hook(&GameScreen::eOnLevelChange, mNextScreen, &GameScreenManager::InitiateScreenChange);
			tempScreen1 = NULL;
			break;

		case SCREEN_LEVEL2:
			tempScreen2 = new GameScreenLevel2();
			mNextScreen = (GameScreen*)tempScreen2;
			__hook(&GameScreen::eOnLevelChange, mNextScreen, &GameScreenManager::InitiateScreenChange);
			tempScreen2 = NULL;
			break;

		case SCREEN_GAMEOVER:
			break;

		case SCREEN_HIGHSCORES:
			break;

		default:
			break;
		}
}

void GameScreenManager::ChangeScreens()
{
	if (mPreviousScreen != nullptr)
	{
		mCurrentScreen = nullptr;
		delete mPreviousScreen;
		mPreviousScreen = nullptr;
	}

	mCurrentScreen = mNextScreen;
	mNextScreen = nullptr;
}

//--------------------------------------------------------------------------------------------------