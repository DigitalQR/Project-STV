#pragma once
#include <array>
#include "Dependencies\glm\common.hpp"

using namespace std;
using namespace glm;


class Mouse 
{
public:

	static Mouse* getMain()
	{
		static Mouse* MAIN = new Mouse();
		return MAIN;
	}

	static void SetState(unsigned int i, bool state) 
	{
		getMain()->_states[i] = state;
	}

	static bool isButtonDown(unsigned int i)
	{
		return getMain()->_states[i];
	}

	static bool isButtonUp(unsigned int i)
	{
		return !getMain()->_states[i];
	}

	static void UpdateMouseInfo(int x, int y) 
	{
		Mouse& m = *getMain();

		vec2 new_coords
			(
				((float)(x) / (float)(glutGet(GLUT_WINDOW_WIDTH)) - 0.5f) * 2.0f,
				((float)(y) / (float)(glutGet(GLUT_WINDOW_HEIGHT)) - 0.5f) * -2.0f
				);


		m.velocity = new_coords - m.mouse_coords;
		m.mouse_coords = new_coords;
	}

	static void WarpTo(int x, int y) 
	{
		glutWarpPointer(x, y);
	}

	static vec2 GetPosition() 
	{
		return getMain()->mouse_coords;
	}

	static vec2 GetVelocity() 
	{
		return getMain()->velocity;
	}

	static void SetScrollAmount(int amount)
	{
		getMain()->scroll_amount = (float)amount;
	}

	static float GetScrollAmount() 
	{
		float amount = getMain()->scroll_amount;
		getMain()->scroll_amount = 0;
		return amount;
	}

private:
	array<bool, 3> _states;
	vec2 mouse_coords;
	vec2 velocity;
	float scroll_amount;
};