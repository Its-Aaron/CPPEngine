#ifndef _TRIGGERVOLUME_H
#define _TRIGGERVOLUME_H

#include "Component.h"
#include "ColliderManager.h"

class TriggerVolume : public Component
{
public:
	TriggerVolume();
	~TriggerVolume() {}

	void Init();

	void OnBeginOverlap(GameObject* other);

private:
	BoxCollider* bc;
};

#endif