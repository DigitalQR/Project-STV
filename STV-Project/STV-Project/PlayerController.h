#pragma once
#include "Component.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Body.h"
#define PI 3.141592

class PlayerController : public Component
{
private:
	Body* body;
	vec2 mouse_velocity;

	bool last_lock_state = false;
	bool mouse_locked = false;

	void CheckForMouseLock() 
	{
		const bool state = Keyboard::isKeyDown('t');
		if (last_lock_state != state) 
		{
			last_lock_state = state;

			//Toggle
			if (state) 
			{
				mouse_locked = !mouse_locked;
				if (mouse_locked) 
				{
					glutSetCursor(GLUT_CURSOR_NONE);
					glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
				}
				else 
				{
					glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
				}
			}
		}

		if (mouse_locked)
		{
			mouse_velocity = Mouse::GetPosition();
			Mouse::WarpTo(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
		}
	}

public:
	void Start() 
	{
		Camera::getMain()->location = this->parent->location;
		body = parent->GetComponent<Body>();
		//body->gravity_scale = 0.0f;
	}
	

	void LogicUpdate() 
	{
		CheckForMouseLock();

		Camera::getMain()->location = parent->location;
		Camera::getMain()->rotation = parent->rotation;

		vec3 direction = Camera::getMain()->getForward();
		direction.y = 0;
		const float speed = 0.04f;

		vec3 acceleration;

		if (Keyboard::isKeyDown('a'))
			acceleration -= cross(direction, vec3(0, 1, 0))*speed;

		if (Keyboard::isKeyDown('d'))
			acceleration += cross(direction, vec3(0, 1, 0))*speed;

		if (Keyboard::isKeyDown('w'))
			acceleration += direction*speed;

		if (Keyboard::isKeyDown('s'))
			acceleration -= direction*speed;

		if (Keyboard::isKeyDown('e'))
			acceleration += vec3(0, 1, 0)*speed;

		if (Keyboard::isKeyDown('q'))
			acceleration -= vec3(0, 1, 0)*speed;

		body->velocity += acceleration;
		GameManager::getMain()->player_light.location = parent->location;
	};

	void VisualUpdate() 
	{
		const float sensitivity = 0.6f;

		if (mouse_locked)
		{
			vec2 movement = mouse_velocity * sensitivity;
			parent->rotation += vec3(movement.y, -movement.x, 0.0f);

			if (parent->rotation.x >= PI / 2.0f)
				parent->rotation.x = PI / 2.0f;
			if (parent->rotation.x <= -PI / 2.0f)
				parent->rotation.x = -PI / 2.0f;
		}

	};

};