#include "Light.h"
#include "Transform.h"

Light::Light(GLenum _lightName)
{
	componentTypeName = "Light";
	glEnable(_lightName);
	lightName = _lightName;
	fullbright = false;

	ambient = Vector3D(0.2f, 0.2f, 0.2f);
	diffuse = Vector3D(0.7f, 0.7f, 0.7f);
	specular = Vector3D(0.5f, 0.5f, 0.5f);

	spotlight = false;
	transparency = 1;
	cutoff = 180;
}

Light::~Light()
{}

void Light::Initialise()
{
	transform = gameObject->transform;
	initialised = true;
}

void Light::Render()
{
	if (!initialised)
		Initialise();

	if (cutoff < 0 || (cutoff > 90 && cutoff != 180))
		cutoff = 180;

	GLfloat renderAmbient[4] = { fullbright ? 1.0f : ambient.x, fullbright ? 1.0f : ambient.y, fullbright ? 1.0f : ambient.z, 1.0f };
	GLfloat renderDiffuse[4] = { fullbright ? 1.0f : diffuse.x, fullbright ? 1.0f : diffuse.y, fullbright ? 1.0f : diffuse.z, 1.0f };
	GLfloat renderSpecular[4] = { fullbright ? 1.0f : specular.x, fullbright ? 1.0f : specular.y, fullbright ? 1.0f : specular.z, 1.0f };
	GLfloat renderPosition[4] = { transform->position.x, transform->position.y, transform->position.z, (float)spotlight };
	GLfloat renderRotation[3] = { transform->rotation.x, transform->rotation.y, transform->rotation.z }; //gameObject->transform->rotation.z};
	GLfloat cutoff[1] = { 90.0f };


	glLightfv(lightName, GL_AMBIENT, renderAmbient);
	glLightfv(lightName, GL_DIFFUSE, renderDiffuse);
	glLightfv(lightName, GL_SPECULAR, renderSpecular);
	glLightfv(lightName, GL_POSITION, renderPosition);

	if (spotlight)
	{
		glLightfv(lightName, GL_SPOT_DIRECTION, renderRotation);
		glLightfv(lightName, GL_SPOT_CUTOFF, cutoff);
	}
}