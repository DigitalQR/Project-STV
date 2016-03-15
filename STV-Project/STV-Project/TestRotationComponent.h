#pragma once
#include "Component.h"

#include <iostream>

class TestRotationComponent : public Component
{
public:
	~TestRotationComponent() 
	{
		cout << "TTTTTTEEEEEEEEEESSSSSSSSSSTTTTTTTTTTTTTT" << endl;
	}
	void Start()
	{
		cout << "START" << endl;
	}


	void VisualUpdate() 
	{
		parent->rotation += vec3(0.0f, 0.001f, 0.0f);
	};
};

