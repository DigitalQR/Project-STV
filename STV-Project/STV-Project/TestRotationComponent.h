#pragma once
#include "Component.h"

#include <iostream>

class TestRotationComponent :
	public Component
{
public:

	void LogicUpdate() {};
	void VisualUpdate() 
	{
		parent->rotation += vec3(0.0, 0.01f, 0.0);
	};
};

