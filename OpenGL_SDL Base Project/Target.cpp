#include "Target.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"

Target::Target(Mesh* _mesh, Mix_Chunk* _sound, int _points)
{	
	mesh = _mesh;
	sound = _sound;
	points = _points;

	componentTypeName = "Target";
}

void Target::Init()
{
	gameObject->AttachComponent(new MeshRenderer(mesh));
	bc = (BoxCollider*)gameObject->AttachCollider(BOX);
	bc->dimensions = Vector3D(1.5f, 1.5f, 1.5f);
}

void Target::SetPositions(Vector3D _startPos, Vector3D _endPos)
{
	transform->position = _startPos;
	endPos = _endPos;
	startDistance = (endPos - _startPos).Magnitude();
}

void Target::Update(float deltaTime)
{
	if (destroyingNextFrame)
		Destroy();

	transform->position += (endPos - transform->position).Normalise() * movementSpeed * deltaTime;

	if (falling && transform->rotation.x < 90)
		transform->rotation.x += fallSpeed * deltaTime;
	if (raising && transform->rotation.x > 0)
		transform->rotation.x -= fallSpeed * deltaTime;

	if ((transform->position - endPos).Magnitude() < 1 && !destroyingNextFrame)
		DestroyNextFrame();

}

void Target::DestroyNextFrame() 
{ 
	__raise eOnDestroyNextFrame(this);  
	destroyingNextFrame = true;
}

void Target::OnTakeDamage(float damage, GameObject* shotBy, GameObject* damageInstigator)
{
	if (!beenHit)
	{
		__raise eOnTargetHit(points, damageInstigator);
		falling = true;
		PlayMySound(sound);
		beenHit = true;
		((MeshRenderer*)gameObject->GetComponent("MeshRenderer"))->renderRed = true;
	}
}