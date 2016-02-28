#pragma once
#include "MemoryListener.h"
#include "Transform.h"

class Component
{
public:
	bool enabled = true;
	Transform* parent;

	Component();
	~Component();

	void Attach(Transform* parent);
	virtual void LogicUpdate() = 0;
	virtual void VisualUpdate() = 0;
};

