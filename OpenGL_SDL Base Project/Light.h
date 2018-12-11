#ifndef _LIGHT_H
#define _LIGHT_H

#include "Commons.h"
#include "Component.h"
#include <SDL_opengl.h>
#include "Transform.h"


class Light : public Component
{
public:
	Light(GLenum _lightName);
	~Light();

	Transform* transform;
	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
	GLenum lightName;
	bool spotlight;
	bool fullbright;
	//0 - 1
	float transparency;
	float cutoff;

	void Render();
private:
	void Initialise();
};

#endif