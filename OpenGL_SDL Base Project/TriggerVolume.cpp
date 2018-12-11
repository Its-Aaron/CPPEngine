#include "TriggerVolume.h"

TriggerVolume::TriggerVolume()
{
	componentTypeName = "TriggerVolume";
}

void TriggerVolume::Init()
{
	bc = (BoxCollider*)gameObject->AttachComponent(ColliderManager::GetInstance()->AddBoxCollider());
}

void TriggerVolume::OnBeginOverlap(GameObject* other)
{
	//cout << "I made it" << endl;
}