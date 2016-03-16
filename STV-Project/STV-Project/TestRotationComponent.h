#pragma once
#include "Component.h"
#include "Keyboard.h"

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
		if (Keyboard::isKeyDown('a'))
			parent->rotation += vec3(0.0f, 0.05f, 0.0f);

		if (Keyboard::isKeyDown('d'))
			parent->rotation -= vec3(0.0f, 0.05f, 0.0f);

		if (Keyboard::isKeyDown('w'))
			parent->location -= vec3(0.0f, 0.5f, 0.0f);

		if (Keyboard::isKeyDown('s'))
			parent->location += vec3(0.0f, 0.5f, 0.0f);

		if (Keyboard::isKeyDown('q'))
			parent->location -= vec3(0.0f, 0.0f, 0.5f);

		if (Keyboard::isKeyDown('e'))
			parent->location += vec3(0.0f, 0.0f, 0.5f);

	};
};

