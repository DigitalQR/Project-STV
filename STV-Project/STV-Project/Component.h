#pragma once
#include "MemoryListener.h"
#include "Entity.h"

class Entity;

class Component
{
public:
	virtual ~Component() {};
	bool enabled = true;
	Entity* parent;
	
	void Attach(Entity* parent)
	{
		this->parent = parent;
	}
	virtual void Start() {};
	virtual void LogicUpdate() {};
	virtual void VisualUpdate() {};
};

