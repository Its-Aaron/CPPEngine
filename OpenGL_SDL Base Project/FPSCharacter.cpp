#include "FPSCharacter.h"
#include "Camera.h"
#include "BoxCollider.h"

FPSCharacter::FPSCharacter()
{
	componentTypeName = "FPSCharacter";
}

void FPSCharacter::Update(float deltaTime)
{
	Move(deltaTime);

	if (restrictedToBounds)
		KeepInBounds();
}

void FPSCharacter::Init()
{
	cam = Camera::GetInstance();
	cam->SetLockedTransform(true, gameObject->transform);
}

void FPSCharacter::SetRestrictedToBoundsManual(bool restricted, Bounds _bounds)
{
	restrictedToBounds = restricted;
	bounds = _bounds;
	bcBounds = nullptr;
}

void FPSCharacter::SetRestrictedToBoundsCollider(bool restricted, BoxCollider* bc)
{
	restrictedToBounds = restricted;
	bcBounds = &bc->bounds;
	bounds = *bcBounds;
}

void FPSCharacter::Move(float deltaTime)
{
	if (input->GetKey(SDLK_LSHIFT))
		currentSpeed = sprintSpeed;
	else
		currentSpeed = moveSpeed;

	if (input->GetMouseMotion().first != 0)
		gameObject->transform->rotation.y -= input->GetMouseMotion().first * deltaTime * turnSpeed;
	if (input->GetMouseMotion().second != 0)
		gameObject->transform->rotation.z -= input->GetMouseMotion().second * deltaTime * turnSpeed;

	if (input->GetKey(SDLK_d))
		gameObject->transform->position += gameObject->transform->RightVector() * moveSpeed * deltaTime;
	if (input->GetKey(SDLK_a))
		gameObject->transform->position -= gameObject->transform->RightVector() * moveSpeed * deltaTime;

	if (input->GetKey(SDLK_w))
		gameObject->transform->position -= gameObject->transform->RightVector().Cross(Vector3D(0, 1, 0)) * currentSpeed * deltaTime;
	if (input->GetKey(SDLK_s))
		gameObject->transform->position += gameObject->transform->RightVector().Cross(Vector3D(0, 1, 0)) * moveSpeed * deltaTime;

	if (gameObject->transform->rotation.y > 360)
		gameObject->transform->rotation.y -= 360;
	if (gameObject->transform->rotation.y < -360)
		gameObject->transform->rotation.y += 360;

	if (gameObject->transform->rotation.z > 89)
		gameObject->transform->rotation.z = 89;
	if (gameObject->transform->rotation.z < -89)
		gameObject->transform->rotation.z = -89;
}

void FPSCharacter::KeepInBounds()
{
	if (bcBounds != nullptr)
		bounds = *bcBounds;

	if (transform->position.x < bounds.min.x)
		transform->position.x = bounds.min.x;

	if (transform->position.x > bounds.max.x)
		transform->position.x = bounds.max.x;

	if (transform->position.z < bounds.min.z)
		transform->position.z = bounds.min.z;

	if (transform->position.z > bounds.max.z)
		transform->position.z = bounds.max.z;
}

#include "glut.h"
#include "Constants.h"

void FPSCharacter::RenderCrosshair()
{
	glColor3f(0.0, 0.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);


	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glVertex2f((SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2));
	glVertex2f((SCREEN_WIDTH / 2) + 10, (SCREEN_HEIGHT / 2));
	glVertex2f((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 10);
	glVertex2f((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 10);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
}