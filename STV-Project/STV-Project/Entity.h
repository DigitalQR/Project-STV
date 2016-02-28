#pragma once
#include "MemoryListener.h"
#include <vector>
#include "TexturedModel.h"
#include "Transform.h"
#include "Component.h"

using namespace std;

class Entity : public Transform
{
public:
	Entity(TexturedModel* textured_model);
	~Entity();

	TexturedModel* textured_model;

	virtual void LogicUpdate();
	virtual void VisualUpdate();

	void AddComponent(Component* component);
	void RemoveComponentAndDelete(Component* component);

private:
	vector<Component*> _components;
};

