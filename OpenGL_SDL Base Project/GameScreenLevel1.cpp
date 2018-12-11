#include "GameScreenLevel1.h"
#include <time.h>
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "../gl/glut.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include "ClassIncludes.h"

using namespace::std;
//-------------------------------------------------------------------
GameScreenLevel1::GameScreenLevel1() : GameScreen()
{
	InstantiateObjects();
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


	//Texture stuff
	glEnable(GL_TEXTURE_2D);

	cam->transform.position = Vector3D(0, 30, -70);
	cam->transform.rotation = Vector3D(0, 0, -40);
	//cam->mobile = true;

	sScream = Mix_LoadWAV_RW(SDL_RWFromFile("Scream.wav", "rb"), 1);
	sFanfare = Mix_LoadWAV_RW(SDL_RWFromFile("fanfare.wav", "rb"), 1);
}
//-------------------------------------------------------------------
GameScreenLevel1::~GameScreenLevel1()
{}

void GameScreenLevel1::InstantiateObjects()
{
	denzil = new GameObject({ new MeshRenderer(man), new MovingCharacter() });
	denzil->SetTag("Denzil");
	denzil->transform->position = Vector3D(20, 0, -47);
	((MeshRenderer*)denzil->GetComponent("MeshRenderer"))->offsets.scale = Vector3D(0.05f, 0.05f, 0.05f);
	((MovingCharacter*)denzil->GetComponent("MovingCharacter"))->forcedInBounds = true;
	__hook(&MovingCharacter::eOnHitByBall, (MovingCharacter*)denzil->GetComponent("MovingCharacter"), &GameScreenLevel1::IncrementHitCount);
	BoxCollider* dbc = (BoxCollider*)denzil->AttachCollider(BOX);
	dbc->dimensions = Vector3D(2, 10, 2);
	dbc->offset.y += 5;

	GameObject* trig = new GameObject(new TriggerVolume());
	trig->SetTag("TriggerVolume");
	BoxCollider* trigBC = (BoxCollider*)trig->GetComponent("BoxCollider");
	trigBC->dimensions = Vector3D(3, 5, 8);
	trig->transform->position = Vector3D(-20.5f, 0, 44);

	__hook(&GameObject::eOnBeginOverlap, trig, &GameScreenLevel1::CheckForEndLevel);

	GameObject* cannonB = prefab->GetCannon(ball);
	cannonB->transform->rotation = Vector3D(0, 90, -15);
	cannonB->transform->position = Vector3D(-30, 20, 30);

	GameObject* cannonC = prefab->GetCannon(ball);
	cannonC->transform->rotation = Vector3D(0, -90, -15);
	cannonC->transform->position = Vector3D(30, 20, 20);

	GameObject* cannonD = prefab->GetCannon(ball);
	cannonD->transform->rotation = Vector3D(0, 90, -15);
	cannonD->transform->position = Vector3D(-30, 20, 10);

	GameObject* cannonE = prefab->GetCannon(ball);
	cannonE->transform->rotation = Vector3D(0, -90, -15);
	cannonE->transform->position = Vector3D(30, 20, 0);

	GameObject* cannonF = prefab->GetCannon(ball);
	cannonF->transform->rotation = Vector3D(0, 90, -15);
	cannonF->transform->position = Vector3D(-30, 20, -10);

	GameObject* cannonG = prefab->GetCannon(ball);
	cannonG->transform->rotation = Vector3D(0, -90, -15);
	cannonG->transform->position = Vector3D(30, 20, -20);

	GameObject* cannonH = prefab->GetCannon(ball);
	cannonH->transform->rotation = Vector3D(0, 90, -15);
	cannonH->transform->position = Vector3D(-30, 20, -30);


#pragma endregion Balls

	CreateGameObject("Court", new MeshRenderer(court));
	lastItemCreated->transform->position.y = -4.2f;
	lastItemCreated->SetTag("Court");
	BoxCollider* bc = (BoxCollider*)lastItemCreated->AttachComponent(colliderManager->AddBoxCollider());
	bc->dimensions = Vector3D(51.2f, 6.4f, 102.4f);
}

//-------------------------------------------------------------------
void GameScreenLevel1::Update(float deltaTime)
{
	if (currentState == PLAYING)
		GameScreen::Update(deltaTime);

	if (input->GetKeyDown(SDLK_SPACE))
	{
		if (currentState == PAUSED)
			SetGameState(PLAYING);
		if (currentState == LOSESCREEN)
			ChangeLevel(SCREEN_MENU);
		if (currentState == WINSCREEN)
			ChangeLevel(SCREEN_LEVEL2);
	}

}

void GameScreenLevel1::Render()
{
	GameScreen::Render();

	stringstream stream;
	stream << fixed << setprecision(2) << mCurrentTime;
	timeString = stream.str();

	OutputLine(1, SCREEN_HEIGHT - 20, "Time: " + timeString, TLEFT);
	OutputLine(1, SCREEN_HEIGHT - 40, "Hits: " + to_string(hitCount), TLEFT);

	if (currentState == PAUSED)
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "Press spacebar to begin!");
	if (currentState == LOSESCREEN)
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "Game over! Press spacebar to return to menu");
	if (currentState == WINSCREEN)
	{
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 52,	"You win!");
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 26,	"Balls touched : " + to_string(hitCount));
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,			"Time: " + timeString);
		OutputLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 26,	"Press space to continue");
	}
}

//-------------------------------------------------------------------

void GameScreenLevel1::IncrementHitCount(GameObject* theBall) 
{ 
	if (++hitCount == 4) 
		GameOver(); 
	theBall->CancelDestroy(true);
	//((RigidBody*)theBall->GetComponent("RigidBody"))->enabled = false;
	//denzil->SetChild(theBall); 
	//theBall->transform->position = Vector3D(0, 0, -3 * hitCount);
	switch (hitCount)
	{
	case 1:
		((MovingCharacter*)denzil->GetComponent("MovingCharacter"))->ball1RB = (RigidBody*)theBall->GetComponent("RigidBody");
		break;
	case 2:
		((MovingCharacter*)denzil->GetComponent("MovingCharacter"))->ball2RB = (RigidBody*)theBall->GetComponent("RigidBody");
		break;
	case 3:
		((MovingCharacter*)denzil->GetComponent("MovingCharacter"))->ball3RB = (RigidBody*)theBall->GetComponent("RigidBody");
		break;
	}
	theBall->DestroyComponent(theBall->GetComponent("SphereCollider"));
}

void GameScreenLevel1::EndLevel()
{
	PlayMySound(sFanfare); 
	SetGameState(WINSCREEN);
}