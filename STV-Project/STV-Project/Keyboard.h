#pragma once
class Keyboard
{
public:
	static void setState(char key, bool state) { _key_states[key] = state; }
	static bool isKeyDown(char key) { return _key_states[key]; }
	static bool isKeyUp(char key) { return !_key_states[key]; }


private:
	static bool _key_states[256];

};

