#include "Camera.h"

static Camera* instance = 0;
static float moveSpeed = 20;
static float turnSpeed = 2;

Camera* Camera::GetInstance()
{
	if (instance == 0)
		instance = new Camera();

	return instance;
}

Camera::Camera()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	transform = Transform();
	transform.position = Vector3D(0, 5, -10);
	initialised = false;
	input = Input::GetInstance();
	fps = true;
}

Camera::~Camera()
{}

//void Camera::Init()
//{
//	gameObject = new GameObject(this);
//	transform = gameObject->transform;
//	initialised = true;
//}

#include <SDL_opengl.h>
#include <GL\glu.h>

void Camera::Render()
{
	Vector3D lookAt = transform.position + transform.ForwardVector();

	gluLookAt
	(
		transform.position.x, transform.position.y, transform.position.z,
		lookAt.x, lookAt.y, lookAt.z,
		transform.UpVector().x, transform.UpVector().y, transform.UpVector().z
	);
}

void Camera::Update(float deltaTime)
{
	if (mobile && !lockedToOtherTransform)
	{
		float currentSpeed = (input->GetKey(SDLK_LCTRL)) ? 0.25f : 1.0f;

		if (input->GetMouseMotion().first != 0)
			transform.rotation.y -= input->GetMouseMotion().first * deltaTime * turnSpeed;
		if (input->GetMouseMotion().second != 0)
			transform.rotation.z -= input->GetMouseMotion().second * deltaTime * turnSpeed;
	
		if (input->GetKey(SDLK_d))
			transform.position += transform.RightVector() * ((moveSpeed * currentSpeed) * deltaTime);
		if (input->GetKey(SDLK_a))
			transform.position -= transform.RightVector() * ((moveSpeed * currentSpeed) * deltaTime);

		if (input->GetKey(SDLK_w))
		{
			if (input->GetKey(SDLK_LSHIFT))
				transform.position += transform.UpVector() * ((moveSpeed * currentSpeed) * deltaTime);
			else transform.position += transform.ForwardVector() * ((moveSpeed * currentSpeed) * deltaTime);
		}
		if (input->GetKey(SDLK_s))
		{
			if (input->GetKey(SDLK_LSHIFT))
				transform.position -= transform.UpVector() * ((moveSpeed * currentSpeed) * deltaTime);
			else transform.position -= transform.ForwardVector() * ((moveSpeed * currentSpeed) * deltaTime);
		}

		if (transform.rotation.y > 360)
			transform.rotation.y -= 360;
		if (transform.rotation.y < -360)
			transform.rotation.y += 360;

		if (transform.rotation.z > 360)
			transform.rotation.z -= 360;
		if (transform.rotation.z < -360)
			transform.rotation.z += 360;
	}
	else if (lockedToOtherTransform)
		transform = *lockedTransform;
}