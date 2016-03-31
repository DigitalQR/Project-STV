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
		Camera::getMain()->setOffset(vec3(0, 0, 0));
	}


	void LogicUpdate() 
	{
		vec3 direction = vec3(
			sin(Camera::getMain()->getOffset().y),
			0,
			cos(Camera::getMain()->getOffset().y)
			);

		if (Keyboard::isKeyDown('a'))
			Camera::getMain()->setPosition(Camera::getMain()->getPosition() - cross(direction, vec3(0, 1, 0))*0.2f);

		if (Keyboard::isKeyDown('d'))
			Camera::getMain()->setPosition(Camera::getMain()->getPosition() + cross(direction, vec3(0, 1, 0))*0.2f);

		if (Keyboard::isKeyDown('w'))
			Camera::getMain()->setPosition(Camera::getMain()->getPosition() + direction*0.2f);

		if (Keyboard::isKeyDown('s'))
			Camera::getMain()->setPosition(Camera::getMain()->getPosition() - direction*0.2f);

		if (Keyboard::isKeyDown('e'))
			Camera::getMain()->setPosition(Camera::getMain()->getPosition() + vec3(0.0f, 0.2f, 0.0f));

		if (Keyboard::isKeyDown('q'))
			Camera::getMain()->setPosition(Camera::getMain()->getPosition() + vec3(0.0f, -0.2f, 0.0f));


		if (Keyboard::isKeyDown('j'))
			Camera::getMain()->setOffset(Camera::getMain()->getOffset() + vec3(0.0f, 0.05f, 0.0f));

		if (Keyboard::isKeyDown('l'))
			Camera::getMain()->setOffset(Camera::getMain()->getOffset() + vec3(0.0f, -0.05f, 0.0f));

		if (Keyboard::isKeyDown('i'))
			Camera::getMain()->setOffset(Camera::getMain()->getOffset() + vec3(0.05f, 0.0f, 0.0f));

		if (Keyboard::isKeyDown('k'))
			Camera::getMain()->setOffset(Camera::getMain()->getOffset() + vec3(-0.05f, 0.0f, 0.0f));		
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

