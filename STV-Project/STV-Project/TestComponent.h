#pragma once
#include "MemoryListener.h"

#include "Component.h"
#include <iostream>

using namespace std;

class TestComponent :
	public Component
{
public:
	TestComponent();
	~TestComponent();
	void LogicUpdate() {};
	void VisualUpdate() 
	{
		cout << "Test" << endl;
	};
};

