#pragma once
#include "Component.h"
#include "Camera.h"
#include "Keyboard.h"
#define PI 3.141592

class PlayerController : public Component
{
public:
	void Start() 
	{
		Camera::getMain()->location = this->parent->location;
	}


	void LogicUpdate() 
	{
		Camera::getMain()->location = parent->location;
		Camera::getMain()->rotation = parent->rotation;

		vec3 direction = Camera::getMain()->getForward();
		direction.y = 0;
		float speed = 0.4f;
		float sensitivity = 0.1f;

		if (Keyboard::isKeyDown('a'))
			parent->location -= cross(direction, vec3(0, 1, 0))*speed;

		if (Keyboard::isKeyDown('d'))
			parent->location += cross(direction, vec3(0, 1, 0))*speed;

		if (Keyboard::isKeyDown('w'))
			parent->location += direction*speed;

		if (Keyboard::isKeyDown('s'))
			parent->location -= direction*speed;

		if (Keyboard::isKeyDown('e'))
			parent->location += vec3(0, 1, 0)*speed;

		if (Keyboard::isKeyDown('q'))
			parent->location -= vec3(0, 1, 0)*speed;


		if (Keyboard::isKeyDown('j'))
			parent->rotation += vec3(0.0f, sensitivity, 0.0f);

		if (Keyboard::isKeyDown('l'))
			parent->rotation -= vec3(0.0f, sensitivity, 0.0f);

		if (Keyboard::isKeyDown('i'))
		{
			parent->rotation += vec3(sensitivity, 0.0f, 0.0f);
			if (parent->rotation.x >= PI / 2.0f)
				parent->rotation.x = PI / 2.0f;
		}

		if (Keyboard::isKeyDown('k'))
		{
			parent->rotation -= vec3(sensitivity, 0.0f, 0.0f);
			if (parent->rotation.x <= -PI / 2.0f)
				parent->rotation.x = -PI / 2.0f;
		}
	};

	void VisualUpdate() {};

};