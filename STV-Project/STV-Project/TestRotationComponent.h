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
		parent->rotation += vec3(0.005f, 0.011f, 0.01f);
	};
};

