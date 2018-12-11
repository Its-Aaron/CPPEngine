#include "ShootingStall.h"
#include "MeshRenderer.h"

ShootingStall::ShootingStall(Mesh* _stallBox)
{
	stallBox = _stallBox;
	componentTypeName = "ShootingStall";
}

void ShootingStall::Init()
{
	GameObject* stallLeft = new GameObject(gameObject, new MeshRenderer(stallBox));
	stallLeft->transform->scale = Vector3D(5, 6, 1);
	stallLeft->transform->position = Vector3D(17, 6, 0);

	GameObject* stallRight = new GameObject(gameObject, new MeshRenderer(stallBox));
	stallRight->transform->scale = Vector3D(5, 6, 1);
	stallRight->transform->position = Vector3D(-17, 6, 0);

	GameObject* stallFront = new GameObject(gameObject, new MeshRenderer(stallBox));
	stallFront->transform->scale = Vector3D(12, 2, 1);
	stallFront->transform->position = Vector3D(0, 2, 0);

	GameObject* stallBack = new GameObject(gameObject, new MeshRenderer(stallBox));
	stallBack->transform->scale = Vector3D(20, 6, 1);
	stallBack->transform->position = Vector3D(0, 6, 7);
}

void ShootingStall::AddTargetMesh(Mesh* mesh, Transform offset, Mix_Chunk* sound, int _points)
{
	if (mesh != NULL) 
	{
		meshes.push_back(mesh);
		offsets[mesh] = offset; 
		sounds[mesh] = sound; 
		points[mesh] = _points;
	}
}

void ShootingStall::Update(float deltaTime)
{
	if (targetsToUnhook.size() > 0)
		UnhookEvents();

	if (active)
	{
		elapsedTime += deltaTime;
		if (elapsedTime > spawnRate)
			SpawnTarget();
	}
}

void ShootingStall::SpawnTarget()
{
	if ((int)meshes.size() > 0)
	{
		int i = rand() % (int)meshes.size();

		GameObject* target = new GameObject(new Target(meshes[i], sounds[meshes[i]], points[meshes[i]]));
		target->SetTag("Target");
		Target* targetScript = (Target*)target->GetComponent("Target");
		((MeshRenderer*)target->GetComponent("MeshRenderer"))->offsets = offsets[meshes[i]];
		targetScript->movementSpeed = movementSpeed;
		targetScript->SetPositions(transform->position + startPos, transform->position + endPos);
		currentTargets.push_back(targetScript);
		__hook(&Target::eOnTargetHit, targetScript, &ShootingStall::GainPoints);
		__hook(&Target::eOnDestroyNextFrame, targetScript, &ShootingStall::AddToUnhookList);

		elapsedTime = 0.0f;
	}
	else
		cout << "Error in shooting stall: No meshes available" << endl;
}

void ShootingStall::UnhookEvents() 
{
	for (unsigned int i = 0; i < targetsToUnhook.size();)
	{
		for (unsigned int j = 0; j < currentTargets.size(); j++)
		{
			if (targetsToUnhook[0] == currentTargets[j])
			{
				currentTargets.erase(currentTargets.begin() + j);
				break;
			}
		}
		__unhook(&Target::eOnDestroyNextFrame, targetsToUnhook[0], &ShootingStall::AddToUnhookList);
		__unhook(&Target::eOnTargetHit, targetsToUnhook[0], &ShootingStall::GainPoints);
		targetsToUnhook.erase(targetsToUnhook.begin());
	}
}

void ShootingStall::AddToUnhookList(Target* target)
{
	targetsToUnhook.push_back(target);
}