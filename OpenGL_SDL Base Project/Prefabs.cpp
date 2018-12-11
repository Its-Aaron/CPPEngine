#include "Prefabs.h"
#include "ClassIncludes.h"

static Prefabs* instance = 0;

Prefabs* Prefabs::GetInstance()
{
	if (instance == 0)
		instance = new Prefabs();

	return instance;
}

GameObject* Prefabs::GetCannon(Mesh* ballMesh, bool visible)
{
	GameObject* cannon = new GameObject({ new Cannon(ballMesh), new MeshRenderer(ballMesh) });
	cannon->SetTag("Cannon");
	MeshRenderer* mr = (MeshRenderer*)cannon->GetComponent("MeshRenderer");
	mr->isRendering = visible;
	mr->offsets.scale = Vector3D(0.05f, 0.05f, 0.05f);
	Cannon* cScript = (Cannon*)cannon->GetComponent("Cannon");
	cScript->fireStrength = 40;
	cScript->fireRateMin = 1.0f;
	cScript->fireRateMax = 3.0f;
	cScript->automatic = true;
	cScript->randomFireInRange = true;
	cScript->randomSpreadInRange = true;
	cScript->randomSpreadAmount = 0.2f;
	/*
	Rotator* rt = (Rotator*)cannon->AttachComponent(new Rotator());
	rt->SetWobble(true);
	*/

	return cannon;
}