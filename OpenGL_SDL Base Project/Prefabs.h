#ifndef _PREFABS_H
#define _PREFABS_H

#include "GameObject.h"
class Mesh;


class Prefabs
{
public:
	static Prefabs* GetInstance();
	GameObject* GetCannon(Mesh* ballMesh, bool visible = false);

private:
	Prefabs() {}
	~Prefabs() {}
};

#endif