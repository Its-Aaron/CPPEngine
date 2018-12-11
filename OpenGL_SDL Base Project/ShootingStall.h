#ifndef _SHOOTINGSTALL_H
#define _SHOOTINGSTALL_H

#include <map>
#include "Target.h"
#include <random>
class Mesh;

class ShootingStall : public Component
{
public:
	ShootingStall(Mesh* _stallBox);
	~ShootingStall() {}

	void AddTargetMesh(Mesh* mesh, Transform offset, Mix_Chunk* sound, int _points = 5);

	void Update(float deltaTime);

	void Init();

	vector<Target*> GetTargets() { return currentTargets; }

	__event void eOnGainPoints(int _points, GameObject* shotBy);
	float spawnRate = 2.0f;
	bool active = true;
	float elapsedTime = 0.0f;
	float movementSpeed = 5.0f;

private:
	void SpawnTarget();
	void GainPoints(int _points, GameObject* shotBy) { __raise eOnGainPoints(_points, shotBy); movementSpeed += 1; }
	void UnhookEvents();
	void AddToUnhookList(Target* target);

	vector<Target*> targetsToUnhook;

	vector<Target*> currentTargets;

	random_device rd;
	Mesh* stallBox;

	vector<Mesh*> meshes;
	std::map<Mesh*, Transform> offsets;
	std::map<Mesh*, Mix_Chunk*> sounds;
	std::map<Mesh*, int> points;

	Vector3D startPos = Vector3D(-18, 6, 5);
	Vector3D endPos = Vector3D(18, 6, 5);
};

#endif