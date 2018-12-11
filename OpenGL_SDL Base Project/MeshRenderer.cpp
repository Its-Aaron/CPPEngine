#include "MeshRenderer.h"
#include "Mesh.h"
#include <time.h>
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "../gl/glut.h"
#include "Constants.h"

MeshRenderer::MeshRenderer()
{
	componentTypeName = "MeshRenderer";
}

MeshRenderer::MeshRenderer(Mesh* _mesh)
{
	componentTypeName = "MeshRenderer";
	mesh = _mesh;
}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::Init()
{
	transform = gameObject->transform;
	initialised = true;
}

void MeshRenderer::Update(float deltaTime)
{
}

void MeshRenderer::Render()
{
	if (mesh != nullptr && isRendering)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glScalef(offsets.scale.x, offsets.scale.y, offsets.scale.z);

		glTranslatef(offsets.position.x, offsets.position.y, offsets.position.z);

		glRotatef(offsets.rotation.x, 1, 0, 0);
		glRotatef(offsets.rotation.y, 0, 1, 0);
		glRotatef(offsets.rotation.z, 0, 0, 1);
		
		if (renderRed)
			glColor3f(1, 0, 0);
		else
			glColor3f(1, 1, 1);

		glCallList(mesh->GetListNumber());

		glPopMatrix();

	}
}