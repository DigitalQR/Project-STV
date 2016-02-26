#pragma once
#include <iostream>

class Keyboard
{
public:
	Keyboard()
	{
		_key_states = new bool[256];
	}
	~Keyboard()
	{
		cout << "Deleting Keyboard." << endl;
		delete[] _key_states;
	}

	/**
	Generates a singleton

	@returns Keyboard singleton
	*/
	static Keyboard* getMain()
	{
		static Keyboard* MAIN = new Keyboard();
		return MAIN;
	}

	static void setState(char key, bool state) {
		getMain()->_key_states[key] = &state;
	}
	static bool isKeyDown(char key) { return getMain()->_key_states[key]; }
	static bool isKeyUp(char key) { return !getMain()->_key_states[key]; }


private:
	bool* _key_states;

};

