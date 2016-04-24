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
			mouse_velocity += Mouse::GetPosition();
			mouse_velocity.x = round(mouse_velocity.x * 100) / 100.0f;
			mouse_velocity.y = round(mouse_velocity.y * 100) / 100.0f;
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

		vec2 forward = Camera::getMain()->getFlatForward();
		vec3 direction = vec3(forward.x, 0, forward.y);
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


		if (mouse_locked)
		{
			const float sensitivity = 2.0f;
			vec2 movement = mouse_velocity * sensitivity;
			mouse_velocity = vec2(0, 0);
			parent->rotation += vec3(movement.y, -movement.x, 0.0f);

			if (parent->rotation.x >= PI / 2.0f)
				parent->rotation.x = PI / 2.0f;
			if (parent->rotation.x <= -PI / 2.0f)
				parent->rotation.x = -PI / 2.0f;
		}
	};

	void VisualUpdate() 
	{

	};

};