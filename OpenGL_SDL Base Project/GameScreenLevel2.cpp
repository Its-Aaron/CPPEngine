#include "GameScreenLevel2.h"
#include "ClassIncludes.h"
#include <time.h>
#include <sstream>
#include <iomanip>

GameScreenLevel2::GameScreenLevel2() : GameScreen()
{
	srand(time(NULL));
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

	glEnable(GL_TEXTURE_2D);

	cam->mobile = true;

	InstantiateObjects();

}

GameScreenLevel2::~GameScreenLevel2()
{}

void GameScreenLevel2::InstantiateObjects()
{
	//-------STALL
	GameObject* stall = new GameObject(new ShootingStall(crate));
	stall->name = "stall";
	stall->transform->position.z = 10;
	stallScript = (ShootingStall*)stall->GetComponent("ShootingStall");
	__hook(&ShootingStall::eOnGainPoints, stallScript, &GameScreenLevel2::GainPoints);
	Transform offset = Transform(Vector3D(), Vector3D(0.75, 0.75, 0.75), Vector3D());
	stallScript->AddTargetMesh(crate, offset, hitWood);
	offset = Transform(Vector3D(0, -2, 2), Vector3D(0.75, 0.75, 0.75), Vector3D(-90, 0, -90));
	stallScript->AddTargetMesh(toilet, offset, hitCeramic);
	offset = Transform(Vector3D(0, -60, 0), Vector3D(0.01f, 0.01f, 0.02f), Vector3D());
	stallScript->AddTargetMesh(man, offset, hitMetal);
	//----------

	//---------- FLOORS
	GameObject* playerFloor = new GameObject(new MeshRenderer(crate));
	playerFloor->name = "floor";
	playerFloor->transform->position = Vector3D(9, -1, -10);
	playerFloor->transform->scale = Vector3D(7, 1, 10);
	BoxCollider* boundsCollider = (BoxCollider*)playerFloor->AttachCollider(BOX);
	boundsCollider->dimensions = Vector3D(2, 5, 2);
	boundsCollider->CalculateBounds();

	GameObject* AIFloor = new GameObject(new MeshRenderer(crate));
	AIFloor->name = "floor";
	AIFloor->transform->position = Vector3D(-9, -1, -10);
	AIFloor->transform->scale = Vector3D(7, 1, 10);
	//-----------

	//------PLAYER
	GameObject* player = new GameObject(new FPSCharacter());
	player->name = "player";
	player->transform->position = Vector3D(9, 6, -10);
	player->transform->rotation = Vector3D();
	player->SetTag("Player");
	((FPSCharacter*)player->GetComponent("FPSCharacter"))->SetRestrictedToBoundsCollider(true, boundsCollider);

	GameObject* gun = new GameObject(new Shooter(ball, &cam->transform));
	gun->name = "gun";
	playerGun = (Shooter*)gun->GetComponent("Shooter");
	playerGun->projectileMeshOffsets.scale = Vector3D(0.01f, 0.01f, 0.01f);
	playerGun->owner = player;
	//----------

	
	GameObject* AI = new GameObject({ new Shooter(ball), new AITargetShooter() , new MeshRenderer(toilet) });
	AI->name = "AI";
	AI->transform->position = Vector3D(-9, 6, -10);
	aiGun = (Shooter*)AI->GetComponent("Shooter");
	AITargetShooter* aiScript = (AITargetShooter*)AI->GetComponent("AITargetShooter");
	aiScript->SetGun(aiGun);
	aiScript->stall = stallScript;
	aiGun->projectileMeshOffsets.scale = Vector3D(0.01f, 0.01f, 0.01f);
	aiGun->owner = AI;

}

void GameScreenLevel2::Update(float deltaTime) 
{

	if (currentState == PLAYING)
	{
		GameScreen::Update(deltaTime);
		if (timeLeft > 0)
			timeLeft -= deltaTime;
		else
		{
			timeLeft = 0;
			SetGameState(playerPoints > aiPoints ? WINSCREEN : (playerPoints == aiPoints ? TIESCREEN: LOSESCREEN));
		}
	}

	if (input->GetKeyDown(SDLK_SPACE))
	{
		if (currentState == PAUSED)
			SetGameState(PLAYING);
		if (currentState == LOSESCREEN || currentState == WINSCREEN || currentState == TIESCREEN)
			ChangeLevel(SCREEN_MENU);
	}

}

void GameScreenLevel2::Render()
{
	GameScreen::Render();

	stringstream stream;
	stream << fixed << setprecision(2) << timeLeft;

	if (currentState == PLAYING)
	{
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20, "Time remaining: " + stream.str(), TCENTRE);
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 40, "Player score: " + to_string(playerPoints), TCENTRE);
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60, "AI score: " + to_string(aiPoints), TCENTRE);
		string reloadingString = "Ammo : " + (string)(playerGun->GetReloading() ? "Reloading!" : "Ready to fire");
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80, reloadingString, TCENTRE);
	}
	else if (currentState == PAUSED)
	{
		OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 10, "Press spacebar to start.", TCENTRE, Colour(1,1,1));
		OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) -10, "Try to knock down as many targets as you can!", TCENTRE, Colour(1, 1, 1));
	}
	else if (currentState == LOSESCREEN || currentState == WINSCREEN || currentState == TIESCREEN)
	{
		string stateString;
		switch (currentState)
		{
		case WINSCREEN:
			stateString = "You win!";
			break;
		case LOSESCREEN:
			stateString = "You lose";
			break;
		case TIESCREEN:
			stateString = "It's a tie!";
				break;
		default:
			break;
		}
		OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 30, stateString, TCENTRE, Colour(1, 1, 1));
		OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 10, "Player score: " + to_string(playerPoints), TCENTRE, Colour(1, 1, 1));
		OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 10, "AI score: " + to_string(aiPoints), TCENTRE, Colour(1, 1, 1));
		OutputLine(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 30, "Press spacebar to return to menu", TCENTRE, Colour(1, 1, 1));
	}
}