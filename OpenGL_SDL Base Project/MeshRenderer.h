#ifndef _MESHRENDERER_H
#define _MESHRENDERER_H

#include "Component.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* _mesh);
	~MeshRenderer();
	
	Mesh* mesh;

	void Update(float deltaTime);
	void Render();

	bool isRendering = true;
	Transform offsets;

	bool renderRed = false;

private:
	Transform* transform;
	void Init();
};

#endif