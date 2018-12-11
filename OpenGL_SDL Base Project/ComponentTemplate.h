#ifndef _COMPONENTTEMPLATE_H
#define _COMPONENTTEMPLATE_H

#include "Component.h"

class ComponentTemplate : public Component
{
public:
	ComponentTemplate();
	~ComponentTemplate() {}


	void Update(float deltaTime);

	void Init();
};

#endif