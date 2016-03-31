#pragma once
#include "Component.h"
#include "Keyboard.h"
#include "Camera.h"

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
		Camera::getMain()->location = vec3(0, 70, 0);
	}


	void LogicUpdate() 
	{
		vec3 direction = Camera::getMain()->getForward();
		direction.y = 0;
		float speed = 0.4f;
		float sensitivity = 0.1f;

		if (Keyboard::isKeyDown('a'))
			Camera::getMain()->location -= cross(direction, vec3(0, 1, 0))*speed;

		if (Keyboard::isKeyDown('d'))
			Camera::getMain()->location += cross(direction, vec3(0, 1, 0))*speed;

		if (Keyboard::isKeyDown('w'))
			Camera::getMain()->location += direction*speed;

		if (Keyboard::isKeyDown('s'))
			Camera::getMain()->location -= direction*speed;

		if (Keyboard::isKeyDown('e'))
			Camera::getMain()->location += vec3(0, 1, 0)*speed;

		if (Keyboard::isKeyDown('q'))
			Camera::getMain()->location -= vec3(0, 1, 0)*speed;


		if (Keyboard::isKeyDown('j'))
			Camera::getMain()->rotation += vec3(0.0f, sensitivity, 0.0f);

		if (Keyboard::isKeyDown('l'))
			Camera::getMain()->rotation -= vec3(0.0f, sensitivity, 0.0f);

		if (Keyboard::isKeyDown('i'))
			Camera::getMain()->rotation += vec3(sensitivity, 0.0f, 0.0f);

		if (Keyboard::isKeyDown('k'))
			Camera::getMain()->rotation -= vec3(sensitivity, 0.0f, 0.0f);
	};

	void VisualUpdate() 
	{
		/*
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
			*/

	};
};

