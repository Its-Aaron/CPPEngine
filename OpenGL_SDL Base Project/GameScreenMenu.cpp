#include "GameScreenMenu.h"

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	// not using perspective projection for this exercise
	gluPerspective(60.0f, aspect, 0.1f, 1000.0f);
	// use orthographic (flat) projection
	//glOrtho(-5, 5, -5 / aspect, 5 / aspect, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	//Background colour.
	glClearColor(.7f, .7f, .7f, 1.0f);
	// specify line rather than filled polygons
	glPolygonMode(GL_FRONT, GL_LINE);
	//enable lighting
	//glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);


	//Texture stuff
	glEnable(GL_TEXTURE_2D);

}

void GameScreenMenu::Update(float deltaTime)
{
	if (input->GetKeyDown(SDLK_s))
		chosenOption++;
	if (input->GetKeyDown(SDLK_w))
		chosenOption--;
	if (input->GetKeyDown(SDLK_SPACE))
	{
		switch (chosenOption)
		{
		case 0:
			ChangeLevel(SCREEN_LEVEL1);
			break;
		case 1:
			ChangeLevel(SCREEN_LEVEL2);
			break;
		case 2:
			QuitGame();
			break;
		default:
			break;
		}
	}

	if (chosenOption > 2)
		chosenOption = 0;
	if (chosenOption < 0)
		chosenOption = 2;

	GameScreen::Update(deltaTime);
}

void GameScreenMenu::Render()
{
	GameScreen::Render();

	currentInfo = chosenOption == 0 || chosenOption == 1 ? (chosenOption == 0 ? &level1Info : &level2Info) : &quitInfo;

	OutputLines(1, SCREEN_HEIGHT / 2, *currentInfo, TLEFT, 25.0f);

	OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 120, "Use W and S to choose an option");
	OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 100, "Press space to confirm");

	OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 40, chosenOption == 0 ? "Level One <-" : "Level One");
	OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2), chosenOption == 1 ? "Level Two <-" : "Level Two");
	OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 40, chosenOption == 2 ? "Exit Game <-" : "Exit Game");
}